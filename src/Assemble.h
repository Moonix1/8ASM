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
    std::vector<std::any> m_Exprs;

	uint16_t m_OriginAddr;
};

}