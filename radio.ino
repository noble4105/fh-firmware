#include "LoRaWan_APP.h"
#include "Arduino.h"

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             5        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 30 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

int16_t demod(int16_t rssi);
int16_t scaleSignal(int16_t dist, int8_t range);
int16_t timeReturn(void);
int16_t scaleTime(int16_t time, int16_t range);

typedef enum
{
    LOWPOWER,
    STATE_RX,
    STATE_TX
}States_t;

int16_t txNumber;
States_t state;
bool sleepMode = false;
int16_t Rssi,rxSize;


void setupRadio() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);
    txNumber=0;
    Rssi=0;

    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxDone = OnRxDone;

    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
    state=STATE_TX;
}



void loopRadio()
{
  switch(state)
  {
    case STATE_TX:
      delay(1000);
      txNumber++;
      sprintf(txpacket,"hello %d, Rssi : %d",txNumber,Rssi);

      Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) );
      state=LOWPOWER;

      //Hopefully this works
      drawCircles(scaleSignal(demod(Rssi), 117));

      break;
    case STATE_RX:
      Serial.println("into RX mode");
      Radio.Rx( 0 );
      state=LOWPOWER;
      break;
    case LOWPOWER:
      Radio.IrqProcess( );
      break;
    default:
      break;
  }
}

void OnTxDone( void )
{
  Serial.print("TX done......");
  state=STATE_RX;
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.print("TX Timeout......");
    state=STATE_TX;
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    Radio.Sleep( );

    // Hoping this will work too. Should just give packet time data for us to test and create distance vs time approximations
    int16_t receiveTime = timeReturn();
    Serial.printf("\r\nreceived packet \"%s\" with Rssi %d , length %d , packet time %d\r\n",rxpacket,Rssi,rxSize, receiveTime);
    Serial.println("wait to send next packet");

    state=STATE_TX;
}

/*I'm using the absolute value of the dbm because at -100 dbm it will
be converted to approximately 10MW and -5 dbm will be 3mW. IMO nice because
a farther distance is just a bigger number.*/
int16_t demod(int16_t rssi)
{
  uint16_t dist = ceil((pow(10, (abs(rssi)/10))));
 // Converts signal strenght from dBm but below a certain margin will just put it to 0 ie right beside each other

  return dist;
}

//Since our maximum value was around -100 dbm, I will put the scale range to a bit above 10MW
int16_t scaleSignal(int16_t dist, int8_t range)
{
  //dist will be the value obtained from demod
  //range will be the input range for the different UI display types
  //ie for drawCircles its 0-117 so I will put 117 as range
  int16_t max = 100000000;

  float ratio = dist/max;

  int16_t scaled = floor(ratio*range);

  return scaled;
}

//Returns the time it's been since last this function ran
int16_t timeReturn(void)
{
  uint8_t tgl = 0;
  uint16_t newTime = micros();
  uint16_t oldTime;

  if(tgl != 0)
  {
    int16_t interval = newTime - oldTime;

    return interval;
  }
  else
  {
    //literally only exists so it has to run more than once before functioning
    tgl = 1;
  }

  oldTime = newTime;
}

int16_t scaleTime(int16_t time, int16_t range)
{
  int16_t maxTime = 100; //placeholder that assumes farthest distance is 100us travel time
  int16_t ratio = (time-1000000 /*1 second delay*/)/maxTime;

  // range will be maximum input for draw functions
  int16_t dist = (ratio*range);
}







