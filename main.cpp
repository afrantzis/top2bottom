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

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
    std::string word;
    std::vector<std::string> lines;
    std::ios_base::sync_with_stdio(false);

    std::string line;
    while (std::getline(std::cin, line))
        lines.push_back(std::move(line));

    try
    {
        for (auto const lptr : top2bottom(lines))
            std::cout << *lptr << std::endl;

        return 0;
    }
    catch (ConversionError const& error)
    {
        std::cerr << "WARNING: Could not convert input to bottom posted,"
                  << " leaving unchanged." << std::endl;
        std::cerr << "WARNING: Offending line is " << error.line_index + 1 << ": "
                  << lines[error.line_index] << std::endl;

        for (auto const& l: lines)
            std::cout << l << std::endl;

        return 1;
    }
}
