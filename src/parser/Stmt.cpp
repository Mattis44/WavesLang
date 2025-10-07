#include "Stmt.h"
#include <iostream>

void ImportStmt::accept(StmtVisitor& visitor) {
	visitor.visitImportStmt(*this);
}

void PlayStmt::accept(StmtVisitor& visitor) {
	visitor.visitPlayStmt(*this);
}

void SetStmt::accept(StmtVisitor& visitor) {
    visitor.visitSetStmt(*this);
}

void CpmStmt::accept(StmtVisitor& visitor) {
    visitor.visitCpmStmt(*this);
}

void LoopStmt::accept(StmtVisitor& visitor) {
    visitor.visitLoopStmt(*this);
}