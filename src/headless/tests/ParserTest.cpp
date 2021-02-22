#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <headless/app/Parser.hpp>

using namespace enkidu;

TEST_CASE("Parser test", "Parser::parse")
{
    GIVEN("an enkidu parser")
    {
        headless::Parser parser;

        WHEN("parse \"exit\"")
        {
            auto commands = parser.parse({"exit"});

            THEN("recieve exit command")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<headless::QuitCommand>(c));
            }
        }
    }
}
