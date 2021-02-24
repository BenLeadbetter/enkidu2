#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <headless/app/Parser.hpp>

#include <sstream>

using namespace enkidu;

TEST_CASE("Parser test", "Parser::parse")
{
    GIVEN("an enkidu parser")
    {
        std::ostringstream err;
        headless::Parser parser(err);

        WHEN("parse empty string")
        {
            auto commands = parser.parse("");

            THEN("no commands are recieved")
            {
                REQUIRE(commands.empty());
            }

            THEN("no error message written to ostream")
            {
                REQUIRE(err.str().empty());
            }
        }

        WHEN("parse \"quit\"")
        {
            auto commands = parser.parse({"quit"});

            THEN("recieve quit command")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<headless::QuitCommand>(c));
            }
        }

        WHEN("parse \"add --node\"")
        {
            auto commands = parser.parse("add --node");

            THEN("recieve add node command")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<model::AddNodeAction>(c));
            }
        }

        WHEN("parse \"add -n\"")
        {
            auto commands = parser.parse("add -n");

            THEN("recieve add node command")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<model::AddNodeAction>(c));
            }
        }

        WHEN("parse \"add\"")
        {
            auto commands = parser.parse("add");

            THEN("recieve no commands")
            {
                REQUIRE(commands.size() == 0);
            }

            THEN("error message written to ostream")
            {
                REQUIRE(!err.str().empty());
            }
        }

        WHEN("parse connect")
        {
            auto commands = parser.parse("connect");

            THEN("recieve error and no command")
            {
                REQUIRE(commands.size() == 0);
                REQUIRE(!err.str().empty());
            }
        }

        WHEN("parse connect")
        {
            auto commands = parser.parse("connect 1 2");

            THEN("recieve a connect command with correct ports")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<model::ConnectAction>(c));
                auto connect = std::get<model::ConnectAction>(c);
                REQUIRE(connect.input == 1);
                REQUIRE(connect.output == 2);
            }
        }

        WHEN("parse remove")
        {
            auto commands = parser.parse("remove");

            THEN("recieve no commands")
            {
                REQUIRE(commands.empty());
            }
            THEN("error written to output")
            {
                REQUIRE(!err.str().empty());
            }
        }

        WHEN("parse remove -n 5")
        {
            auto commands = parser.parse("remove -n 5");

            THEN("recieve command to remove node with correct id")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<model::RemoveNodeAction>(c));
                auto ra = std::get<model::RemoveNodeAction>(c);
                REQUIRE(ra.id == 5);
            }
        }

        WHEN("parse remove -c 2")
        {
            auto commands = parser.parse("remove -c 2");

            THEN("recieve command to remove connection with correct id")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<model::DisconnectAction>(c));
                auto da = std::get<model::DisconnectAction>(c);
                REQUIRE(da.connectionid == 2);
            }
        }

        WHEN("parse show -n")
        {
            auto commands = parser.parse("show -n");

            THEN("recieve command to show nodes")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<headless::ShowCommand>(c));
                auto sc = std::get<headless::ShowCommand>(c);
                REQUIRE(sc.option == headless::ShowCommand::Nodes);
            }
        }

        WHEN("parse show -c")
        {
            auto commands = parser.parse("show -c");

            THEN("recieve command to show connections")
            {
                REQUIRE(commands.size() == 1);
                auto c = commands.front();
                REQUIRE(std::holds_alternative<headless::ShowCommand>(c));
                auto sc = std::get<headless::ShowCommand>(c);
                REQUIRE(sc.option == headless::ShowCommand::Connections);
            }
        }

        WHEN("parse two conecutive lines")
        {
            auto commands1 = parser.parse("add -n");
            auto commands2 = parser.parse("show -n");

            THEN("recieve correct commands for each call")
            {
                REQUIRE(commands1.size() == 1);
                auto c1 = commands1.front();
                REQUIRE(std::holds_alternative<model::AddNodeAction>(c1));

                REQUIRE(commands2.size() == 1);
                auto c2 = commands2.front();
                REQUIRE(std::holds_alternative<headless::ShowCommand>(c2));
                auto sc = std::get<headless::ShowCommand>(c2);
                REQUIRE(sc.option == headless::ShowCommand::Nodes);
            }
        }
    }
}
