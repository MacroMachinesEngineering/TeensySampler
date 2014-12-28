
int playOffset;

void startPlaying() {
  Serial.print("startPlaying(): ");
  Serial.println(sampleNames[selectedSample]);
  mixer1.gain(0, 0); // mute original
  mixer1.gain(1, 1); // unmute sample
  playOffset = map(analogRead(15), 0, 1024, 0, playRaw1.lengthMillis());
  playRaw1.playSkip(sampleNames[selectedSample], playOffset);

  Serial.print(playOffset);
  Serial.print("/");
  Serial.println(playRaw1.lengthMillis());

}

void continuePlaying() {
  if (!playRaw1.isPlaying()) {
    playRaw1.stop();
    mode = 0;
  }
}

void stopPlaying() {
  Serial.print("stopPlaying(): ");
  Serial.println(selectedSample);
  mixer1.gain(1, 0);  // mute sample
  mixer1.gain(0, 1);  // unmute original
  if (mode == 2 || mode == 3) playRaw1.stop();
  mode = 0;
  Serial.print("stopPlaying() end: ");
  Serial.println(selectedSample);
}

