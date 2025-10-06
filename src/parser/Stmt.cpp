#include "Stmt.h"
#include <iostream>

void ImportStmt::accept(StmtVisitor& visitor) {
	visitor.visitImportStmt(*this);
}

void PlayStmt::accept(StmtVisitor& visitor) {
	visitor.visitPlayStmt(*this);
}