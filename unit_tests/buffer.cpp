#include <catch/catch.hpp>

#define UTIL_BUFFER_IMPL
#include <utilities/buffer.hpp>


TEST_CASE("Buffer")
{

  buffer::buffer somebuffer;

  SECTION("Un-initialized buffer")
  {
    REQUIRE(buffer::size(&somebuffer) == 0);
    REQUIRE(buffer::capacity(&somebuffer) == 0); 
    REQUIRE(buffer::empty(&somebuffer) == true);
  }

  SECTION("Initialize Buffer")
  {
    buffer::init(
      &somebuffer,
      sizeof(int),
      1
    );

    REQUIRE(buffer::size(&somebuffer) == 0);
    REQUIRE(buffer::capacity(&somebuffer) == 1); 
    REQUIRE(buffer::empty(&somebuffer) != false);
  }

  SECTION("Push")
  {
    buffer::init(
      &somebuffer,
      sizeof(int),
      1
    );

    int *array = nullptr;

    buffer::push(&somebuffer);
    array = (int*)buffer::data(&somebuffer);
    array[0] = 1;

    REQUIRE(buffer::size(&somebuffer) == 1);
    REQUIRE(buffer::capacity(&somebuffer) == 1);

    buffer::push(&somebuffer);
    array = (int*)buffer::data(&somebuffer);
    array[1] = 2;

    REQUIRE(buffer::size(&somebuffer) == 2);
    REQUIRE(buffer::capacity(&somebuffer) == 2);

    buffer::push(&somebuffer);
    array = (int*)buffer::data(&somebuffer);
    array[2] = 3;

    REQUIRE(buffer::size(&somebuffer) == 3);
    REQUIRE(buffer::capacity(&somebuffer) == 4);

    REQUIRE(array[0] == 1);
    REQUIRE(array[1] == 2);
    REQUIRE(array[2] == 3);
  }

  SECTION("Resize")
  {
    buffer::init(
      &somebuffer,
      sizeof(int),
      1
    );
    
    REQUIRE(buffer::capacity(&somebuffer) == 1);

    buffer::resize(&somebuffer, 32);

    REQUIRE(buffer::capacity(&somebuffer) == 32);

    int *array = (int*)buffer::data(&somebuffer);
    for(uint32_t i = 0; i < 32; ++i)
    {
      array[i] = i;
    }

    buffer::resize(&somebuffer, 64);

    REQUIRE(buffer::capacity(&somebuffer) == 64);
    array = (int*)buffer::data(&somebuffer);

    for(uint32_t i = 0; i < 32; ++i)
    {
      REQUIRE(i == array[i]);   
    }   
  }

  SECTION("Reserve")
  {
    buffer::init(
      &somebuffer,
      sizeof(int),
      1
    );

    buffer::reserve(&somebuffer, 32);

    REQUIRE(buffer::capacity(&somebuffer) == 32);
    REQUIRE(buffer::size(&somebuffer) == 0);
  }

  SECTION("Insert")
  {
    buffer::init(
      &somebuffer,
      sizeof(int),
      4
    );

    buffer::push(&somebuffer);
    buffer::push(&somebuffer);
    buffer::push(&somebuffer);

    int *array = (int*)buffer::data(&somebuffer);
    array[0] = 1;
    array[1] = 2;
    array[2] = 3; 
  
    buffer::insert(&somebuffer, 1);
    array = (int*)buffer::data(&somebuffer);

    REQUIRE(array[0] == 1);
    REQUIRE(array[2] == 2);
    REQUIRE(array[3] == 3);
  }

  SECTION("Erase")
  {
    buffer::init(
      &somebuffer,
      sizeof(int),
      4
    );

    buffer::push(&somebuffer);
    buffer::push(&somebuffer);
    buffer::push(&somebuffer);

    int *array = (int*)buffer::data(&somebuffer);
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
  
    buffer::erase(&somebuffer, 1);

    REQUIRE(array[1] == 3);
  }

  SECTION("clear")
  {
    buffer::init(
      &somebuffer,
      sizeof(int),
      4
    );

    buffer::push(&somebuffer);
    buffer::push(&somebuffer);

    REQUIRE(buffer::size(&somebuffer) == 2);

    buffer::clear(&somebuffer);

    REQUIRE(buffer::size(&somebuffer) == 0);
  }
}

