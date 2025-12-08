// https://adventofcode.com/2025/day/7
// Advent of code : 2025
// day            : 7
// part           : 2

#include <numeric>
#include <print>
#include <unordered_map>
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

    std::vector<u64> counters(w);
    counters[lines[0].find('S')] = 1;

    for (const auto& line : lines)
    {
        for (size_t x = 0; x < w; x++)
        {
            if (line[x] == '^' && counters[x])
            {
                counters[x - 1] += counters[x];
                counters[x + 1] += counters[x];
                counters[x] = 0;
            }
        }
    }

    return std::accumulate(counters.begin(), counters.end(), 0UL, std::plus<>());
}

u64 do_program_memoization(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    std::unordered_map<size_t, u64> memoized_timelines;

    auto timelines = [&](this auto& self, size_t depth, size_t x) -> u64 {
        for (size_t i = depth; i < lines.size(); i++)
        {
            if (lines[i][x] == '^')
            {
                // whacky hash for a 2d point
                if (auto it = memoized_timelines.find(x | i << 32); it != memoized_timelines.end())
                {
                    return it->second;
                }
                u64 left = self(i + 1, x - 1);
                u64 right = self(i + 1, x + 1);
                memoized_timelines.emplace(x | i << 32, left + right);
                return left + right;
            }
        }
        return 1;
    };

    u64 res = timelines(1, lines[0].find('S'));

    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 40U);

    u64 result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
