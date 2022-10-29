#ifndef _SCRATCH_H_
#define _SCRATCH_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <utility>

namespace shiro {
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

    class scratch_mutation {
    public:
        std::unordered_map<std::string, std::string> data; 
    };

    class scratch_block {
    public:
        std::string id;
        std::string opcode;
        bool top_level;
        std::string next;
        std::string parent;
        std::unordered_map<std::string, scratch_input *> inputs;
        scratch_mutation *mutation = nullptr;
    };

    class scratch_costume {
    public:
        std::string asset_id;
        std::string name;
        // bitmapresolution ?
        std::string md5ext;
        std::string data_format;
        std::string rotation_center_x;
        std::string rotation_center_y;
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
        scratch_target *stage;
    };

    class scratch_procedure {
    public:
        int id;
        std::string proccode;
        bool is_warp;
        std::vector<std::pair<std::string, std::string>> arg_name_id;
    };
}

#endif // !_SCRATCH_H_
