#include "Lexer.h"
#include <cctype>
#include <iostream>
#include <unordered_map>

static const std::unordered_map<char, TokenType> single_char_tokens = {
    {'{',                                       TokenType::LEFT_BRACE},
    {'}',                                       TokenType::RIGHT_BRACE},
    {',',                                       TokenType::COMMA},
    {'.',                                       TokenType::DOT},
    {'/',                                       TokenType::SLASH},
	{';',                                       TokenType::SEMICOLON}
};

static const std::unordered_map<std::string, TokenType> double_char_tokens = {
    {"//",                                      TokenType::COMMENT}
};

static const std::unordered_map<std::string, TokenType> keywords = {
    {"play",                                   TokenType::PLAY},
	{"imp",                                    TokenType::IMP},
	{"as",                                     TokenType::AS},

	{"cpm",									   TokenType::CPM},
	{"pattern",                                TokenType::PATTERN},
	{"set",                                    TokenType::SET},
	{"sample",                                 TokenType::SAMPLE},
	{"volume",                                 TokenType::VOLUME},
	{"pitch",                                  TokenType::PITCH},
	{"loop",                                   TokenType::LOOP},
};

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

char Lexer::peek_next() const {
	return (current + 1 >= source.size()) ? '\0' : source[current + 1];
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

    std::string two_chars;
    two_chars += c;
    two_chars += peek();

    auto dbl = double_char_tokens.find(two_chars);
    if (dbl != double_char_tokens.end()) {
        if (dbl->second == TokenType::COMMENT) {
            advance();

            while (peek() != '\n' && !isAtEnd()) advance();

            return;
        } else {
            advance();
            addToken(dbl->second);
            return;
        }
    }

    auto it = single_char_tokens.find(c);
    if (it != single_char_tokens.end()) {
        addToken(it->second);
        return;
    }

    switch (c) {
        case '\n': line++; break;
        case ' ':
        case '\r':
        case '\t':
            break;
		case '"': string(); break;
        default:
			if (std::isdigit(c)) {
				number();
				break;
			} else if (std::isalpha(c)) {
                identifier();
				break;
            } else {
                std::cerr << "[Lexer] Unexpected character: '" << c
                          << "' at line " << line << "\n";
            }
            break;
    }
}


void Lexer::identifier() {
	while (std::isalnum(peek()) || peek() == '_') advance();

	std::string text = source.substr(start, current - start);
	auto it = keywords.find(text);
	TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
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

void Lexer::number() {
	while(std::isdigit(peek())) advance();
	// 1

	if (peek() == '.' && std::isdigit(peek_next())) {
        advance();
        while(std::isdigit(peek())) {
            advance();
        }
	}
	std::cout << start + " " + (int)(current - start) + peek();
	std::string value = source.substr(start, current - start);
	tokens.emplace_back(TokenType::NUMBER, value, line);
}