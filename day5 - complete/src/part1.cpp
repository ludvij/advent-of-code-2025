
// https://adventofcode.com/2025/day/5
// Advent of code : 2025
// day            : 5
// part           : 1

#include "ludutils/lud_parse.hpp"
#include <fstream>
#include <list>
#include <print>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <string>

#define INPUT_PATH "inputs/day5/"

struct Range
{
    u64 min;
    u64 max;
};

auto do_program(auto path)
{
    std::ifstream file(path);
    std::string line;
    std::list<Range> ranges;

    u64 res = 0;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            break;
        }
        const auto parts = Lud::Split(line, '-');
        auto min = Lud::is_num<u64>(parts[0]).value();
        auto max = Lud::is_num<u64>(parts[1]).value();

        auto it = ranges.begin();

        while (it != ranges.end())
        {
            auto& [mapped_min, mapped_max] = *it;
            if (mapped_max < min || max < mapped_min) // no collisions
            {
                it++;
                continue;
            }
            min = mapped_min < min ? mapped_min : min;
            max = mapped_max > max ? mapped_max : max;

            it = ranges.erase(it);
        }

        ranges.emplace_back(min, max);
    }

    while (std::getline(file, line))
    {
        auto number = Lud::is_num<u64>(line).value();

        for (const auto& [min, max] : ranges)
        {
            if (min <= number && number <= max)
            {
                res++;
                break;
            }
        }
    }
    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 3U);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
