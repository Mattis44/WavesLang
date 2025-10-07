#include "Parser.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens)
	: tokens(tokens) { }

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!isAtEnd()) {
        if (match(TokenType::SEMICOLON)) continue;
        statements.push_back(declaration());
    }

    return statements;
}

std::unique_ptr<Stmt> Parser::declaration() {
	if (match(TokenType::IMP)) return importStatement();
	if (match(TokenType::PLAY)) return playStatement();
	if (match(TokenType::SET)) return setStatement();
	if (match(TokenType::CPM)) return cpmStatement();

	std::cerr << "[Parser] Unexpected token '" << peek().lexeme << "'\n";
	advance();
	return nullptr;
}

std::unique_ptr<Stmt> Parser::importStatement() {
	if (!match(TokenType::LEFT_BRACE)) {
		std::cerr << "[Parser] Expected '{' after 'imp'.\n";
		return nullptr;
	}

	std::vector<ImportEntry> entries;

	while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
		Token name = advance();
		if (name.type != TokenType::IDENTIFIER) {
			std::cerr << "[Parser] Expected module name.\n";
			break;
		}

		if (!match(TokenType::AS)) {
			std::cerr << "[Parser] Expected 'as' after module name.\n";
			break;
		}

		Token alias = advance();
		if (alias.type != TokenType::IDENTIFIER) {
			std::cerr << "[Parser] Expected alias after 'as'.\n";
			break;
		}

		entries.push_back({ name.lexeme, alias.lexeme });

		if (match(TokenType::COMMA)) continue;
		else break;
	}

	if (!match(TokenType::RIGHT_BRACE)) {
		std::cerr << "[Parser] Expected '}' after import list.\n";
	}

	return std::make_unique<ImportStmt>(std::move(entries));
}

std::unique_ptr<Stmt> Parser::playStatement() {
	Token alias = advance();
	if (alias.type != TokenType::IDENTIFIER) {
		std::cerr << "[Parser] Expected alias after 'play'.\n";
		return nullptr;
	}

	return std::make_unique<PlayStmt>(alias.lexeme);
}

std::unique_ptr<Stmt> Parser::setStatement() {
	Token alias = advance();

	if (alias.type != TokenType::IDENTIFIER) {
		std::cerr << "[Parser] Expected alias after 'set'.\n";
		return nullptr;
	}

	if (!match(TokenType::LEFT_BRACE)) {
		std::cerr << "[Parser] Expected '{' after alias in 'set'.\n";
		return nullptr;
	}

	std::vector<ParamEntry> params;

	while(!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
		Token name = advance();
		std::cout << name.lexeme << std::endl;
		if (name.type != TokenType::IDENTIFIER) {
            std::cerr << "[Parser] Expected parameter name.\n";
            break;
        }

		Token value = advance();
		if (value.type != TokenType::STRING && value.type != TokenType::IDENTIFIER) {
			std::cerr << "[Parser] Expected value after " << name.lexeme << ".\n";
			break;
		}

		params.push_back({ name.lexeme, value.lexeme});

		if (!match(TokenType::SEMICOLON)) {
            std::cerr << "[Parser] Expected ';' after parameter '" << name.lexeme << "'\n";
            break;
        }

		if (match(TokenType::COMMA)) continue;
		else break;
	}
	
	if (!match(TokenType::RIGHT_BRACE)) {
        std::cerr << "[Parser] Expected '}' after set block.\n";
    }

	std::make_unique<SetStmt>(alias.lexeme, std::move(params));
}

std::unique_ptr<Stmt> Parser::cpmStatement() {
	Token number = advance();

	if (number.type != TokenType::IDENTIFIER && number.type != TokenType::NUMBER) {
		std::cerr << "[Parser] Expected numeric value after 'cpm'.\n";
		return nullptr;
	}

	double value = 0.0;

	try {
		value = std::stod(number.lexeme);
	} catch (...) {
		std::cerr << "[Parser] Invalid CPM value: " << number.lexeme << "\n";
	}

	match(TokenType::SEMICOLON);
	return std::make_unique<CpmStmt>(value);
}

bool Parser::match(TokenType type) {
	if (check(type)) {
		advance();
		return true;
	}
	return false;
}

bool Parser::check(TokenType type) const {
	if (isAtEnd()) return false;
	return peek().type == type;
}

Token Parser::advance() {
	if (!isAtEnd()) current++;
	return previous();
}

bool Parser::isAtEnd() const {
	return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek() const {
	return tokens[current];
}

Token Parser::previous() const {
	return tokens[current - 1];
}