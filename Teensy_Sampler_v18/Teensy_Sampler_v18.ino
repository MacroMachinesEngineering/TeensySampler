
// Three pushbuttons need to be connected:
//   Record Button: pin 0 to GND
//   Reverse Button:   pin 1 to GND
//   Play Button:   pin 2 to GND

#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);


// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=63.333343505859375,68.88888602786594
AudioPlaySdRaw           playRaw1;       //xy=85.5555648803711,127.77777290344238
AudioAnalyzePeak         peak1;          //xy=203.33334350585938,83.33333492279053
AudioRecordQueue         queue1;         //xy=205.55553436279297,37.77777099609375
AudioEffectEnvelope      envelope2;      //xy=231.11108779907227,162.22223472595215
AudioEffectEnvelope      envelope1;      //xy=232.22216033935547,126.66667556762695
AudioFilterStateVariable filter1;        //xy=394.4444389343262,153.33333587646484
AudioMixer4              mixer2;         //xy=527.7777824401855,144.44445419311523
AudioMixer4              mixer1;         //xy=663.8889465332031,81.11111068725586
AudioOutputI2S           i2s1;           //xy=779.3333778381348,84.00000381469727

AudioConnection          patchCord1(i2s2, 0, queue1, 0);
AudioConnection          patchCord2(i2s2, 0, mixer1, 0);
AudioConnection          patchCord3(i2s2, 0, peak1, 0);
AudioConnection          patchCord4(playRaw1, envelope1);
AudioConnection          patchCord5(envelope2, 0, filter1, 1);
AudioConnection          patchCord6(envelope1, 0, filter1, 0);
AudioConnection          patchCord7(envelope1, 0, mixer2, 0);
AudioConnection          patchCord8(filter1, 0, mixer2, 1);
AudioConnection          patchCord9(filter1, 1, mixer2, 2);
AudioConnection          patchCord10(filter1, 2, mixer2, 3);
AudioConnection          patchCord11(mixer2, 0, mixer1, 1);
AudioConnection          patchCord12(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=784.444465637207,136.66666793823242
// GUItool: end automatically generated code



#define reverse 1
#define forward 0
int selectedSample = 0;
int fwdRev = 0; // 0 == forward, 1 == reverse
char *sampleNames[2][16] = {{"R0.RAW","R1.RAW","R2.RAW","R3.RAW","R4.RAW","R5.RAW","R6.RAW","R7.RAW","R8.RAW","R9.RAW","R10.RAW","R11.RAW","R12.RAW","R13.RAW","R14.RAW","R15.RAW"},
{"R0R.RAW","R1R.RAW","R2R.RAW","R3R.RAW","R4R.RAW","R5R.RAW","R6R.RAW","R7R.RAW","R8R.RAW","R9R.RAW","R10R.RAW","R11R.RAW","R12R.RAW","R13R.RAW","R14R.RAW","R15R.RAW"}};

byte LEDpin = 6;

// Bounce objects to easily and reliably read the buttons
Bounce buttonRecord = Bounce(0, 8);
Bounce buttonReverse = Bounce(1, 8);  // 8 = 8 ms debounce time
Bounce buttonPlay =   Bounce(2, 8);

// which input on the audio shield will be used?
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Remember which mode we're doing
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
//   display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
//   display.setTextSize(1);
//   display.setTextColor(WHITE);
  setFilterMode(noFilter);

}



void loop() {
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

  // If we're playing or recording, carry on...
  if (mode == 1) {
    continueRecording();
  }
  if (mode == 2 || mode == 3) {
    continuePlaying();
  }


}




void setFilterMode(byte filterMode){
  for(byte mixChan = 0; mixChan < 4; mixChan++){
    if(mixChan == filterMode){
      mixer2.gain(mixChan, 1);}
    else{
      mixer2.gain(mixChan, 0);}}
}
      

