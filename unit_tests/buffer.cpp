#include <catch/catch.hpp>

#define UTIL_BUFFER_IMPL
#include <utilities/buffer.hpp>


TEST_CASE("Buffer")
{

  util::buffer::data somebuffer;

  SECTION("Un-initialized buffer")
  {
    REQUIRE(util::buffer::size(&somebuffer) == 0);
    REQUIRE(util::buffer::capacity(&somebuffer) == 0);
    REQUIRE(util::buffer::empty(&somebuffer) == true);
  }

  SECTION("Initialize Buffer")
  {
    util::buffer::init(
      &somebuffer,
      sizeof(int),
      1
    );

    REQUIRE(util::buffer::size(&somebuffer) == 0);
    REQUIRE(util::buffer::capacity(&somebuffer) == 1);
    REQUIRE(util::buffer::empty(&somebuffer) != false);
  }

  SECTION("Push")
  {
    util::buffer::init(
      &somebuffer,
      sizeof(int),
      1
    );

    int *array = nullptr;

    util::buffer::push(&somebuffer);
    array = (int*)util::buffer::bytes(&somebuffer);
    array[0] = 1;

    REQUIRE(util::buffer::size(&somebuffer) == 1);
    REQUIRE(util::buffer::capacity(&somebuffer) == 1);

    util::buffer::push(&somebuffer);
    array = (int*)util::buffer::bytes(&somebuffer);
    array[1] = 2;

    REQUIRE(util::buffer::size(&somebuffer) == 2);
    REQUIRE(util::buffer::capacity(&somebuffer) == 2);

    util::buffer::push(&somebuffer);
    array = (int*)util::buffer::bytes(&somebuffer);
    array[2] = 3;

    REQUIRE(util::buffer::size(&somebuffer) == 3);
    REQUIRE(util::buffer::capacity(&somebuffer) == 4);

    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 2);
    REQUIRE(array[2] == 3);
  }

  SECTION("Resize")
  {
    util::buffer::init(
      &somebuffer,
      sizeof(int),
      1
    );

    REQUIRE(util::buffer::capacity(&somebuffer) == 1);

    util::buffer::resize(&somebuffer, 32);

    REQUIRE(util::buffer::capacity(&somebuffer) == 32);

    int *array = (int*)util::buffer::bytes(&somebuffer);
    for(uint32_t i = 0; i < 32; ++i)
    {
      array[i] = i;
    }

    util::buffer::resize(&somebuffer, 64);

    REQUIRE(util::buffer::capacity(&somebuffer) == 64);
    array = (int*)util::buffer::bytes(&somebuffer);

    for(uint32_t i = 0; i < 32; ++i)
    {
      REQUIRE(i == array[i]);
    }
  }

  SECTION("Reserve")
  {
    util::buffer::init(
      &somebuffer,
      sizeof(int),
      1
    );

    util::buffer::reserve(&somebuffer, 32);

    REQUIRE(util::buffer::capacity(&somebuffer) == 32);
    REQUIRE(util::buffer::size(&somebuffer) == 0);
  }

  SECTION("Insert")
  {
    util::buffer::init(
      &somebuffer,
      sizeof(int),
      4
    );

    util::buffer::push(&somebuffer);
    util::buffer::push(&somebuffer);
    util::buffer::push(&somebuffer);

    int *array = (int*)util::buffer::bytes(&somebuffer);
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;

    util::buffer::insert(&somebuffer, 1);
    array = (int*)util::buffer::bytes(&somebuffer);

    REQUIRE(array[0] == 1);
    REQUIRE(array[2] == 2);
    REQUIRE(array[3] == 3);
  }

  SECTION("Erase")
  {
    util::buffer::init(
      &somebuffer,
      sizeof(int),
      4
    );

    util::buffer::push(&somebuffer);
    util::buffer::push(&somebuffer);
    util::buffer::push(&somebuffer);

    int *array = (int*)util::buffer::bytes(&somebuffer);
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;

    util::buffer::erase(&somebuffer, 1);

    REQUIRE(array[1] == 3);
  }

  SECTION("clear")
  {
    util::buffer::init(
      &somebuffer,
      sizeof(int),
      4
    );

    util::buffer::push(&somebuffer);
    util::buffer::push(&somebuffer);

    REQUIRE(util::buffer::size(&somebuffer) == 2);

    util::buffer::clear(&somebuffer);

    REQUIRE(util::buffer::size(&somebuffer) == 0);
  }

  SECTION("last")
  {
    util::buffer::init(
      &somebuffer,
      sizeof(int),
      4
    );

    int *array = (int*)util::buffer::last(&somebuffer);

    REQUIRE(array == nullptr);

    util::buffer::push(&somebuffer);
    util::buffer::push(&somebuffer);
    util::buffer::push(&somebuffer);
    util::buffer::push(&somebuffer);
    array = (int*)util::buffer::last(&somebuffer);

    REQUIRE(array != nullptr);

    array = (int*)util::buffer::bytes(&somebuffer);
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    array[3] = 4;

    array = (int*)util::buffer::last(&somebuffer);
    REQUIRE(*array == 4);
  }
}
