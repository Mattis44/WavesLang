#pragma once
#include "parser/Stmt.h"
#include "runtime/ImportManager.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>

class Interpreter : public StmtVisitor {
public:
    Interpreter();

    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);

    void visitImportStmt(ImportStmt& stmt) override;
    void visitPlayStmt(PlayStmt& stmt) override;
    void visitSetStmt(SetStmt& stmt) override;
    void visitCpmStmt(CpmStmt& stmt) override;
    void visitLoopStmt(LoopStmt& stmt) override;

private:
    ImportManager importManager;

    int cpm = 120;
    double currentVolume = 1.0;
    double currentPitch = 1.0;
    std::string currentSample;

    std::unordered_map<std::string, std::function<void(const ParamEntry&)>> paramHandlers;
    std::unordered_map<std::string, std::function<void(const ParamEntry&)>> loopActions;

    void initParamHandlers();
    void initLoopActions();
};
