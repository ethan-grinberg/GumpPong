#include <pong_table.h>

namespace gpong {

const std::string PongTable::pong_sfx_file_ = "pong-ball.wav";
const std::string PongTable::kGumpUrl =
    "https://i.pinimg.com/originals/32/83/2a/"
    "32832af61cf01dc2acd6077aa9515be6.png";

PongTable::PongTable(int window_length, int window_height) {
  x_boundary_ = static_cast<float>(window_length);
  y_boundary_ = static_cast<float>(window_height);
  test_ = false;

  gump_face_image_ =
      ci::gl::Texture2d::create(ci::loadImage(ci::loadUrl(kGumpUrl)));
  SetUpAudio();
}

PongTable::PongTable(int window_length, int window_height, bool test) {
  test_ = test;
  x_boundary_ = static_cast<float>(window_length);
  y_boundary_ = static_cast<float>(window_height);;
}

void PongTable::AdvanceOneFrame(vector<Ball>& pong_balls,
                                const Paddle& paddle, const bool spin) {
  CheckCollisions(pong_balls, paddle, spin);
  for (Ball& ball : pong_balls) {
    ball.MoveBall();
  }
}

void PongTable::CheckCollisions(vector<Ball>& pong_balls, const Paddle& paddle,
                                const bool spin) {
  for (size_t current_ball = 0; current_ball < pong_balls.size();
       current_ball++) {
    ExecuteWallCollision(pong_balls[current_ball], spin);
    ExecutePaddleCollision(pong_balls[current_ball], paddle, current_ball,
                           pong_balls);

    // This prevents the last ball to be compared with itself
    if (current_ball == pong_balls.size() - 1) {
      return;
    }
    ExecuteBallCollision(current_ball, pong_balls);
  }
}

float PongTable::GetSpin(float current_y) {
  float new_y = 0;
  if ((current_y > 0) || (current_y == 0)) {
    new_y = current_y + kSpin;
  } else if (current_y < 0) {
    new_y = current_y - kSpin;
  }
  return new_y;
}

void PongTable::ExecuteWallCollision(Ball& ball, const bool spin) {
  vec2 ball_position = ball.GetPosition();
  float ball_radius = ball.GetRadius();
  vec2 ball_velocity = ball.GetVelocity();

  vec2 new_velocity;
  vec2 wall_velocity = vec2(0, 0);
  vec2 wall_position;

  // Checks if the particle bounced off a right vertical wall.
  if (ball_position.x + ball_radius >= x_boundary_) {
    wall_position = vec2(x_boundary_, ball_position.y);

    if (CheckMovingTowards(ball_velocity, wall_velocity, ball_position,
                           wall_position)) {

      //adds spin if necessary
      float y_component = ball_velocity.y;
      if (spin) {
        y_component = GetSpin(y_component);
      }


      //Accelerates off of the right wall
      new_velocity = vec2(-ball_velocity.x - kPaddleAcceleration, y_component);
      if (!test_) {
        PlaySoundEffect();
      }

      // Sets particles new velocity
      ball.SetVelocity(new_velocity);
      return;
    }
  }

  // Checks if particle hit bottom y boundary
  if (ball.GetPosition().y + ball.GetRadius() >= y_boundary_) {
    wall_position = vec2(ball_position.x, y_boundary_);

    if (CheckMovingTowards(ball_velocity, wall_velocity, ball_position,
                           wall_position)) {
      new_velocity = vec2(ball_velocity.x, -ball_velocity.y);

      // Sets particles new velocity
      ball.SetVelocity(new_velocity);
    }

    // Checks if particle hits top y boundary
  } else if (ball.GetPosition().y - ball.GetRadius() <= 0) {
    wall_position = vec2(ball_position.x, 0);

    if (CheckMovingTowards(ball_velocity, wall_velocity, ball_position,
                           wall_position)) {
      new_velocity = vec2(ball_velocity.x, -ball_velocity.y);

      // Sets particles new velocity
      ball.SetVelocity(new_velocity);
    }
  }
}

void PongTable::ExecuteBallCollision(const int current_ball,
                                         vector<Ball>& pong_balls) {
  for (size_t comparing_ball = current_ball + 1;
       comparing_ball < pong_balls.size(); comparing_ball++) {

    vec2 ball_position = pong_balls[current_ball].GetPosition();
    vec2 ball_velocity = pong_balls[current_ball].GetVelocity();
    float ball_radius = pong_balls[current_ball].GetRadius();

    vec2 comparing_particle_position =
        pong_balls[comparing_ball].GetPosition();

    vec2 comparing_particle_velocity =
        pong_balls[comparing_ball].GetVelocity();

    float comparing_particle_radius =
        pong_balls[comparing_ball].GetRadius();

    float distance_between_particles =
        glm::distance(ball_position, comparing_particle_position);

    float sum_of_particle_radii = ball_radius + comparing_particle_radius;

    // Collision occurred if the distance between
    // particles is less than the sum of their radii.
    if (distance_between_particles <= sum_of_particle_radii) {
      // Only updates particle velocities if
      // they are also moving towards each other.
      if (CheckMovingTowards(ball_velocity, comparing_particle_velocity,
                             ball_position, comparing_particle_position)) {
        SetPostCollisionVelocities(pong_balls[current_ball],
                                   pong_balls[comparing_ball]);
      }
    }
  }
}

bool PongTable::CheckMovingTowards(const vec2& velocity1,
                                      const vec2& velocity2,
                                      const vec2& position1,
                                      const vec2& position2) const {
  vec2 relative_velocities = velocity1 - velocity2;
  vec2 relative_positions = position1 - position2;
  float dot_product = glm::dot(relative_velocities, relative_positions);

  return dot_product < 0;
}

void PongTable::SetPostCollisionVelocities(Ball& ball1, Ball& ball2) {
  vec2 ball1_position = ball1.GetPosition();
  vec2 ball1_velocity = ball1.GetVelocity();

  vec2 ball2_position = ball2.GetPosition();
  vec2 ball2_velocity = ball2.GetVelocity();

  vec2 ball1_new_velocity = CalculateElasticCollision(
      ball1_velocity, ball2_velocity, ball1_position,
      ball2_position);

  // Have to switch which one is particle 1 and 2 for the equation
  vec2 ball2_new_velocity = CalculateElasticCollision(
      ball2_velocity, ball1_velocity, ball2_position,
      ball1_position);

  ball1.SetVelocity(ball1_new_velocity);
  ball2.SetVelocity(ball2_new_velocity);
}

vec2 PongTable::CalculateElasticCollision(const vec2& velocity1,
                                          const vec2& velocity2,
                                          const vec2& position1,
                                          const vec2& position2) const {

  vec2 difference_in_position = position1 - position2;
  vec2 difference_in_velocity = velocity1 - velocity2;

  float d_prod_velocity_position =
      glm::dot(difference_in_velocity, difference_in_position);

  float diff_position_squared =
      std::pow(glm::length(difference_in_position), 2);

  return velocity1 - ((d_prod_velocity_position / (diff_position_squared)) *
                     difference_in_position);
}

void PongTable::ExecutePaddleCollision(Ball& ball, const Paddle& paddle,
                                       size_t position,
                                       vector<Ball>& pong_balls) {
  float paddle_x_bound =
      paddle.GetPosition().x + static_cast<float>(paddle.GetXRadius());
  float paddle_start = paddle.GetPosition().y - paddle.GetYRadius();
  float paddle_end = paddle.GetPosition().y + paddle.GetYRadius();

  if ((ball.GetPosition().x - ball.GetRadius()) <= paddle_x_bound) {

    if ((ball.GetPosition().y + ball.GetRadius() >= paddle_start) &&
        (ball.GetPosition().y - ball.GetRadius() <= paddle_end)) {

      vec2 paddle_pos = vec2(paddle_x_bound, ball.GetPosition().y);

      if (CheckMovingTowards(ball.GetVelocity(), vec2(0, 0), ball.GetPosition(),
                             paddle_pos)) {
        vec2 new_velocity = vec2(-ball.GetVelocity().x + kPaddleAcceleration,
                                 ball.GetVelocity().y);
        ball.SetVelocity(new_velocity);
        if (!test_) {
          PlaySoundEffect();
        }
      }
      //Removes ball from vector if it goes past the end
    } else {
      pong_balls.erase(pong_balls.begin() + (position));
    }
  }
}

void PongTable::Display() {
  ci::gl::drawSolidRect(
      ci::Rectf(vec2((x_boundary_ / 2) - kLineLength, 0),
                vec2((x_boundary_ / 2) + kLineLength, y_boundary_)));

  ci::Rectf pos(
      vec2((x_boundary_ / 2) - kImageSize, (y_boundary_ / 2) - kImageSize),
      vec2((x_boundary_ / 2) + kImageSize, (y_boundary_ / 2) + kImageSize));

  ci::gl::draw(gump_face_image_, pos);
}

void PongTable::PlaySoundEffect() {
  pong_sfx_ -> start();
}

void PongTable::SetUpAudio() {
  ci::audio::SourceFileRef sourceFile =
      ci::audio::load(ci::app::loadAsset(pong_sfx_file_));

  pong_sfx_ = ci::audio::Voice::create(sourceFile);
}

} // namespace gpong