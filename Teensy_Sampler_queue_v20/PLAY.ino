
int playOffset;

/*
void startPlaying() {
  mixer1.gain(0, 0); // mute original
  mixer1.gain(1, 1); // unmute sample
  playOffset = map(analogRead(15), 0, 1024, 0, frec.size());
  //playRaw1.playSkip(sampleNames[fwdRev][selectedSample], playOffset);
  playSkipper(sampleNames[fwdRev][selectedSample], playOffset);
  playOutputBuffer();
  Serial.print(playOffset);
  Serial.print("/");
  Serial.println(frec.size());
}
*/

void startPlaying() {
  if (SD.exists(sampleNames[fwdRev][selectedSample])) { 
    frec = SD.open(sampleNames[fwdRev][selectedSample], FILE_READ);}
  if (frec) {
    mixer1.gain(0, 0); // mute original
    mixer1.gain(1, 1); // unmute sample
    
  }
}






void continuePlaying() {
//  if (!playRaw1.isPlaying()) {
//    playRaw1.stop();
//    mode = 0;
//  }
  playOffset = map(analogRead(15), 0, 1024, 0, frec.size());
  //playRaw1.playSkip(sampleNames[fwdRev][selectedSample], playOffset);
  playSkipper(sampleNames[fwdRev][selectedSample], playOffset);
  playOutputBuffer();
  Serial.print(playOffset);
  Serial.print("/");
  Serial.println(frec.size());
}




void stopPlaying() {
 // Serial.print("stopPlaying(): ");
 // Serial.println(selectedSample);
  mixer1.gain(1, 0);  // mute sample
  mixer1.gain(0, 1);  // unmute original
 // if (mode == 2 || mode == 3) playRaw1.stop();
  mode = 0;
 // Serial.print("stopPlaying() end: ");
 // Serial.println(selectedSample);
}




void playOutputBuffer(){
   int16_t *outBuf = queue2.getBuffer(); // output buffer
   memcpy(outBuf, outBuffer, 256);
   queue2.playBuffer();
}





#define B2M (uint32_t)((double)4294967296000.0 / AUDIO_SAMPLE_RATE_EXACT / 2.0) // 97352592

void playSkipper(const char *filename, uint64_t skip){
//   stop();
//   AudioStartUsingSPI();
//   __disable_irq();

//  __enable_irq();
  
//  if (!rawfile) {
    //Serial.println("unable to open file");
//	return false; }
   
   unsigned long file_size = frec.size();
   unsigned long file_offset = (skip<<32) / B2M;

   file_offset /= 2;
   file_offset *= 2;
    
    frec.seek(file_offset);
    frec.read(outBuffer, 512);

    //Serial.println("able to open file");
   // playing = true;
   // return true;
}
