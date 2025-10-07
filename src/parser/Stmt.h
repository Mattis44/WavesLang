#pragma once
#include <vector>
#include <string>
#include <memory>
#include "runtime/ImportManager.h"
#include "common/Entries.h"

class StmtVisitor;

class Stmt {
public:
	virtual ~Stmt() = default;
	virtual void accept(StmtVisitor& visitor) = 0;
};

class ImportStmt : public Stmt {
public:
	std::vector<ImportEntry> entries;
	ImportStmt(std::vector<ImportEntry> e) : entries(std::move(e)) {}
	void accept(StmtVisitor& visitor) override;
};

class PlayStmt : public Stmt {
public:
	std::string alias;
	PlayStmt(std::string a) : alias(std::move(a)) {}
	void accept(StmtVisitor& visitor) override;
};

class SetStmt : public Stmt {
public:
	std::string alias;
	std::vector<ParamEntry> params;
	SetStmt(std::string a, std::vector<ParamEntry> p) : alias(std::move(a)), params(std::move(p)) {}
	void accept(StmtVisitor& visitor) override;
};

class CpmStmt : public Stmt {
	public:
		double value;
		CpmStmt(double v) : value(std::move(v)) {}
		void accept(StmtVisitor& visitor) override; 
};

class StmtVisitor {
public:
	virtual void visitImportStmt(ImportStmt& stmt) = 0;
	virtual void visitPlayStmt(PlayStmt& stmt) = 0;
	virtual void visitSetStmt(SetStmt& stmt) = 0;
	virtual void visitCpmStmt(CpmStmt& stmt) = 0;
};