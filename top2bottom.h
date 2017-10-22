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

#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct ConversionError { std::size_t line_index; };

std::vector<std::string const*> top2bottom(std::vector<std::string> const& lines);
