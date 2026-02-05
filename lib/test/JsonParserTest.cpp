#include <catch2/catch_test_macros.hpp>
#include "JsonParser.h"
#include <sstream>

TEST_CASE("JsonParser parses valid JSON with type and attributes", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"type": "Player", "name": "test"})");
    REQUIRE(parser.parse(json) == true);
}

TEST_CASE("JsonParser parses empty object", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json("{}");
    REQUIRE(parser.parse(json) == true);
}

TEST_CASE("JsonParser parses JSON with whitespace", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"(
        {
            "type" : "Enemy" ,
            "health" : "100"
        }
    )");
    REQUIRE(parser.parse(json) == true);
}

TEST_CASE("JsonParser handles escape sequences in values", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"message": "line1\nline2\ttab"})");
    REQUIRE(parser.parse(json) == true);
}

TEST_CASE("JsonParser handles escaped backslash", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"path": "C:\\Users\\test"})");
    REQUIRE(parser.parse(json) == true);
}

TEST_CASE("JsonParser handles escaped quotes", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"text": "He said \"hello\""})");
    REQUIRE(parser.parse(json) == true);
}

TEST_CASE("JsonParser fails on missing opening brace", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"("type": "Player"})");
    REQUIRE(parser.parse(json) == false);
}

TEST_CASE("JsonParser fails on missing closing brace", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"type": "Player")");
    REQUIRE(parser.parse(json) == false);
}

TEST_CASE("JsonParser fails on missing quotes around key", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({type: "Player"})");
    REQUIRE(parser.parse(json) == false);
}

TEST_CASE("JsonParser fails on missing quotes around value", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"health": 100})");
    REQUIRE(parser.parse(json) == false);
}

TEST_CASE("JsonParser fails on missing colon", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"type" "Player"})");
    REQUIRE(parser.parse(json) == false);
}

TEST_CASE("JsonParser fails on missing comma between pairs", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"type": "Player" "name": "test"})");
    REQUIRE(parser.parse(json) == false);
}

TEST_CASE("JsonParser parses multiple key-value pairs", "[JsonParser]")
{
    JsonParser parser;
    std::istringstream json(R"({"type": "NPC", "name": "Bob", "health": "50", "speed": "10"})");
    REQUIRE(parser.parse(json) == true);
}

TEST_CASE("JsonParser valueOf returns attribute value", "[JsonParser][valueOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"type": "Player", "name": "hero", "health": "100"})");
    REQUIRE(parser.parse(json) == true);
    REQUIRE(parser.valueOf("name") == "hero");
    REQUIRE(parser.valueOf("health") == "100");
}

TEST_CASE("JsonParser valueOf returns empty string for missing attribute", "[JsonParser][valueOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"type": "Player", "name": "hero"})");
    REQUIRE(parser.parse(json) == true);
    REQUIRE(parser.valueOf("missing") == "");
}

TEST_CASE("JsonParser valueOf does not return type as attribute", "[JsonParser][valueOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"type": "Player", "name": "hero"})");
    REQUIRE(parser.parse(json) == true);
    REQUIRE(parser.valueOf("type") == "");
}

TEST_CASE("JsonParser valueOf preserves escape sequences", "[JsonParser][valueOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"message": "line1\nline2", "path": "C:\\Users"})");
    REQUIRE(parser.parse(json) == true);
    REQUIRE(parser.valueOf("message") == "line1\nline2");
    REQUIRE(parser.valueOf("path") == "C:\\Users");
}

TEST_CASE("JsonParser parses entity with position and rotation", "[JsonParser][valueOf]")
{
    JsonParser parser;
    std::istringstream json(R"({
        "type": "Entity",
        "position": "10.5, 20.0, -5.25",
        "rotation": "0.707, 0.707"
    })");
    REQUIRE(parser.parse(json) == true);
    REQUIRE(parser.valueOf("position") == "10.5, 20.0, -5.25");
    REQUIRE(parser.valueOf("rotation") == "0.707, 0.707");
}

// Array tests

TEST_CASE("JsonParser parses simple array", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"assets": ["abc123", "def456", "ghi789"]})");
    REQUIRE(parser.parse(json) == true);
    auto assets = parser.arrayOf("assets");
    REQUIRE(assets.size() == 3);
    REQUIRE(assets[0] == "abc123");
    REQUIRE(assets[1] == "def456");
    REQUIRE(assets[2] == "ghi789");
}

TEST_CASE("JsonParser parses empty array", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"items": []})");
    REQUIRE(parser.parse(json) == true);
    auto items = parser.arrayOf("items");
    REQUIRE(items.empty());
}

TEST_CASE("JsonParser parses single element array", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"tags": ["single"]})");
    REQUIRE(parser.parse(json) == true);
    auto tags = parser.arrayOf("tags");
    REQUIRE(tags.size() == 1);
    REQUIRE(tags[0] == "single");
}

TEST_CASE("JsonParser parses array with whitespace", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({
        "values": [
            "first",
            "second",
            "third"
        ]
    })");
    REQUIRE(parser.parse(json) == true);
    auto values = parser.arrayOf("values");
    REQUIRE(values.size() == 3);
    REQUIRE(values[0] == "first");
    REQUIRE(values[1] == "second");
    REQUIRE(values[2] == "third");
}

TEST_CASE("JsonParser parses array with escape sequences", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"paths": ["C:\\Users", "line1\nline2", "tab\there"]})");
    REQUIRE(parser.parse(json) == true);
    auto paths = parser.arrayOf("paths");
    REQUIRE(paths.size() == 3);
    REQUIRE(paths[0] == "C:\\Users");
    REQUIRE(paths[1] == "line1\nline2");
    REQUIRE(paths[2] == "tab\there");
}

TEST_CASE("JsonParser parses mixed string values and arrays", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({
        "type": "Scene",
        "name": "level1",
        "assets": ["texture1", "model1", "sound1"]
    })");
    REQUIRE(parser.parse(json) == true);
    REQUIRE(parser.valueOf("name") == "level1");
    auto assets = parser.arrayOf("assets");
    REQUIRE(assets.size() == 3);
    REQUIRE(assets[0] == "texture1");
}

TEST_CASE("JsonParser arrayOf returns empty vector for missing attribute", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"name": "test"})");
    REQUIRE(parser.parse(json) == true);
    auto missing = parser.arrayOf("missing");
    REQUIRE(missing.empty());
}

TEST_CASE("JsonParser fails on array with missing quotes", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"items": [unquoted]})");
    REQUIRE(parser.parse(json) == false);
}

TEST_CASE("JsonParser fails on array with missing comma", "[JsonParser][arrayOf]")
{
    JsonParser parser;
    std::istringstream json(R"({"items": ["a" "b"]})");
    REQUIRE(parser.parse(json) == false);
}
