// https://adventofcode.com/2025/day/10
// Advent of code : 2025
// day            : 10
// part           : 1

#include <algorithm>
#include <deque>
#include <print>
#include <ranges>
#include <string_view>
#include <unordered_set>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day10/"

namespace stdv = std::views;
namespace stdr = std::ranges;

struct machine
{
    u16 target{};
    std::vector<u64> masks;
};

struct bfs_state
{
    u16 current;
    u16 depth;
};

machine parse_machine(const std::string_view line)
{
    machine m;

    const auto parts = Lud::Split(line, ' ');

    const auto& f = parts.front();

    // annoying ahh parenthesis
    const auto target = f.substr(1, f.size() - 2);
    const auto buttons = std::span(parts.begin() + 1, parts.end() - 1);

    for (size_t i = 0; i < target.size(); i++)
    {
        if (target[i] == '#')
        {
            m.target |= (1 << i);
        }
    }

    m.masks.reserve(buttons.size());

    for (const auto& button : buttons)
    {
        m.masks.emplace_back(
            stdr::fold_left(
                Lud::Split(button.substr(1, button.size() - 2), ',') |
                    stdv::transform(Lud::parse_integer<u16>()),
                0UL,
                [](const auto& curr, const auto& num) { return curr | (1 << num); }
            )
        );
    }

    return m;
}

u64 bfs(const machine& machine)
{
    std::deque<bfs_state> queue;
    const u16 target = machine.target;
    queue.emplace_back(0, 0);
    std::unordered_set<u16> visited;

    while (!queue.empty())
    {
        const auto front = queue.front();
        queue.pop_front();
        const u16 current = front.current;
        const u16 depth = front.depth;

        if (current == target)
        {
            return depth;
        }

        if (visited.contains(current))
        {
            continue;
        }
        visited.insert(current);

        for (u16 mask : machine.masks)
        {
            queue.emplace_back(current ^ mask, depth + 1);
        }
    }
    return 0;
}

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    return stdr::fold_left(
        lines |
            stdv::transform(parse_machine) |
            stdv::transform(bfs),
        0UL,
        std::plus<>()
    );
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 7U);

    u64 result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
