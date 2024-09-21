#pragma once

#include "utils.hpp"

size_t generate_full_build();
size_t generate_build();
size_t generate_simple_build();
size_t generate_tiny_build();
size_t generate_test_build();
size_t handle_template_generation(int argc, char** argv);
int handle_cl_args(int argc, char** argv);
