#include <catch2/catch.hpp>
#include <ball.h>

using gpong::Ball;

TEST_CASE("Basic particle movement") {
  vec2 position = vec2(1000,1000);
  SECTION("Horizontal negative velocity") {
    vec2 velocity = vec2(-30,0);

    Ball ball(position);

    ball.SetVelocity(velocity);
    ball.MoveBall();
    REQUIRE(ball.GetPosition() == vec2(970.000, 1000.000));
  }

  SECTION("Horizontal positive velocity") {
    vec2 velocity = vec2(30,0);

    Ball ball(position);

    ball.SetVelocity(velocity);
    ball.MoveBall();
    REQUIRE(ball.GetPosition() == vec2(1030, 1000));
  }

  SECTION("Vertical positive velocity") {
    vec2 velocity = vec2(30,30);

    Ball ball(position);

    ball.SetVelocity(velocity);
    ball.MoveBall();
    REQUIRE(ball.GetPosition() == vec2(1030,1030));
  }

  SECTION("Diagonal velocity") {
    vec2 velocity = vec2(30, -30);

    Ball ball(position);

    ball.SetVelocity(velocity);
    ball.MoveBall();
    REQUIRE(ball.GetPosition() == vec2(1030, 970));
  }
}
