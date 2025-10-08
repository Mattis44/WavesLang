#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "interpreter/Interpreter.h"
#include "ast/AstPrinter.h"
#include <fstream>
#include <sstream>
#include "audio/engine.h"

using namespace std;

int main() {
	std::ifstream file("examples/example.wv");
	if (!file) {
		std::cerr << "Error opening file";
		return 1;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string source = buffer.str();

	Lexer lexer(source);
	auto tokens = lexer.scanTokens();

	Parser parser(tokens);
	auto statements = parser.parse();

	std::cout << "PARSER : \n" << std::endl;
	AstPrinter printer;
	for (auto& st : statements) {
		if (st) st->accept(printer);
	}

	std::cout << "\nINTERPRETER : \n" << std::endl;

	initAudio();

	Interpreter interpreter;
	interpreter.interpret(statements);

	shutdownAudio();
	return 0;
}