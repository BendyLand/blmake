#pragma once

#include <fstream>
#include <string>
#include "utils.hpp"

size_t copy_template_file(const std::string& from, const std::string& to);
size_t generate_full_build();
size_t generate_build();
size_t generate_simple_build();
size_t generate_tiny_build();
size_t generate_test_build();
size_t handle_template_generation(int argc, char** argv);
