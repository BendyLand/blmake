import os
import sys


def main():
    if len(sys.argv) > 1:
        arg = sys.argv[1]
        match arg:
            case "blmake":
                os.system(
                    """
				cd src/templates/blmake
				xxd -i full_build_template.txt > full_build_template.h
				xxd -i build_template.txt > build_template.h
				xxd -i simple_build_template.txt > simple_build_template.h
				xxd -i tiny_build_template.txt > tiny_build_template.h
				xxd -i test_build_template.txt > test_build_template.h
				"""
                )
            case "cmake":
                os.system(
                    """
				cd src/templates/cmake
				xxd -i full_build_cmake.txt > full_build_cmake.h
				xxd -i simple_build_cmake.txt > simple_build_cmake.h
				xxd -i tiny_build_cmake.txt > tiny_build_cmake.h
				"""
                )
            case "premake":
                os.system(
                    """
				cd src/templates/premake
				xxd -i full_build_premake.txt > full_build_premake.h
				xxd -i build_premake.txt > build_premake.h
				xxd -i simple_build_premake.txt > simple_build_premake.h
				xxd -i tiny_build_premake.txt > tiny_build_premake.h
				xxd -i test_build_premake.txt > test_build_premake.h
				"""
                )
            case _:
                print("Invalid arg. Please use blmake, cmake, or premake.")
    else:
        print("Usage: python3 regen_templates.py blmake|cmake|premake")


if __name__ == "__main__":
    main()
