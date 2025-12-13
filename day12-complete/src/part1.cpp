// https://adventofcode.com/2025/day/12
// Advent of code : 2025
// day            : 12
// part           : 1

#include <algorithm>
#include <format>
#include <print>
#include <ranges>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day12/"

namespace v = std::views;
namespace r = std::ranges;

struct present
{
    constexpr static size_t w{3uz};
    constexpr static size_t h{3uz};

    std::vector<char> shape{w * h};
};

struct under_tree
{
    size_t w;
    size_t h;

    std::vector<u64> requirements;

    std::string dimensions;

    under_tree(size_t width, size_t height, auto reqs)
        : w(width)
        , h(height)
        , requirements(reqs.begin(), reqs.end())
    {
        dimensions.resize(w * h);
        r::fill(dimensions, ' ');
    }
};

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto blocks = Lud::Split(file, "\n\n");

    std::array<present, 6> presents;

    for (size_t i = 0; i < 6; i++)
    {
        auto& present = presents[i].shape;
        present.insert_range(present.begin(), Lud::Split(blocks[i], '\n') | v::drop(1) | v::join);
    }
    std::vector<under_tree> trees;

    for (const auto& line : Lud::Split(blocks.back(), '\n'))
    {
        const auto parts = Lud::Split(line, ':');
        const auto dimensions = Lud::Split(parts[0], 'x') | v::transform(Lud::parse_integer<u64>());

        trees.emplace_back(
            dimensions[0],
            dimensions[1],
            Lud::Split(parts[1], ' ') | v::transform(Lud::parse_integer<u64>())
        );
    }

    return r::count_if(trees, [](const under_tree& tree) {
        return tree.w * tree.h >= 9 * r::fold_left(tree.requirements, 0UL, std::plus<>());
    });
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 1U);

    u64 result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
