#ifndef _COMPILER_H_
#define _COMPILER_H_

#define CLIPCC_COMPILER_VERSION 20220511ll

#include <string>
#include <map>
#include <unordered_map>
#include <functional>

#include "scratch.h"

namespace clipcc {
    class compiler {
    public:
        compiler();
        ~compiler();
        std::string compile_project(const std::string &src);

        scratch_project *project;
        std::string code;
        std::string main_code;
        std::unordered_map<scratch_target *, std::unordered_map<std::string, int>> var_map;
        std::vector<scratch_procedure> procedure_prototypes;
        std::vector<std::string> target_name;

        std::unordered_map<std::string, std::function<void(
            compiler *, scratch_target *, scratch_block *)>> block_map;
        std::unordered_map<std::string, std::function<void(
            compiler *, scratch_target *, scratch_block *)>> input_map;

        void init_block_map(); // in block_map.cpp
        void compile();
        void compile_target(scratch_target *target, int i);
        void compile_substack(scratch_target *target, scratch_block *substack);
        void compile_input(scratch_target *target, scratch_input *input);
    };
}

#endif // !_COMPILER_H_
