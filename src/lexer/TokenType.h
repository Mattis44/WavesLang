#pragma once

enum class TokenType {
	LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT,

	IMP,
	AS,
	PLAY,

	IDENTIFIER,
	STRING,

	END_OF_FILE
};