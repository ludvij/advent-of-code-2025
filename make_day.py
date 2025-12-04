import os
from sys import argv

cmake_format = """
cmake_minimum_required(VERSION 3.31)

project({year}_{day:02})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${{PROJECT_NAME}}_01)
add_executable(${{PROJECT_NAME}}_02)

target_sources(${{PROJECT_NAME}}_01 PRIVATE src/part1.cpp)
target_sources(${{PROJECT_NAME}}_02 PRIVATE src/part2.cpp)

target_link_libraries(${{PROJECT_NAME}}_01 PRIVATE ludutils)
target_link_libraries(${{PROJECT_NAME}}_02 PRIVATE ludutils)
"""

main_cpp_format = """
// https://adventofcode.com/{year}/day/{day}
// Advent of code : {year}
// day            : {day: 2}
// part           : {part}


#include <print>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>

#define INPUT_PATH "inputs/day{day}/"

auto do_program(auto path)
{{
    return 0U;
}}

int main(int argc, char** argv)
{{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 1U);

	auto result = do_program(INPUT_PATH "input.txt");

	std::println("[RESULT]: {{}}", result);
    return 0;
}}
"""

def main():
	if len(argv) < 2:
		print(f'Usage: {argv[0]} number')
		return
	
	day = int(argv[1])
	path = f'day{argv[1]}'

	os.makedirs(f'{path}/src', exist_ok=True)
	os.makedirs(f'inputs/day{day}', exist_ok=True)

	with open(f'{path}/CMakeLists.txt', 'w', encoding='utf-8') as f:
		f.write(cmake_format.format(year=2025, day=day))

	with open(f'{path}/src/part1.cpp', 'w', encoding='utf-8') as f:
		f.write(main_cpp_format.format(year=2025, day=day, part=1))
	
	with open(f'{path}/src/part2.cpp', 'w', encoding='utf-8') as f:
		f.write(main_cpp_format.format(year=2025, day=day, part=2))

	with open('CMakeLists.txt', 'a', encoding='utf-8') as f:
		f.write(f"""
add_subdirectory(day{day})

set_target_properties(2025_{day:02}_01 PROPERTIES FOLDER "DAY{day:02}")
set_target_properties(2025_{day:02}_02 PROPERTIES FOLDER "DAY{day:02}")
""")


if __name__ == '__main__':
	main()