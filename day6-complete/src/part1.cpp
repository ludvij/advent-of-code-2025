// https://adventofcode.com/2025/day/6
// Advent of code : 2025
// day            : 6
// part           : 1

#include <cstddef>
#include <numeric>
#include <print>
#include <ranges>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day6/"

namespace stdv = std::views;

u64 parse_number(std::string_view x)
{
    return Lud::is_num<u64>(x).value();
}

char parse_operand(std::string_view x)
{
    return x[0];
}

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    const auto operands = Lud::Split(lines.back(), ' ') |
                          stdv::transform(parse_operand);

    const size_t rows = lines.size() - 1;
    const size_t cols = operands.size();

    std::vector<std::vector<u64>> columns(cols);

    for (size_t row = 0; row < rows; row++)
    {
        const auto numbers = Lud::Split(lines[row], ' ') |
                             std::views::transform(parse_number);
        for (size_t col = 0; col < cols; col++)
        {
            columns[col].push_back(numbers[static_cast<ptrdiff_t>(col)]);
        }
    }

    u64 res = 0;

    for (size_t col = 0; col < cols; col++)
    {
        switch (operands[static_cast<ptrdiff_t>(col)])
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
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 4'277'556U);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
