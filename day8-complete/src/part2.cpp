// https://adventofcode.com/2025/day/8
// Advent of code : 2025
// day            : 8
// part           : 2

#include <format>
#include <functional>
#include <list>
#include <print>
#include <queue>
#include <ranges>
#include <unordered_set>
#include <vector>

#include <ludutils/lud_assert.hpp>
#include <ludutils/lud_misc.hpp>
#include <ludutils/lud_parse.hpp>
#include <ludutils/lud_timer.hpp>

#define INPUT_PATH "inputs/day8/"

namespace stdr = std::ranges;
namespace stdv = std::views;

struct Point3D
{
    using SizeT = u64;
    SizeT x;
    SizeT y;
    SizeT z;

    bool operator==(const Point3D& rhs) const
    {
        return x == rhs.x &&
               y == rhs.y &&
               z == rhs.z;
    }
};

Point3D::SizeT distance(const Point3D& o1, const Point3D& o2)
{
    Point3D::SizeT dx = (o2.x - o1.x) * (o2.x - o1.x);
    Point3D::SizeT dy = (o2.y - o1.y) * (o2.y - o1.y);
    Point3D::SizeT dz = (o2.z - o1.z) * (o2.z - o1.z);

    return dx + dy + dz;
}

struct Point3DPair
{
    Point3D::SizeT distance;
    Point3D p1;
    Point3D p2;

    bool operator>(const Point3DPair& rhs) const
    {
        return distance > rhs.distance;
    }
};

template <>
struct std::hash<Point3D>
{
    std::size_t operator()(const Point3D& p) const
    {
        return (p.x * 73'856'093UL) ^ (p.y * 19'349'663UL) ^ (p.z * 83'492'791UL);
    }
};

u64 parse_num(const std::string_view sv)
{
    return Lud::is_num<Point3D::SizeT>(sv).value();
}

auto get_distances(std::vector<Point3D>& boxes)
{
    std::priority_queue<Point3DPair, std::vector<Point3DPair>, std::greater<>> distances;
    for (size_t i = 0; i < boxes.size(); i++)
    {
        for (size_t j = i + 1; j < boxes.size(); j++)
        {
            distances.emplace(distance(boxes[i], boxes[j]), boxes[i], boxes[j]);
        }
    }
    return distances;
}

u64 do_program(const char* path)
{
    const auto file = Lud::Slurp(path);
    const auto lines = Lud::Split(file, '\n');

    std::vector<Point3D> boxes;
    boxes.reserve(lines.size());

    for (const auto& line : lines)
    {
        auto c = Lud::Split(line, ',') |
                 stdv::transform(parse_num);

        boxes.emplace_back(c[0], c[1], c[2]);
    }
    auto distances = get_distances(boxes);
    std::list<std::unordered_set<Point3D>> circuits;
    Point3DPair last;
    while (!distances.empty())
    {
        auto d = distances.top();
        distances.pop();
        auto it = circuits.begin();
        std::vector<decltype(it)> inserted;
        for (; it != circuits.end(); ++it)
        {
            if (it->contains(d.p1) && it->contains(d.p2))
            {
                goto skip_iter;
            }
            if (it->contains(d.p1) || it->contains(d.p2))
            {
                inserted.push_back(it);
            }
        }
        if (inserted.size() > 0)
        {
            inserted[0]->insert(d.p1);
            inserted[0]->insert(d.p2);
            if (inserted.size() == 2)
            {
                inserted[0]->insert_range(*inserted[1]);

                circuits.erase(inserted[1]);
            }
        }
        else
        {
            circuits.emplace_back();
            circuits.back().insert(d.p1);
            circuits.back().insert(d.p2);
        }
        if (circuits.size() == 1)
        {
            last = d;
        }
    skip_iter:
    }

    return last.p1.x * last.p2.x;
}

int main(int argc, char** argv)
{
    Lud::assert::eq(do_program(INPUT_PATH "test.txt"), 25272U);

    u64 result = do_program(INPUT_PATH "input.txt");

    std::println("[RESULT]: {}", result);
    return 0;
}
