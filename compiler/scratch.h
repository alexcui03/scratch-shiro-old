#ifndef _SCRATCH_H_
#define _SCRATCH_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

namespace clipcc {
    enum value_type {
        integer, real_number, string, var, any
    };

    class scratch_input {
    public:
        std::string name;
        std::string block_id;
        std::string shadow;
        value_type type;
        std::string value;
    };

    class scratch_block {
    public:
        std::string id;
        std::string opcode;
        bool top_level;
        std::string next;
        std::string parent;
        std::unordered_map<std::string, scratch_input *> inputs;
    };

    class scratch_costume {

    };

    class scratch_sound {

    };

    class scratch_variable {
    public:
        bool is_cloud;
        std::string id;
        std::string name;
        value_type type;
        std::variant<int, double, std::string> value;
        std::string original;
    };

    class scratch_list {
    public:
        bool is_cloud;
        std::string id;
        std::string name;
        value_type type;
        std::vector<std::string> value;
    };

    class scratch_broadcast {

    };

    class scratch_comment {

    };

    class scratch_target {
    public:
        std::string name;
        int x;
        int y;
        int direction;
        int size;
        bool visible;
        int current_costume;
        std::string rotation_style;
        bool is_stage;
        bool draggable;
        int tempo;
        int volume;
        int video_transparency;
        std::string video_state;
        std::string text_to_speech_language;

        std::vector<scratch_variable *> variables;
        std::vector<scratch_list *> lists;
        std::vector<scratch_broadcast *> broadcasts;
        std::vector<scratch_comment *> comments;
        std::vector<scratch_costume *> costumes;
        std::vector<scratch_sound *> sounds;
        std::vector<scratch_block *> blocks;
        std::unordered_map<std::string, scratch_block *> blocks_map;
    };

    class scratch_project {
    public:
        std::vector<scratch_target *> targets;
    };
}

#endif // !_SCRATCH_H_
