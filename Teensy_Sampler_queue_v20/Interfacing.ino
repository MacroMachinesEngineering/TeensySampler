
void handleButtons(){  
  buttonRecord.update();
  buttonReverse.update();
  buttonPlay.update();

  // Respond to button presses
  if (buttonRecord.fallingEdge()) {
    fwdRev = forward;
    Serial.println("Record Button Press");
    startRecording(); }
  if (buttonRecord.risingEdge()) {
    fwdRev = forward;
    Serial.println("Record Button Up");
    stopRecording(); }
    
  if (buttonReverse.fallingEdge()) {
    fwdRev = reverse;
    envelope1.noteOn();
    envelope2.noteOn();
    if (mode == 0) {mode = 3; startPlaying();}
    if (mode == 1) {stopRecording(); mode = 3; startPlaying();} 
    if (mode == 2) {stopPlaying(); mode = 3; startPlaying();}
    if (mode == 3) {stopPlaying(); mode = 3; startPlaying();}
  }
  if (buttonReverse.risingEdge()) {
    Serial.println("Reverse Button Up");
    Serial.println(sampleNames[fwdRev][selectedSample]);
    envelope1.noteOff();
    envelope2.noteOff();
    stopPlaying();
  }
  
  if (buttonPlay.fallingEdge()) {
    fwdRev = forward;
    envelope1.noteOn();
    envelope2.noteOn();
    if (mode == 3) {stopPlaying(); mode = 2; startPlaying();}
    if (mode == 2) {mode = 2; startPlaying();}
    if (mode == 1) {stopRecording(); mode = 2; startPlaying();}
    if (mode == 0) {mode = 2; startPlaying();}
  }
  if (buttonPlay.risingEdge()) {
    envelope1.noteOff();
    envelope2.noteOff();
    stopPlaying();
  }
}
