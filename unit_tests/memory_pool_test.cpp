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
  
    util::memory_chunk chunk_with_name = util::memory_pool_get_chunk(&pool, 64, "foo");

    REQUIRE(!strcmp(chunk_with_name.name, "foo"));
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


  SECTION("Index and count")
  {
    REQUIRE(util::memory_pool_get_number_of_chunks(&pool) == 1);

    util::memory_chunk chunk = util::memory_pool_get_chunk(&pool, 512, "bar");

    REQUIRE(util::memory_pool_get_number_of_chunks(&pool) == 2);
 
    util::memory_chunk by_index_1 = util::memory_pool_get_chunk_by_index(&pool, 0);
    util::memory_chunk by_index_2 = util::memory_pool_get_chunk_by_index(&pool, 1);

    REQUIRE(!strcmp(by_index_1.name, "bar"));
    REQUIRE(!strcmp(by_index_2.name, "none"));
  }


  SECTION("Memory corruption")
  {
    util::memory_chunk chunk_a = util::memory_pool_get_chunk(&pool, 64, "bar");
    util::memory_chunk chunk_b = util::memory_pool_get_chunk(&pool, 64, "foo");
    util::memory_chunk chunk_c = util::memory_pool_get_chunk(&pool, 512, "boo");

    uint32_t size = sizeof(util::detail::memory_chunk_header);

    // If this memset wipes out the name of the next chunk we have an issue.
    memset(chunk_a.chunk_start, 1, 64);
    memset(chunk_b.chunk_start, 1, 64);
    memset(chunk_c.chunk_start, 1, 512);

    util::memory_chunk chunk_end = util::memory_pool_get_chunk_by_index(&pool, 3);

    REQUIRE(!strcmp(chunk_end.name, "none")); 
  }


  SECTION("Run through")
  {
    util::memory_chunk chunk_01 = util::memory_pool_get_chunk(&pool, 64);
    util::memory_chunk chunk_02 = util::memory_pool_get_chunk(&pool, 512);
    util::memory_chunk chunk_03 = util::memory_pool_get_chunk(&pool, 512);
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
