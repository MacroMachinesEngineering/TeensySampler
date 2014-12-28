
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

void reverseFile(File from, File to)
{
  unsigned long roffs;
  int n;
  uint16_t ldata[512];
  roffs = (from.size()/512)*512; /* should truncate to 512-byte boundary */
  while (roffs >= 0) {
    from.seek(roffs);
    n = from.read(ldata, 512);
    reverseMem(ldata, n);
    to.write((char *)ldata, n);
    roffs -= 512;
    }
}
