
void reverseMem(uint16_t *data, int len)
{
  int i;
  uint16_t tmp;
  
  for (i=0; i<len/2; i++) {
    tmp = data[i];
    data[i] = data[len-i];
    data[len-i] = data[i];
    }
}

void reverseFile(char *from, char *to)
{
  long roffs;
  int n;
  uint16_t ldata[512];
  File fh;
  
  if (SD.exists(to)) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove(to); 
    }
  
  fh = SD.open(from, FILE_READ);
  roffs = (fh.size()/512)*512; /* should truncate to 512-byte boundary */
  fh.close();
  while (roffs >= 0) {
    Serial.print("roffs = ");
    Serial.println(roffs);
    
    fh = SD.open(from, FILE_READ);
    fh.seek(roffs);
    n = fh.read(ldata, 512);
    // reverseMem(ldata, n);
    fh.close();
    fh = SD.open(to, FILE_WRITE);
    fh.write((char *)ldata, n);
    fh.close();
    roffs -= 512;
    }
}

/*
void reverseFile(char *from, char *to) 
{
  File Ffrom, Fto;

  Serial.print("reverseFile(");
  Serial.print(from);
  Serial.print(",");
  Serial.print(to);
  Serial.println(")");

  
  Ffrom = SD.open(from, FILE_READ);
  Fto = SD.open(to, FILE_WRITE);
  
  reverseFh(Ffrom, Fto);
}
*/
