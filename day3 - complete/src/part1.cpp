
// https://adventofcode.com/2025/day/3
// advent of code : 2025
// day            : 3
// part           : 1
// author         : Lud

#include <cstddef>
#include <print>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day3/"

auto do_program(auto path)
{

    const auto file = Lud::Slurp(path);

    u64 res = 0;
    for (const auto& line : Lud::Split(file, '\n'))
    {
        u8 digit = 0;
        u8 tenth = 0;
        size_t tenth_place = 0;
        for (size_t i = 0; i < (line.size() - 1); i++)
        {
            if (line[i] > tenth)
            {
                tenth = line[i];
                tenth_place = i;
            }
        }
        for (size_t i = tenth_place + 1; i < line.size(); i++)
        {
            if (line[i] > digit)
            {
                digit = line[i];
            }
        }
        u64 num = (tenth - 48) * 10 + (digit - 48);
        res += num;
    }
    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 357U);

    auto result = do_program(INPUT_PATH "input.txt");
    std::println("[RESULT]: {}", result);

    return 0;
}
