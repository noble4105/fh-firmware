
/*I'm using the absolute value of the dbm because at -100 dbm it will
be converted to approximately 10MW and -5 dbm will be 3mW. IMO nice because
a farther distance is just a bigger number.*/
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
  //ie for drawCircles its 0-117 so I will put 117 as range
  float max = 10000.0;

  float ratio = dist/max;

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

// testing variables
int counter = 0;
int reversed = false;
//

//Testing functions \/
int getValue() {
  if ((counter < 100) && !reversed) {
    counter++;
  }
  else if ((counter > 0) && reversed) {
    counter--;
  }

  if (counter == 100) {
    reversed = true;
  } else if (counter == 0) {
    reversed = false;
  }

  return counter;
}

int fakeVal(void)
{
  int fake = 0 + rand()%(118);
  return fake;
}
