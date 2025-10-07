#pragma once
#include "parser/Stmt.h"
#include "runtime/ImportManager.h"
#include <vector>
#include <memory>

class Interpreter : public StmtVisitor {
public:
	void interpret(const std::vector<std::unique_ptr<Stmt>>& statement);

	void visitImportStmt(ImportStmt& stmt) override;
	void visitPlayStmt(PlayStmt& stmt) override;
	void visitSetStmt(SetStmt& stmt) override;
	void visitCpmStmt(CpmStmt& stmt) override;

private:
	ImportManager importManager;
};