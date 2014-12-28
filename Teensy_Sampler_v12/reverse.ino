
void reverseMem(uint16_t *data, int sampleCount)
{
  int i;
  uint16_t tmp;
  
  for (i=0; i<sampleCount/2-1; i++) {
    tmp = data[i];
    data[i] = data[sampleCount-i-1];
    data[sampleCount-i-1] = tmp;
    }
}

void reverseFile(char *from, char *to)
{
  long infile_offset;
  int bytes_read;
  uint16_t data[512];
  File fh_in;
  File fh_out;
  char debug[128];
  
  Serial.println("removing");
  if (SD.exists(to)) {SD.remove(to);}

  Serial.println("opening");
  fh_in = SD.open(from, FILE_READ);
  fh_out = SD.open(to, FILE_WRITE);

  Serial.println("generating debug string");
  sprintf(debug, "Reversing file %s, length %d\n", from, fh_in.size());
  Serial.println(debug);

  infile_offset = (fh_in.size()/512)*512; /* should truncate to 512-byte boundary */
  while (infile_offset >= 0) {    
    fh_in.seek(infile_offset);
    bytes_read = fh_in.read(data, 512);
    reverseMem((uint16_t*)data, bytes_read/2);
    // sprintf(debug, "%d -> %d", infile_offset, fh_out.position());
    // Serial.println(debug);
    fh_out.write((char *)data, bytes_read);
    infile_offset -= 512;
    }
  Serial.println("done");
  fh_out.close();
  fh_in.close();
}

