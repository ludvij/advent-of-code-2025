// https://adventofcode.com/2025/day/2
// advent of code : 2025
// day            : 2
// part           : 2
// author         : Lud

#include <print>
#include <unordered_map>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day2/"

struct mul_divisor
{
    u64 mul;
    u64 imul;
    u32 times;
};

auto do_program(auto path)
{
    const auto data = Lud::Slurp<std::string>(path);
    const auto id_ranges = Lud::Split(data, ',');

    u64 res = 0;

    std::unordered_map<u32, std::vector<mul_divisor>> div_mults;

    const auto num_len = [](u64 number) {
        u32 len = 0;
        while (number > 0)
        {
            number /= 10;
            len++;
        }
        return len;
    };

    const auto prepare_divisors = [&](u32 len) {
        if (div_mults.contains(len))
        {
            return;
        }

        for (u32 i = 1; i <= len / 2; i++)
        {
            if (len % i != 0)
            {
                continue;
            }
            u64 mul = 10;
            for (u32 j = 1; j < i; j++)
            {
                mul *= 10;
            }
            u64 imul = mul;
            for (u32 j = i; j < len - i; j++)
            {
                imul *= 10;
            }
            div_mults[len].emplace_back(mul, imul, len / i);
        }
    };

    for (const auto& id : id_ranges)
    {
        const auto parts = Lud::Split(id, '-');
        const auto min = Lud::is_num<u64>(parts[0]).value();
        const auto max = Lud::is_num<u64>(parts[1]).value();
        for (u64 i = min; i <= max; i++)
        {
            const u32 len = num_len(i);
            prepare_divisors(len);
            for (const auto& muldiv : div_mults[len])
            {
                const u64 test_part = i / muldiv.imul;
                u64 test = test_part;
                for (u32 j = 1; j < muldiv.times; j++)
                {
                    test = test * muldiv.mul + test_part;
                }
                if (test == i)
                {
                    res += i;
                    break;
                }
            }
        }
    }
    return res;
}
int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 4'174'379'265UL);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
