#include <catch/catch.hpp>
#include <utilities/memory_pool.hpp>


TEST_CASE("Memory Pool")
{
  util::memory_pool pool = util::memory_pool_create(1024);


  SECTION("Create a memory pool")
  { 
    REQUIRE(pool.header->start_of_chunk != nullptr);
    REQUIRE(pool.header->size_of_chunk == (1024 - sizeof(util::detail::memory_chunk_header)));
    REQUIRE(pool.header->next == nullptr);
    REQUIRE(pool.header->prev == nullptr);
    REQUIRE(pool.header->available == true);
  }


  SECTION("Get a successful chunk from the pool")
  {
    util::memory_chunk chunk = util::memory_pool_get_chunk(&pool, 512);

    REQUIRE(chunk.chunk_start != nullptr);
    REQUIRE(chunk.bytes_in_chunk == 512);
  }

  
  SECTION("Ask for too much data from the pool")
  { 
    util::memory_chunk chunk = util::memory_pool_get_chunk(&pool, 2048);

    REQUIRE(chunk.chunk_start == nullptr);
    REQUIRE(chunk.bytes_in_chunk == 0);
  }

  
  SECTION("Get a chunk and return it")
  {
    util::memory_chunk chunk = util::memory_pool_get_chunk(&pool, 512);
    REQUIRE(pool.header->available == false);
    REQUIRE(pool.header->next != nullptr);
    
    util::memory_pool_return_chunk(&pool, &chunk);
    REQUIRE(pool.header->available == true);
    REQUIRE(pool.header->next == nullptr);
  }


  SECTION("Run through")
  {
    util::memory_chunk chunk_01 = util::memory_pool_get_chunk(&pool, 64);
    util::memory_chunk chunk_02 = util::memory_pool_get_chunk(&pool, 512);
    util::memory_chunk chunk_03 = util::memory_pool_get_chunk(&pool, 1024);
    util::memory_chunk chunk_04 = util::memory_pool_get_chunk(&pool, 32);

    util::memory_pool_return_chunk(&pool, &chunk_03);

    util::memory_chunk chunk_05 = util::memory_pool_get_chunk(&pool, 512);
    util::memory_chunk chunk_06 = util::memory_pool_get_chunk(&pool, 256);
  
    util::memory_pool_return_chunk(&pool, &chunk_05);
    util::memory_pool_return_chunk(&pool, &chunk_04);
    util::memory_pool_return_chunk(&pool, &chunk_02);
    util::memory_pool_return_chunk(&pool, &chunk_01);

    REQUIRE(pool.header->available == true);
    REQUIRE(pool.header->next == nullptr);
  }

}
