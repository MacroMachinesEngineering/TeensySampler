#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=77,57
AudioRecordQueue         queue2;         //xy=208,57
AudioPlayQueue           queue1;         //xy=327,55
AudioOutputI2S           i2s2;           //xy=459,52
AudioConnection          patchCord1(i2s1, 0, queue2, 0);
AudioConnection          patchCord2(queue1, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=446,103
// GUItool: end automatically generated code

uint16_t buffer[512];

void setup(){
  AudioMemory(64);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.volume(0.8);
  sgtl5000_1.lineInLevel(0);
  sgtl5000_1.lineOutLevel(13);
  queue2.begin();
}

void loop(){
  if(queue2.available() >= 2){ // input buffer
    memcpy(buffer, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    int16_t *outBuf = queue1.getBuffer(); // output buffer
    memcpy(outBuf, buffer, 256);
    queue1.playBuffer();}
}
