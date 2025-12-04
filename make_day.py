import os
from sys import argv
import subprocess

cmake_format = """
cmake_minimum_required(VERSION 3.31)

project({year}_{day:02})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${{PROJECT_NAME}}_01 src/part1.cpp)
add_executable(${{PROJECT_NAME}}_02 src/part2.cpp)

target_link_libraries(${{PROJECT_NAME}}_01 PRIVATE ludutils)
target_link_libraries(${{PROJECT_NAME}}_02 PRIVATE ludutils)
"""

main_cpp_format = """
// https://adventofcode.com/{year}/day/{day}
// Advent of code : {year}
// day            : {day}
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

cmake_super_append_format = """
add_subdirectory(day{day})

set_target_properties(2025_{day:02}_01 PROPERTIES FOLDER "DAY{day:02}")
set_target_properties(2025_{day:02}_02 PROPERTIES FOLDER "DAY{day:02}")
"""

def main():
	if len(argv) < 2:
		print(f'Usage: {argv[0]} number')
		return
	
	day  = int(argv[1])
	year = 2025
	path = f'day{argv[1]}'

	cpp_part_1 = f'{path}/src/part1.cpp'
	cpp_part_2 = f'{path}/src/part2.cpp'
	cmake_file = f'{path}/CMakeLists.txt'
	input_file = f'inputs/day{day}/input.txt'


	os.makedirs(f'{path}/src', exist_ok=True)
	os.makedirs(f'inputs/day{day}', exist_ok=True)

	if not os.path.exists(cmake_file):
		with open(f'{path}/CMakeLists.txt', 'w', encoding='utf-8') as f:
			f.write(cmake_format.format(year=year, day=day))

		with open('CMakeLists.txt', 'a', encoding='utf-8') as f:
			f.write(cmake_super_append_format.format(year=year, day=day))

	if not os.path.exists(cpp_part_1):
		with open(cpp_part_1, 'w', encoding='utf-8') as f:
			f.write(main_cpp_format.format(year=year, day=day, part=1))

	if not os.path.exists(cpp_part_2):
		with open(cpp_part_2, 'w', encoding='utf-8') as f:
			f.write(main_cpp_format.format(year=year, day=day, part=2))

	if not os.path.exists(input_file):
		with open('session.txt', 'r', encoding='utf-8') as f:
			session = f.read()
		# hacky but whatever, could install python-requests
		with open(input_file, 'w', encoding='utf-8') as f:
			subprocess.run([
				'curl', 
				'-A', f'https://github.com/ludvij/advent-of-code-{year}',
				'-H', f'Cookie: session={session}',
				f'https://adventofcode.com/{year}/day/{day}/input'
			], stdout=f)



if __name__ == '__main__':
	main()