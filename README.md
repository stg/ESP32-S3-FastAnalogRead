ESP32-S3-FastAnalogRead
=======================

Arduino IDE sketch that reads ADC1 very fast, and provides asynchronous ADC conversions on the ESP32-S3.

I needed to read the ADC really fast, and I needed asynchronous ADC conversions.  
I didn't need thread-safety.

At the point of writing (esp32 version 1.0.5), the API didn't have me covered.

*Performance of regular 'ol functions:*

    analogRead:           16313 samples/second
    analogReadMilliVolts: 10909 samples/second
	
*Performance of these functions:*

    AnalogReadFast:           109984 samples/second
    AnalogReadMilliVoltsFast: 107987 samples/second
	
Thats **670%** over regular analogRead and **980%** over calibrated millivolt readings.

API
---

`dcReadFastInit(<number-of-pins>, <pin>(, <pin>(, ...))`

Initializes ADC and pins - don't use regular analogRead after initializing!

`analogReadFast(<channel>)`

Convert and return result for the specified **channel (NOT pin)** - corresponding pin must be in the list of pins during initialization.

`analogReadMilliVoltsFast(<channel>)`

Same as analogReadFast, but returns a calibrated millivolt reading.

`adcConvert(<channel>)`

Start a conversion and return immediately - don't call unless you know the ADC is not busy.

`adcBusy()`

Returns true if the ADC is still converting.

`adcResult()`

Return the result of the conversion - don't call unless you know a conversion has completed.

`adcApply(<value>)`

Apply calibration on, and convert ADC result (from `analogReadFast` or `adcResult`) to millivolts.  
Takes a value in the range 0-2**ADC_CAL_RESOLUTION (default 0-4095).
 
