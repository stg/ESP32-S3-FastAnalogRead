#include "analog.h"

void setup() {
  // Initialize serial port
  Serial.begin(115200);

  // Print performance of regular functions
  performanceAnalogRead();
  performanceAnalogReadMilliVolts();

  // Initialize ADC for reading really fast on 2 pins: 5 and 6
  // NOTE that regular AnalogRead must NOT be used beyond this point
  // NOTE that pin 5 is on channel 4 and pin 6 is on channel 5
  analogReadFastInit(2, 5, 6);

  // Print performance of optimized functions
  performanceAnalogReadFast();
  performanceAnalogReadMilliVoltsFast();

  // Run asynchronous conversion example
  exampleAsync();

  // Done, go loop.
  Serial.printf("All done\n");
}

// Loop for T milliseconds
#define loopMillis(T) for(int t = millis(); (millis() - t) < T;)   

// Profile analogRead and print performance report
void performanceAnalogRead() {
  int value1 = 0, value2 = 0, count = 0;
  loopMillis(4000) {
    value1 += analogRead(5);
    value2 += analogRead(6);
    count++;
  }
  Serial.printf("analogRead              : %u samples/second (ch4:%u ch5:%u)\n", count / 2, value1 / count, value2 / count);
}

// Profile analogReadMilliVolts and print performance report
void performanceAnalogReadMilliVolts() {
  int t = millis();
  int value1 = 0, value2 = 0, count = 0;
  loopMillis(4000) {
    value1 += analogReadMilliVolts(5);
    value2 += analogReadMilliVolts(6);
    count++;
  }
  Serial.printf("analogReadMilliVolts    : %u samples/second (ch4:%umV ch5:%umV)\n", count / 2, value1 / count, value2 / count);
}

// Profile analogReadFast and print performance report
void performanceAnalogReadFast() {
  int t = millis();
  int value1 = 0, value2 = 0, count = 0;
  loopMillis(4000) {
    // NOTE that analogReadFast takes CHANNEL NUMBERS, NOT pin numbers
    value1 += analogReadFast(4);
    value2 += analogReadFast(5);
    count++;
  }
  Serial.printf("AnalogReadFast          : %u samples/second (ch4:%u ch5:%u)\n", count / 2, value1 / count, value2 / count);
}

// Profile analogReadMilliVoltsFast and print performance report
void performanceAnalogReadMilliVoltsFast() {
  int t = millis();
  int value1 = 0, value2 = 0, count = 0;
  loopMillis(4000) {
    // NOTE that analogReadMilliVoltsFast takes CHANNEL NUMBERS, NOT pin numbers
    value1 += analogReadMilliVoltsFast(4);
    value2 += analogReadMilliVoltsFast(5);
    count++;
  }
  Serial.printf("AnalogReadMilliVoltsFast: %u samples/second (ch4:%umV ch5:%umV)\n", count / 2, value1 / count, value2 / count);
}

// Asynchronous example
void exampleAsync() {
  // Start conversion on channel 4
  adcConvert(4);
  // Sit around like a nitwit, until the conversion is done
  while(adcBusy());
  // Get the result
  Serial.printf("Asynchronous read (ch4) : %u\n", adcResult());

  // Start conversion on channel 5
  adcConvert(5);
  // Go do some funky stuff in the meantime... òÓ
  int prime = ({int p=2,n=0,f;for(;n</**/100/**/;p++)for(f=2;(f<p||!++n)&&p%f++;);--p;});
  // Get the results without even checking for completions, just like all the cool kidz do it
  Serial.printf("Asynchronous read (ch5) : %u\n", adcResult());
  Serial.printf("The 100th prime number  : %d\n", prime);
}

void loop() {
  // If you can code, code in C.
  // If you can't code in C, go find something that will coddle your need for type safety, lambdas and endless tail recursion.
  // And get off my lawn.
}