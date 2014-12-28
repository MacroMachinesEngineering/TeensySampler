




void startRecording() {
  if (SD.exists(sampleNames[fwdRev][selectedSample])) {
    SD.remove(sampleNames[fwdRev][selectedSample]); }
 
  frec = SD.open(sampleNames[fwdRev][selectedSample], FILE_WRITE);
  Serial.print("Recording to ");
  Serial.println(sampleNames[fwdRev][selectedSample]);
  if (frec) {
    queue1.begin();
    mode = 1;
  }
}



void continueRecording() {
  if (queue1.available() >= 2) {
    byte buffer[512];
    // Fetch 2 blocks from the audio library and copy
    // into a 512 byte buffer.  The Arduino SD library
    // is most efficient when full 512 byte sector size
    // writes are used.
    memcpy(buffer, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    memcpy(buffer+256, queue1.readBuffer(), 256);
    queue1.freeBuffer();
    // write all 512 bytes to the SD card
    elapsedMicros usec = 0;
    frec.write(buffer, 512);
    // Uncomment these lines to see how long SD writes
    // are taking.  A pair of audio blocks arrives every
    // 5802 microseconds, so hopefully most of the writes
    // take well under 5802 us.  Some will take more, as
    // the SD library also must write to the FAT tables
    // and the SD card controller manages media erase and
    // wear leveling.  The queue1 object can buffer
    // approximately 301700 us of audio, to allow time
    // for occasional high SD card latency, as long as
    // the average write time is under 5802 us.
    //Serial.print("SD write, us=");
    //Serial.println(usec);
  }
}

void stopRecording() {
  Serial.print("stopRecording():");
  Serial.println(selectedSample);
  queue1.end();
  if (mode == 1) {
    while (queue1.available() > 0) {
      frec.write((byte*)queue1.readBuffer(), 256);
      queue1.freeBuffer();
    }
    frec.close();
    reverseFile(sampleNames[forward][selectedSample], sampleNames[reverse][selectedSample]);
  }

  mode = 0;
}

