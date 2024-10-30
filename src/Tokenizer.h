#pragma once
#include <unordered_map>
#include <variant>
#include <fstream>
#include <vector>
#include <string>

namespace EASM {

enum TokenType {
	// Operand values
    IMMEDIATE,
    REG,
	ADDR,

	// Types
	WORD,
	BYTE,

	// Opcodes
    MOV,
	ADD,
	ADC,
	SUB,
	SBC,
	MUL,
	DIV,
	AND,
	OR,
	XOR,
	NOT,
	SHL,
	SHR,
	INC,
	DEC,
	CALLF,
	RETF,
	JUMP,
	JUMP_ZERO,
	JUMP_NOT_ZERO,
	JUMP_SIGN,
	JUMP_NOT_SIGN,
	JUMP_CARRY,
	JUMP_NOT_CARRY,
	HALT,

	// Assembler Directives
	ORG,

	// Other
	LABEL,

    COMMA,

    UNKNOWN,
};

struct Token {
    TokenType type;
    std::string value;
};

class Tokenizer {
public:
    void TokenizeFile(std::string filePath);

    inline const std::vector<Token> &GetTokens() const { return m_Tokens; }
private:
    void TokenizeLine(std::string line);
    void TokenizeAlnum(int &pos, std::string line);
	void TokenizeDirective(int &pos, std::string line);
	void TokenizeAddr(int &pos, std::string line);
    void TokenizeImmediate(int &pos, std::string line);
private:
    std::vector<Token> m_Tokens;
};

}