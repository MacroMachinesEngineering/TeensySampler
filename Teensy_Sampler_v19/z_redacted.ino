  
  
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
