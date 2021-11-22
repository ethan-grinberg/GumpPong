#include "gump_pong_app.h"

using gpong::GumpPongApp;

void prepareSettings(GumpPongApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(GumpPongApp, ci::app::RendererGl, prepareSettings);
