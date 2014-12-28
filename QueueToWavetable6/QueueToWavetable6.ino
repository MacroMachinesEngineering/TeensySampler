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
AudioRecordQueue         queue2;         //xy=186,38
AudioPlayQueue           queue1;         //xy=296,93
AudioSynthWaveform       waveform1;      //xy=338,39
AudioOutputI2S           i2s2;           //xy=466,44
AudioConnection          patchCord1(i2s1, 0, queue2, 0);
AudioConnection          patchCord2(waveform1, 0, i2s2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=446,103
// GUItool: end automatically generated code


int16_t buffer[1024];


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
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.volume(0.8);
  sgtl5000_1.lineInLevel(0);
  sgtl5000_1.lineOutLevel(13);
  queue2.begin();
  waveform1.begin(0.8, 172, WAVEFORM_ARBITRARY);
  waveform1.arbitraryWaveform(buffer, 172);  // was 344
}

void loop(){
  handleButtons();
  if(queue2.available() >= 1){ // input buffer
    memcpy(buffer, queue2.readBuffer(), 256);
    queue2.freeBuffer();
    waveform1.phase(0);
  }
  waveform1.frequency(map(analogRead(15), 0, 1024, 172, 172*2));
}
