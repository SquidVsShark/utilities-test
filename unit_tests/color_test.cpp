#include <catch/catch.hpp>
#include <utilities/color.hpp>
#include <algorithm>
#include <array>


TEST_CASE("colors")
{
  SECTION("Init with Hex")
  {
    const util::color::color color_1 = util::color::init(0xFFFFFFFF);

    REQUIRE(util::color::get_channel_1i(color_1) == 255);
    REQUIRE(util::color::get_channel_2i(color_1) == 255);
    REQUIRE(util::color::get_channel_3i(color_1) == 255);
    REQUIRE(util::color::get_channel_4i(color_1) == 255);

    const util::color::color color_2 = util::color::init(0xFFCC3300);

    REQUIRE(util::color::get_channel_1i(color_2) == 255);
    REQUIRE(util::color::get_channel_2i(color_2) == 204);
    REQUIRE(util::color::get_channel_3i(color_2) == 51);
    REQUIRE(util::color::get_channel_4i(color_2) == 0);
  }

  SECTION("Init integer with components")
  {
    const std::array<uint8_t, 4> color_data = {255,128,64,32};

    const util::color::color color_1 = util::color::init(color_data.at(0), color_data.at(1), color_data.at(2), color_data.at(3));

    REQUIRE(util::color::get_channel_1i(color_1) == color_data.at(0));
    REQUIRE(util::color::get_channel_2i(color_1) == color_data.at(1));
    REQUIRE(util::color::get_channel_3i(color_1) == color_data.at(2));
    REQUIRE(util::color::get_channel_4i(color_1) == color_data.at(3));

    const util::color::color color_2 = util::color::init(color_data.data());

    REQUIRE(util::color::get_channel_1i(color_2) == (int32_t)color_data.at(0));
    REQUIRE(util::color::get_channel_2i(color_2) == (int32_t)color_data.at(1));
    REQUIRE(util::color::get_channel_3i(color_2) == (int32_t)color_data.at(2));
    REQUIRE(util::color::get_channel_4i(color_2) == (int32_t)color_data.at(3));
  }

  SECTION("Init with float components")
  {
    auto is_near= [](const auto a, const auto b) -> bool
    {
      return std::abs(std::max(a,b) - std::min(a,b)) < 0.01;
    };

    const std::array<float, 4> color_data = {1.f, 0.5f, 0.25f, 0.125f};

    const util::color::color color_1 = util::color::init(color_data.at(0), color_data.at(1), color_data.at(2), color_data.at(3));

    REQUIRE(is_near(util::color::get_channel_1f(color_1), color_data.at(0)));
    REQUIRE(is_near(util::color::get_channel_2f(color_1), color_data.at(1)));
    REQUIRE(is_near(util::color::get_channel_3f(color_1), color_data.at(2)));
    REQUIRE(is_near(util::color::get_channel_4f(color_1), color_data.at(3)));

    const util::color::color color_2 = util::color::init(color_data.data());

    REQUIRE(is_near(util::color::get_channel_1f(color_1), color_data.at(0)));
    REQUIRE(is_near(util::color::get_channel_2f(color_1), color_data.at(1)));
    REQUIRE(is_near(util::color::get_channel_3f(color_1), color_data.at(2)));
    REQUIRE(is_near(util::color::get_channel_4f(color_1), color_data.at(3)));
  }
}
