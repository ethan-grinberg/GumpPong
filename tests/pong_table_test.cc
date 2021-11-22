#include <catch2/catch.hpp>

#include <ball.h>
#include <pong_table.h>

using std::vector;
using gpong::Ball;
using gpong::PongTable;
using gpong::Paddle;

TEST_CASE("Wall collisions") {
  Paddle paddle(1000);

  SECTION("Right vertical wall collision") {
    vec2 position = vec2(950, 500);
    vec2 velocity = vec2(30,0);

    Ball ball(position);
    ball.SetVelocity(velocity);
    vector<Ball> balls(1, ball);

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);

    REQUIRE(balls.at(0).GetVelocity() == vec2(-37,0));
  }

  SECTION("Upper horizontal wall collision") {
    vec2 position = vec2(500, 50);
    vec2 velocity = vec2(30,-30);

    Ball ball(position);
    ball.SetVelocity(velocity);
    vector<Ball> balls(1, ball);

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);

    REQUIRE(balls.at(0).GetVelocity() == vec2(30,30));
  }

  SECTION("Bottom horizontal wall collision") {
    vec2 position = vec2(500, 950);
    vec2 velocity = vec2(30,30);

    Ball ball(position);
    ball.SetVelocity(velocity);
    vector<Ball> balls(1, ball);

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);

    REQUIRE(balls.at(0).GetVelocity() == vec2(30,-30));
  }

  SECTION("Corner of horizontal and vertical wall collision") {
    vec2 position = vec2(950, 950);
    vec2 velocity = vec2(30,30);

    Ball ball(position);
    ball.SetVelocity(velocity);
    vector<Ball> balls(1, ball);

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);

    REQUIRE(balls.at(0).GetVelocity() == vec2(-37, 30));
  }

  SECTION("Particle is touching wall but not moving towards it") {
    vec2 position = vec2(950, 500);
    vec2 velocity = vec2(-30,30);

    Ball ball(position);
    ball.SetVelocity(velocity);
    vector<Ball> balls(1, ball);

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);

    REQUIRE(balls.at(0).GetVelocity() == vec2(-30,30));
  }
}

TEST_CASE("Particle collisions") {
  Paddle paddle(1000);
  SECTION("Horizontal head on collision") {
    vec2 position1 = vec2(500, 500);
    vec2 velocity1 = vec2(30, 0);

    vec2 position2 = vec2(600, 500);
    vec2 velocity2 = vec2(-30, 0);

    Ball ball1(position1);
    ball1.SetVelocity(velocity1);

    Ball ball2(position2);
    ball2.SetVelocity(velocity2);

    vector<Ball> balls = {ball1, ball2};

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);
    REQUIRE(balls.at(0).GetVelocity().x == Approx(-30));
    REQUIRE(balls.at(0).GetVelocity().y == Approx(0));
    REQUIRE(balls.at(1).GetVelocity().x == Approx(30));
    REQUIRE(balls.at(1).GetVelocity().y == Approx(0));
  }

  SECTION("Diagonal particle velocities") {
    vec2 position1 = vec2(500, 500);
    vec2 velocity1 = vec2(30, 30);

    vec2 position2 = vec2(550, 550);
    vec2 velocity2 = vec2(-30, -30);

    Ball ball1(position1);
    ball1.SetVelocity(velocity1);

    Ball ball2(position2);
    ball2.SetVelocity(velocity2);

    vector<Ball> balls = {ball1, ball2};

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);
    REQUIRE(balls.at(0).GetVelocity().x == Approx(-30));
    REQUIRE(balls.at(0).GetVelocity().y == Approx(-30));
    REQUIRE(balls.at(1).GetVelocity().x == Approx(30));
    REQUIRE(balls.at(1).GetVelocity().y == Approx(30));
  }

  SECTION("Multiple pairs of different collisions") {
    vec2 position1 = vec2(500, 700);
    vec2 velocity1 = vec2(30, 0);

    vec2 position2 = vec2(600, 700);
    vec2 velocity2 = vec2(-30, 0);

    vec2 position3 = vec2(500, 200);
    vec2 velocity3 = vec2(30, 0);

    vec2 position4 = vec2(600, 200);
    vec2 velocity4 = vec2(-30, 0);

    Ball ball1(position1);
    ball1.SetVelocity(velocity1);

    Ball ball2(position2);
    ball2.SetVelocity(velocity2);

    Ball ball3(position3);
    ball3.SetVelocity(velocity3);

    Ball ball4(position4);
    ball4.SetVelocity(velocity4);

    vector<Ball> balls = {ball1, ball2, ball3, ball4};

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);
    REQUIRE(balls.at(0).GetVelocity().x == Approx(-30));
    REQUIRE(balls.at(0).GetVelocity().y == Approx(0));
    REQUIRE(balls.at(1).GetVelocity().x == Approx(30));
    REQUIRE(balls.at(1).GetVelocity().y == Approx(0));
    REQUIRE(balls.at(2).GetVelocity().x == Approx(-30));
    REQUIRE(balls.at(2).GetVelocity().y == Approx(0));
    REQUIRE(balls.at(3).GetVelocity().x == Approx(30));
    REQUIRE(balls.at(3).GetVelocity().y == Approx(0));

  }

  SECTION("Particles touching but not going towards each other") {
    vec2 position1 = vec2(500, 500);
    vec2 velocity1 = vec2(30, 0);

    vec2 position2 = vec2(600, 500);
    vec2 velocity2 = vec2(30, 0);

    Ball ball1(position1);
    ball1.SetVelocity(velocity1);

    Ball ball2(position2);
    ball2.SetVelocity(velocity2);

    vector<Ball> balls = {ball1, ball2};

    PongTable table(1000, 1000, true);

    table.AdvanceOneFrame(balls, paddle, false);
    REQUIRE(balls.at(0).GetVelocity() == vec2(30, 0));
    REQUIRE(balls.at(1).GetVelocity() == vec2(30, 0));
  }
}