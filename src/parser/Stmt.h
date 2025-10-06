#pragma once
#include <vector>
#include <string>
#pragma once
#include <memory>

struct ImportEntry {
	std::string name;
	std::string alias;
};

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

class StmtVisitor {
public:
	virtual void visitImportStmt(ImportStmt& stmt) = 0;
	virtual void visitPlayStmt(PlayStmt& stmt) = 0;
};