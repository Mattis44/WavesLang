// WavesLang.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "interpreter/Interpreter.h"

using namespace std;

int main()
{
	std::string source = R"(
imp {
	drum as d,
	piano as p
}
play d
)";

	Lexer lexer(source);
	auto tokens = lexer.scanTokens();
	
	Parser parser(tokens);
	auto statements = parser.parse();

	Interpreter interpreter;
	interpreter.interpret(statements);

	return 0;
}
