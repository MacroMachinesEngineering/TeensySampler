
int playOffset;

void startPlaying() {
  mixer1.gain(0, 0); // mute original
  mixer1.gain(1, 1); // unmute sample
  playOffset = map(analogRead(15), 0, 1024, 0, loadRaw1.lengthMillis()-200);
  loadRaw1.setPos(playOffset);
  loadRaw1.load(sampleNames[selectedSample]);
  loadRaw1.play();

  Serial.print(playOffset);
  Serial.print("/");
  Serial.println(loadRaw1.lengthMillis());
}

void continuePlaying() {
  if (!loadRaw1.isPlaying()) {
    loadRaw1.stop();
    mode = 0;
  }
}

void stopPlaying() {
  Serial.print("stopPlaying(): ");
  Serial.println(selectedSample);
  mixer1.gain(1, 0);  // mute sample
  mixer1.gain(0, 1);  // unmute original
  if (mode == 2 || mode == 3) loadRaw1.stop();
  mode = 0;
  Serial.print("stopPlaying() end: ");
  Serial.println(selectedSample);
}

