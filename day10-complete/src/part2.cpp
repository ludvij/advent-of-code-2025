// https://adventofcode.com/2025/day/10
// Advent of code : 2025
// day            : 10
// part           : 2

#include "ludutils/lud_timer.hpp"
#include <algorithm>
#include <functional>
#include <limits>
#include <print>
#include <ranges>
#include <unordered_set>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day10/"

namespace v = std::views;
namespace r = std::ranges;

struct machine
{
    std::vector<u16> masks;
    std::vector<u16> joltage;

    machine(auto b, auto j)
        : masks(b.begin(), b.end())
        , joltage(j.begin(), j.end())
    {
    }
};

struct bfs_state
{
    u16 current;
    std::unordered_set<u16> visited;
};

constexpr auto INVALID_SET = std::numeric_limits<u64>::max();

machine parse_machine(const std::string_view line)
{
    const auto parts = Lud::Split(line, ' ');
    const auto filtered = parts | v::transform([](const auto& s) { return s.substr(1, s.size() - 2); });

    // annoying ahh parenthesis
    const auto joltage = Lud::Split(filtered.back(), ',');

    const auto parse_mask = [](const std::string_view s) {
        return r::fold_left(
            Lud::Split(s, ',') | v::transform(Lud::parse_integer<u16>()),
            u16{},
            [](u16 res, u16 curr) {
                return res | (1 << curr);
            }
        );
    };

    return {
        filtered | v::drop(1) | v::take(parts.size() - 2) | v::transform(parse_mask),
        joltage | v::transform(Lud::parse_integer<u16>())
    };
}

auto validate_combination(const auto& combination, u16 target)
{
    u16 current = 0;
    for (const u16 mask : combination)
    {
        current ^= mask;
    }
    return current == target;
}

std::vector<u16> update_joltages(const std::vector<u16>& masks, std::vector<u16> joltages)
{
    for (size_t i = 0; i < joltages.size(); i++)
    {
        for (const u16 mask : masks)
        {
            joltages[i] -= (mask >> i) & 1;
        }
    }
    return joltages;
}

u64 subdivide(const machine& machine)
{
    auto combinations = Lud::combinations(machine.masks);
    std::unordered_map<u64, u64> memo;

    const auto hash_joltage = [&](const std::vector<u16>& joltages) {
        u64 result = 0;
        for (const u16 x : joltages)
        {
            result = result * 10 + x;
        }
        return result;
    };

    const auto recurse = [&](this auto& self, const std::vector<u16>& masks, const std::vector<u16>& joltages) -> u64 {
        if (r::all_of(joltages, [](u16 x) { return x == 0; }))
        {
            return 0;
        }
        const auto hash = hash_joltage(joltages);
        if (auto it = memo.find(hash); it != memo.end())
        {
            return it->second;
        }

        // create the target mask using parity
        u16 target = r::fold_left(v::iota(0uz, joltages.size()), u16{}, [&](u16 res, u16 x) { return res | (joltages[x] % 2) << x; });

        // compute all possible button combinations to reach that target

        auto valid = combinations |
                     v::filter([&](const auto& set) { return validate_combination(set, target); });

        u64 min = INVALID_SET;
        for (const auto& combination : valid)
        {
            auto next = update_joltages(combination, joltages);

            if (r::any_of(next, [](s16 x) { return x < 0; }))
            {
                continue;
            }
            r::for_each(next, [](u16& elem) { elem /= 2; });

            const u64 half_presses = self(masks, next);
            if (half_presses == INVALID_SET)
            {
                continue;
            }

            const u64 presses = 2 * half_presses + combination.size();

            if (presses < min)
            {
                min = presses;
            }
        }
        if (min != INVALID_SET)
        {
            memo.emplace(hash, min);
        }
        return min;
    };
    return recurse(machine.masks, machine.joltage);
}

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    std::vector<machine> machines;
    machines.reserve(lines.size());

    for (const auto& line : lines)
    {
        machines.push_back(parse_machine(line));
    }
    Lud::Timer timer("operation");
    return r::fold_left(machines | v::transform(subdivide), 0UL, std::plus<>());
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 33U);

    std::println("[RESULT]: {}", do_program(INPUT_PATH "input.txt"));

    return 0;
}
