#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=55,38
AudioRecordQueue         queue2;         //xy=186,38
AudioPlayQueue           queue1;         //xy=296,93
AudioSynthWaveform       waveform1;      //xy=338,39
AudioOutputI2S           i2s2;           //xy=466,44
AudioConnection          patchCord1(i2s1, 0, queue2, 0);
AudioConnection          patchCord2(waveform1, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=446,103
// GUItool: end automatically generated code


int16_t buffer[1024];

void setup(){
  AudioMemory(64);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.volume(0.8);
  sgtl5000_1.lineInLevel(0);
  sgtl5000_1.lineOutLevel(13);
  queue2.begin();
  waveform1.begin(0.8, 172, WAVEFORM_ARBITRARY);
  waveform1.arbitraryWaveform(buffer, 344);
}

void loop(){
  if(queue2.available() >= 1){ // input buffer
    memcpy(buffer, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    waveform1.phase(0);
  }
  waveform1.frequency(map(analogRead(15), 0, 1024, 172, 172*2));
}
