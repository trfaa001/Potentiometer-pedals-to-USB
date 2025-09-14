#include <Joystick.h>



Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  0, 0,                    // Button Count, Hat Switch Count
  false, false, false,     // No x, Y or Z
  true, true, true,        // Rx, Ry, or Rz
  false, false,            // No rudder or throttle
  false, false, false);    // No accelerator, brake, or steering

// Define IO pins
const int pedal1 = A0;
const int pedal2 = A1;
const int pedal3 = A2;

// Start values for Min and Max values
int pedal1Max = 100;
int pedal1Min = 0;

int pedal2Max = 100;
int pedal2Min = 0;

int pedal3Max = 100;
int pedal3Min = 0;

// Time keeping
unsigned long calibrationStart;
unsigned long timeLastMaxCalibration;

// Variables for logic
unsigned long analogReadings; // Variable to store readings that add up to one big number

int amountOfReadings; // Variable to store the amount of analog readings taken

float dataPedal1[5]; // A collection of the most recent reading from pedal1
float dataPedal2[5]; // A collection of the most recent reading from pedal2
float dataPedal3[5]; // A collection of the most recent reading from pedal3

float kernel[5]; // The kernel for pedal1

float filterOutputPedal1[5]; // The output of the filter from pedal 1
float filterOutputPedal2[5]; // The output of the filter from pedal 2
float filterOutputPedal3[5]; // The output of the filter from pedal 3

float kernelSum = 0; 


// Constant variables for logic
const int amountOfReadingsForMax = 10;
const int millisForMinCalibration = 1000; // The time for taking the average of the "minimum" value of each pedal
const unsigned long timeBetweenMaxCalibration = 5; // The time between checking if there is a new max

const int kernelSize = 5; // Variables for Guassian kernel
const float sigma = 2.0; // Standard deviation

void setup() {
  pinMode(pedal1, INPUT);
  pinMode(pedal2, INPUT);
  pinMode(pedal3, INPUT);

  gaussianKernel(); // Calculates the gaussian kernel

  // Calibrate the minimum of the pedals
  pedal1Min = minCalibration(pedal1);
  pedal2Min = minCalibration(pedal2);
  pedal3Min = minCalibration(pedal3);

  Joystick.begin(); // Begin the joystick library

  setCalibrationForAxis();

  Serial.begin(9600);


}

void loop() {

  if ((millis() - timeLastMaxCalibration) > timeBetweenMaxCalibration){
      pedal1Max = maxCalibration(1, pedal1Max);
      pedal2Max = maxCalibration(2, pedal2Max);
      pedal3Max = maxCalibration(3, pedal3Max);
      setCalibrationForAxis();

      timeLastMaxCalibration = millis();
  }

  takeInputs();
  calculateOutput();

  Joystick.setRxAxis(filterOutputPedal1[kernelSize-1]);
  Joystick.setRyAxis(filterOutputPedal2[kernelSize-1]);
  Joystick.setRzAxis(filterOutputPedal3[kernelSize-1]);

}

void setCalibrationForAxis(){
  // Sets the range for the pedals

  Joystick.setRxAxisRange(pedal1Min, pedal1Max);
  Joystick.setRyAxisRange(pedal2Min, pedal2Max);
  Joystick.setRzAxisRange(pedal3Min, pedal3Max);
}

int minCalibration(int analogPin){
  // Reset the variables so the function can be used multiple times
  analogReadings = 0;
  amountOfReadings = 0;
  calibrationStart = millis();

  //Take the average of all the reading in a second and return it (asume the user doesnt touch the pedals the first 3 seconds)
  while ((millis() - calibrationStart) < millisForMinCalibration){
    analogReadings += analogRead(analogPin);
    amountOfReadings += 1;

  }

  return analogReadings / amountOfReadings;
}

int maxCalibration(int pedal, int pedalMax){
  // Reset the variables so the function can be used multiple times
  analogReadings = 0;
  amountOfReadings = amountOfReadingsForMax;

  if (pedal == 1){
    for (int i = 0; i < amountOfReadingsForMax; i++){ //We are only doing 10 readings to save on proccesing time
      analogReadings += filterOutputPedal1[kernelSize-1];
    }
  }
  else if (pedal == 2){
    for (int i = 0; i < amountOfReadingsForMax; i++){ //We are only doing 10 readings to save on proccesing time
      analogReadings += filterOutputPedal2[kernelSize-1];
    }
  }
  else if (pedal == 3){
    for (int i = 0; i < amountOfReadingsForMax; i++){ //We are only doing 10 readings to save on proccesing time
      analogReadings += filterOutputPedal3[kernelSize-1];
    }
  }


  if ((analogReadings / amountOfReadingsForMax) > pedalMax){
    
    return analogReadings / amountOfReadingsForMax;
  }
  else{
    return pedalMax;
  }

    
}

void takeInputs(){
  // Moves the data in the data array one point forward, registering the input and saving the 4 previous ones for later use

  for (int i = 0; i < kernelSize; i++){
    if (i == kernelSize - 1){
      dataPedal1[i] = analogRead(pedal1);
      dataPedal2[i] = analogRead(pedal2);
      dataPedal3[i] = analogRead(pedal3);
    }
    else{ 
      dataPedal1[i] = dataPedal1[i + 1];
      dataPedal2[i] = dataPedal2[i + 1];
      dataPedal3[i] = dataPedal3[i + 1];
    } 
  }
}

void gaussianKernel() {
  kernelSum = 0;

  for (int i = 0; i < kernelSize; i++) {
    float x = i - kernelSize / 2;
    kernel[i] = exp(-(pow(x, 2)) / (2 * pow(sigma, 2)));
    kernelSum += kernel[i];
  }
  for (int i = 0; i < kernelSize; i++) {
    kernel[i] /= kernelSum;
  }
}

void convolve1d(float* data, float* kernel, float* output, int size) {
  int halfSize = size / 2;

  for (int i = 0; i < size; i++) {
    float sum = 0.0;
    for (int j = 0; j < size; j++) {
      int dataIndex = i + j - halfSize;
      if (dataIndex >= 0 && dataIndex < size) {
        sum += data[dataIndex] * kernel[j];
      }
    }
    output[i] = sum;
  }
}

void calculateOutput(){
  convolve1d(dataPedal1, kernel, filterOutputPedal1, kernelSize);
  convolve1d(dataPedal2, kernel, filterOutputPedal2, kernelSize);
  convolve1d(dataPedal3, kernel, filterOutputPedal3, kernelSize);
}