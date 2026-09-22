#include <iostream>

#include "../../SysBuild.h"
#include <filesystem>

int main() {
    auto ci = CompilerInf::FromToolchain("C:/msys64/mingw64/bin");
    SysBuild sb(&ci, 3);

    BuildTarget bt;
    bt.name = "test";
    bt.output_type = BuildOutputType::Executable;
    bt.output_name = "test.exe";
    bt.output_dir = "assets/build";
    bt.obj_dir = "assets/obj";
    bt.sources.push_back("assets/main.cpp");
    bt.sources.push_back("assets/test.cpp");
    sb.AddTarget(bt);

    const auto result = sb.BuildAll();
    if (result.err) {
        std::cerr << "Build failed: " << result.err << "\n";
        std::cerr << sb.GetLastError() << "\n";
        return 1;
    }
    return 0;
}