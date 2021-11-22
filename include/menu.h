#pragma once

#include "cinder/gl/gl.h"

namespace gpong {

using glm::vec2;
using std::vector;

/**
 * Class that displays the menu screen.
 */
class Menu {
 public:
  Menu(int window_height, int window_length);

  Menu();

  /**
  * Displays the game options, game title, and highest score.
  */
  void Display();

  /**
  * Adds the score from the most recent round.
  */
  void AddScore(int score);

 private:
  int x_boundary_;
  int y_boundary_;

  int option1_x_loc_;
  int option2_x_loc_;
  int option3_x_loc_;
  int option_y_loc_;
  static const int kOptionSpace = 230;
  static const int kOptionSize = 300;
  static const int kTextSpace = 175;
  static const ci::Color kColor;
  static const float kFontSize;

  static const std::string kGumpImageUrl;
  static const int kImageLength = 1000;
  static const int kImageHeight = 1125;
  ci::gl::Texture2dRef gump_image_;

  vector<int> scores_;
  int highest_score_;
  static const int kScoreSpace = 50;

  /**
  * Displays the game options.
  */
  void DisplayOptions();

  /**
  * Displays the game title.
  */
  void DisplayTitle();

  /**
  * Displays the highest score of any round.
  */
  void DisplayScore();

  /**
  * Finds the highest score out of the score of all the previous rounds.
  */
  int GetHighestScore();

};

}  // namespace gpong