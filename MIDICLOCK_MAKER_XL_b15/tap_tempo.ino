void tapTempo()
{
  static unsigned long lastTap = 0;
  unsigned long now = millis(); // the current time in milliseconds
  static byte tapsNumber = 0; // how many bpms have been added to the bpmTaps array
  int bpmSum = 0; // the sum of the values in the bpmTaps[] array 
  byte bpmTaps[10] = { // required for tap tempo
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  if ((now - lastTap) < 1888) // a quarter is 1333ms long at 45bpm (our slowest allowed bpm)
  {
    bpmTaps[tapsNumber % 10] = 60000/(now - lastTap);
    tapsNumber++;
    lastTap = now;
  }
  else
  {
    for (byte i = 0; i < 10; i++) // clear the bpmTaps array
    {
      bpmTaps[i] = 0;
      tapsNumber = 0;
    }
    lastTap = now;
  }

  for (byte i = 0; i < 10; i++)
  {
    if (bpmTaps[i] > 0)
    {
      bpmSum += bpmTaps[i];
    }
  }

  if (bpmSum > 0)
  {
    byte i = (tapsNumber < 11) ? tapsNumber : 10;
    bpm_tap = (bpmSum / i) *12;
    if(bpm_tap <= 200 && bpm_tap >= 20){
      bpm_int=bpm_tap;
    }

  }
}


