#include "Parser.h"
#include <iostream>
#include <unordered_set>

static const std::unordered_set<TokenType> parameterKeywords = {
	TokenType::SAMPLE,
	TokenType::VOLUME,
	TokenType::PITCH
};

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
	if (match(TokenType::LOOP)) return loopStatement();

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

	while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
		Token name = advance();
		if (name.type != TokenType::IDENTIFIER && !parameterKeywords.count(name.type)) {
			std::cerr << "[Parser] Expected parameter name.\n";
			synchronize();
			continue;
		}

		if (isAtEnd()) break;
		Token value = advance();

		if (value.type != TokenType::STRING &&
			value.type != TokenType::NUMBER &&
			value.type != TokenType::IDENTIFIER) {
			std::cerr << "[Parser] Expected value after '" << name.lexeme << "'.\n";
			synchronize();
			continue;
		}

		params.push_back({ name.lexeme, value.lexeme });

		if (match(TokenType::SEMICOLON)) {
			continue;
		}

		if (check(TokenType::RIGHT_BRACE)) break;

		synchronize();
	}

	if (!match(TokenType::RIGHT_BRACE)) {
		std::cerr << "[Parser] Expected '}' after set block.\n";
		return nullptr;
	}

	return std::make_unique<SetStmt>(alias.lexeme, std::move(params));
}


std::unique_ptr<Stmt> Parser::cpmStatement() {
	Token number = advance();

	if (number.type != TokenType::IDENTIFIER && number.type != TokenType::NUMBER) {
		std::cerr << "[Parser] Expected numeric value after 'cpm'.\n";
		return nullptr;
	}

	int value = 0;

	try {
		value = std::stoi(number.lexeme);
	} catch (...) {
		std::cerr << "[Parser] Invalid CPM value: " << number.lexeme << "\n";
	}

	match(TokenType::SEMICOLON);
	return std::make_unique<CpmStmt>(value);
}

std::unique_ptr<Stmt> Parser::loopStatement() {
	if (!match(TokenType::LEFT_BRACE)) {
		std::cerr << "[Parser] Expected '{' in loop.\n";
		return nullptr;
	}

	std::vector<ParamEntry> params;

	while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
		Token name = advance();

		if (name.type != TokenType::IDENTIFIER && parameterKeywords.count(name.type)) {
			std::cerr << "[Parser] Expected parameter name.\n";
			synchronize();
			continue;
		}

		if (isAtEnd()) break;

		Token value = advance();
		
		if (value.type != TokenType::IDENTIFIER &&
			value.type != TokenType::STRING &&
			value.type != TokenType::NUMBER) {
			std::cerr << "[Parser] Expected value after '" << name.lexeme << "'.\n";
			synchronize();
			continue;
		}

		params.push_back({ name.lexeme, value.lexeme });
		if (!match(TokenType::SEMICOLON)) {
			std::cerr << "[Parser] Expected ';' after parameter '" << name.lexeme << "'.\n";
			synchronize();
		}
	}

	if (!match(TokenType::RIGHT_BRACE)) {
		std::cerr << "[Parser] Expected '}' after set block.\n";
		return nullptr;
	}

	return std::make_unique<LoopStmt>(std::move(params));
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

void Parser::synchronize() {
	while (!isAtEnd()) {
		if (previous().type == TokenType::SEMICOLON) return;
		switch (peek().type) {
		case TokenType::RIGHT_BRACE:
			return;
		default:
			advance();
		}
	}
}