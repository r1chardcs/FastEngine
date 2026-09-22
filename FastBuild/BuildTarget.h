#ifndef FASTENGINE_BUILDTARGET_H
#define FASTENGINE_BUILDTARGET_H

#include <Platform.h>

enum class BuildOutputType {
    Executable,
    StaticLibrary,
    SharedLibrary
};

struct BuildTarget {
    STRING name;

    VECTOR<STRING> sources;
    VECTOR<STRING> include_dirs;
    VECTOR<STRING> defines;
    VECTOR<STRING> library_dirs;
    VECTOR<STRING> libraries;
    VECTOR<STRING> extra_compile_flags;
    VECTOR<STRING> extra_link_flags;

    BuildOutputType output_type = BuildOutputType::Executable;
    STRING output_name;
    STRING output_dir = "build/bin";
    STRING obj_dir = "build/obj";

    BOOL optimize = true;
    BOOL debug_symbols = false;
    STRING cpp_standard = "c++20";
};

STRING GetExecutablePath();

#endif //FASTENGINE_BUILDTARGET_H