// https://adventofcode.com/2025/day/4
// Advent of code : 2025
// day            : 4
// part           : 2

// could be done using:
// matric convolutions
// floodfille
// bruteforce

#include <cstddef>
#include <stack>
#include <vector>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>
#include <ludutils/lud_timer.hpp>

#define INPUT_PATH "inputs/day4/"

struct Point
{
    size_t x;
    size_t y;
};

u64 flood_fill(std::vector<char>& cells, size_t x, size_t y, size_t w, size_t h)
{
    std::stack<Point> p;
    p.emplace(x, y);

    u64 res = 0;

    while (!p.empty())
    {
        Point top = p.top();
        p.pop();
        if (cells[top.x + top.y * w] != '@')
        {
            continue;
        }
        // clang-format off
		const size_t y_begin = top.y == 0     ? top.y : top.y - 1;
		const size_t y_end   = top.y == h - 1 ? top.y : top.y + 1;
		const size_t x_begin = top.x == 0     ? top.x : top.x - 1;
		const size_t x_end   = top.x == w - 1 ? top.x : top.x + 1;
        // clang-format on
        u8 count = 0;

        for (size_t by = y_begin; by <= y_end; by++)
        {
            for (size_t bx = x_begin; bx <= x_end; bx++)
            {
                if (cells[bx + by * w] == '@')
                {
                    count++;
                }
            }
        }
        if (count < 5)
        {
            cells[top.x + top.y * w] = 'x';
            res++;

            for (size_t by = y_begin; by <= y_end; by++)
            {
                for (size_t bx = x_begin; bx <= x_end; bx++)
                {
                    p.emplace(bx, by);
                }
            }
        }
    }

    return res;
}

u64 do_program_flood_fill(auto path)
{
    const std::string file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    const size_t w = lines[0].size();
    const size_t h = lines.size();

    std::vector<char> cells;
    cells.reserve(w * h);

    for (const auto& line : lines)
    {
        cells.insert_range(cells.end(), line);
    }

    u64 res = 0;

    for (size_t y = 0; y < h; y++)
    {
        for (size_t x = 0; x < w; x++)
        {
            if (cells[x + y * w] == '@')
            {
                res += flood_fill(cells, x, y, w, h);
            }
        }
    }

    return res;
}

auto do_program_bruteforce(auto path)
{
    const std::string file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    const size_t w = lines[0].size();
    const size_t h = lines.size();

    std::vector<char> cells;
    cells.reserve(w * h);

    for (const auto& line : lines)
    {
        cells.insert_range(cells.end(), line);
    }

    u64 res = 0;
    u64 last_removed = 0;
    do
    {
        last_removed = 0;

        for (size_t y = 0; y < h; y++)
        {
            for (size_t x = 0; x < w; x++)
            {
                if (cells[x + y * w] != '@')
                {
                    continue;
                }
                u8 count = 0;
                // clang-format off
                const size_t y_begin = y == 0     ? y : y - 1;
                const size_t y_end   = y == h - 1 ? y : y + 1;
                const size_t x_begin = x == 0     ? x : x - 1;
                const size_t x_end   = x == w - 1 ? x : x + 1;
                // clang-format on

                for (size_t by = y_begin; by <= y_end; by++)
                {
                    for (size_t bx = x_begin; bx <= x_end; bx++)
                    {
                        if (cells[bx + by * w] == '@')
                        {
                            count++;
                        }
                    }
                }
                if (count < 5)
                {
                    last_removed++;
                    cells[x + y * w] = 'x';
                }
            }
        }
        res += last_removed;
    }
    while (last_removed != 0);

    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program_bruteforce(INPUT_PATH "test.txt"), 43U);
    Lud::assert::eq(do_program_flood_fill(INPUT_PATH "test.txt"), 43U);

    u64 floodfill_r, bruteforce_r;
    {
        Lud::Timer timer("bruteforce");
        bruteforce_r = do_program_bruteforce(INPUT_PATH "input.txt");
    }

    {
        Lud::Timer timer("floodfill");
        floodfill_r = do_program_flood_fill(INPUT_PATH "input.txt");
    }

    Lud::assert::eq(floodfill_r, bruteforce_r);

    std::println("[RESULT]: {}", bruteforce_r);
    return 0;
}
