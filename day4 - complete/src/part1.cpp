
// https://adventofcode.com/2025/day/4
// Advent of code : 2025
// day            : 4
// part           : 1

#include <print>
#include <vector>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day4/"

auto do_program(auto path)
{
    auto data = Lud::Slurp(path);

    u64 res = 0;

    std::vector<std::vector<char>> cells;
    for (const auto& line : Lud::Split(data, '\n'))
    {
        cells.emplace_back();
        cells.back().reserve(line.size());
        for (char c : line)
        {
            cells.back().push_back(c);
        }
    }
    for (size_t y = 0; y < cells.size(); y++)
    {
        for (size_t x = 0; x < cells[0].size(); x++)
        {
            if (cells[y][x] != '@')
            {
                continue;
            }
            u8 count = 0;
            for (s32 ay = -1; ay <= 1; ay++)
            {
                for (s32 ax = -1; ax <= 1; ax++)
                {
                    if ((ax == 0 && ay == 0) ||                 // current cell
                        (ax < 0 && x == 0) ||                   // underflow
                        (ay < 0 && y == 0) ||                   // underflow
                        (ax > 0 && x == cells[0].size() - 1) || // overflow
                        (ay > 0 && y == cells.size() - 1)       // overflow
                    )
                    {
                        continue;
                    }
                    if (cells[y + ay][x + ax] == '@')
                    {
                        count++;
                    }
                }
            }
            if (count < 4)
            {
                res++;
            }
        }
    }

    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 13U);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
