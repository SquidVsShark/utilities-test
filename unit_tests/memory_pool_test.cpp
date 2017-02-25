#include <catch/catch.hpp>
#include <utilities/memory_pool.hpp>
#include <cstring>


TEST_CASE("Memory Pool")
{
  lib::memory_pool pool = lib::memory_pool_create(1024);


  SECTION("Create a memory pool")
  {
    REQUIRE(pool.header->start_of_chunk != nullptr);
    REQUIRE(pool.header->size_of_chunk == (1024 - sizeof(lib::detail::memory_chunk_header)));
    REQUIRE(pool.header->next == nullptr);
    REQUIRE(pool.header->prev == nullptr);
    REQUIRE(pool.header->available == true);
  }


  SECTION("Get a successful chunk from the pool")
  {
    lib::memory_chunk chunk = lib::memory_pool_get_chunk(&pool, 512);

    REQUIRE(chunk.chunk_start != nullptr);
    REQUIRE(chunk.bytes_in_chunk == 512);

    lib::memory_chunk chunk_with_name = lib::memory_pool_get_chunk(&pool, 64, "foo");

    REQUIRE(!strcmp(chunk_with_name.name, "foo"));
  }


  SECTION("Ask for too much data from the pool")
  {
    lib::memory_chunk chunk = lib::memory_pool_get_chunk(&pool, 2048);

    REQUIRE(chunk.chunk_start == nullptr);
    REQUIRE(chunk.bytes_in_chunk == 0);
  }


  SECTION("Get a chunk and return it")
  {
    lib::memory_chunk chunk = lib::memory_pool_get_chunk(&pool, 512);
    REQUIRE(pool.header->available == false);
    REQUIRE(pool.header->next != nullptr);

    lib::memory_pool_return_chunk(&pool, &chunk);
    REQUIRE(pool.header->available == true);
    REQUIRE(pool.header->next == nullptr);
  }


  SECTION("Index and count")
  {
    REQUIRE(lib::memory_pool_get_number_of_chunks(&pool) == 1);

    lib::memory_chunk chunk = lib::memory_pool_get_chunk(&pool, 512, "bar");

    REQUIRE(lib::memory_pool_get_number_of_chunks(&pool) == 2);

    lib::memory_chunk by_index_1 = lib::memory_pool_get_chunk_by_index(&pool, 0);
    lib::memory_chunk by_index_2 = lib::memory_pool_get_chunk_by_index(&pool, 1);

    REQUIRE(!strcmp(by_index_1.name, "bar"));
    REQUIRE(!strcmp(by_index_2.name, "none"));
  }


  SECTION("Memory corruption")
  {
    constexpr size_t initial_alloc = 1 << 22;

    lib::memory_pool big_pool = lib::memory_pool_create(initial_alloc);

    constexpr size_t chunk_reserve_1 = 1 << 3;
    lib::memory_chunk chunk_a = lib::memory_pool_get_chunk(&big_pool, chunk_reserve_1, "bar");
    memset(chunk_a.chunk_start, 0, chunk_reserve_1);

    constexpr size_t chunk_reserve_2 = 1 << 17;
    lib::memory_chunk chunk_b = lib::memory_pool_get_chunk(&big_pool, chunk_reserve_2, "foo");
    memset(chunk_b.chunk_start, 0, chunk_reserve_2);

    uint32_t size = sizeof(lib::detail::memory_chunk_header);

    const size_t no_of_chunks = lib::memory_pool_get_number_of_chunks(&big_pool);
    const lib::memory_chunk chunk_end = lib::memory_pool_get_chunk_by_index(&big_pool, no_of_chunks - 1);

    REQUIRE(!strcmp(chunk_end.name, "none"));
  }


  SECTION("Run through")
  {
    lib::memory_chunk chunk_01 = lib::memory_pool_get_chunk(&pool, 64);
    lib::memory_chunk chunk_02 = lib::memory_pool_get_chunk(&pool, 512);
    lib::memory_chunk chunk_03 = lib::memory_pool_get_chunk(&pool, 512);
    lib::memory_chunk chunk_04 = lib::memory_pool_get_chunk(&pool, 32);

    lib::memory_pool_return_chunk(&pool, &chunk_03);

    lib::memory_chunk chunk_05 = lib::memory_pool_get_chunk(&pool, 512);
    lib::memory_chunk chunk_06 = lib::memory_pool_get_chunk(&pool, 256);

    lib::memory_pool_return_chunk(&pool, &chunk_05);
    lib::memory_pool_return_chunk(&pool, &chunk_04);
    lib::memory_pool_return_chunk(&pool, &chunk_02);
    lib::memory_pool_return_chunk(&pool, &chunk_01);

    REQUIRE(pool.header->available == true);
    REQUIRE(pool.header->next == nullptr);
  }

}
