#include <gump_pong_app.h>

namespace gpong {

const vector<std::string> GumpPongApp::audio_files_ = {
    "jenny.wav", "chocolate.wav", "stupid.wav", "tired.wav"};

const vector<std::string> GumpPongApp::music_files_ = {"paint_black.wav",
                                                       "worth.wav"};

const float GumpPongApp::kMusicVolume = static_cast<float>(-0.6);

const float GumpPongApp::kAudioVolume = 7.5;

GumpPongApp::GumpPongApp() {
  ci::app::setWindowSize(kWindowLength, kWindowHeight);

  menu_on_ = true;
  menu_ = Menu(kWindowHeight, kWindowLength);
  //makes new seed everytime application runs
  srand(static_cast<unsigned int>(time(nullptr)));
  LoadAudioFile();
  PlayMusic();
}

void GumpPongApp::draw() {
  ci::Color background_color("purple");
  ci::gl::clear(background_color);

  if (menu_on_) {
    menu_.Display();
  } else {
    paddle_.Display();
    table_.Display();
    engine_.Display();

    //Display each ball
    for (const Ball& ball : pong_balls_) {
      ball.DrawBall();
    }
  }
}

void GumpPongApp::update() {
  //Checks to see if music stopped playing, and restarts it if so.
  if (!music_output_->isPlaying()) {
    PlayMusic();
  }

  if (menu_on_) {
    return;
  }

  table_.AdvanceOneFrame(pong_balls_, paddle_, spin_);
  if (moving_up_) {
    paddle_.MoveUp();
  } else if (moving_down_) {
    paddle_.MoveDown();
  }
  engine_.Update(pong_balls_.size(), ball_count_, spin_, game_over_);
}

void GumpPongApp::keyDown(ci::app::KeyEvent event) {
  if (menu_on_) {
    MenuKeyDown(event);
    return;
  } else if (game_over_) {
    GameOverKeyDown(event);
    return;
  }
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_UP: {
      moving_up_ = true;
      break;
    }
    case ci::app::KeyEvent::KEY_DOWN: {
      moving_down_ = true;
      break;
    }
    case ci::app::KeyEvent::KEY_SPACE: {
      if (ball_count_ < max_balls_) {
        pong_balls_.push_back(Ball(kWindowLength, kWindowHeight));
        ball_count_++;
      }
      break;
    }
  }
}

void GumpPongApp::keyUp(ci::app::KeyEvent event) {
  if (event.getCode() == ci::app::KeyEvent::KEY_UP ||
      event.getCode() == ci::app::KeyEvent::KEY_DOWN) {
    moving_down_ = false;
    moving_up_ = false;
    paddle_.Stop();
  }
}

void GumpPongApp::PlayQuote() {
  audio_output_->start();
}

void GumpPongApp::PlayMusic() {
  music_output_->start();
}

void GumpPongApp::LoadAudioFile() {
  //Load quote
  size_t rand_num = rand() % (audio_files_.size());

  ci::audio::SourceFileRef sourceFile =
      ci::audio::load(ci::app::loadAsset(audio_files_[rand_num]));

  audio_output_ = ci::audio::Voice::create(sourceFile);
  audio_output_->setVolume(kAudioVolume);

  //load music
  size_t rand_num_music = rand() % (music_files_.size());
  ci::audio::SourceFileRef sourceFile_music =
      ci::audio::load(ci::app::loadAsset(music_files_[rand_num_music]));
  music_output_ = ci::audio::Voice::create(sourceFile_music);
  music_output_->setVolume(kMusicVolume);
}

void GumpPongApp::MenuKeyDown(const ci::app::KeyEvent& event) {
  if (event.getCode() == ci::app::KeyEvent::KEY_2) {
    max_balls_ = 2;
    menu_on_ = false;
    StartGame();
    PlayQuote();
  } else if (event.getCode() == ci::app::KeyEvent::KEY_3) {
    max_balls_ = 3;
    menu_on_ = false;
    StartGame();
    PlayQuote();
  } else if (event.getCode() == ci::app::KeyEvent::KEY_5) {
    max_balls_ = 5;
    menu_on_ = false;
    StartGame();
    PlayQuote();
  }
}

void GumpPongApp::StartGame() {
  paddle_ = Paddle(kWindowHeight);
  pong_balls_ = vector<Ball>(1, Ball(kWindowLength, kWindowHeight));
  table_ = PongTable(kWindowLength, kWindowHeight);
  engine_ = GameEngine(kWindowHeight, kWindowLength, max_balls_);
  ball_count_ = 1;
  moving_up_ = false;
  moving_down_ = false;
  spin_ = false;
  game_over_ = false;
}

void GumpPongApp::GameOverKeyDown(const ci::app::KeyEvent& event) {
  if (event.getCode() == ci::app::KeyEvent::KEY_m) {
    menu_on_ = true;
    menu_.AddScore(engine_.GetScore());
  }
}

}  // namespace gpong
