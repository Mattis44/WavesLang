#pragma once
#include "Token.h"
#include "TokenType.h"
#include <string>
#include <vector>

class Lexer {
public:
	Lexer(const std::string& source);

	std::vector<Token> scanTokens();

 private:
	std::string source;
	std::vector<Token> tokens;
	size_t start = 0;
	size_t current = 0;
	int line = 1;

	bool isAtEnd() const;
	char advance();
	char peek() const;
	bool match(char expected);

	void scanToken();
	void addToken(TokenType type);
	void identifier();
	void string();
	TokenType keywordType(const std::string& text);
};