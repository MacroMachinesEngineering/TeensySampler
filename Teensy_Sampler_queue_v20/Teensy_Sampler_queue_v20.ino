
// Three pushbuttons need to be connected:
//   Record Button: pin 0 to GND
//   Reverse Button:   pin 1 to GND
//   Play Button:   pin 2 to GND

#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>


// GUItool: begin automatically generated code

AudioInputI2S            i2s2;           //xy=146.66667938232422,66.66666030883789
AudioAnalyzePeak         peak1;          //xy=286.6666793823242,81.11110920376248
AudioRecordQueue         queue1;         //xy=288.8888702392578,35.555545277065704

AudioPlayQueue           queue2;         //xy=91.11111068725586,125.55556774139404
AudioPlaySdRaw           playRaw1;       //xy=95.55556869506836,165.55554962158203
AudioEffectEnvelope      envelope1;      //xy=315.5554962158203,124.44444984859891
AudioEffectEnvelope      envelope2;      //xy=314.4444236755371,160.0000090069241
AudioFilterStateVariable filter1;        //xy=477.777774810791,151.1111101574368
AudioMixer4              mixer2;         //xy=611.1111183166504,142.2222284740872

AudioMixer4              mixer1;         //xy=747.222282409668,78.88888496822781
AudioOutputI2S           i2s1;           //xy=862.6667137145996,81.77777809566922

AudioConnection          patchCord1(queue2, envelope1);
AudioConnection          patchCord2(i2s2, 0, queue1, 0);
AudioConnection          patchCord3(i2s2, 0, mixer1, 0);
AudioConnection          patchCord4(i2s2, 0, peak1, 0);
AudioConnection          patchCord5(envelope2, 0, filter1, 1);
AudioConnection          patchCord6(envelope1, 0, filter1, 0);
AudioConnection          patchCord7(envelope1, 0, mixer2, 0);
AudioConnection          patchCord8(filter1, 0, mixer2, 1);
AudioConnection          patchCord9(filter1, 1, mixer2, 2);
AudioConnection          patchCord10(filter1, 2, mixer2, 3);
AudioConnection          patchCord11(mixer2, 0, mixer1, 1);
AudioConnection          patchCord12(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=867.7778015136719,134.44444221920438
// GUItool: end automatically generated code


#define reverse 1
#define forward 0
int selectedSample = 0;
int fwdRev = 0; // 0 == forward, 1 == reverse
char *sampleNames[2][16] = {{"R0.RAW","R1.RAW","R2.RAW","R3.RAW","R4.RAW","R5.RAW","R6.RAW","R7.RAW","R8.RAW","R9.RAW","R10.RAW","R11.RAW","R12.RAW","R13.RAW","R14.RAW","R15.RAW"},
{"R0R.RAW","R1R.RAW","R2R.RAW","R3R.RAW","R4R.RAW","R5R.RAW","R6R.RAW","R7R.RAW","R8R.RAW","R9R.RAW","R10R.RAW","R11R.RAW","R12R.RAW","R13R.RAW","R14R.RAW","R15R.RAW"}};

byte LEDpin = 6;

Bounce buttonRecord = Bounce(0, 8);
Bounce buttonReverse = Bounce(1, 8);  // 8 = 8 ms debounce time
Bounce buttonPlay =   Bounce(2, 8);

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

#define modeStop 0
#define modeRecord 1
#define modePlay 2
#define modeReverse 3
int mode = 0;  // 0=stopped, 1=recording, 2=playing, 3=reverse

#define noFilter 0
#define lowPass 1
#define bandPass 2
#define highPass 3
File frec; // The file where data is recorded

uint16_t outBuffer[512];

void setup() {
  // Configure the pushbutton pins
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  //pinMode(LEDpin, PWM);

  AudioMemory(60);

  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(0.8);
  sgtl5000_1.lineInLevel(0);
  sgtl5000_1.lineOutLevel(13);
//  sgtl5000_1.adcHighPassFilterDisable();

  // Initialize the SD card
  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(10))) {
    // stop here if no SD card, but print a message
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  
  filter1.frequency(15000); // 2000
  filter1.octaveControl(7);
  filter1.resonance(.7);
  envelope1.decay(200);
  envelope2.decay(200);
  envelope1.hold(200);
  envelope2.hold(200);
  envelope1.sustain(1); // 0.5
  envelope2.sustain(1); // 0.1
  envelope1.release(5);
  envelope2.release(10);
  setFilterMode(noFilter);
}



void loop() {
  handleButtons();

  if (mode == 1) { continueRecording(); }   // If we're playing or recording, carry on...
  if (mode == 2 || mode == 3) { continuePlaying(); }

}






void setFilterMode(byte filterMode){
  for(byte mixChan = 0; mixChan < 4; mixChan++){
    if(mixChan == filterMode){
      mixer2.gain(mixChan, 1);}
    else{
      mixer2.gain(mixChan, 0);}}
}
      

