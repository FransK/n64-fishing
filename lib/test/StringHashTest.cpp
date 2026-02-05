#include <catch2/catch_test_macros.hpp>
#include "StringHash.h"

TEST_CASE("StringHash produces deterministic output", "[StringHash]")
{
    std::string input = "hello";
    auto hash1 = StringHash::hash(input);
    auto hash2 = StringHash::hash(input);
    REQUIRE(hash1 == hash2);
}

TEST_CASE("StringHash produces different hashes for different strings", "[StringHash]")
{
    auto hash1 = StringHash::hash("hello");
    auto hash2 = StringHash::hash("world");
    REQUIRE(hash1 != hash2);
}

TEST_CASE("StringHash handles empty string", "[StringHash]")
{
    auto hash = StringHash::hash("");
    REQUIRE(hash == 5381); // DJB2 initial value unchanged
}

TEST_CASE("StringHash handles single character", "[StringHash]")
{
    auto hashA = StringHash::hash("a");
    auto hashB = StringHash::hash("b");
    REQUIRE(hashA != hashB);
}

TEST_CASE("StringHash handles longer strings", "[StringHash]")
{
    std::string longString = "The quick brown fox jumps over the lazy dog";
    auto hash1 = StringHash::hash(longString);
    auto hash2 = StringHash::hash(longString);
    REQUIRE(hash1 == hash2);

    // Slightly different string should produce different hash
    auto hash3 = StringHash::hash("The quick brown fox jumps over the lazy cat");
    REQUIRE(hash1 != hash3);
}
