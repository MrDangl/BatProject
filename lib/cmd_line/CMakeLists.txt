add_library(
    cmd_line STATIC
    src/cmd_line.hpp
    src/cmd_line.cpp
)
set_property(TARGET cmd_line PROPERTY CXX_STANDARD 17)
target_compile_features(cmd_line  PUBLIC cxx_std_17)