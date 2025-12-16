// https://adventofcode.com/2025/day/10
// Advent of code : 2025
// day            : 10
// part           : 2

#include <algorithm>
#include <functional>
#include <limits>
#include <print>
#include <ranges>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day10/"

namespace v = std::views;
namespace r = std::ranges;

struct Machine
{
    std::vector<u16> masks;
    std::vector<u16> joltage;

    Machine(auto b, auto j)
        : masks(b.begin(), b.end())
        , joltage(j.begin(), j.end())
    {
    }
};

constexpr auto INVALID_SET = std::numeric_limits<u32>::max();

Machine parse_machine(const std::string_view line)
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

bool validate_combination(const auto& combination, u16 target)
{
    return r::fold_left(combination, 0UL, std::bit_xor<>()) == target;
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

u64 subdivide(const Machine& machine)
{
    auto combinations = Lud::combinations(machine.masks);

    std::unordered_map<std::vector<u16>, u64, Lud::HashNumericVector<u16>> memo;
    std::unordered_map<u64, std::vector<std::vector<u16>>> validated;

    const auto recurse = [&](this auto& self, const std::vector<u16>& joltages) -> u64 {
        if (r::all_of(joltages, [](u16 x) { return x == 0; }))
        {
            return 0;
        }
        if (auto it = memo.find(joltages); it != memo.end())
        {
            return it->second;
        }

        // create the target mask using parity
        u16 target = 0;
        for (size_t i = 0; i < joltages.size(); i++)
        {
            target |= (joltages[i] & 1) << i;
        }

        if (!validated.contains(target))
        {
            validated.emplace(
                target,
                combinations |
                    v::filter([&](const auto& set) {
                        return validate_combination(set, target);
                    }) |
                    r::to<std::vector>()
            );
        }

        u64 min = INVALID_SET;
        for (const auto& combination : validated.at(target))
        {
            auto next = update_joltages(combination, joltages);

            if (r::any_of(next, [](s16 x) { return x < 0; }))
            {
                continue;
            }

            for (auto& j : next)
            {
                j /= 2;
            }

            const u64 half = self(next);
            if (half == INVALID_SET)
            {
                continue;
            }

            const u64 presses = 2 * half + combination.size();

            if (presses < min)
            {
                min = presses;
            }
        }
        memo.emplace(joltages, min);
        return min;
    };
    return recurse(machine.joltage);
}

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    return r::fold_left(
        lines |
            v::transform(parse_machine) |
            v::transform(subdivide),
        0UL,
        std::plus<>()
    );
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 33U);

    std::println("[RESULT]: {}", do_program(INPUT_PATH "input.txt"));

    return 0;
}
