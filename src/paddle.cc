#include <paddle.h>

namespace gpong {

const ci::Color Paddle::kColor = ci::Color("white");

Paddle::Paddle() = default;

Paddle::Paddle(int window_height) {
  y_radius_ = (static_cast<float>(window_height) / 9);
  position_ = vec2(kXRadius, window_height / 2);
  velocity_ = vec2(0,0);
}

void Paddle::Display() const {
  ci::gl::color(kColor);

  vec2 upper = vec2(position_.x - kXRadius, position_.y - y_radius_);

  vec2 lower = vec2(position_.x + kXRadius, position_.y + y_radius_);

  ci::gl::drawSolidRect(ci::Rectf(upper, lower));
}

void Paddle::MoveUp() {
  velocity_ = vec2(velocity_.x, -kInitialYSpeed);
  position_ += velocity_;
}

void Paddle::MoveDown() {
  velocity_ = vec2(velocity_.x, kInitialYSpeed);
  position_ += velocity_;
}

vec2 Paddle::GetPosition() const {
  return position_;
}

int Paddle::GetXRadius() const {
  return kXRadius;
}

float Paddle::GetYRadius() const {
  return y_radius_;
}

void Paddle::Stop() {
  velocity_ = vec2(0,0);
}

} // namespace gpong
