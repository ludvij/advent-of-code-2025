
// https://adventofcode.com/2025/day/4
// Advent of code : 2025
// day            : 4
// part           : 1
// author         : Lud

#include <print>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>

#define INPUT_PATH "inputs/day4/"

auto do_program(auto path)
{
    return 0U;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 1U);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
