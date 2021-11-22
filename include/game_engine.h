#pragma once

#include <ctime>
#include <chrono>
#include "cinder/gl/gl.h"

namespace gpong {

using std::chrono::system_clock;
using glm::vec2;

/**
 * Class for determining the score continuously and making decisions
 * about the game.
 */
class GameEngine {
 public:
  GameEngine(int window_height, int window_length, int max_balls);

  GameEngine();

  /**
   * Displays the score as the game is being played. If the game is over it
   * displays the game over screen.
   */
  void Display();

  /**
   * Updates the game engine based on the number of pong balls on the table and
   * decides if the game is over or not.
   */
  void Update(int num_balls, int ball_count, bool& spin, bool& game_over);

  int GetScore() const;

 private:
  //const variables
  static const int kScoreMultiplier = 15;
  static const int kSpinMultiplier = 2;
  static const int kUpdateInterval = 20;
  static const int kEndScreenSize = 600;
  static const int kSpacing = 100;
  static const int kMaxTime = 25;
  static const int kInsaneTime = 70;
  static const ci::Color kColor;
  static const float kFontSize;


  int x_boundary_;
  int y_boundary_;
  int max_balls_;
  int score_;
  int update_count_;
  bool game_over_;
  bool spin_;
  system_clock::time_point start_time_;
  vec2 score_pos_;

  /**
   * Displays the score.
   */
  void DisplayScore();

  /**
   * Displays the game over screen if the game is over.
   */
  void DisplayGameOver();

  /**
   * Adds spin to the ball if too much time has elapsed as they are
   * playing the game.
   */
  void AddSpin(bool& spin, std::chrono::duration<double> elapsed_seconds,
               int num_balls);
};

} // namespace gpong