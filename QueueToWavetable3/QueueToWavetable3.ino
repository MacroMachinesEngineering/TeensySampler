#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=55,38
AudioRecordQueue         queue2;         //xy=186,38
AudioSynthWaveformSine   sine1;          //xy=302.85713958740234,79.99999904632568
AudioSynthWaveform       waveform1;      //xy=312.2856979370117,40.42856788635254
AudioEffectMultiply      multiply1;      //xy=455.7142906188965,45.71428680419922
AudioSynthWaveformSine   sine2;          //xy=308.57141494750977,162.85712099075317
AudioSynthWaveform       waveform2;      //xy=308.5714416503906,128.5714235305786
AudioEffectMultiply      multiply2;      //xy=452.8571363176618,132.85714135851177
AudioSynthWaveformSine   sine3;          //xy=310.00000762939453,270.0000009536743
AudioSynthWaveform       waveform3;      //xy=304.2857131958008,231.4285593032837
AudioEffectMultiply      multiply3;      //xy=451.4286422729492,245.71429538726807
AudioSynthWaveformSine   sine4;          //xy=297.1428527832031,361.42855644226074
AudioSynthWaveform       waveform4;      //xy=302.8571472167969,321.428564786911
AudioEffectMultiply      multiply4;      //xy=467.14283752441406,329.9999837875366
AudioMixer4              mixer1;         //xy=628.5714378356934,187.14285564422607
AudioPlayQueue           queue1;         //xy=720.4285888671875,361.2857074737549
AudioOutputI2S           i2s2;           //xy=757.4286193847656,189.71427631378174
AudioConnection          patchCord1(i2s1, 0, queue2, 0);
AudioConnection          patchCord2(sine4, 0, multiply4, 1);
AudioConnection          patchCord3(sine1, 0, multiply1, 1);
AudioConnection          patchCord4(waveform4, 0, multiply4, 0);
AudioConnection          patchCord5(waveform3, 0, multiply3, 0);
AudioConnection          patchCord6(waveform2, 0, multiply2, 0);
AudioConnection          patchCord7(sine2, 0, multiply2, 1);
AudioConnection          patchCord8(sine3, 0, multiply3, 1);
AudioConnection          patchCord9(waveform1, 0, multiply1, 0);
AudioConnection          patchCord10(multiply3, 0, mixer1, 2);
AudioConnection          patchCord11(multiply2, 0, mixer1, 1);
AudioConnection          patchCord12(multiply1, 0, mixer1, 0);
AudioConnection          patchCord13(multiply4, 0, mixer1, 3);
AudioConnection          patchCord14(mixer1, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=744.5713729858398,254.42857027053833
// GUItool: end automatically generated code

int16_t buffer[1024];
int tableFreq = 172;


void setup(){
  AudioMemory(64);
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.volume(0.8);
  sgtl5000_1.lineInLevel(0);
  sgtl5000_1.lineOutLevel(13);
  queue2.begin();
  waveform1.begin(0.8, tableFreq, WAVEFORM_ARBITRARY);=
  waveform1.arbitraryWaveform(buffer, 344);
}

void loop(){
  if(queue2.available() >= 4){ // input buffer
    memcpy(buffer, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    memcpy(buffer+256, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    memcpy(buffer+512, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    memcpy(buffer+768, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    waveform1.phase(0);
  }
  waveform1.frequency(map(analogRead(15), 0, 1024, 172, 344));
}
