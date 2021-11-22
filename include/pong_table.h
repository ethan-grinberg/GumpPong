#pragma once

#include <vector>

#include "ball.h"
#include "paddle.h"
#include "cinder/audio/audio.h"
#include "cinder/app/App.h"

namespace gpong {

using std::vector;

class PongTable {
 public:
  PongTable() = default;

  PongTable(int window_length, int window_height);

  //For testing
  PongTable(int window_length, int window_height, bool test);

  /**
   * Displays the pong table, including the image of forrest gump and line
   * down the middle.
   */
  void Display();

  /**
   * Updates the table by executing all the collisions.
   */
  void AdvanceOneFrame(vector<Ball>& pong_balls, const Paddle& paddle,
                       const bool spin);


 private:
  float x_boundary_;

  float y_boundary_;

  static const int kPaddleAcceleration = 7;

  static const int kSpin = 15;

  ci::gl::Texture2dRef gump_face_image_;

  bool test_;

  static const int kImageSize = 200;

  static const int kLineLength = 50;

  static const std::string kGumpUrl;

  static const std::string pong_sfx_file_;

  ci::audio::VoiceRef pong_sfx_;

  /**
  * Checks each ball to see if it collides with a wall or another ball.
  * If it does, it executes the appropriate collision.
  */
  void CheckCollisions(vector<Ball>& pong_balls, const Paddle& paddle,
                       const bool spin);

  /**
   * Returns true if two balls are moving towards each other.
   */
  bool CheckMovingTowards(const vec2& velocity1, const vec2& velocity2,
                          const vec2& position1, const vec2& position2) const;

  /**
   * Helper function to CheckCollisions(), if a ball collides with any of
   * the walls, the ball's corresponding velocity component is negated. The left
   * wall is ignored, this is the wall with the paddle.
   */
  void ExecuteWallCollision(Ball& ball, const bool spin);

  /**
   * Helper function to CheckCollisions(), decides if a ball collides with
   * any of the other ball, it updates each ball's
   * velocity appropriately.
   */
  void ExecuteBallCollision(const int current_ball, vector<Ball>& pong_balls);

  /**
   * Helper function to ExecuteParticleCollision(). It takes the two balls
   * that collided and updates their respective velocities after the collision.
   */
  void SetPostCollisionVelocities(Ball& ball1, Ball& ball2);

  /**
   * Returns the new velocity of a ball that just collided using the elastic
   * collision formula.
   */
  vec2 CalculateElasticCollision(const vec2& velocity1, const vec2& velocity2,
                                 const vec2& position1,
                                 const vec2& position2) const;

  /**
   * Executes a collision of the paddle and ping pong ball.
   */
  void ExecutePaddleCollision(Ball& ball, const Paddle& paddle, size_t position,
                              vector<Ball>& pong_balls);

  /**
   * Loads the ping pong sound effects.
   */
  void SetUpAudio();

  /**
   * Plays the ping pong sound effect.
   */
  void PlaySoundEffect();

  /**
   * Figures out the spin if, spin is true.
   */
  float GetSpin(float current_y);
};

} // namespace gpong
