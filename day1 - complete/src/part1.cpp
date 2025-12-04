// https://adventofcode.com/2025/day/1
// advent of code : 2025
// day            : 1
// part           : 1
// author         : Lud

#include <print>
#include <sstream>
#include <string>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>

#define INPUT_PATH "inputs/day1/"

// literally mod(x, 100)
auto do_program(auto path)
{
    std::ifstream stream(path);

    s32 dial = 50;

    u32 count = 0;

    const auto mod_100 = [](s32 a) {
        return (a % 100 + 100) % 100;
    };

    std::string line;
    while (std::getline(stream, line))
    {
        std::istringstream iss(line);
        char rot;
        s32 val;
        iss >> rot;
        iss >> val;
        if (rot == 'L')
        {
            val = -val;
        }

        dial = mod_100(dial + val);

        if (dial == 0)
        {
            count++;
        }
    }
    return count;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 3U);

    auto result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
