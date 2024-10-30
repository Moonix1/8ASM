#include "Parser.h"
#include "Log/Log.h"

#include <tuple>

namespace EASM {

void Parser::ParseExpr() {
    for (size_t pos = 0 ; pos < m_Tokens.size() ; ) {
        Token tok = FetchToken(pos);

        switch (tok.type) {
		case ORG: {
			Token addr = FetchToken(pos);
			m_Exprs.push_back(OrgExpr { .value = std::make_tuple(addr.type, addr.value) });
		} break;
		case LABEL: {

		} break;
        case MOV: {
			Token destOrType = FetchToken(pos);
			switch (destOrType.type) {
			case REG: {
				Token valueOrType = FetchTokenC(pos);

				switch (valueOrType.type) {
				case IMMEDIATE:
				case REG: {
					m_Exprs.push_back(MovExpr {
						.dest = std::make_tuple(destOrType.type, destOrType.value),
						.isAddr = false,
						.size = UNKNOWN,
						.value = std::make_tuple(valueOrType.type, valueOrType.value)
					});
				} break;
				
				case WORD:
				case BYTE: {
					Token addr = FetchToken(pos);
					m_Exprs.push_back(MovExpr {
						.dest = std::make_tuple(destOrType.type, destOrType.value),
						.isAddr = true,
						.size = valueOrType.type,
						.value = std::make_tuple(addr.type, addr.value)
					});
				} break;
				}
				
			} break;
			case WORD:
			case BYTE: {
				Token addr = FetchToken(pos);
				Token value = FetchTokenC(pos);
				m_Exprs.push_back(MovAddrExpr {
					.size = destOrType.type,
					.dest = std::make_tuple(addr.type, addr.value),
					.isAddr = (value.type == ADDR) ? true : false,
					.value = std::make_tuple(value.type, value.value)
				});
			} break;
			}
        } break;
		case ADD:
		case ADC:
		case SUB:
		case SBC:
		case MUL:
		case DIV: {
			Token dest = FetchToken(pos);
			Token value1 = FetchTokenC(pos);
			Token value2 = FetchTokenC(pos);

			m_Exprs.push_back(ArithmeticExpr {
				.type = tok.type,
				.dest = std::make_tuple(dest.type, dest.value),
				.value1 = std::make_tuple(value1.type, value1.value),
				.value2 = std::make_tuple(value2.type, value2.value),
			});
		} break;
		case HALT: {
            m_Exprs.push_back(Halt {});
		} break;
        default: {
            LOG_ERROR("Invalid token: {0}", tok.value);
            exit(1);
        } break;
        }
    }
}

Token Parser::FetchToken(size_t &pos) {
    if (pos >= m_Tokens.size()) {
        LOG_ERROR("attempted to fetch token out of bounds at position: {0}", pos);
        exit(1);
    }
    return m_Tokens[pos++];
}

Token Parser::FetchTokenC(size_t &pos) {
   	if (pos >= m_Tokens.size() || m_Tokens[pos].type != COMMA) {
        LOG_ERROR("expected a comma but found: {0} at position {1}", m_Tokens[pos].value, pos);
        exit(1);
    }
    pos++;
    
    if (pos >= m_Tokens.size()) {
        LOG_ERROR("Attempted to fetch token out of bounds after comma at position: {0}", pos);
        exit(1);
    }
    return m_Tokens[pos++];
}

}