#pragma once

#include "ball.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "paddle.h"
#include "pong_table.h"
#include "cinder/audio/audio.h"
#include "game_engine.h"
#include "menu.h"


namespace gpong {

using std::vector;

/**
 * App for running Gump Pong.
 */
class GumpPongApp : public ci::app::App {
 public:
  GumpPongApp();

  void draw() override;
  void update() override;
  void keyDown(ci::app::KeyEvent event) override;
  void keyUp(ci::app::KeyEvent event) override;

  /**
   * Plays an audio quote from the Forrest Gump movie
   */
  void PlayQuote();

  /**
   * Plays music while playing the game.
   */
  void PlayMusic();

  /**
   * Loads the quotes and music. Randomly picks a different song and quote
   * every new instance of loading the app.
   */
  void LoadAudioFile();

  const int kWindowHeight = 1900;

  const int kWindowLength = 3650;

 private:
  //Game object variables
  Paddle paddle_;
  vector<Ball> pong_balls_;
  PongTable table_;
  GameEngine engine_;
  Menu menu_;

  //game state variables
  bool menu_on_;
  bool game_over_;

  //Gameplay variables
  size_t ball_count_;
  size_t max_balls_;
  bool moving_up_;
  bool moving_down_;
  bool spin_;

  //Audio variables
  ci::audio::VoiceRef audio_output_;
  ci::audio::VoiceRef music_output_;
  static const float kMusicVolume;
  static const float kAudioVolume;
  static const vector<std::string> audio_files_;
  static const vector<std::string> music_files_;

  /**
   * When the menu is open, this method handles key events.
   */
  void MenuKeyDown(const ci::app::KeyEvent& event);

  /**
   * Resets the game's variables before every new round.
   */
  void StartGame();

  /**
   * When on the game over screen this method handles key events.
   */
  void GameOverKeyDown(const ci::app::KeyEvent& event);
};

} // namespace gpong