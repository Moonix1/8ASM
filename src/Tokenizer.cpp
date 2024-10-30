#include "Tokenizer.h"
#include "StringManip.h"

#include "Log/Log.h"

namespace EASM {

void Tokenizer::TokenizeFile(std::string filePath) {
    std::ifstream file(filePath);

    std::string line;
    while (std::getline(file, line)) {
        TokenizeLine(line);
    }
}

void Tokenizer::DumpTokens() {
    std::unordered_map<TokenType, std::string> tokenTypeToString = {
        { TokenType::IMMEDIATE, "immediate" },
        { TokenType::REG, "reg" },
		{ TokenType::ADDR, "addr" },
		{ TokenType::LABEL, "label" },
		{ TokenType::ORG, "org" },
        { TokenType::MOV, "mov" },
		{ TokenType::ADD, "add" },
		{ TokenType::ADC, "adc" },
		{ TokenType::SUB, "sub" },
		{ TokenType::SBC, "sbc" },
		{ TokenType::MUL, "mul" },
		{ TokenType::DIV, "div" },
		{ TokenType::JUMP, "jmp" },
		{ TokenType::HALT, "hlt" },
		{ TokenType::WORD, "word" },
		{ TokenType::BYTE, "byte" },
        { TokenType::COMMA, "comma" },
        { TokenType::UNKNOWN, "unknown" },
    };

    for (Token &tok : m_Tokens) {
        LOG_INFO("{0} -> {1}", tokenTypeToString[tok.type], tok.value);
    }
}

void Tokenizer::TokenizeLine(std::string line) {
    line = StringManip::Trim(line);

	if (line.find(':') != std::string::npos) {
		size_t colonPos = line.find(':');
		
		std::string label = line.substr(0, colonPos);
		line = line.substr(colonPos + 1);

		m_Tokens.push_back(Token { .type = LABEL, .value = label });

		line = StringManip::Trim(line);
	}


    if (!line.empty()) {
        for (int pos = 0 ; pos < line.size() ; pos++) {
            char current = line[pos];

            // Skip whitespaces
            if (isspace(current)) { continue; }

            if (isalnum(current) || current == '_') {
                TokenizeAlnum(pos, line);
                continue;
            }

            switch (current) {
			case '.': TokenizeDirective(pos, line); break;
			case '[': TokenizeAddr(pos, line); break;
            case '#': TokenizeImmediate(pos, line); break;
            case ',': m_Tokens.push_back(Token { .type = COMMA, .value = std::string(1, current) }); break;
            default: m_Tokens.push_back(Token { .type = UNKNOWN, .value = std::string(1, current) }); break;
            }
        }
    }
}

void Tokenizer::TokenizeAlnum(int &pos, std::string line) {
    int start = pos;
    while (pos < line.size() && (isalnum(line[pos]) || line[pos] == '_')) { pos++; }
    std::string value = line.substr(start, pos - start);
	pos--;

    if (value == "mov" || value == "MOV") {
        m_Tokens.push_back(Token { .type = MOV, .value = value });
        return;
	} else if (value == "add" || value == "ADD") {
        m_Tokens.push_back(Token { .type = ADD, .value = value });
        return;
	} else if (value == "adc" || value == "ADC") {
        m_Tokens.push_back(Token { .type = ADC, .value = value });
        return;
	} else if (value == "sub" || value == "SUB") {
        m_Tokens.push_back(Token { .type = SUB, .value = value });
        return;
	} else if (value == "sbc" || value == "SBC") {
        m_Tokens.push_back(Token { .type = SBC, .value = value });
        return;
	} else if (value == "mul" || value == "MUL") {
        m_Tokens.push_back(Token { .type = MUL, .value = value });
        return;
	} else if (value == "div" || value == "DIV") {
        m_Tokens.push_back(Token { .type = DIV, .value = value });
        return;
	} else if (value == "jmp" || value == "JMP") {
		m_Tokens.push_back(Token { .type = JUMP, .value = value });
        return;
	} else if (value == "jz" || value == "JZ") {
		m_Tokens.push_back(Token { .type = JUMP_ZERO, .value = value });
        return;
	} else if (value == "jnz" || value == "JNZ") {
		m_Tokens.push_back(Token { .type = JUMP_NOT_ZERO, .value = value });
        return;
	} else if (value == "js" || value == "JS") {
		m_Tokens.push_back(Token { .type = JUMP_SIGN, .value = value });
        return;
	} else if (value == "jns" || value == "JNS") {
		m_Tokens.push_back(Token { .type = JUMP_NOT_SIGN, .value = value });
        return;
	} else if (value == "jc" || value == "JC") {
		m_Tokens.push_back(Token { .type = JUMP_CARRY, .value = value });
        return;
	} else if (value == "jnc" || value == "JNC") {
		m_Tokens.push_back(Token { .type = JUMP_NOT_CARRY, .value = value });
    } else if (value == "hlt" || value == "HLT") {
        m_Tokens.push_back(Token { .type = HALT, .value = value });
        return;
    }

    if (value.find('r') == 0 || value.find('R') == 0) {
        m_Tokens.push_back(Token { .type = REG, .value = value });
        return;
    }

    m_Tokens.push_back(Token { .type = UNKNOWN, .value = value });
}

void Tokenizer::TokenizeDirective(int &pos, std::string line) {
	pos++;

    int start = pos;
    while (pos < line.size() && isalnum(line[pos])) { pos++; }
    std::string value = line.substr(start, pos - start);

    if (value == "org" || value == "ORG") {
        m_Tokens.push_back(Token { .type = ORG, .value = value });
        return;
    }

	if (value == "word" || value == "WORD") {
        m_Tokens.push_back(Token { .type = WORD, .value = value });
        return;
    } else if (value == "byte" || value == "BYTE") {
        m_Tokens.push_back(Token { .type = BYTE, .value = value });
        return;
    }

    m_Tokens.push_back(Token { .type = UNKNOWN, .value = value });
}

void Tokenizer::TokenizeAddr(int &pos, std::string line) {
    pos++;

    int start = pos;
    while (pos < line.size() && isalnum(line[pos])) { pos++; }
    std::string value = line.substr(start, pos - start);

    m_Tokens.push_back(Token { .type = ADDR, .value = value });
}

void Tokenizer::TokenizeImmediate(int &pos, std::string line) {
    pos++;

    int start = pos;
    while (pos < line.size() && isalnum(line[pos])) { pos++; }
    std::string value = line.substr(start, pos - start);

    m_Tokens.push_back(Token { .type = IMMEDIATE, .value = value });
}

}