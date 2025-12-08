// https://adventofcode.com/2025/day/6
// Advent of code : 2025
// day            : 6
// part           : 2

#include <cstddef>
#include <numeric>
#include <print>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day6/"

namespace stdv = std::views;

u64 parse_number(std::string_view x)
{
    return Lud::is_num<u64>(x).value();
}

// could just matix transpose this
u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);

    const auto lines = Lud::Split(file, '\n');

    std::vector<char> operands;
    std::ranges::copy_if(lines.back(), std::back_inserter(operands), [](char c) { return c != ' '; });

    const size_t cols = operands.size();
    const size_t rows = lines.size() - 1;

    std::vector<std::vector<u64>> columns(cols);

    size_t curr_col = 0;
    for (size_t col = 0; col < lines[0].size(); col++)
    {
        std::string curr(rows, ' ');
        for (size_t row = 0; row < rows; row++)
        {
            curr[row] = lines[row][col];
        }
        if (Lud::IsBlank(curr))
        {
            curr_col++;
        }
        else
        {
            columns[curr_col].push_back(Lud::is_num<u64>(curr).value());
        }
    }

    u64 res = 0;

    for (size_t col = 0; col < cols; col++)
    {
        switch (operands[col])
        {
        case '*':
            res += std::accumulate(columns[col].begin(), columns[col].end(), 1UL, std::multiplies<>());
            break;
        case '+':
            res += std::accumulate(columns[col].begin(), columns[col].end(), 0UL, std::plus<>());
            break;
        default:
            break;
        }
    }

    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 3'263'827U);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
