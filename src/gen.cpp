#include "gen.hpp"

size_t generate_full_build()
{
    int err = copy_template_file("src/templates/full_build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t generate_build()
{
    int err = copy_template_file("src/templates/build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t generate_simple_build()
{
    int err = copy_template_file("src/templates/simple_build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t generate_tiny_build()
{
    int err = copy_template_file("src/templates/tiny_build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t generate_test_build()
{
    int err = copy_template_file("src/templates/test_build_template.txt", "./blmake.lua");
    if (err) {
        std::cerr << "Error generating config file." << std::endl;
        return 1;
    }
    std::cout << "Config file generated successfully!" << std::endl;
    return 0;
}

size_t handle_template_generation(int argc, char** argv)
{
    if (argc > 2) {
        std::string check = to_lower(argv[2]);
        if (check == std::string("full")) {
            std::cout << "Generating Full_build in blmake.lua..." << std::endl;
            size_t err = generate_full_build();
            if (err) exit(1);
        }
        else if (check == std::string("build")) {
            std::cout << "Generating Build in blmake.lua..." << std::endl;
            size_t err = generate_build();
            if (err) exit(1);
        }
        else if (check == std::string("simple")) {
            std::cout << "Generating Simple_build in blmake.lua..." << std::endl;
            size_t err = generate_simple_build();
            if (err) exit(1);
        }
        else if (check == std::string("tiny")) {
            std::cout << "Generating Tiny_build in blmake.lua..." << std::endl;
            size_t err = generate_tiny_build();
            if (err) exit(1);
        }
        else if (check == std::string("test")) {
            std::cout << "Generating Test_build in blmake.lua..." << std::endl;
            size_t err = generate_test_build();
            if (err) exit(1);
        }
        else {
            std::cerr << "Unknown template type." << std::endl;
            return 1;
        }
    }
    else {
        std::cout << "Generating Full_build in blmake.lua..." << std::endl;
        size_t err = generate_full_build();
        if (err) exit(1);
    }
    return 0;
}
