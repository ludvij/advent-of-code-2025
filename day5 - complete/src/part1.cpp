// https://adventofcode.com/2025/day/5
// Advent of code : 2025
// day            : 5
// part           : 1

#include <algorithm>
#include <compare>
#include <fstream>
#include <list>
#include <print>
#include <string>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day5/"

struct Range
{
    u64 min;
    u64 max;

    bool operator<(u64 rhs) const { return max < rhs; }
    bool operator==(u64 rhs) const { return min <= rhs && rhs <= max; }

    std::strong_ordering operator<=>(const Range& rhs) const
    {
        if (rhs.max < min)
        {
            return std::strong_ordering::greater;
        }
        if (rhs.min > max)
        {
            return std::strong_ordering::less;
        }
        return std::strong_ordering::equal;
    };
};

bool binary_search(const std::span<Range> spn, u64 number)
{
    std::ptrdiff_t L = 0;
    std::ptrdiff_t R = std::distance(spn.cbegin(), spn.cend());

    while (L < R)
    {
        std::ptrdiff_t m = L + (R - L) / 2;
        if (spn[m] == number)
        {
            return true;
        }
        if (spn[m].max < number)
        {
            L = m + 1;
        }
        else
        {
            R = m;
        }
    }
    return false;
}

auto do_program(auto path)
{
    std::ifstream file(path);
    std::string line;
    std::list<Range> ranges;

    u64 res = 0;

    while (std::getline(file, line) && !line.empty())
    {
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
            }
            else
            {
                min = mapped_min < min ? mapped_min : min;
                max = mapped_max > max ? mapped_max : max;

                it = ranges.erase(it);
            }
        }

        ranges.emplace_back(min, max);
    }

    std::vector<Range> vec(ranges.begin(), ranges.end());
    std::ranges::sort(vec, std::less<Range>());

    while (std::getline(file, line))
    {
        auto number = Lud::is_num<u64>(line).value();

        if (binary_search(vec, number))
        {
            res++;
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
