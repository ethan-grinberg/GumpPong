#include "menu.h"

namespace gpong {

const ci::Color Menu::kColor = ci::Color("white");

const float Menu::kFontSize = 90.0;

const std::string Menu::kGumpImageUrl =
    "http://lmc-mag.com/wp-content/uploads/2017/10/forrestgump_opener-1.jpg";

Menu::Menu(int window_height, int window_length) {
  x_boundary_ = window_length;
  y_boundary_ = window_height;

  option2_x_loc_ = (x_boundary_ / 3);
  option1_x_loc_ = option2_x_loc_ - (kOptionSpace + (2 * kOptionSize));
  option3_x_loc_ = option2_x_loc_ + (2 * kOptionSize) + kOptionSpace;
  option_y_loc_ = y_boundary_ - kOptionSize;

  gump_image_ =
      ci::gl::Texture2d::create(ci::loadImage(ci::loadUrl(kGumpImageUrl)));

  highest_score_ = 0;
}

Menu::Menu() = default;

void Menu::Display() {
  ci::gl::color(ci::Color(kColor));
  DisplayTitle();
  DisplayOptions();
  DisplayScore();
}

void Menu::DisplayOptions() {
  ci::ColorA color(1, 1, 100, 1);
  ci::Font font("Arial", kFontSize);

  // Display Option 1
  vec2 upper = vec2((option1_x_loc_)-kOptionSize, (option_y_loc_)-kOptionSize);
  vec2 lower =
      vec2((option1_x_loc_) + kOptionSize, (option_y_loc_) + kOptionSize);
  ci::gl::drawStrokedRect(ci::Rectf(upper, lower));

  vec2 option1_pos = vec2(option1_x_loc_, upper.y + kTextSpace);
  ci::gl::drawStringCentered("Max Pong Balls: 2", option1_pos, color, font);
  ci::gl::drawStringCentered("Press 2",
                             vec2(option1_x_loc_, (option1_pos.y + kTextSpace)),
                             color, font);

  // Display option 2
  vec2 upper2 = vec2((option2_x_loc_)-kOptionSize, (option_y_loc_)-kOptionSize);
  vec2 lower2 =
      vec2((option2_x_loc_) + kOptionSize, (option_y_loc_) + kOptionSize);
  ci::gl::drawStrokedRect(ci::Rectf(upper2, lower2));

  vec2 option2_pos = vec2(option2_x_loc_, upper2.y + kTextSpace);
  ci::gl::drawStringCentered("Max Pong Balls: 3", option2_pos, color, font);
  ci::gl::drawStringCentered("Press 3",
                             vec2(option2_x_loc_, (option2_pos.y + kTextSpace)),
                             color, font);

  vec2 upper3 = vec2((option3_x_loc_)-kOptionSize, (option_y_loc_)-kOptionSize);
  vec2 lower3 =
      vec2((option3_x_loc_) + kOptionSize, (option_y_loc_) + kOptionSize);
  ci::gl::drawStrokedRect(ci::Rectf(upper3, lower3));

  vec2 option3_pos = vec2(option3_x_loc_, upper3.y + kTextSpace);
  ci::gl::drawStringCentered("Max Pong Balls: 5", option3_pos, color, font);
  ci::gl::drawStringCentered("Press 5",
                             vec2(option3_x_loc_, (option3_pos.y + kTextSpace)),
                             color, font);
}

void Menu::DisplayTitle() {
  ci::ColorA color(1, 1, 100, 1);
  ci::Font font("Arial", (kFontSize * 4));

  ci::Rectf pos(vec2((x_boundary_ / 2) - kImageLength, 0),
                vec2((x_boundary_ / 2) + kImageLength, kImageHeight));
  ci::gl::draw(gump_image_, pos);

  ci::gl::drawStringRight(
      "Gump", vec2((x_boundary_ / 2) - kImageLength, (kImageHeight / 2)), color,
      font);

  ci::gl::drawString("Pong",
                     vec2((x_boundary_ / 2) + kImageLength, (kImageHeight / 2)),
                     color, font);
}

void Menu::AddScore(int score) {
  scores_.push_back(score);
  highest_score_ = GetHighestScore();
}

int Menu::GetHighestScore() {
  int highest = 0;
  for (int score : scores_) {
    if (score >= highest) {
      highest = score;
    }
  }
  return highest;
}

void Menu::DisplayScore() {
  ci::ColorA color(1, 1, 100, 1);
  ci::Font font("Arial", (kFontSize * 1.5));

  vec2 pos((option3_x_loc_ + kScoreSpace + kOptionSize),
           (option_y_loc_ - kOptionSize));

  std::string score_string = "Highest Score: " + std::to_string(highest_score_);
  ci::gl::drawString(score_string, pos, color, font);
}

}  // namespace gpong