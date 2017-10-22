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

#include "catch.hpp"

using namespace Catch::Matchers;

namespace
{

std::vector<std::string> lines_of(std::vector<std::string const*> const& v)
{
    std::vector<std::string> ret;

    for (auto const lptr : v)
        ret.push_back(*lptr);

    return ret;
}

}

SCENARIO("top2bottom", "")
{
    GIVEN("An email with no quoting")
    {
        std::vector<std::string> const email{"a", "b", "c"};

        WHEN("converting to bottom")
        {
            auto const bottom = top2bottom(email);

            THEN("email is unchanged")
            {
                REQUIRE_THAT(lines_of(bottom), Equals(email));
            }
        }
    }

    GIVEN("An email with a single level of quoting")
    {
        std::vector<std::string> const email{"a", "", "ATTRIBUTION", "> b"};

        WHEN("converting to bottom")
        {
            auto const bottom = top2bottom(email);

            THEN("email is converted to bottom")
            {
                REQUIRE_THAT(lines_of(bottom), Equals(
                    std::vector<std::string>{"ATTRIBUTION", "> b", "", "a"}));
            }
        }
    }

    GIVEN("An email with spaces after attribution")
    {
        std::vector<std::string> const email{"a", "", "ATTRIBUTION", "", "", "> b"};

        WHEN("converting to bottom")
        {
            auto const bottom = top2bottom(email);

            THEN("spaces stay after attribution")
            {
                REQUIRE_THAT(lines_of(bottom), Equals(
                    std::vector<std::string>{"ATTRIBUTION", "", "", "> b", "", "a"}));
            }
        }
    }

    GIVEN("An email with multiple levels of quoting")
    {
        std::vector<std::string> const email{
            "a",
            "b",
            "",
            "ATTRIBUTION",
            "",
            "> b",
            "> c",
            ">",
            "> ATTRIBUTION",
            ">",
            ">> d",
            ">> e",
            ">> ",
            ">> ATTRIBUTION",
            ">>> ",
            ">>> f",
            };

        WHEN("converting to bottom")
        {
            auto const bottom = top2bottom(email);

            THEN("email is converted to bottom")
            {
                REQUIRE_THAT(lines_of(bottom), Equals(
                    std::vector<std::string>{
                        "ATTRIBUTION",
                        "",
                        "> ATTRIBUTION",
                        ">",
                        ">> ATTRIBUTION",
                        ">>> ",
                        ">>> f",
                        ">> ",
                        ">> d",
                        ">> e",
                        ">",
                        "> b",
                        "> c",
                        "",
                        "a",
                        "b"}));
            }
        }
    }

    GIVEN("An email that is not top posted")
    {
        std::vector<std::string> const email{
            "a",
            "b",
            "",
            "> c",
            "d",
            "> e",
            };

        WHEN("converting to bottom")
        {
            ConversionError conversion_error{0};

            try
            {
                top2bottom(email);
            }
            catch (ConversionError const& error)
            {
                conversion_error = error;
            }

            THEN("exception with offending line index is thrown")
            {
                REQUIRE(conversion_error.line_index == 5);
            }
        }
    }

    GIVEN("An email with empty quoted lines at the end")
    {
        std::vector<std::string> const email{
            "a",
            "b",
            "",
            "ATTRIBUTION",
            "> c",
            ">",
            "> ATTRIBUTION",
            "> > e",
            "> >",
            ">",
            ">",
            "",
            };

        WHEN("converting to bottom")
        {
            auto const bottom = top2bottom(email);

            THEN("empty quoted lines are added to the right quoting level")
            {
                REQUIRE_THAT(lines_of(bottom), Equals(
                    std::vector<std::string>{
                        "ATTRIBUTION",
                        "> ATTRIBUTION",
                        "> > e",
                        "> >",
                        ">",
                        "> c",
                        ">",
                        ">",
                        "",
                        "a",
                        "b",
                        "",
                        }));
            }
        }
    }

    GIVEN("An email with a quote symbol followed by a normal symbol")
    {
        std::vector<std::string> const email{
            "a",
            "",
            "ATTRIBUTION",
            "> >c",
            ">",
            "> ATTRIBUTION <",
            "> >:",
            ">",
            "> > e",
            };

        WHEN("converting to bottom")
        {
            auto const bottom = top2bottom(email);

            THEN("the quote symbol is considered part of the line contents")
            {
                REQUIRE_THAT(lines_of(bottom), Equals(
                    std::vector<std::string>{
                        "ATTRIBUTION",
                        "> ATTRIBUTION <",
                        "> >:",
                        ">",
                        "> > e",
                        ">",
                        "> >c",
                        "",
                        "a",
                        }));
            }
        }
    }

    GIVEN("An originally bottom-posted email continued as top-posted")
    {
        std::vector<std::string> const email{
            "a",
            "",
            "ATTRIBUTION",
            "> b",
            ">",
            "> ATTRIBUTION",
            ">> ATTRIBUTION",
            ">>> ATTRIBUTION",
            ">>>> c",
            ">>>> d",
            ">>> e",
            ">> f",
            ">",
            ""
            };

        WHEN("converting to bottom")
        {
            auto const bottom = top2bottom(email);

            THEN("the email is converted to bottom")
            {
                REQUIRE_THAT(lines_of(bottom), Equals(
                    std::vector<std::string>{
                        "ATTRIBUTION",
                        "> ATTRIBUTION",
                        ">> ATTRIBUTION",
                        ">>> ATTRIBUTION",
                        ">>>> c",
                        ">>>> d",
                        ">>> e",
                        ">> f",
                        ">",
                        "> b",
                        ">",
                        "",
                        "a",
                        "",
                        }));
            }
        }
    }
}
