#include <game_engine.h>

namespace gpong {

const float GameEngine::kFontSize = 60.0;
const ci::Color GameEngine::kColor = ci::Color("black");

GameEngine::GameEngine() = default;

GameEngine::GameEngine(int window_height, int window_length, int max_balls) {
  x_boundary_ = window_length;
  y_boundary_ = window_height;
  score_ = 0;
  spin_ = false;
  game_over_ = false;
  max_balls_ = max_balls;
  update_count_ = 0;
  score_pos_ = vec2(window_length - (3*kSpacing), kSpacing);
  start_time_ = std::chrono::system_clock::now();
}

void GameEngine::Update(int num_balls, int ball_count, bool& spin,
                        bool& game_over) {
  // means game is over
  if ((num_balls == 0) && (ball_count == max_balls_)) {
    game_over_ = true;
    //Tells the app that the game is over
    game_over = true;
    return;
  }
  // So score does not change constantly
  if (update_count_ % kUpdateInterval != 0) {
    update_count_++;
    return;
  }
  update_count_++;
  //prevents player of running up score while no balls in table
  if (num_balls == 0) {
    return;
  }

  system_clock::time_point current_time = system_clock::now();
  std::chrono::duration<double> elapsed_seconds = current_time - start_time_;

  //Adds spin if too much time elapsed
  AddSpin(spin, elapsed_seconds, num_balls);

  if (!spin_) {
    score_ = score_ + ((kScoreMultiplier * ball_count) +
                       static_cast<int>(elapsed_seconds.count()));
  } else {
    //Multiplies by multiplier if there's spin
    score_ = score_ + ((kScoreMultiplier * ball_count * kSpinMultiplier) +
                       static_cast<int>(elapsed_seconds.count()));
  }
}

void GameEngine::Display() {
  if (game_over_) {
    DisplayGameOver();
  } else {
    DisplayScore();
  }
}

void GameEngine::DisplayScore() {
  ci::ColorA color(1,1,100,1);
  ci::Font font("Arial", kFontSize);
  std::string score_string = "Score: " + std::to_string(score_);
  ci::gl::drawStringCentered(score_string, score_pos_, color, font);

  if (spin_) {
    ci::Font font2("Arial", (2 * kFontSize));
    std::string spin_str = "Adding Spin!";
    vec2 pos = vec2(score_pos_.x - kSpacing, score_pos_.y + (2*kSpacing));
    ci::gl::drawStringCentered(spin_str, pos, color, font2);
  }
}

void GameEngine::DisplayGameOver() {
  //Display game over box
  ci::gl::color(kColor);
  vec2 upper = vec2((x_boundary_ / 2) - kEndScreenSize,
                    (y_boundary_ / 2) - kEndScreenSize);
  vec2 lower = vec2((x_boundary_ / 2) + kEndScreenSize,
                    (y_boundary_ / 2) + kEndScreenSize);
  ci::gl::drawSolidRect(ci::Rectf(upper, lower));

  //Display game over
  ci::ColorA color(1,1,100,1);
  ci::Font font("Arial", (4 * kFontSize));
  vec2 game_over_pos = vec2(x_boundary_ / 2, upper.y + kSpacing);
  ci::gl::drawStringCentered("Game Over", game_over_pos, color, font);

  //Display score
  ci::Font font2("Arial", (2 * kFontSize));
  std::string score_string = "Score: " + std::to_string(score_);
  vec2 score_pos = vec2(game_over_pos.x, game_over_pos.y + (3*kSpacing));
  ci::gl::drawStringCentered(score_string, score_pos, color, font2);

  //Display instructions
  std::string instructions = "Press m for main menu";
  vec2 ins_pos = vec2(game_over_pos.x, score_pos.y + (2 * kSpacing));
  ci::gl::drawStringCentered(instructions, ins_pos, color, font2);
}

void GameEngine::AddSpin(bool& spin,
                         std::chrono::duration<double> elapsed_seconds,
                         int num_balls) {
  //One ball and too much time
  if ((elapsed_seconds.count() > kMaxTime) && num_balls == 1) {
    spin = true;
    spin_ = true;
  } else if (elapsed_seconds.count() > kInsaneTime){
    spin = true;
    spin_ = true;
  } else {
    spin = false;
    spin_ = false;
  }
}

int GameEngine::GetScore() const {
  return score_;
}

} // namespace gpong

