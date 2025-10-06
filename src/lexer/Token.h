#pragma once
#include <string>
#include "TokenType.h"

struct Token {
	TokenType type;
	std::string lexeme;
	int line;

	Token(TokenType type, std::string lexeme, int line)
		: type(type), lexeme(std::move(lexeme)), line(line) { }


};