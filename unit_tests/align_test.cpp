#include <catch/catch.hpp>
#include <utilities/impl.hpp>
#include <utilities/alignment.hpp>


TEST_CASE("Alignment")
{
  const uintptr_t is_aligned_01 = 0;
  const uintptr_t is_aligned_02 = 16;

  REQUIRE(util::align::is_aligned_16(is_aligned_01));
  REQUIRE(util::align::is_aligned_16(is_aligned_02));

  const uintptr_t not_aligned_01 = 15;
  const uintptr_t not_aligned_02 = 17;

  REQUIRE(!util::align::is_aligned_16(not_aligned_01));
  REQUIRE(!util::align::is_aligned_16(not_aligned_02));

  REQUIRE(util::align::get_boundry_16(not_aligned_01) == 16);
  REQUIRE(util::align::get_boundry_16(not_aligned_02) == 32);
};
