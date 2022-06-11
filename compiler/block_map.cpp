#include <functional>

#include "compiler.h"
#include "scratch.h"
#include "util.h"

namespace clipcc {

#define PARAMS compiler *compiler, scratch_target *target, scratch_block *block
#define PARSE_INPUT(x) compiler->compile_input(target, block->inputs[x])
#define PARSE_SUBSTACK(x) compiler->compile_substack(target, target->blocks_map[block->inputs[x]->block_id]);

void compiler::init_block_map() {
    // in this function, we will initialize all the original scratch blocks for generation.
    // mention that all the command blocks should be set in block_map, and all the reporters
    // should be set in input_map.

    // motion
    this->block_map["motion_movesteps"] = [](PARAMS) -> void {
        compiler->code << "{ int steps = ";
        PARSE_INPUT("STEPS");
        compiler->code << "; double theta = clipcc::rad(90 - this->direction); "
            "this->x += std::cos(theta) * steps; "
            "this->y += std::sin(theta) * steps; request_redraw(); }";
    };
    this->block_map["motion_gotoxy"] = [](PARAMS) -> void {
        compiler->code << "this->x = ";
        PARSE_INPUT("X");
        compiler->code << "; this->y = ";
        PARSE_INPUT("Y");
        compiler->code << "; request_redraw();";
    };
    this->block_map["motion_goto"] = [](PARAMS) -> void {
        compiler->code << "{ auto target = runtime->getTarget(";
        PARSE_INPUT("TO");
        compiler->code << "); this->x = target.x; this->y = target.y; request_redraw(); }";
    };
    this->block_map["motion_turnright"] = [](PARAMS) -> void {
        compiler->code << "this->direction += ";
        PARSE_INPUT("DEGREES");
        compiler->code << "; request_redraw();";
    };
    this->block_map["motion_turnleft"] = [](PARAMS) -> void {
        compiler->code << "this->direction -= ";
        PARSE_INPUT("DEGREES");
        compiler->code << "; request_redraw();";
    };
    this->block_map["motion_pointindirection"] = [](PARAMS) -> void {
        compiler->code << "this->direction = ";
        PARSE_INPUT("DIRECTION");
        compiler->code << "; request_redraw();";
    };
    this->block_map["motion_pointtowards"] = [](PARAMS) -> void {
        compiler->code << "{ auto target = runtime->getTarget(";
        PARSE_INPUT("TOWARDS");
        compiler->code << "); this->direction = 90 - clipcc::deg(std::atan((target->y - this->y) / (target->x - this->x))); "
            "request_redraw(); }";
    };
    this->block_map["motion_glidesecstoxy"] = [](PARAMS) -> void {
        compiler->code << "{ int d = ";
        PARSE_INPUT("SECS");
        compiler->code << " * 1000; int end_x = ";
        PARSE_INPUT("X");
        compiler->code << ", end_y = ";
        PARSE_INPUT("Y");
        compiler->code <<"; "
            "if (d <= 0) { this->x = endx; this->y = endy; } else { "
            "auto start_time = std::chrono::steady_clock::now(); "
            "int start_x = this->x, start_y = this->y; "
            "auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(start_time - start_time); "
            "while (dur.count() < d) {"
            "double frac = static_cast<double>(dur.count()) / d; "
            "this->x = start_x + frac * (end_x - start_x); "
            "this->y = start_y + frac * (end_y - start_y); "
            "request_redraw(); "
            "co_await std::suspend_always{}; dur = std::chrono::steady_clock::now() - start_time; } "
            "this->x = end_x; this->y = end_y; request_redraw(); } }";
    };
    this->block_map["motion_glideto"] = [](PARAMS) -> void {
        compiler->code << "{ int d = ";
        PARSE_INPUT("SECS");
        compiler->code << " * 1000; auto target = runtime->getTarget(";
        PARSE_INPUT("TO");
        compiler->code << "); int end_x = target.x, end_y = targe.y; "
            "if (d <= 0) { this->x = end_x; this->y = end_y; } else { "
            "auto start_time = std::chrono::steady_clock::now(); "
            "int start_x = this->x, start_y = this->y; "
            "auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(start_time - start_time); "
            "while (dur.count() < d) {"
            "double frac = static_cast<double>(dur.count()) / d; "
            "this->x = start_x + frac * (end_x - start_x); "
            "this->y = start_y + frac * (end_y - start_y); "
            "request_redraw(); "
            "co_await std::suspend_always{}; dur = std::chrono::steady_clock::now() - start_time; } "
            "this->x = end_x; this->y = end_y; request_redraw(); } }";
    };/* @todo
    this->block_map["motion_ifonedgebounce"] = [](PARAMS) -> void {

    };*/
    this->block_map["motion_setrotationstyle"] = [](PARAMS) -> void {
        // this block's arg is an field, so we process it in compile time
        auto &style = block->inputs["STYLE"]->value;
        if (style == "left-right") {
            compiler->code << "this->rotation_style = clipcc::rotation::left_right;";
        }
        else if (style == "don\'t rotate") {
            compiler->code << "this->rotation_style = clipcc::rotation::no_rotation;";
        }
        else { // all around
            compiler->code << "this->rotation_style = clipcc::rotation::all_around;";
        }
    };
    this->block_map["motion_changexby"] = [](PARAMS) -> void {
        compiler->code << "this->x += ";
        PARSE_INPUT("DX");
        compiler->code << "; request_redraw();";
    };
    this->block_map["motion_changeyby"] = [](PARAMS) -> void {
        compiler->code << "this->y += ";
        PARSE_INPUT("DY");
        compiler->code << "; request_redraw();";
    };
    this->block_map["motion_setx"] = [](PARAMS) -> void {
        compiler->code << "this->x = ";
        PARSE_INPUT("X");
        compiler->code << "; request_redraw();";
    };
    this->block_map["motion_sety"] = [](PARAMS) -> void {
        compiler->code << "this->y = ";
        PARSE_INPUT("Y");
        compiler->code << "; request_redraw();";
    };
    this->input_map["motion_xposition"] = [](PARAMS) -> void {
        compiler->code << "this->x";
    };
    this->input_map["motion_yposition"] = [](PARAMS) -> void {
        compiler->code << "this->y";
    };
    this->input_map["motion_direction"] = [](PARAMS) -> void {
        compiler->code << "this->direction";
    };
    // lagacy no-op blocks:
    // motion_scroll_right, motion_scroll_up, motion_align_scene, motion_xscroll, motion_yscroll

    // looks
    this->block_map["looks_say"] = [](PARAMS) -> void {
        compiler->code << "say(";
        PARSE_INPUT("MESSAGE");
        compiler->code << ");";
    };

    // sounds

    // events

    // control
    this->block_map["control_repeat"] = [](PARAMS) -> void {
        compiler->code << "for (int i = ";
        PARSE_INPUT("TIMES");
        compiler->code << "; i < time; ++i) {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}";
    };
    this->block_map["control_repeat_until"] = [](PARAMS) -> void {
        compiler->code << "while (!(";
        PARSE_INPUT("CONDITION");
        compiler->code << ")) {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}";
    };
    this->block_map["control_while"] = [](PARAMS) -> void {
        compiler->code << "while (";
        PARSE_INPUT("CONDITION");
        compiler->code << ") {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "co_await std::suspend_always{};" ENDL "}";
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
        compiler->code << "co_await std::suspend_always{};" ENDL "}" ENDL "}";
    };
    this->block_map["control_forever"] = [](PARAMS) -> void {
        compiler->code << "while (true) {" ENDL;
        PARSE_SUBSTACK("SUBSTACK")
        compiler->code << "co_await std::suspend_always{};" ENDL "}";
    };
    this->block_map["control_wait"] = [](PARAMS) -> void {
        compiler->code << "{ int d = ";
        PARSE_INPUT("DURATION");
        compiler->code << "; "
            "auto start_time = std::chrono::steady_clock::now(); "
            "co_await std::suspend_always{}; "
            "while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < d) "
            "co_await std::suspend_always{}; }";
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
        compiler->code << "}";
    };
    this->block_map["control_if_else"] = [](PARAMS) -> void {
        compiler->code << "if (";
        PARSE_INPUT("CONDITION");
        compiler->code << ") {" ENDL;
        PARSE_SUBSTACK("SUBSTACK");
        compiler->code << "}" ENDL "else {" ENDL;
        PARSE_SUBSTACK("SUBSTACK2");
        compiler->code << "}";
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
        compiler->code << "++runtime->counter;";
    };
    this->block_map["control_clear_counter"] = [](PARAMS) -> void {
        compiler->code << "runtime->counter = 0;";
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
        compiler->code << ");";
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
    this->input_map["operator_subtract"] = [](PARAMS) -> void {
        compiler->code << "static_cast<double>(";
        PARSE_INPUT("NUM1");
        compiler->code << ") - static_cast<double>(";
        PARSE_INPUT("NUM2");
        compiler->code << ")";
    };
    this->input_map["operator_multiply"] = [](PARAMS) -> void {
        compiler->code << "static_cast<double>(";
        PARSE_INPUT("NUM1");
        compiler->code << ") * static_cast<double>(";
        PARSE_INPUT("NUM2");
        compiler->code << ")";
    };
    this->input_map["operator_divide"] = [](PARAMS) -> void {
        compiler->code << "static_cast<double>(";
        PARSE_INPUT("NUM1");
        compiler->code << ") / static_cast<double>(";
        PARSE_INPUT("NUM2");
        compiler->code << ")";
    };
    this->input_map["operator_lt"] = [](PARAMS) -> void {
        compiler->code << "(";
        PARSE_INPUT("OPERAND1");
        compiler->code << ") < (";
        PARSE_INPUT("OPERAND2");
        compiler->code << ")";
    };
    this->input_map["operator_equals"] = [](PARAMS) -> void {
        compiler->code << "(";
        PARSE_INPUT("OPERAND1");
        compiler->code << ") == (";
        PARSE_INPUT("OPERAND2");
        compiler->code << ")";
    };
    this->input_map["operator_gt"] = [](PARAMS) -> void {
        compiler->code << "(";
        PARSE_INPUT("OPERAND1");
        compiler->code << ") > (";
        PARSE_INPUT("OPERAND2");
        compiler->code << ")";
    };
    this->input_map["operator_and"] = [](PARAMS) -> void {
        compiler->code << "(";
        PARSE_INPUT("OPERAND1");
        compiler->code << ") && (";
        PARSE_INPUT("OPERAND2");
        compiler->code << ")";
    };
    this->input_map["operator_or"] = [](PARAMS) -> void {
        compiler->code << "(";
        PARSE_INPUT("OPERAND1");
        compiler->code << ") || (";
        PARSE_INPUT("OPERAND2");
        compiler->code << ")";
    };
    this->input_map["operator_not"] = [](PARAMS) -> void {
        compiler->code << "!(";
        PARSE_INPUT("OPERAND");
        compiler->code << ")";
    };
    this->input_map["operator_random"] = [](PARAMS) -> void {
        compiler->code << "clipcc::random((";
        PARSE_INPUT("FROM");
        compiler->code << "), (";
        PARSE_INPUT("TO");
        compiler->code << "))";
    };
    this->input_map["operator_join"] = [](PARAMS) -> void {
        compiler->code << "clipcc::join(";
        PARSE_INPUT("STRING1");
        compiler->code << ", ";
        PARSE_INPUT("STRING2");
        compiler->code << ")";
    };
    // operator_indexof
    this->input_map["operator_letter_of"] = [](PARAMS) -> void {
        compiler->code << "clipcc::letter_of(";
        PARSE_INPUT("STRING");
        compiler->code << ", ";
        PARSE_INPUT("LETTER");
        compiler->code << ")";
    };
    this->input_map["operator_length"] = [](PARAMS) -> void {
        compiler->code << "clipcc::length(";
        PARSE_INPUT("STRING");
        compiler->code << ")";
    };
    this->input_map["operator_contains"] = [](PARAMS) -> void {
        compiler->code << "clipcc::contains(";
        PARSE_INPUT("STRING1");
        compiler->code << ", ";
        PARSE_INPUT("STRING2");
        compiler->code << ")";
    };
    this->input_map["operator_mod"] = [](PARAMS) -> void {
        compiler->code << "clipcc::mod(";
        PARSE_INPUT("NUM1");
        compiler->code << ", ";
        PARSE_INPUT("NUM2");
        compiler->code << ")";
    };
    this->input_map["operator_round"] = [](PARAMS) -> void {
        compiler->code << "std::round(";
        PARSE_INPUT("NUM");
        compiler->code << ")";
    };
    this->input_map["operator_mathop"] = [](PARAMS) -> void {
        auto &op = block->inputs["OPERATOR"]->value;
        if (op == "abs") {
            compiler->code << "std::abs";
        }
        else if (op == "floor") {
            compiler->code << "std::floor";
        }
        else if (op == "ceiling") {
            compiler->code << "std::ceil";
        }
        else if (op == "sqrt") {
            compiler->code << "std::sqrt";
        }
        else if (op == "sin") {
            compiler->code << "std::sin";
        }
        else if (op == "cos") {
            compiler->code << "std::cos";
        }
        else if (op == "tan") {
            compiler->code << "std::tan";
        }
        else if (op == "asin") {
            compiler->code << "std::asin";
        }
        else if (op == "acos") {
            compiler->code << "std::acos";
        }
        else if (op == "atan") {
            compiler->code << "std::atan";
        }
        else if (op == "ln") {
            compiler->code << "std::ln";
        }
        else if (op == "log") {
            compiler->code << "std::log";
        }
        else if (op == "e ^") {
            compiler->code << "std::exp";
        }
        
        if (op == "10 ^") {
            compiler->code << "std::pow(10, ";
            PARSE_INPUT("NUM");
            compiler->code << ")";
        }
        else {
            compiler->code << "(";
            PARSE_INPUT("NUM");
            compiler->code << ")";
        }
    };

    // data
    this->block_map["data_setvariableto"] = [](PARAMS) -> void {
        block->inputs["VARIABLE"]->type = value_type::var; // it's parsed from field
        PARSE_INPUT("VARIABLE");
        compiler->code << " = ";
        PARSE_INPUT("VALUE");
        compiler->code << ";";
    };

    // procedures
    this->block_map["procedures_call"] = [](PARAMS) -> void {
        auto &proccode = block->mutation->data["proccode"];
        for (auto &c : compiler->procedure_prototypes) {
            if (c.proccode == proccode) {
                compiler->code << "co_yield func_" << std::to_string(c.id) << "(";
                for (auto i = c.arg_name_id.size(); auto &arg : c.arg_name_id) {
                    PARSE_INPUT(arg.second);
                    if (--i > 0) compiler->code << ", ";
                }
                compiler->code << ");";
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
