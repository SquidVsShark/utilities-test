#include <catch/catch.hpp>
#include <utilities/generic_id.hpp>
#include <array>


TEST_CASE("Generic ID")
{
  SECTION("Init id")
  {
    const util::generic_id null_id = util::generic_id_invalid();

    REQUIRE(null_id == 0);
    REQUIRE(!util::generic_id_is_valid(null_id));

    const util::generic_id id = util::generic_id_init(123);

    REQUIRE(id == 123);
    REQUIRE(util::generic_id_is_valid(id));
  }


  SECTION("To void* and back")
  {
    const util::generic_id id = util::generic_id_init(123);

    const void *user_data = util::generic_id_to_ptr(id);
    const util::generic_id gen_id = util::generic_id_from_ptr(user_data);
     
    REQUIRE(gen_id == 123);
  }


  SECTION("Linear search")
  {
    std::array<util::generic_id, 10> ids;

    for(uint32_t i = 0; i < ids.size(); ++i)
    {
      ids.at(i) = util::generic_id_init(i);
    }

    const util::generic_id id_to_find = util::generic_id_init(ids.size() / 2);
    size_t index;

    REQUIRE(util::generic_id_search_linearly(&index, id_to_find, ids.data(), ids.size()));
    REQUIRE(index == ids.size() / 2);
  }


  SECTION("Binary search")
  {
    std::array<util::generic_id, 10> ids;

    for(uint32_t i = 0; i < ids.size(); ++i)
    {
      ids.at(i) = i;
    }

    const util::generic_id id_to_find = util::generic_id_init(ids.at(0));
    size_t index;

    REQUIRE(util::generic_id_search_binary(&index, id_to_find, ids.data(), ids.size()));
    REQUIRE(index == 0);
  }
}
