  
  
 /* // Peak Detection
  if( peak1.available()) { 
     //Serial.println( peak1.read() );
     if( peak1.read() > .59) {analogWrite(LEDpin, 255);}   // turn the LED on (HIGH is the voltage level)
    // else {analogWrite(LEDpin, 0);} 
     analogWrite(LEDpin, peak1.read()*200);
   }*/

   /*    
     display.clearDisplay();
     display.setCursor(0,0);}
     display.println(peak1.read());
     display.display();*/ 
    
    // UNCOMMENT FOR SELECTING SAMPLE NUMBER
    // selectedSample = analogRead(15)>>6;

  // if(!(millis()%10000)) {Serial.println(AudioMemoryUsageMax());}
  
  
  //#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);


//   display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
//   display.setTextSize(1);
//   display.setTextColor(WHITE);
