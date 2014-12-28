// Record sound as raw data to a SD card, and play it back.
//
// Requires the audio shield:
//   http://www.pjrc.com/store/teensy3_audio.html
//
// Three pushbuttons need to be connected:
//   Record Button: pin 0 to GND
//   Reverse Button:   pin 1 to GND
//   Play Button:   pin 2 to GND
//
// This example code is in the public domain.

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
AudioInputI2S            i2s2;           //xy=70,39
AudioPlaySdRaw           playRaw1;       //xy=225,56
AudioAnalyzePeak         peak1;          //xy=225,89
AudioRecordQueue         queue1;         //xy=225,124
AudioMixer4              mixer1;         //xy=351,53
AudioOutputI2S           i2s1;           //xy=473,54
AudioConnection          patchCord1(i2s2, 0, peak1, 0);
AudioConnection          patchCord2(i2s2, 0, queue1, 0);
AudioConnection          patchCord3(i2s2, 0, mixer1, 0);
AudioConnection          patchCord4(playRaw1, 0, mixer1, 1);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=355,121
// GUItool: end automatically generated code

int selectedSample = 0;
int fwdRev = 0; // 0 == forward, 1 == reverse
#define reverse 1
#define forward 0
//char *sampleNamesFwd[] = {"R0.RAW","R1.RAW","R2.RAW","R3.RAW","R4.RAW","R5.RAW","R6.RAW","R7.RAW","R8.RAW","R9.RAW","R10.RAW","R11.RAW","R12.RAW","R13.RAW","R14.RAW","R15.RAW"};
//char *sampleNamesRev[] = {"R0R.RAW","R1R.RAW","R2R.RAW","R3R.RAW","R4R.RAW","R5R.RAW","R6R.RAW","R7R.RAW","R8R.RAW","R9R.RAW","R10R.RAW","R11R.RAW","R12R.RAW","R13R.RAW","R14R.RAW","R15R.RAW"};
//char **sampleNames = sampleNamesFwd;
char *sampleNames[2][16] = {{"R0.RAW","R1.RAW","R2.RAW","R3.RAW","R4.RAW","R5.RAW","R6.RAW","R7.RAW","R8.RAW","R9.RAW","R10.RAW","R11.RAW","R12.RAW","R13.RAW","R14.RAW","R15.RAW"},
{"R0R.RAW","R1R.RAW","R2R.RAW","R3R.RAW","R4R.RAW","R5R.RAW","R6R.RAW","R7R.RAW","R8R.RAW","R9R.RAW","R10R.RAW","R11R.RAW","R12R.RAW","R13R.RAW","R14R.RAW","R15R.RAW"}};


// char sample0name[] = "R0.RAW";
// char ** sampleNames = (char**) &sample0name;


byte LEDpin = 6;


// Bounce objects to easily and reliably read the buttons
Bounce buttonRecord = Bounce(0, 8);
Bounce buttonReverse = Bounce(1, 8);  // 8 = 8 ms debounce time
Bounce buttonPlay =   Bounce(2, 8);


// which input on the audio shield will be used?
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Remember which mode we're doing
int mode = 0;  // 0=stopped, 1=recording, 2=playing, 3=reverse

// The file where data is recorded
File frec;




void setup() {
  // Configure the pushbutton pins
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  //pinMode(LEDpin, PWM);

  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory(60);

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(0.9);

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
//   display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
//   display.setTextSize(1);
//   display.setTextColor(WHITE);

}



static int i = 0;


void loop() {
  
  // First, read the buttons
  buttonRecord.update();
  buttonReverse.update();
  buttonPlay.update();

 // if (!i) {
 //   delay(5000);
 //   }

 // if (i<10) {
 //   Serial.print("loop(): ");
 //   Serial.println(selectedSample);
 //   }

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
    if (mode == 0) {mode = 3; startPlaying();}
    if (mode == 1) {stopRecording(); mode = 3; startPlaying();} 
    if (mode == 2) {stopPlaying(); mode = 3; startPlaying();}
    if (mode == 3) {stopPlaying(); mode = 3; startPlaying();}
  }
  if (buttonReverse.risingEdge()) {
    Serial.println("Reverse Button Up");
    Serial.println(sampleNames[fwdRev][selectedSample]);
    stopPlaying();
  }
  
  if (buttonPlay.fallingEdge()) {
    fwdRev = forward;
    if (mode == 3) {stopPlaying(); mode = 2; startPlaying();}
    if (mode == 2) {mode = 2; startPlaying();}
    if (mode == 1) {stopRecording(); mode = 2; startPlaying();}
    if (mode == 0) {mode = 2; startPlaying();}
  }
  if (buttonPlay.risingEdge()) {
    Serial.println("Play Button Up");
    Serial.println(sampleNames[fwdRev][selectedSample]);
    mixer1.gain(1, 0);
    mixer1.gain(0, 1);
    stopPlaying();
    Serial.println("Play Button Up end");
    Serial.println(selectedSample);
    Serial.println(sampleNames[fwdRev][selectedSample]);
  }

  // If we're playing or recording, carry on...
  if (mode == 1) {
    continueRecording();
  }
  if (mode == 2 || mode == 3) {
    continuePlaying();
  }

  // when using a microphone, continuously adjust gain
 // if (myInput == AUDIO_INPUT_MIC) adjustMicLevel();
  
  /*
  // Peak Detection
  if( peak1.available()) { 
     //Serial.println( peak1.read() );
     if( peak1.read() > .89) {analogWrite(LEDpin, 255);}   // turn the LED on (HIGH is the voltage level)
     else {analogWrite(LEDpin, 0);} 
    // analogWrite(LEDpin, peak1.read()*200);
   }
  */
   /*
     display.clearDisplay();
     display.setCursor(0,0);}
     display.println(peak1.read());
     display.display();*/
     
    
    // UNCOMMENT FOR SELECTING SAMPLE NUMBER
    // selectedSample = analogRead(15)>>6;

    /*
    if (i<10) {
      Serial.print("loop() end: ");
      Serial.println(selectedSample);
      }
    i++;
     */
     if(!(millis()%10000)) {Serial.println(AudioMemoryUsageMax());}
}



void adjustMicLevel() {
  // TODO: read the peak1 object and adjust sgtl5000_1.micGain()
  // if anyone gets this working, please submit a github pull request :-)
}

