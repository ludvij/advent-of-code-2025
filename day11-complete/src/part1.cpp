// https://adventofcode.com/2025/day/11
// Advent of code : 2025
// day            : 11
// part           : 1

#include <functional>
#include <numeric>
#include <print>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>
#include <unordered_map>
#include <unordered_set>

#define INPUT_PATH "inputs/day11/"

namespace v = std::views;
namespace r = std::ranges;

using Node = std::unordered_set<std::string_view>;
using Graph = std::unordered_map<std::string_view, Node>;

u64 cached_dfs(Graph& graph, const std::string_view origin, const std::string_view target)
{
    std::unordered_map<std::string_view, u64> memo;
    const auto dfs = [&](this auto& self, const std::string_view next) -> u64 {
        if (next == target)
        {
            return 1;
        }
        else if (auto it = memo.find(next); it != memo.end())
        {
            return it->second;
        }

        // clangd complains idk why
        // const s64 res = r::fold_left(graph[next] | v::transform(self), 0UL, std::plus<>());
        u64 res = std::transform_reduce(
            graph[next].begin(),
            graph[next].end(),
            0UL,
            std::plus<>(),
            self
        );

        memo.emplace(next, res);

        return res;
    };

    return dfs(origin);
}

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    Graph graph;

    for (const auto& line : lines)
    {
        const auto parts = Lud::Split(line, ':');
        auto& node = graph[parts[0]];
        for (const auto& children : Lud::Split(parts[1], ' '))
        {
            node.emplace(children);
        }
    }

    return cached_dfs(graph, "you", "out");
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test_part1.txt"), 5U);

    u64 result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
