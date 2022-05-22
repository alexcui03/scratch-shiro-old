#include <iostream>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <cerrno>
#include <cstring>
#include <argparse.h>
#include <zip.h>

#include "compiler.h"

void process_zip_error(int err) {
    char str[1024];
    zip_error_to_str(str, 1024, err, errno);
    std::cout << "error: " << str << std::endl;
}

void process_zip_error(zip_t *zip) {
    zip_error_t *err = zip_get_error(zip);
    std::cerr << "error: " << err->str << std::endl;
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser parser("clipcc-compiler", "");
    parser.add_argument("-s", "--src", "project file path", true);
    parser.add_argument("-t", "--temp", "temp directory path", false);
    parser.enable_help();

    auto err = parser.parse(argc, const_cast<const char**>(argv));
    if (err) {
        std::cerr << err << std::endl;
    }
    if (!parser.exists("src")) {
        parser.print_help();
        return 0;
    }

    std::string project_file = parser.get<std::string>("src");
    std::filesystem::path temp_dir = parser.exists("temp") ? parser.get<std::string>("temp") : project_file + "_source";
    
    // clear the dir if the temp dir already exists, otherwise create the dir
    if (std::filesystem::exists(temp_dir)) {
        if (!std::filesystem::is_directory(temp_dir)) {
            std::cerr << "error: " << temp_dir << " is not a valid directory." << std::endl;
            return -1;
        }
        std::filesystem::remove_all(temp_dir);
    }
    std::filesystem::create_directories(temp_dir);
    // @todo process error code

    int error = 0;
    zip_t *zip = zip_open(project_file.c_str(), ZIP_RDONLY, &error);
    if (!zip) return process_zip_error(error), -1;

    zip_int64_t entries = zip_get_num_entries(zip, ZIP_FL_UNCHANGED);
    for (zip_int64_t i = 0; i < entries; ++i) {
        const char *name = zip_get_name(zip, i, ZIP_FL_ENC_GUESS);
        if (!name) return process_zip_error(zip), -1;
        zip_file_t *file = zip_fopen(zip, name, ZIP_FL_UNCHANGED);
        if (!file) return process_zip_error(zip), -1;

        if (name[strlen(name) - 1] == '/') {
            if (!std::filesystem::exists(temp_dir / name)) {
                std::filesystem::create_directories(temp_dir / name);
            }
            continue;
        }

        std::ofstream fout(temp_dir / name);
        if (!fout.is_open()) {
            std::cerr << "error: cannot create file " << (temp_dir / name) << " in temp dir." << std::endl;
            continue;
        }

        constexpr int BUFFSIZE = 8192;
        zip_int64_t n = 0;
        char buf[BUFFSIZE + 1];
        while ((n = zip_fread(file, buf, BUFFSIZE)) > 0) {
            fout.write(buf, n);
        }

        zip_fclose(file);
        fout.close();
    }

    if (zip_close(zip)) return process_zip_error(zip), -1;

    // start compile
    std::ifstream fin(temp_dir / "project.json");
    if (!fin.is_open()) {
        std::cerr << "error: cannot open project.json." << std::endl;
        return -1;
    }
    std::string src((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    fin.close();

    clipcc::compiler compiler;
    std::string code = compiler.compile_project(src);

    std::ofstream fout(temp_dir / "output.cpp");
    if (!fout.is_open()) {
        std::cerr << "error: cannot open output cpp source." << std::endl;
        return -1;
    }
    fout << code;
    fout.close();

    return 0;
}

