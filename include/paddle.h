#pragma once

#include "cinder/gl/gl.h"


namespace gpong {

using glm::vec2;

/**
 * Class representing the paddle in Gump Pong.
 */
class Paddle {
 public:
  Paddle();

  explicit Paddle(int window_height);

  /**
   * Displays the paddle.
   */
  void Display() const;

  /**
   * Moves the paddle up on the screen.
   */
  void MoveUp();

  /**
   * Moves the paddle down on the screen.
   */
  void MoveDown();

  /**
   * Stops the paddle.
   */
  void Stop();

  vec2 GetPosition() const;

  float GetYRadius() const;

  int GetXRadius() const;

 private:
  static const ci::Color kColor;

  static const int kXRadius = 35;

  static const int kInitialYSpeed = 60;

  float y_radius_;

  vec2 velocity_;

  //Represents the center of the paddle
  vec2 position_;

};

} // namespace gpong
