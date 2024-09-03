#include "gen.hpp"

size_t copy_template_file(const std::string& templateFilePath, const std::string& newFilePath) 
{
    std::ifstream templateFile(templateFilePath);  
    if (!templateFile.is_open()) {
        std::cerr << "Error opening template file: " << templateFilePath << std::endl;
        return 1;
    }
    std::string fileContents((std::istreambuf_iterator<char>(templateFile)), std::istreambuf_iterator<char>());
    templateFile.close();  
    std::ofstream newFile(newFilePath);  
    if (!newFile.is_open()) {
        std::cerr << "Error creating new file: " << newFilePath << std::endl;
        return 1;
    }
    newFile << fileContents;  
    newFile.close();  
    return 0;
}

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
        std::string full = "full";
        std::string build = "build";
        std::string simple = "simple";
        std::string tiny = "tiny";
        std::string test = "test";
        std::string check = to_lower(argv[2]);
        if (check == full) {
            std::cout << "Generating Full_build in blmake.lua..." << std::endl;
            size_t err = generate_full_build();
            if (err) exit(1);
        }
        else if (check == build) {
            std::cout << "Generating Build in blmake.lua..." << std::endl;
            size_t err = generate_build();
            if (err) exit(1);
        }
        else if (check == simple) {
            std::cout << "Generating Simple_build in blmake.lua..." << std::endl;
            size_t err = generate_simple_build();
            if (err) exit(1);
        }
        else if (check == tiny) {
            std::cout << "Generating Tiny_build in blmake.lua..." << std::endl;
            size_t err = generate_tiny_build();
            if (err) exit(1);
        }
        else if (check == test) {
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
