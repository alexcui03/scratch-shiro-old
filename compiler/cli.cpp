#include <iostream>
#include <fstream>
#include <streambuf>
#include <argparse.h>

#include "compiler.h"

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
    std::ifstream fin(project_file);
    if (!fin.is_open()) {
        std::cerr << "error: cannot open project file." << std::endl;
        return -1;
    }
    std::string src((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    fin.close();

    clipcc::compiler compiler;
    std::string code = compiler.compile_project(src);

    std::cout << code << std::endl;

    std::ofstream fout("output.cpp");
    if (!fout.is_open()) {
        std::cerr << "error: cannot open output cpp source." << std::endl;
        return -1;
    }
    fout << code;
    fout.flush();
    fout.close();

    return 0;
}

