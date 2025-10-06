#include "Lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source)
	: source(source) { }

std::vector<Token> Lexer::scanTokens() {
	while (!isAtEnd()) {
		start = current;
		scanToken();
	}

	tokens.emplace_back(TokenType::END_OF_FILE, "EOF", line);
	return tokens;
}

bool Lexer::isAtEnd() const {
	return current >= source.size();
}

char Lexer::advance() {
	return source[current++];
}

char Lexer::peek() const {
	return isAtEnd() ? '\0' : source[current];
}

bool Lexer::match(char expected) {
	if (isAtEnd() || source[current] != expected) return false;
	current++;
	return true;
}

void Lexer::addToken(TokenType type) {
	std::string text = source.substr(start, current - start);
	tokens.emplace_back(type, text, line);
}

void Lexer::scanToken() {
	char c = advance();
	switch (c) {
		case '{': addToken(TokenType::LEFT_BRACE); break;
		case '}': addToken(TokenType::RIGHT_BRACE); break;
		case ',': addToken(TokenType::COMMA); break;
		case '.': addToken(TokenType::DOT); break;

		case '\n': line++; break;
		case ' ':
		case '\r':
		case '\t':
			break;
		default:
			if (std::isalpha(c)) {
				identifier();
			}
			else if (c == '"') {
				string();
			}
			else {
				std::cerr << "[Lexer] Unexpected character: '" << c << "' at line " << line << "\n";
			}
			break;
	}
}

void Lexer::identifier() {
	while (std::isalnum(peek()) || peek() == '_') advance();

	std::string text = source.substr(start, current - start);
	TokenType type = keywordType(text);
	addToken(type);
}

void Lexer::string() {
	while (peek() != '"' && !isAtEnd()) {
		if (peek() == '\n') line++;
		advance();
	}

	if (isAtEnd()) {
		std::cerr << "[Lexer] Unterminated string at line " << line << "\n";
		return;
	}

	advance();

	std::string value = source.substr(start + 1, current - start - 2);
	tokens.emplace_back(TokenType::STRING, value, line);
}

TokenType Lexer::keywordType(const std::string& text) {
	if (text == "imp") return TokenType::IMP;
	if (text == "as") return TokenType::AS;
	if (text == "play") return TokenType::PLAY;
	return TokenType::IDENTIFIER;
}