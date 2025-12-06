// https://adventofcode.com/2025/day/3
// advent of code : 2025
// day            : 3
// part           : 2
// author         : Lud

#include <print>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day3/"

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);

    u64 res = 0;
    const u32 batteries = 12;
    for (const auto& line : Lud::Split(file, '\n'))
    {
        size_t last_pos = 0;
        u64 num = 0;
        for (u32 i = 0; i < batteries; i++)
        {
            u8 val = 0;
            for (size_t j = last_pos; j < line.size() - (batteries - 1) + i; j++)
            {
                if (line[j] > val)
                {
                    val = line[j];
                    last_pos = j;
                }
            }
            num = num * 10 + (val - 48);
            last_pos++;
        }
        res += num;
    }
    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 3'121'910'778'619UL);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
