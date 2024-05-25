#include "NVSELexer.h"
#include "NVSECompilerUtils.h"

NVSELexer::NVSELexer(const std::string& input) : input(input), pos(0) {
	// Messy way of just getting string lines to start for later error reporting
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = input.find('\n', prev)) != std::string::npos)
    {
        lines.push_back(input.substr(prev, pos - prev - 1));
        prev = pos + 1;
    }
    lines.push_back(input.substr(prev));
}

NVSEToken NVSELexer::GetNextToken() {
    while (pos < input.size() && std::isspace(input[pos])) {
        pos++;
        linePos++;
        if (input[pos] == '\n') {
            line++;
            pos++;
            linePos = 1;
        }
    }

    if (pos == input.size()) return MakeToken(NVSETokenType::Eof, "");

    char current = input[pos];
    if (std::isdigit(current)) {
        size_t len;
        double value = std::stod(&input[pos], &len);
        if (input[pos + len - 1] == '.') {
            len--;
        }
        pos += len;
        linePos += len;
        return MakeToken(NVSETokenType::Number, input.substr(pos - len, len), value);
    }

    if (std::isalpha(current) || current == '_') {
        // Extract identifier
        size_t start = pos;
        while (pos < input.size() && (std::isalnum(input[pos]) || input[pos] == '_')) ++pos;
        std::string identifier = input.substr(start, pos - start);

        // Keywords
        if (identifier == "if") return MakeToken(NVSETokenType::If, identifier);
        if (identifier == "else") return MakeToken(NVSETokenType::Else, identifier);
        if (identifier == "while") return MakeToken(NVSETokenType::While, identifier);
        if (identifier == "fn") return MakeToken(NVSETokenType::Fn, identifier);
        if (identifier == "return") return MakeToken(NVSETokenType::Return, identifier);
        if (identifier == "for") return MakeToken(NVSETokenType::For, identifier);
        if (identifier == "name") return MakeToken(NVSETokenType::Name, identifier);

        // Types
        if (identifier == "int") return MakeToken(NVSETokenType::IntType, identifier);
        if (identifier == "double") return MakeToken(NVSETokenType::DoubleType, identifier);
        if (identifier == "ref") return MakeToken(NVSETokenType::RefType, identifier);
        if (identifier == "string") return MakeToken(NVSETokenType::StringType, identifier);
        if (identifier == "array") return MakeToken(NVSETokenType::ArrayType, identifier);

        // Boolean literals
        if (identifier == "true") return MakeToken(NVSETokenType::Bool, identifier, 1);
        if (identifier == "false") return MakeToken(NVSETokenType::Bool, identifier, 0);

        // See if it is a begin block type
        for (auto& g_eventBlockCommandInfo : g_eventBlockCommandInfos) {
            if (!strcmp(g_eventBlockCommandInfo.longName, identifier.c_str())) {
                return MakeToken(NVSETokenType::BlockType, identifier);
            }
        }

        return MakeToken(NVSETokenType::Identifier, identifier);
    }

    if (current == '"') {
        size_t start = pos++;
        while (pos < input.size() && (input[pos] != '"' || input[pos - 1] == '\\')) pos++;
        if (input[pos] != '\"') {
            throw std::runtime_error("Unexpected EOF");
        }
        pos++;
        const auto len = pos - start;
        std::string text = input.substr(start + 1, len - 2);
        std::string lexeme = '\"' + text + '\"';
        return MakeToken(NVSETokenType::String, lexeme, text);
    }

    pos++;
    switch (current) {
        // Operators
    case '+': {
        if (Match('=')) {
            return MakeToken(NVSETokenType::PlusEq, "+=");
        }
        else if (Match('+')) {
            return MakeToken(NVSETokenType::PlusPlus, "++");
        }
        return MakeToken(NVSETokenType::Plus, "+");
    }
    case '-': {
        if (Match('=')) {
            return MakeToken(NVSETokenType::MinusEq, "-=");
        }
        else if (Match('-')) {
            return MakeToken(NVSETokenType::MinusMinus, "--");
        }
        return MakeToken(NVSETokenType::Minus, "-");
    }
    case '*':
        if (Match('=')) {
            return MakeToken(NVSETokenType::StarEq, "*=");
        }
        return MakeToken(NVSETokenType::Star, "*");
    case '/':
        if (Match('=')) {
            return MakeToken(NVSETokenType::SlashEq, "/=");
        }
        return MakeToken(NVSETokenType::Slash, "/");
    case '=':
        if (Match('=')) {
            return MakeToken(NVSETokenType::EqEq, "==");
        }
        return MakeToken(NVSETokenType::Eq, "=");
    case '!':
        if (Match('=')) {
            return MakeToken(NVSETokenType::BangEq, "!=");
        }
        return MakeToken(NVSETokenType::Bang, "!");
    case '<':
        if (Match('=')) {
            return MakeToken(NVSETokenType::LessEq, "<=");
        }
        return MakeToken(NVSETokenType::Less, "<");
    case '>':
        if (Match('=')) {
            return MakeToken(NVSETokenType::GreaterEq, ">=");
        }
        return MakeToken(NVSETokenType::Greater, ">");
    case '|':
        if (Match('|')) {
            return MakeToken(NVSETokenType::LogicOr, "||");
        }
        return MakeToken(NVSETokenType::BitwiseOr, "|");
    case '~': return MakeToken(NVSETokenType::Tilde, "~");
    case '&':
        if (Match('&')) {
            return MakeToken(NVSETokenType::LogicAnd, "&&");
        }
        return MakeToken(NVSETokenType::BitwiseAnd, "&");
    case '$': return MakeToken(NVSETokenType::Dollar, "$");

        // Braces
    case '{': return MakeToken(NVSETokenType::LeftBrace, "{");
    case '}': return MakeToken(NVSETokenType::RightBrace, "}");
    case '[': return MakeToken(NVSETokenType::LeftBracket, "[");
    case ']': return MakeToken(NVSETokenType::RightBracket, "]");
    case '(': return MakeToken(NVSETokenType::LeftParen, "(");
    case ')': return MakeToken(NVSETokenType::RightParen, ")");

        // Misc
    case ',': return MakeToken(NVSETokenType::Comma, ",");
    case ';': return MakeToken(NVSETokenType::Semicolon, ";");
    case '?': return MakeToken(NVSETokenType::Ternary, "?");
    case ':': return MakeToken(NVSETokenType::Colon, ":");
    case '.': return MakeToken(NVSETokenType::Dot, ".");

    default: throw std::runtime_error("Unexpected character");
    }
}

bool NVSELexer::Match(char c) {
    if (pos >= input.size()) {
        return false;
    }

    if (input[pos] == c) {
        pos++;
        return true;
    }

    return false;
}

NVSEToken NVSELexer::MakeToken(NVSETokenType type, std::string lexeme) {
    NVSEToken t(type, lexeme);
    t.line = line;
    t.linePos = linePos;
    linePos += lexeme.length();
    return t;
}

NVSEToken NVSELexer::MakeToken(NVSETokenType type, std::string lexeme, double value) {
    NVSEToken t(type, lexeme, value);
    t.line = line;
    t.linePos = linePos;
    linePos += lexeme.length();
    return t;
}

NVSEToken NVSELexer::MakeToken(NVSETokenType type, std::string lexeme, std::string value) {
    NVSEToken t(type, lexeme, value);
    t.line = line;
    t.linePos = linePos;
    linePos += lexeme.length();
    return t;
}