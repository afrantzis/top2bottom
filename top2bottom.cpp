/*
 * Copyright © 2017 Alexandros Frantzis
 *
 * This file is part of top2bottom.
 *
 * top2bottom is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * top2bottom is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * top2bottom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "top2bottom.h"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

namespace
{

struct LineInfo
{
    std::size_t depth;
    bool has_content;
};

LineInfo parse_line(std::string const& line)
{
    std::size_t depth = 0;
    bool has_content = false;
    bool last_was_quote = false;

    for (auto c : line)
    {
        if (c == '>')
        {
            depth += 1;
            last_was_quote = true;
        }
        else if (std::isspace(c))
        {
            last_was_quote = false;
            continue;
        }
        else
        {
            has_content = true;
            if (last_was_quote)
                depth -= 1;
            break;
        }
    }

    return LineInfo{depth, has_content};
}

inline std::size_t body_key(std::size_t depth)
{
    static auto constexpr size_t_max = std::numeric_limits<std::size_t>::max();

    return size_t_max - 10 * depth;
}

inline std::size_t before_body_key(std::size_t depth)
{
    return body_key(depth) - 1;
}

inline std::size_t attribution_key(std::size_t depth)
{
    return depth;
}

struct AnnotatedLine
{
    std::string const* line;
    LineInfo info;
    std::size_t key;
};

class AnnotationContext
{
public:
    AnnotationContext(std::size_t s)
    {
        annotated.reserve(s);
    }

    void process(std::string const& line)
    {
        auto const info = parse_line(line);

        check_top_posted(info);

        if (info.depth > depth)
            fix_attribution();

        annotated.push_back({&line, info, body_key(info.depth)});

        depth = info.depth;
    }

    std::vector<AnnotatedLine> annotated;

private:
    std::size_t depth = 0;
    bool depth_increasing = true;

    void check_top_posted(LineInfo const& info)
    {
        if (!depth_increasing && info.depth > depth)
            throw ConversionError{annotated.size()};
        else if (info.depth < depth && depth_increasing)
            depth_increasing = false;
    }

    void fix_attribution()
    {
        bool have_seen_content = false;

        for (auto iter = annotated.rbegin();
             iter != annotated.rend();
             ++iter)
        {
            auto const& info = iter->info;
            if (info.depth < depth)
                break;

            have_seen_content = have_seen_content || info.has_content;

            if (have_seen_content && !info.has_content)
            {
                iter->key = before_body_key(depth);
                break;
            }
            else
            {
                iter->key = attribution_key(depth);
            }
        }
    }
};

}

std::vector<std::string const*> top2bottom(std::vector<std::string> const& lines)
{
    AnnotationContext ctx{lines.size()};

    for (auto const& line : lines)
        ctx.process(line);

    std::stable_sort(
        ctx.annotated.begin(), ctx.annotated.end(),
        [] (auto const& al1, auto const& al2) { return al1.key < al2.key; });

    std::vector<std::string const*> output{ctx.annotated.size()};

    std::transform(ctx.annotated.begin(), ctx.annotated.end(), output.begin(),
        [] (auto const& al) { return al.line; });

    return output;
}
