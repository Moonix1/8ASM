#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Tokenizer.h"
#include "Parser.h"
#include "Assemble.h"
#include "Log/Log.h"

static std::string GetFileContents(std::string filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}

void Usage(char *program) {
    LOG_INFO("usage: {0} <SUBCOMMAND> <[ARGS]>", program);
    std::cout << "[SUBCOMMANDS]:" << std::endl;
    std::cout << "   - build <IN.ASM> <OUT.BIN>\n" << std::endl;
    std::cout << "   - help" << std::endl;
}

static char *Shift(int &argc, char ***argv) {
    char *last = **argv;
    *argv += 1;
    argc -= 1;
    return last;
}

int main(int argc, char **argv) {
    EASM::Log::Init();

    char *program = Shift(argc, &argv);
    if (argc < 1) {
        Usage(program);
        LOG_ERROR("invalid arguments!");
        exit(1);
    }

    char *subcommand = Shift(argc, &argv);
    if (std::string(subcommand) == "build") {
        if (argc < 2) {
            Usage(program);
            LOG_ERROR("invalid subargs!");
            exit(1);
        }

        char *input = Shift(argc, &argv);
        char *output = Shift(argc, &argv);

        EASM::Tokenizer tokenizer;
        tokenizer.TokenizeFile(input);
        EASM::Parser parser(tokenizer.GetTokens());
        parser.ParseExpr();
        EASM::Assemble assemble(parser.GetExprs());
        assemble.AssembleExprs(output);
    } else if (std::string(subcommand) == "help") {
        Usage(program);
    } else {
        Usage(program);
        LOG_ERROR("invalid subcommand `{0}`", subcommand);
        exit(1);
    }

    return 0;
}