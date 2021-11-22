#include <ball.h>

namespace gpong {

const float Ball::kRadius = 70.0;

const ci::Color Ball::kColor = ci::Color("white");
const std::string Ball::kGumpUrl =
    "https://i.pinimg.com/originals/32/83/2a/"
    "32832af61cf01dc2acd6077aa9515be6.png";

const vec2 Ball::kStartingVel = vec2(kMinVel, 7);

Ball::Ball(vec2& position) {
  position_ = position;
}

Ball::Ball(int window_length, int window_height) {
  position_ = vec2((window_length / 2), (window_height / 2));
  image_bound_ = kRadius - kImageOffset;

  //Randomize direction
  int rand_int = rand() % 2 + 1;
  if (rand_int == 1) {
    velocity_ = vec2(kStartingVel.x, -kStartingVel.y);
  } else {
    velocity_ = kStartingVel;
  }

  gump_face_image_ = ci::gl::Texture2d::create(
      ci::loadImage(ci::loadUrl(kGumpUrl)));
}

void Ball::SetVelocity(const vec2& velocity) {
  velocity_ = velocity;
}

vec2 Ball::GetPosition() const {
  return position_;
}

vec2 Ball::GetVelocity() const {
  return velocity_;
}

ci::Color Ball::GetColor() const {
  return kColor;
}

float Ball::GetRadius() const {
  return kRadius;
}

void Ball::MoveBall() {
  if (std::abs(velocity_.x) >= kMaxVel) {
    if (velocity_.x > 0) {
      velocity_ = vec2(kMaxVel - kDeceleration, velocity_.y);
    } else {
      velocity_ = vec2(-kMaxVel + kDeceleration, velocity_.y);
    }
  }

  if (std::abs(velocity_.x) < kMinVel) {
    if (velocity_.x > 0) {
      velocity_ = vec2(kMinVel, velocity_.y);
    } else {
      velocity_ = vec2(-kMinVel, velocity_.y);
    }
  }

  //Checks if it's going too fast in y direction
  if (std::abs(velocity_.y) >= kMaxYVel) {
    if (velocity_.y > 0) {
      velocity_ = vec2(velocity_.x, kMaxYVel - kDeceleration);
    } else {
      velocity_ = vec2(velocity_.x, -kMaxYVel + kDeceleration);
    }
  }
  position_ += velocity_;
}

void Ball::DrawBall() const {
  ci::gl::color(ci::Color(kColor));
  ci::gl::drawSolidCircle(position_, kRadius);

  ci::Rectf pos(vec2(position_.x - image_bound_, position_.y - image_bound_),
                vec2(position_.x + image_bound_, position_.y + image_bound_));

  ci::gl::draw(gump_face_image_, pos);
}

} // namespace gpong
