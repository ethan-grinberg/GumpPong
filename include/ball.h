#pragma once

#include "cinder/gl/gl.h"
#include <cstdlib>

using glm::vec2;

namespace gpong {

/**
 * Class representing a ping pong ball.
 */
class Ball {

 public:
  Ball(int window_length, int window_height);

  //for testing
  Ball(vec2& position);

  void DrawBall() const;

  void SetVelocity(const vec2& velocity);

  vec2 GetPosition() const;

  vec2 GetVelocity() const;

  ci::Color GetColor() const;

  float GetRadius() const;

  /**
   * Updates a ball's position using its velocity and previous position. If
   * it's velocity is too high, it will decelerate. If it is too slow it will
   * accelerate.
   */
  void MoveBall();

 private:
  static const int kMaxVel = 55;

  static const int kMaxYVel = 45;

  static const int kMinVel = 30;

  static const float kRadius;

  static const ci::Color kColor;

  static const vec2 kStartingVel;

  static const int kDeceleration = 8;

  static const std::string kGumpUrl;

  vec2 velocity_;

  vec2 position_;

  ci::gl::Texture2dRef gump_face_image_;

  float image_bound_;

  static const int kImageOffset = 15;
};

}  // namespace gpong