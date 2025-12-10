// https://adventofcode.com/2025/day/9
// Advent of code : 2025
// day            : 9
// part           : 1

#include <algorithm>
#include <deque>
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
};

u64 distance(const Point2D& o1, const Point2D& o2)
{
    const u64 dx = (o2.x - o1.x) * (o2.x - o1.x);
    const u64 dy = (o2.y - o1.y) * (o2.y - o1.y);

    return dx + dy;
}

double ccw(const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
    // matrix determinant
    // |x1 y1 1|
    // |x2 y2 1|
    // |x3 y3 1|
    // x3 (y1 - y2) + x1 (y2 - y3) + x2 (y3 - y1)
    const auto x1 = static_cast<double>(p1.x);
    const auto x2 = static_cast<double>(p2.x);
    const auto x3 = static_cast<double>(p3.x);
    const auto y1 = static_cast<double>(p1.y);
    const auto y2 = static_cast<double>(p2.y);
    const auto y3 = static_cast<double>(p3.y);
    return x1 * (y2 - y3) +
           x2 * (y3 - y1) +
           x3 * (y1 - y2);
}

u64 parse_num(std::string_view num)
{
    return Lud::is_num<u64>(num).value();
}

// graham scan
std::deque<Point2D> convex_hull(std::vector<Point2D>& points)
{
    std::deque<Point2D> hull;

    // lowest and leftmost point
    const Point2D P0 = *stdr::min_element(points, [](const auto& o1, const auto& o2) {
        return o1.y < o2.y || (o1.y == o2.y && o1.x < o2.x);
    });

    stdr::sort(points, [&](const auto& o1, const auto& o2) {
        const auto orientation = ccw(P0, o1, o2);
        if (orientation == 0)
        {
            return distance(P0, o1) < distance(P0, o2);
        }
        return orientation < 0;
    });

    for (const auto& point : points)
    {
        while (hull.size() > 1 && ccw(hull[hull.size() - 2], hull.back(), point) >= 0)
        {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    return hull;
}

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    std::vector<Point2D> points;
    points.reserve(lines.size());

    for (const auto& line : lines)
    {
        auto parts = Lud::Split(line, ',') |
                     stdv::transform(parse_num);
        points.emplace_back(parts[0], parts[1]);
    }

    u64 res = 0;

    auto hull = convex_hull(points);
    for (size_t i = 0; i < hull.size(); i++)
    {
        for (size_t j = i + 1; j < hull.size(); j++)
        {
            u64 dx = std::max(hull[i].x, hull[j].x) - std::min(hull[i].x, hull[j].x) + 1;
            u64 dy = std::max(hull[i].y, hull[j].y) - std::min(hull[i].y, hull[j].y) + 1;

            u64 area = dx * dy;
            if (area > res)
            {
                res = area;
            }
        }
    }

    return res;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 50U);

    u64 result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
