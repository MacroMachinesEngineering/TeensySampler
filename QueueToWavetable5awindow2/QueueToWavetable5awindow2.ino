#include <Metro.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

AudioInputI2S            i2s1;           //xy=57,61
AudioRecordQueue         queue1;         //xy=176,60
AudioSynthWaveformSine   sine1;          //xy=305,224.75
AudioSynthWaveformDc     dc1;            //xy=305,258.75
AudioSynthWaveform       waveform1;      //xy=451,186.75
AudioMixer4              mixer1;         //xy=456,239.75
AudioEffectMultiply      multiply1;      //xy=599,202.75
AudioSynthWaveformSine   sine2;          //xy=691.0000095367432,340.0000066757202
AudioSynthWaveformDc     dc2;            //xy=692.2500095367432,376.2500066757202
AudioSynthWaveform       waveform2;      //xy=843.5000057220459,298.7500009536743
AudioMixer4              mixer2;         //xy=853.5000133514404,368.7500066757202
AudioEffectMultiply      multiply2;      //xy=998.5000133514404,337.5000066757202
AudioMixer4              mixer3;         //xy=1132.2500114440918,192.5
AudioOutputI2S           i2s2;           //xy=1257.75,192.74999618530273
AudioConnection          patchCord1(i2s1, 0, queue1, 0);
AudioConnection          patchCord2(sine1, 0, mixer1, 0);
AudioConnection          patchCord3(dc1, 0, mixer1, 1);
AudioConnection          patchCord4(waveform1, 0, multiply1, 0);
AudioConnection          patchCord5(mixer1, 0, multiply1, 1);
AudioConnection          patchCord6(multiply1, 0, mixer3, 0);
AudioConnection          patchCord7(sine2, 0, mixer2, 0);
AudioConnection          patchCord8(dc2, 0, mixer2, 1);
AudioConnection          patchCord9(waveform2, 0, multiply2, 0);
AudioConnection          patchCord10(mixer2, 0, multiply2, 1);
AudioConnection          patchCord11(multiply2, 0, mixer3, 1);
AudioConnection          patchCord12(mixer3, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=1415.75,474.75


int16_t buffer1[256];
int16_t buffer2[256];

int tableFreq = 172;


void setup(){
  
  AudioMemory(64);
  
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.volume(0.8);
  sgtl5000_1.lineInLevel(0);
  sgtl5000_1.lineOutLevel(13);
  sgtl5000_1.adcHighPassFilterDisable();
  
  waveform1.begin(1, 0, WAVEFORM_ARBITRARY);
  waveform1.arbitraryWaveform(buffer1, 344);
  waveform2.begin(1, 0, WAVEFORM_ARBITRARY);
  waveform2.arbitraryWaveform(buffer1, 344);

  sine1.amplitude(0.5);
  sine2.amplitude(0.5);
  dc1.amplitude(0.5);
  dc2.amplitude(0.5);
  mixer1.gain(0,1);
  mixer1.gain(1,1);
  mixer2.gain(0,1);
  mixer2.gain(1,1);
  mixer3.gain(0,1);
  mixer3.gain(1,1);
  
  queue1.begin();
}


Metro tempoClock = Metro(1000);

void loop(){
  tableFreq = map(analogRead(15), 0, 1024, 172/4, 172*4);
  tempoClock.interval(1);
  if(tempoClock.check()){
  if(queue1.available() >= 2){ // input buffer
    memcpy(buffer1, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    memcpy(buffer2, queue1.readBuffer(), 256);
    queue1.freeBuffer();}

    AudioNoInterrupts();
    waveform1.phase(0);
    waveform1.frequency(tableFreq);
    sine1.phase(0);
    sine1.frequency(tableFreq);
      
    waveform2.phase(180);
    waveform2.frequency(tableFreq);
    sine2.phase(180);
    sine2.frequency(tableFreq);
    AudioInterrupts(); 
}}
