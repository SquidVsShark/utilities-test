#include <catch/catch.hpp>
#include <utilities/convert.hpp>


TEST_CASE("Conversions")
{
  SECTION("MB to Bytes")
  {
    REQUIRE(util::convert::mb_to_bytes(1) == 1048576);
    REQUIRE(util::convert::mb_to_bytes(2) == 1048576 * 2);
  }
}
