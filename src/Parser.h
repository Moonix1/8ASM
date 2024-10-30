#pragma once
#include <stdint.h>
#include <vector>
#include <memory>
#include <tuple>
#include <any>

#include "Tokenizer.h"

namespace EASM {

struct OrgExpr {
    std::tuple<TokenType, std::string> value;
};

struct LabelExpr {
    std::string name;
};

struct MovExpr {
	std::tuple<TokenType, std::string> dest;
	
	bool isAddr = false;
	TokenType size;
    std::tuple<TokenType, std::string> value;
};

struct MovAddrExpr {
	TokenType size;
	std::tuple<TokenType, std::string> dest;

	bool isAddr = false;
    std::tuple<TokenType, std::string> value;
};

struct ArithmeticExpr {
	TokenType type;
	std::tuple<TokenType, std::string> dest;
	std::tuple<TokenType, std::string> value1;
    std::tuple<TokenType, std::string> value2;
};

struct NotExpr {
	std::tuple<TokenType, std::string> dest;
	std::tuple<TokenType, std::string> value;
};

struct SingleExpr {
	TokenType type;
	std::tuple<TokenType, std::string> value;
};

struct JumpExpr {
	TokenType type;
	std::tuple<TokenType, std::string> dest;
};

struct Halt {};

class Parser {
public:
    Parser(std::vector<Token> tokens)
        : m_Tokens(tokens) {}

    void ParseExpr();
public:
    inline const std::vector<std::any> &GetExprs() const { return m_Exprs; }
private:
    Token FetchToken(size_t &pos);
    Token FetchTokenC(size_t &pos);
private:
    std::vector<std::any> m_Exprs;
    std::vector<Token> m_Tokens;
};

}