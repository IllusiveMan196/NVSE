#pragma once
struct NVSEScript;

struct BeginStmt;
struct FnDeclStmt;
struct VarDeclStmt;

struct ExprStmt;
struct ForStmt;
struct IfStmt;
struct ReturnStmt;
struct ContinueStmt;
struct BreakStmt;
struct WhileStmt;
struct BlockStmt;

struct AssignmentExpr;
struct TernaryExpr;
struct BinaryExpr;
struct UnaryExpr;
struct SubscriptExpr;
struct CallExpr;
struct GetExpr;
struct BoolExpr;
struct NumberExpr;
struct StringExpr;
struct IdentExpr;
struct GroupingExpr;
struct LambdaExpr;

class NVSEVisitor {
public:
	virtual ~NVSEVisitor() = default;
	
	virtual void VisitNVSEScript(const NVSEScript* script) = 0;
	virtual void VisitBeginStmt(const BeginStmt* stmt) = 0;
	virtual void VisitFnStmt(FnDeclStmt* stmt) = 0;
	virtual void VisitVarDeclStmt(const VarDeclStmt* stmt) = 0;

	virtual void VisitExprStmt(const ExprStmt* stmt) = 0;
	virtual void VisitForStmt(const ForStmt* stmt) = 0;
	virtual void VisitIfStmt(IfStmt* stmt) = 0;
	virtual void VisitReturnStmt(ReturnStmt* stmt) = 0;
	virtual void VisitContinueStmt(ContinueStmt* stmt) = 0;
	virtual void VisitBreakStmt(BreakStmt* stmt) = 0;
	virtual void VisitWhileStmt(const WhileStmt* stmt) = 0;
	virtual void VisitBlockStmt(BlockStmt* stmt) = 0;

	virtual void VisitAssignmentExpr(const AssignmentExpr* expr) = 0;
	virtual void VisitTernaryExpr(const TernaryExpr* expr) = 0;
	virtual void VisitBinaryExpr(BinaryExpr* expr) = 0;
	virtual void VisitUnaryExpr(UnaryExpr* expr) = 0;
	virtual void VisitSubscriptExpr(SubscriptExpr* expr) = 0;
	virtual void VisitCallExpr(CallExpr* expr) = 0;
	virtual void VisitGetExpr(GetExpr* expr) = 0;
	virtual void VisitBoolExpr(BoolExpr* expr) = 0;
	virtual void VisitNumberExpr(NumberExpr* expr) = 0;
	virtual void VisitStringExpr(StringExpr* expr) = 0;
	virtual void VisitIdentExpr(IdentExpr* expr) = 0;
	virtual void VisitGroupingExpr(GroupingExpr* expr) = 0;
	virtual void VisitLambdaExpr(LambdaExpr* expr) = 0;
};
