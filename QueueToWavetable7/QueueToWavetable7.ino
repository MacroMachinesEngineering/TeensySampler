#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

char *sampleNames[] = {"R0.RAW","R1.RAW","R2.RAW","R3.RAW","R4.RAW","R5.RAW","R6.RAW","R7.RAW","R8.RAW","R9.RAW","R10.RAW","R11.RAW","R12.RAW","R13.RAW","R14.RAW","R15.RAW"};

Bounce buttonRecord = Bounce(0, 8);
Bounce buttonReverse = Bounce(1, 8);  // 8 = 8 ms debounce time
Bounce buttonPlay =   Bounce(2, 8);

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=55,38
AudioRecordQueue         inQueue;         //xy=186,38
AudioSynthWaveform       outWave;      //xy=338,39
AudioOutputI2S           i2s2;           //xy=466,44
AudioConnection          patchCord1(i2s1, 0, queue2, 0);
AudioConnection          patchCord2(outWave, 0, i2s2, 0);
AudioControlSGTL5000     audioCodec;     //xy=446,103
// GUItool: end automatically generated code


int16_t buffer[4096];


void handleButtons(){  
  buttonRecord.update();
  buttonReverse.update();
  buttonPlay.update();

  // Respond to button presses
  if (buttonRecord.fallingEdge()) {
    Serial.println("Record Button Down");
    }
  if (buttonRecord.risingEdge()) {
    Serial.println("Record Button Up");
    }    
  if (buttonReverse.fallingEdge()) {
    Serial.println("Reverse Button Down");
    }
  if (buttonReverse.risingEdge()) {
    Serial.println("Reverse Button Up");
    }  
  if (buttonPlay.fallingEdge()) {
    Serial.println("Play Button Down");
    }
  if (buttonPlay.risingEdge()) {
    Serial.println("Play Button Up");
    }
}

void setup(){
  AudioMemory(64);
    // Configure the pushbutton pins
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  audioCodec.enable();
  audioCodec.inputSelect(AUDIO_INPUT_LINEIN);
  audioCodec.volume(0.8);
  audioCodec.lineInLevel(0);
  audioCodec.lineOutLevel(13);
  inQueue.begin();
  outWave.begin(0.8, 172, WAVEFORM_ARBITRARY);
  outWave.arbitraryWaveform(buffer, 172);  // was 344
}

void loop(){
  handleButtons();
  if(inQueue.available() >= 1){ // input buffer
    memcpy(buffer, inQueue.readBuffer(), 256);
    inQueue.freeBuffer();
    outWave.phase(0);
  }
  //outWave.frequency(map(analogRead(15), 0, 1024, 172, 172*2));
  outWave.frequency(map(analogRead(15), 0, 1024, 172, 172*2));
}
