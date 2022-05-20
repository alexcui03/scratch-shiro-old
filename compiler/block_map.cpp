#include <functional>

#include "compiler.h"
#include "scratch.h"
#include "util.h"

namespace clipcc {

#define PARAMS compiler *compiler, scratch_target *target, scratch_block *block
#define PARSE_INPUT(x) compiler->compile_input(target, block->inputs[x])
#define PARSE_SUBSTACK(x) compiler->compile_substack(target, target->blocks_map[block->inputs[x]->value]);

void compiler::init_block_map() {
    // in this function, we will initialize all the original scratch blocks for generation.
    // mention that all the command blocks should be set in block_map, and all the reporters
    // should be set in input_map.

    // motion
    this->block_map["motion_movesteps"] = [](PARAMS) -> void {
        compiler->code << "move_steps(";
        PARSE_INPUT("STEPS");
        compiler->code << ");" ENDL;
    };
    this->block_map["motion_gotoxy"] = [](PARAMS) -> void {
        compiler->code << "this->x = (";
        PARSE_INPUT("X");
        compiler->code << ");" ENDL "this->y = (";
        PARSE_INPUT("Y");
        compiler->code << ");" ENDL;
    };
    this->block_map["motion_goto"] = [](PARAMS) -> void {
        compiler->code << "{auto target = getTarget(";
        PARSE_INPUT("TO");
        compiler->code << ");" ENDL "this->x = target.x; this->y = target.y;}";
    };

    // looks
    this->block_map["looks_say"] = [](PARAMS) -> void {
        compiler->code << "say(";
        PARSE_INPUT("MESSAGE");
        compiler->code << ");" ENDL;
    };

    // sounds

    // events

    // control
    this->block_map["control_repeat"] = [](PARAMS) -> void {
        compiler->code << "for (int i = ";
        PARSE_INPUT("TIMES");
        compiler->code << "; i < time; ++i) {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}" ENDL;
    };
    this->block_map["control_repeat_until"] = [](PARAMS) -> void {
        compiler->code << "while (!(";
        PARSE_INPUT("CONDITION");
        compiler->code << ")) {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}" ENDL;
    };
    this->block_map["control_while"] = [](PARAMS) -> void {
        compiler->code << "while (";
        PARSE_INPUT("CONDITION");
        compiler->code << ") {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}" ENDL;
    };
    this->block_map["control_for_each"] = [](PARAMS) -> void {
        block->inputs["VARIABLE"]->type = value_type::var; // it's parsed from field
        compiler->code << "{" ENDL;
        compiler->code << "int value = ";
        PARSE_INPUT("VALUE");
        compiler->code << ";" ENDL "for (int i = 0; i < value; ++i) {" ENDL;
        PARSE_INPUT("VARIABLE");
        compiler->code << "= i;" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}" ENDL "}" ENDL;
    };
    this->block_map["control_forever"] = [](PARAMS) -> void {
        compiler->code << "while (true) {" ENDL;
        PARSE_SUBSTACK("SUBSTACK")
        compiler->code << "}" ENDL;
    };
    this->block_map["control_wait"] = [](PARAMS) -> void {
        compiler->code << "{ int d = ";
        PARSE_INPUT("DURATION");
        compiler->code << "; "
            "auto start_time = std::chrono::steady_clock::now(); "
            "auto end_time = start_time; "
            "while (std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() < d) "
            "co_await std::suspend_alwasy{}; }" ENDL;
    };
    this->block_map["control_wait_until"] = [](PARAMS) -> void {
        compiler->code << "while (";
        PARSE_INPUT("CONDITION");
        compiler->code << ") co_await std::suspend_always{};";
    };
    this->block_map["control_if"] = [](PARAMS) -> void {
        compiler->code << "if (";
        PARSE_INPUT("CONDITION");
        compiler->code << ") {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}" ENDL;
    };
    this->block_map["control_if_else"] = [](PARAMS) -> void {
        compiler->code << "if (";
        PARSE_INPUT("CONDITION");
        compiler->code << ") {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}" ENDL "else {" ENDL;
        PARSE_SUBSTACK("SUBSTACK2");
        compiler->code << "}" ENDL;
    };
    this->block_map["control_stop"] = [](PARAMS) -> void {
        auto &opt = block->inputs["STOP_OPTION"]->value;
        if (opt == "all") {
            compiler->code << "runtime.request_stop();" ENDL "co_return;" ENDL;
        }
        else if (opt == "other scripts in sprite") {
            compiler->code << "runtime.request_stop(this);" ENDL;
        }
        else if (opt == "this script") {
            compiler->code << "co_return;";
        }
    };
    // @todo support clone
    /*this->block_map["control_create_clone_o"] = [](PARAMS) -> void {

    };
    this->block_map["control_delete_this_clone"] = [](PARAMS) -> void {

    };*/
    this->input_map["control_get_counter"] = [](PARAMS) -> void {
        compiler->code << "runtime->counter";
    };
    this->block_map["control_incr_counter"] = [](PARAMS) -> void {
        compiler->code << "++runtime->counter;" ENDL;
    };
    this->block_map["control_clear_counter"] = [](PARAMS) -> void {
        compiler->code << "runtime->counter = 0;" ENDL;
    };
    this->block_map["control_all_at_once"] = [](PARAMS) -> void {
        // Since the "all at once" block is implemented for compatiblity with
        // Scratch 2.0 projects, it behaves the same way it did in 2.0, which
        // is to simply run the contained script (like "if 1 = 1").
        // (In early versions of Scratch 2.0, it would work the same way as
        // "run without screen refresh" custom blocks do now, but this was
        // removed before the release of 2.0.)
    };

    // sensing
    this->block_map["sensing_askandwait"] = [](PARAMS) -> void {
        compiler->code << "co_yield runtime->ask_and_wait(";
        PARSE_INPUT("QUESTION");
        compiler->code << ");" ENDL;
    };
    this->input_map["sensing_answer"] = [](PARAMS) -> void {
        compiler->code << "runtime->answer";
    };

    // operators
    this->input_map["operator_add"] = [](PARAMS) -> void {
        compiler->code << "static_cast<double>(";
        PARSE_INPUT("NUM1");
        compiler->code << ") + static_cast<double>(";
        PARSE_INPUT("NUM2");
        compiler->code << ")";
    };
    this->input_map["operator_random"] = [](PARAMS) -> void {
        compiler->code << "ccvm::random((";
        PARSE_INPUT("FROM");
        compiler->code << "), (";
        PARSE_INPUT("TO");
        compiler->code << "))";
    };

    // data
    this->block_map["data_setvariableto"] = [](PARAMS) -> void {
        block->inputs["VARIABLE"]->type = value_type::var; // it's parsed from field
        PARSE_INPUT("VARIABLE");
        compiler->code << " = ";
        PARSE_INPUT("VALUE");
        compiler->code << ";" ENDL;
    };

    // procedures
    this->block_map["procedures_call"] = [](PARAMS) -> void {
        auto &proccode = block->mutation->data["proccode"];
        for (auto &c : compiler->procedure_prototypes) {
            if (c.proccode == proccode) {
                compiler->code << "co_yield func_" << std::to_string(c.id) << "(";
                for (auto &arg : c.arg_name_id) PARSE_INPUT(arg.second);
                compiler->code << ");" ENDL;
                return;
            }
        }
    };
    this->input_map["argument_reporter_string_number"] = [](PARAMS) -> void {
        auto &arg_name = block->inputs["VALUE"]->value;
        
        // note that the current compiling procedure must be the last one in procedure_prototypes
        auto &arg_list = compiler->procedure_prototypes.rbegin()->arg_name_id;
        for (int i = 0; auto &c : arg_list) {
            if (c.first == arg_name) {
                compiler->code << "arg_" << std::to_string(i);
                return;
            }
            ++i;
        }
    };
    this->input_map["argument_reporter_boolean"] = [](PARAMS) -> void {
        auto &arg_name = block->inputs["VALUE"]->value;
        
        // note that the current compiling procedure must be the last one in procedure_prototypes
        auto &arg_list = compiler->procedure_prototypes.rbegin()->arg_name_id;
        for (int i = 0; auto &c : arg_list) {
            if (c.first == arg_name) {
                compiler->code << "static_cast<bool>(arg_" << std::to_string(i) << ")";
                return;
            }
            ++i;
        }
    };
}

}
