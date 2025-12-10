// https://adventofcode.com/2025/day/9
// Advent of code : 2025
// day            : 9
// part           : 2

#include "ludutils/lud_timer.hpp"
#include <print>
#include <ranges>
#include <vector>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>

#define INPUT_PATH "inputs/day9/"

namespace stdv = std::views;
namespace stdr = std::ranges;

struct Point2D
{
    u64 x;
    u64 y;

    bool operator==(const Point2D& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
};

template <>
struct std::formatter<Point2D>
{
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const Point2D& o, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{: >3d}, {: >3d}]", o.x, o.y);
    }
};

u64 parse_num(std::string_view num)
{
    return Lud::is_num<u64>(num).value();
}
// probably the most scuffed AABB i have ever made
bool check_v_intersect(const std::pair<Point2D, Point2D>& v, const Point2D& tl, const Point2D& tr, const Point2D& bl, const Point2D& br)
{
    const Point2D& top = std::get<0>(v);
    const Point2D& bot = std::get<1>(v);

    const size_t max_y = bl.y;
    const size_t min_y = tl.y;
    const size_t max_x = tr.x;
    const size_t min_x = tl.x;

    // line is completely outside
    if ((top.y > max_y || bot.y < min_y || top.x < min_x || top.x > max_x) ||
        // bot is bot corner or top is on top corner
        (bot == br || bot == bl || top == tl || top == tr) ||
        // bot is on top line or top is on bot line
        ((bot.y == min_y && top.y < min_y) || (top.y == max_y && bot.y > max_y)))
    {
        return false;
    }
    return true;
}

bool check_h_intersect(const std::pair<Point2D, Point2D>& h, const Point2D& tl, const Point2D& tr, const Point2D& bl, const Point2D& br)
{
    const Point2D& lef = std::get<0>(h);
    const Point2D& rig = std::get<1>(h);

    const size_t max_y = bl.y;
    const size_t min_y = tl.y;
    const size_t max_x = tr.x;
    const size_t min_x = tl.x;

    // skip any line outside y range
    // line is completely outside
    if ((lef.x > max_x || rig.x < min_x || lef.y < min_y || lef.y > max_y) ||
        // left is on left corner or right is on right corner
        (lef == tl || lef == bl || rig == tr || rig == br) ||
        // lef is on rig line or rig is on lef line
        ((rig.x == min_x && lef.x < min_x) || (lef.x == max_x && rig.x > max_x)))
    {
        return false;
    }
    return true;
}

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    Lud::Timer timer("operation");

    std::vector<Point2D> points;
    points.reserve(lines.size());

    for (const auto& line : lines)
    {
        auto parts = Lud::Split(line, ',') |
                     stdv::transform(parse_num);
        points.emplace_back(parts[0], parts[1]);
    }
    u64 res = 0;

    std::vector<std::pair<Point2D, Point2D>> vertical_lines;
    std::vector<std::pair<Point2D, Point2D>> horizont_lines;

    for (size_t i = 0; i < points.size(); i++)
    {
        const size_t next = (i + 1) % points.size();
        const auto& p1 = points[i];
        const auto& p2 = points[next];
        if (p1.y == p2.y)
        {
            if (p1.x < p2.x)
            {
                horizont_lines.emplace_back(p1, p2);
            }
            else
            {
                horizont_lines.emplace_back(p2, p1);
            }
        }
        if (p1.x == p2.x)
        {
            if (p1.y < p2.y)
            {
                vertical_lines.emplace_back(p1, p2);
            }
            else
            {
                vertical_lines.emplace_back(p2, p1);
            }
        }
    }

    for (size_t i = 0; i < points.size(); i++)
    {
        for (size_t j = i + 1; j < points.size(); j++)
        {
            const auto& p1 = points[i];
            const auto& p2 = points[j];

            const u64 max_x = std::max(p1.x, p2.x);
            const u64 min_x = std::min(p1.x, p2.x);
            const u64 max_y = std::max(p1.y, p2.y);
            const u64 min_y = std::min(p1.y, p2.y);

            const u64 dx = max_x - min_x + 1;
            const u64 dy = max_y - min_y + 1;

            const u64 area = dx * dy;
            if (area <= res)
            {
                continue;
            }

            const Point2D tl = {.x = min_x, .y = min_y};
            const Point2D tr = {.x = max_x, .y = min_y};
            const Point2D bl = {.x = min_x, .y = max_y};
            const Point2D br = {.x = max_x, .y = max_y};

            const auto wrap_v_intersect = [&](const auto& v) {
                return check_v_intersect(v, tl, tr, bl, br);
            };

            const auto wrap_h_intersect = [&](const auto& h) {
                return check_h_intersect(h, tl, tr, bl, br);
            };
            auto vertical_intesect = vertical_lines | stdv::filter(wrap_v_intersect) | stdv::take(1);
            auto horizont_intesect = horizont_lines | stdv::filter(wrap_h_intersect) | stdv::take(1);

            if (vertical_intesect.empty() && horizont_intesect.empty())
            {
                res = area;
            }
        }
    }

    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 24U);

    u64 result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
