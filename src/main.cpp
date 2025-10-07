#include <iostream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "interpreter/Interpreter.h"
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
	std::ifstream file("src/examples/example.wv");
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string source = buffer.str();

	Lexer lexer(source);
	auto tokens = lexer.scanTokens();
	
	Parser parser(tokens);
	auto statements = parser.parse();

	Interpreter interpreter;
	interpreter.interpret(statements);

	return 0;
}
