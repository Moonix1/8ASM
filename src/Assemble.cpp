#include "Assemble.h"
#include "Log/Log.h"

#include "Defines/Opcodes.h"
#include "Defines/Registers.h"
#include "Defines/Types.h"

#include <typeinfo>
#include <any>

namespace EASM {

uint8_t StringToRegister(std::string reg) {
	if (reg == "R0" || reg == "r0") return R0;
	if (reg == "R1" || reg == "r1") return R1;
	if (reg == "R2" || reg == "r2") return R2;
	if (reg == "R3" || reg == "r3") return R3;
	if (reg == "R4" || reg == "r4") return R4;
	if (reg == "R5" || reg == "r5") return R5;
	if (reg == "R6" || reg == "r6") return R6;
	if (reg == "R7" || reg == "r7") return R7;
	if (reg == "R8" || reg == "r8") return R8;
	if (reg == "R9" || reg == "r9") return R9;
	if (reg == "R10" || reg == "r10") return R10;
	if (reg == "R11" || reg == "r11") return R11;

	LOG_ERROR("Invalid Register: {0}", reg);
	exit(1);
}

void PushU16(std::vector<uint8_t> &program, std::tuple<TokenType, std::string> value) {
	if (std::get<0>(value) == IMMEDIATE) {
		if (std::get<1>(value)[0] == '0'
			&& (std::get<1>(value)[1] == 'x' || std::get<1>(value)[1] == 'X')) {
			char *end;
			const char *sValue = std::get<1>(value).c_str();
			uint16_t value = (uint16_t)std::strtol(sValue, &end, 16);
			program.push_back(value & 0xFF);
			program.push_back((value >> 8) & 0xFF);
		} else {
			char *end;
			const char *sValue = std::get<1>(value).c_str();
			uint16_t value = (uint16_t)std::strtol(sValue, &end, 10);
			program.push_back(value & 0xFF);
			program.push_back((value >> 8) & 0xFF);
		}
	} else if (std::get<0>(value) == ADDR) {
		char *end;
		const char *sValue = std::get<1>(value).c_str();
		uint16_t value = (uint16_t)std::strtol(sValue, &end, 16);
		program.push_back(value & 0xFF);
		program.push_back((value >> 8) & 0xFF);
	} else {
		LOG_ERROR("PushNumberU16: invalid value for translation!");
		exit(1);
	}
}

void PushU8(std::vector<uint8_t> &program, std::tuple<TokenType, std::string> value) {
	const char *sValue = std::get<1>(value).c_str();
	uint8_t val = static_cast<uint8_t>(std::stoi(sValue));
	program.push_back(val);
}

void PushType(std::vector<uint8_t> &program, TokenType type) {
	if (type == WORD) {
		program.push_back(DTWORD);
	} else if (type == BYTE) {
		program.push_back(DTBYTE);
	} else {
		LOG_ERROR("PushType: invalid value for translation!");
		exit(1);
	}
}

void Arithmetic(
	std::vector<uint8_t> &program, ArithmeticExpr arithmeticExpr,
	uint8_t arithmeticTypeR, uint8_t arithmeticTypeRI,
	uint8_t arithmeticTypeIR, uint8_t arithmeticTypeI
) {
	switch (std::get<0>(arithmeticExpr.value1)) {
	case REG: {
		switch (std::get<0>(arithmeticExpr.value2)) {
		case REG: {
			program.push_back(arithmeticTypeR);
			program.push_back(StringToRegister(std::get<1>(arithmeticExpr.dest)));
			program.push_back(StringToRegister(std::get<1>(arithmeticExpr.value1)));
			program.push_back(StringToRegister(std::get<1>(arithmeticExpr.value2)));
		} break;
		case IMMEDIATE: {
			program.push_back(arithmeticTypeRI);
			program.push_back(StringToRegister(std::get<1>(arithmeticExpr.dest)));
			program.push_back(StringToRegister(std::get<1>(arithmeticExpr.value1)));
			PushU16(program, arithmeticExpr.value2);
		} break;
		}
	} break;
	case IMMEDIATE: {
		switch (std::get<0>(arithmeticExpr.value2)) {
		case REG: {
			program.push_back(arithmeticTypeIR);
			program.push_back(StringToRegister(std::get<1>(arithmeticExpr.dest)));
			PushU16(program, arithmeticExpr.value1);
			program.push_back(StringToRegister(std::get<1>(arithmeticExpr.value2)));
		} break;
		case IMMEDIATE: {
			program.push_back(arithmeticTypeI);
			program.push_back(StringToRegister(std::get<1>(arithmeticExpr.dest)));
			PushU16(program, arithmeticExpr.value2);
			PushU16(program, arithmeticExpr.value2);
		} break;
		}
	} break;
	}
}

void Assemble::AssembleExprs(std::string outPath) {
	std::vector<uint8_t> program;

    for (const auto &expr : m_Exprs) {
		if (expr.type() == typeid(OrgExpr)) {
			OrgExpr orgExpr = std::any_cast<OrgExpr>(expr);

			if (std::get<0>(orgExpr.value) == IMMEDIATE) {
				char *end;
				const char *sValue = std::get<1>(orgExpr.value).c_str();
				uint16_t value = (uint16_t)std::strtol(sValue, &end, 16);
				m_OriginAddr = value;
			} else {
				LOG_ERROR("invalid value for org!");
				exit(1);
			}
		} else if (expr.type() == typeid(MovExpr)) {
			MovExpr movExpr = std::any_cast<MovExpr>(expr);
            switch (std::get<0>(movExpr.dest)) {
			case REG: {
				switch (std::get<0>(movExpr.value)) {
				case IMMEDIATE: {
					program.push_back(MOV_IM);
					program.push_back(StringToRegister(std::get<1>(movExpr.dest)));
					PushU16(program, movExpr.value);
				} break;
				case REG: {
					program.push_back(MOV_R);
					program.push_back(StringToRegister(std::get<1>(movExpr.dest)));
					program.push_back(StringToRegister(std::get<1>(movExpr.value)));
				} break;
				case ADDR: {
					program.push_back(MOV_A);
					program.push_back(StringToRegister(std::get<1>(movExpr.dest)));
					PushType(program, movExpr.size);
					PushU16(program, movExpr.value);
				};
				}
			} break;
			}
		} else if (expr.type() == typeid(MovAddrExpr)) {
			MovAddrExpr movAddrExpr = std::any_cast<MovAddrExpr>(expr);
			switch (std::get<0>(movAddrExpr.dest)) {
			case ADDR: {
				switch (std::get<0>(movAddrExpr.value)) {
				case IMMEDIATE: {
					program.push_back(MOV_ADDR_IM);
					PushType(program, movAddrExpr.size);
					PushU16(program, movAddrExpr.dest);
					PushU16(program, movAddrExpr.value);
				} break;
				case REG: {
					program.push_back(MOV_ADDR_R);
					PushType(program, movAddrExpr.size);
					PushU16(program, movAddrExpr.dest);
					program.push_back(StringToRegister(std::get<1>(movAddrExpr.value)));
				} break;
				case ADDR: {
					program.push_back(MOV_ADDR_A);
					PushType(program, movAddrExpr.size);
					PushU16(program, movAddrExpr.dest);
					PushU16(program, movAddrExpr.value);
				};
				}
			} break;
			}
		} else if (expr.type() == typeid(ArithmeticExpr)) {
			ArithmeticExpr arithmeticExpr = std::any_cast<ArithmeticExpr>(expr);

			switch (arithmeticExpr.type) {
			case ADD: {
				Arithmetic(program, arithmeticExpr, ADD_R, ADD_RI, ADD_IR, ADD_I);
			} break;
			case ADC: {
				Arithmetic(program, arithmeticExpr, ADC_R, ADC_RI, ADC_IR, ADC_I);
			} break;
			case SUB: {
				Arithmetic(program, arithmeticExpr, SUB_R, SUB_RI, SUB_IR, SUB_I);
			} break;
			case SBC: {
				Arithmetic(program, arithmeticExpr, SBC_R, SBC_RI, SBC_IR, SBC_I);
			} break;
			case MUL: {
				Arithmetic(program, arithmeticExpr, MUL_R, MUL_RI, MUL_IR, MUL_I);
			} break;
			case DIV: {
				Arithmetic(program, arithmeticExpr, DIV_R, DIV_RI, DIV_IR, DIV_I);
			} break;
			}
		} else if (expr.type() == typeid(Halt)) {
			program.push_back(HLT);
        } else {
            LOG_ERROR("Error finding expr type!");
            exit(1);
        }
    }

	// Generate File
	std::ofstream file(outPath, std::ios::binary);
	if (!file) {
		LOG_ERROR("unable to open file for write!");
		exit(1);
	}

	file.write(reinterpret_cast<const char*>(program.data()), program.size());

	file.close();
}

}