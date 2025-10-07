#pragma once
#include "parser/Stmt.h"
#include <iostream>
#include <string>

class AstPrinter : public StmtVisitor {
public:
    void visitImportStmt(ImportStmt& stmt) {
        std::cout << "[ImportStmt]\n";
        for (auto& entry : stmt.entries) {
            std::cout << "  " << entry.name << " as " << entry.alias << "\n";
        }
    }

    void visitPlayStmt(PlayStmt& stmt) override {
        std::cout << "[PlayStmt] alias=" << stmt.alias << "\n";
    }

    void visitSetStmt(SetStmt& stmt) override {
        std::cout << "[SetStmt] alias=" << stmt.alias << "\n";
        for (auto& p : stmt.params) {
            std::cout << "  " << p.name << " = " << p.value << "\n";
        }
    }

    void visitCpmStmt(CpmStmt& stmt) override {
        std::cout << "[CpmStmt] value=" << stmt.value << "\n";
    }

    void visitLoopStmt(LoopStmt& stmt) override {
        std::cout << "[LoopStmt]" << "\n";
        for (auto& p : stmt.params) {
            std::cout << " " << p.name << " = " << p.value << "\n";
        }
    }
};
