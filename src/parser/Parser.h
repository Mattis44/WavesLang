#pragma once
#include <vector>
#include <memory>
#include "lexer/Token.h"
#include "Stmt.h"

class Parser {
public:
	Parser(const std::vector<Token>& tokens);

	std::vector<std::unique_ptr<Stmt>> parse();

private:
	const std::vector<Token> tokens;
	size_t current = 0;

	std::unique_ptr<Stmt> declaration();
	std::unique_ptr<Stmt> importStatement();
	std::unique_ptr<Stmt> playStatement();
	std::unique_ptr<Stmt> setStatement();
	std::unique_ptr<Stmt> cpmStatement();
	std::unique_ptr<Stmt> loopStatement();

	bool match(TokenType type);
	bool check(TokenType type) const;
	Token advance();
	bool isAtEnd() const;
	Token peek() const;
	Token previous() const;
	void synchronize();
};