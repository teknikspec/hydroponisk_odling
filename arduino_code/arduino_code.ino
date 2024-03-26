// för tds
#define TdsSensorPin A1
#define VREF 5.0              // analog reference voltage(Volt) of the ADC
#define SCOUNT  30            // sum of sample point


#define MINERAL_TAK 600
#define MINERAL_BOTTEN 230


//för on off
#define pin A0
int value = 0;
int minimum = 500;


// för pump
#define pumpPin 8
int nutritionPump = 0;

int analogBuffer[SCOUNT];     // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;


float averageVoltage = 0;
float tdsValue = 0;
float temperature = 20;       // current temperature for compensation


// För on/off vattenmätaren
const int LED = 2;
int check = 0;
int val = 0;

//pump Timer
unsigned long currentMillis = millis();
unsigned long nextStop = 2000;


int mode = 0;


// median filtering algorithm
int getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}


void setup(){
  Serial.begin(9600);
  pinMode(TdsSensorPin, INPUT);


  // För on/off vattenmätaren
  pinMode(pin, INPUT);


  //för pump
  pinMode(pumpPin, OUTPUT);
}

void loop(){
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U) {     // every 40 milliseconds, read the analog value from the ADC
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    // read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT) {
      analogBufferIndex = 0;
    }
  }  


  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U) {
    printTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++) {
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    }


    // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;


    // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    // temperature compensation
    float compensationVoltage = averageVoltage / compensationCoefficient;


    // convert voltage value to tds value
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage * 0.5);

    tdsValue = ((tdsValue-230) / (MINERAL_TAK - MINERAL_BOTTEN)) * 100;
    
    
  }
 
  currentMillis = millis();
  if(currentMillis >= nextStop){
    value = analogRead(pin);

    String printvalue1 = String(tdsValue) + " 1";
    String printvalue0 = String(tdsValue) + " 0";
    if(value > 0){
      Serial.println(printvalue1);
    }
    else{
      Serial.println(printvalue0);
    }
    if(tdsValue < minimum){
      nutritionPump = 1;
    }
    else{
      nutritionPump = 0;
    }
    nextStop = millis()+2000;
  }

  switch (nutritionPump) {
    case 1:
      digitalWrite(pumpPin, HIGH);
      break;
    default:
      digitalWrite(pumpPin, LOW);
  }

}
