// https://adventofcode.com/2025/day/1
// advent of code : 2025
// day            : 1
// part           : 2
// author         : Lud

#include <print>
#include <sstream>
#include <string>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>

#define INPUT_PATH "inputs/day1/"

auto do_program(auto path)
{
    std::ifstream stream(path);

    s32 dial = 50;

    u32 count = 0;

    std::string line;

    const auto mod_100 = [](s32 a) {
        return (a % 100 + 100) % 100;
    };

    while (std::getline(stream, line))
    {
        std::istringstream iss(line);

        char rot;
        s32 val;

        iss >> rot;
        iss >> val;

        count += val / 100;
        if (rot == 'L')
        {
            val = -val;
        }

        s32 next_dial = mod_100(dial + val);

        if ((dial != 0 && val > 0 && dial > next_dial) ||
            (dial != 0 && val < 0 && dial < next_dial) ||
            (next_dial == 0))
        {
            count++;
        }
        dial = next_dial;
    }
    return count;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 6U);

    auto res = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", res);
    return 0;
}
