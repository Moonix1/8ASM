#pragma once
#include <vector>
#include <memory>
#include <any>

#include "Parser.h"

namespace EASM {

class Assemble {
public:
    Assemble(std::vector<std::any> exprs)
        : m_Exprs(exprs) {}

    void AssembleExprs(std::string outFile);
private:
	uint16_t GetLabelAddr(std::string name);
private:
	struct Label {
		std::string name;
		uint16_t addr;
	};

	struct DeferredOperand {
		std::string name;
		uint16_t addr;
	};
private:
    std::vector<std::any> m_Exprs;
	std::vector<Label> m_Labels;
	std::vector<DeferredOperand> m_DeferredOperands;

	uint16_t m_OriginAddr;
};

}