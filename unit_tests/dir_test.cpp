#include <catch/catch.hpp>
#include <utilities/impl.hpp>
#include <utilities/directory.hpp>
#include <stdio.h>

TEST_CASE("Dir Test")
{
  // Hard to test this other than make sure it returns something.
  REQUIRE(strlen(util::dir::exe_path()) > 0);
  // printf("Dir: %s\n", util::dir::exe_path());
}
