// https://adventofcode.com/2025/day/7
// Advent of code : 2025
// day            : 7
// part           : 1

#include <print>
#include <vector>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day7/"

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    const size_t w = lines[0].size();

    u64 res = 0;

    std::vector<bool> beams(w);
    beams[file.find('S')] = true;

    for (const auto& line : lines | std::views::drop(1))
    {
        for (size_t x = 0; x < w; x++)
        {
            if (line[x] == '^' && beams[x])
            {
                res += 1;
                beams[x] = false;

                beams[x - 1] = true;
                beams[x + 1] = true;
            }
        }
    }

    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 21U);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
