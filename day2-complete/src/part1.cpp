// https://adventofcode.com/2025/day/2
// advent of code : 2025
// day            : 2
// part           : 1
// author         : Lud

#include <print>
#include <string>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day2/"

// original string-based solution
// u64 do_program(const char* path)
// {
//     auto data = Lud::Slurp<std::string>(path);
//     auto id_ranges = Lud::Split(data, ',');

//     u64 res = 0;

//     for (const auto& id : id_ranges)
//     {
//         auto parts = Lud::Split(id, '-');
//         auto min = Lud::is_num<u64>(parts[0]).value();
//         auto max = Lud::is_num<u64>(parts[1]).value();
//         for (u64 i = min; i <= max; i++)
//         {
//             auto test_string = std::to_string(i);
//             if (test_string.size() % 2 != 0)
//             {
//                 continue;
//             }

//             std::string_view first(test_string.data(), test_string.size() / 2);
//             std::string_view second(test_string.data() + test_string.size() / 2, test_string.size() / 2);
//             if (first == second)
//             {
//                 res += i;
//             }
//         }
//     }
//     return res;
// }

u64 do_program(const char* path)
{
    auto data = Lud::Slurp<std::string>(path);
    auto id_ranges = Lud::Split(data, ',');

    u64 res = 0;

    const auto num_len = [](u64 number) {
        auto len = 0;
        while (number > 0)
        {
            number /= 10;
            len++;
        }
        return len;
    };

    const auto mult_for = [](u32 len) {
        auto mul = 1;
        for (u32 i = 0; i < len; i++)
        {
            mul *= 10;
        }
        return mul;
    };

    for (const auto& id : id_ranges)
    {
        auto parts = Lud::Split(id, '-');
        auto min = Lud::is_num<u64>(parts[0]).value();
        auto max = Lud::is_num<u64>(parts[1]).value();
        for (u64 i = min; i <= max; i++)
        {
            u32 len = num_len(i);
            if (len % 2 != 0)
            {
                continue;
            }
            u64 mul = mult_for(len / 2);
            u64 test = i / mul + i / mul * mul;
            if (test == i)
            {
                res += i;
            }
        }
    }
    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 1'227'775'554U);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
