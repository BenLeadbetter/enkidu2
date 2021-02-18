#include <catch2/catch.hpp>

#include <utility/flatten_variants.hpp>
#include <utility/is_one_of.hpp>
#include <utility/registry.hpp>

#include <type_traits>
#include <variant>

using namespace enkidu::utility;

TEST_CASE("is_one_of test")
{
    REQUIRE(!is_one_of<int, char, float, void*>::value);
    REQUIRE(is_one_of<int, char, float, void*, int>::value);
    REQUIRE(!is_one_of<const int, char, float, void*, int>::value);

    struct TypeA{};
    struct TypeB{};
    struct TypeC{};
    struct TypeD{};

    REQUIRE(!is_one_of<TypeA, TypeB, TypeC, TypeD>::value);
    REQUIRE(is_one_of<TypeA, TypeB, TypeA, TypeC, TypeD>::value);
    REQUIRE(!is_one_of<const TypeA, TypeB, TypeA, TypeC, TypeD>::value);
    REQUIRE(is_one_of<const TypeA, TypeB, const TypeA, TypeC, TypeD>::value);
    REQUIRE(!is_one_of<TypeA*, TypeB, TypeA, TypeC, TypeD>::value);
    REQUIRE(!is_one_of<TypeA&, TypeB, TypeA, TypeC, TypeD>::value);
    REQUIRE(!is_one_of<const TypeA&, TypeB, TypeA&, TypeC, TypeD>::value);
}

TEST_CASE("registry test")
{
    GIVEN("a registry of items")
    {
        registry<std::function<void()>> reg;

        WHEN("add an item to the registry")
        {
            int count{0};
            const auto func = [&count](){ ++count; };
            const auto exe = [](const auto& f) { f(); };
            auto id = reg.add(func);

            WHEN("apply a function to each item")
            {
                reg.for_each(exe);

                THEN("item is visited")
                {
                    REQUIRE(count == 1);
                }
            }

            WHEN("remove the item from the registry")
            {
                reg.remove(id);

                WHEN("apply function to each item")
                {
                    reg.for_each(exe);

                    THEN("item is not visited")
                    {
                        REQUIRE(count == 0);
                    }
                }
            }
        }
    }
}

TEST_CASE("flatten_variants test")
{
    struct TypeA{};
    struct TypeB{};
    struct TypeC{};
    struct TypeD{};
    struct TypeE{};

    using VariantAB = std::variant<TypeA, TypeB>;
    using VariantCDE = std::variant<TypeC, TypeD, TypeE>;

    REQUIRE(std::is_same<
                flatten_variants<VariantAB, VariantCDE>::type,
                std::variant<TypeA, TypeB, TypeC, TypeD, TypeE>>::value);
}
