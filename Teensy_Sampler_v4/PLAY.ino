

void startPlaying() {
  Serial.println("startPlaying");
  mixer1.gain(0, 0); // mute original
  mixer1.gain(1, 1); // unmute sample
  playRaw1.play("RECORD.RAW");
  mode = 2;
}

void continuePlaying() {
  if (!playRaw1.isPlaying()) {
    playRaw1.stop();
    mode = 0;
  }
}

void stopPlaying() {
  Serial.println("stopPlaying");
  mixer1.gain(1, 0);  // mute sample
  mixer1.gain(0, 1);  // unmute original
  if (mode == 2) playRaw1.stop();
  mode = 0;
}

