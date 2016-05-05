#include <catch/catch.hpp>
#include <utilities/type_instance_id.hpp>
#include <array>


TEST_CASE("Type Instance ID")
{
  SECTION("Init ID")
  {
    const util::type_id null_id = util::type_id_invalid();

    REQUIRE(null_id.type == 0);
    REQUIRE(null_id.instance == 0);

    const util::type_id id = util::type_id_init(1,1);

    REQUIRE(id.type == 1);
    REQUIRE(id.instance == 1);
  }


  SECTION("To void* and back")
  {
    const util::type_id id = util::type_id_init(1,1);
    
    const void *user_data = util::type_id_to_ptr(id);
    const util::type_id user_id = util::type_id_from_ptr(user_data);

    REQUIRE(user_id.type == 1);
    REQUIRE(user_id.instance == 1);
  }


  SECTION("Operators")
  {
    const util::type_id id_1 = util::type_id_init(1,1);
    const util::type_id id_2 = util::type_id_init(1,1);
    const util::type_id id_3 = util::type_id_init(2,2);

    REQUIRE((id_1 == id_2));
    REQUIRE((id_1 != id_3));
    REQUIRE((id_1 < id_3));
  }


  SECTION("Linear search")
  {
    std::array<util::type_id, 10> ids;

    for(uint32_t i = 0; i < ids.size(); ++i)
    {
      ids.at(i) = util::type_id_init(i + 1, i + 1);
    }

    const util::type_id id_to_find = ids.at(ids.size() / 2);
    size_t index;

    REQUIRE(util::type_id_search_linearly(&index, id_to_find, ids.data(), ids.size()));
    REQUIRE(index == ids.size() / 2);
  }


  SECTION("Binary search")
  {
    std::array<util::type_id, 10> ids;

    for(uint32_t i = 0; i < ids.size(); ++i)
    {
      ids.at(i) = util::type_id_init(i + 1, i + 1);
    }

    const util::type_id id_to_find = ids.at(ids.size() / 2);
    size_t index;

    REQUIRE(util::type_id_search_binary(&index, id_to_find, ids.data(), ids.size()));
    REQUIRE(index == ids.size() / 2);
  }

}
