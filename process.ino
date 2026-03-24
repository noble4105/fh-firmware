
/*I'm using the absolute value of the dbm because at -100 dbm it will
be converted to approximately 10MW and -5 dbm will be 3mW. IMO nice because
a farther distance is just a bigger number.*/

const float scaleMax = 10000.0;

void frequencyHarmonize()
{
  int currentTime = millis();
  timeout = 3000 + rand()%8001;

  loopRadio();

  if(tgl == 1)
  {
    //int demodded = round(demod(mainrssi));REPLACED BY NICKS FUNCTION // Interpret signal strength from pingpong

    arrStore(mainrssi); // Shift newest demodded value into the averaging array

    int finalAverage = findAvg(avgArray); // Compute latest average signal strength

    int demodded = round(dBToMeters(finalAverage));

    scaled = scaleSignal(demodded, 115); // Scale average to usable value


    for(int i = 0; i < avgsize; i++)
    {
      Serial.printf("\nEntry %i is %i", i, avgArray[i]);
    }

    //Print numbers for testing purposes
    Serial.printf("\nrssi returned is %i, averaged rssi %i, meter conversion %i, scaled %i\n", mainrssi, finalAverage, demodded, scaled);

    cycleDisplay(displayState, scaled); //Display on screen!!!

    receivedTime = millis(); 

    tgl = 0; // Reset toggle so this only happens when new data is pulled
  }

  if((currentTime - receivedTime) > 10000 && tgl != 2)
  {
    Serial.printf("\nTime diff is %i", (currentTime-receivedTime));
    noDevices();
    tgl = 2;
  }
}

double demod(int16_t rssi)
{
  double dist = (pow(10, (abs(rssi)*0.1)))*0.000001; // 800 beside, across dcc like 300k
 // Converts signal strenght from dBm but below a certain margin will just put it to 0 ie right beside each other
  if(dist > 10000)
  {
    double averagedDist = findAvg(avgArray)*1.0;
    Serial.printf("\n THE AVERAGE WAS RETURNED AS %f", averagedDist);

    return averagedDist;
  }
  else
  {
    return dist;
  }
}

//My demod functon has changed to interpret enormous numbers from the power output
//This hasn't changed much aside from max being different since the numbers
// are slightly smaller
int scaleSignal(int dist, int range)
{
  //dist will be the value obtained from demod
  //range will be the input range for the different UI display types
  //ie for drawCircles its 0-115 so I will put 115 as range
  float ratio = dist/scaleMax;

  int scaled = round(ratio*range);

  if(scaled > range)
  {
    return range;
  }
  else
  {
    return scaled;
  }
}

//I'm making this to smooth out the perceived distance...
//The values jump a few hundred 
int findAvg(int* arr)
{
  int sum = 0;

  for(int i = 0; i < avgsize; i++)
  {
     sum = sum + arr[i];
  }

  return floor(sum/avgsize);
}

void arrStore(int item)
{
  int tempArr[avgsize];
  
  memcpy(tempArr, avgArray, sizeof(avgArray));

  for(int i = 0; i < avgsize-1; i++)
  {
    avgArray[i+1] = tempArr[i];
  }

  avgArray[0] = item;
}

void initScaling() 
{
  dBSpan = abs(dBMin - dBMax);
  powerForRange = log10(meterRange);
}

double dBToMeters(double dB)
{
  if (dB > dBMax) dB = dBMax;
  if (dB < dBMin) dB = dBMin;

  double returnval = pow(10.0, 4.0 * (dBMax - dB) / dBSpan);

  if(returnval > scaleMax)
  {
    returnval = findAvg(avgArray)*1.0;

    Serial.printf("THE AVERAGE WAS RETURNED AS %0.3f", returnval);
    return returnval;
  }

  return returnval;
}



