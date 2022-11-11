#ifndef _ANALOGREADFAST_H_
#define _ANALOGREADFAST_H_
#ifdef __cplusplus
extern "C" {
#endif

/* "Library" to read ADC1 really fast - 6-10x faster than the regular API.
 * Also supports doing asynchronous conversions.
 * BUT it's not thread-safe.
 * 
 * adcReadFastInit(<number-of-pins>, <pin>(, <pin>(, ...))
 * Initializes ADC and pins - don't use regular analogRead after initializing!
 * 
 * analogReadFast(<channel>)
 * Read the specified channel, which should be in the list of pins during initialization
 * 
 * analogReadMilliVoltsFast(<channel>)
 * Same as analogReadFast, but returns calibrated millivolt reading.
 * 
 * adcConvert(<channel>)
 * Start a conversion and return immediately - don't call unless you know the ADC is not busy
 * 
 * adcBusy()
 * Returns true if the ADC is still converting
 * 
 * adcResult()
 * Return the result of the conversion - don't call unless you know a conversion has completed
 * 
 * adcApply(<value>)
 * Apply calibration and conversion to millivolts
 * Takes a value in the range 0-2**ADC_CAL_RESOLUTION (typically 0-4095)
 */

#include <stdint.h>
#include <stdbool.h>
#include <hal/adc_hal.h>
#include <esp32-hal-adc.h>

#define ADC_CAL_USE
#define ADC_RESOLUTION           12 // Set ADC to 12-bit ADC resolution
#define ADC_ATTEN          ADC_11db // Set ADC to 11dB attenuation

#ifdef ADC_CAL_USE
#define ADC_CAL_SIZE              6 // Calibration table with (1**N)=64 points (RAM use: 2 bytes per point)
#define ADC_CAL_RESOLUTION       12 // Conversion takes input in range 0-4095 (1**N-1)
// Number of bits to shift left by to make analogReadFast result suitable for applyCalibration, given settings above
#define ADC_CAL_SHIFT (ADC_CAL_RESOLUTION - ADC_RESOLUTION)
#endif

void     analogReadFastInit(uint8_t pins, ...);
#ifdef ADC_CAL_USE
uint16_t adcApply(uint32_t v);
#endif

static inline void  __attribute__((always_inline)) adcConvert(uint8_t channel) {
    SENS.sar_meas1_ctrl2.sar1_en_pad = (1 << channel);
    SENS.sar_meas1_ctrl2.meas1_start_sar = 0;
    SENS.sar_meas1_ctrl2.meas1_start_sar = 1;
}

static inline bool __attribute__((always_inline)) adcBusy() {
    return !(bool)SENS.sar_meas1_ctrl2.meas1_done_sar;
}

static inline uint16_t  __attribute__((always_inline)) adcResult() {
    return HAL_FORCE_READ_U32_REG_FIELD(SENS.sar_meas1_ctrl2, meas1_data_sar);
}

static inline uint16_t  __attribute__((always_inline)) analogReadFast(uint8_t channel) {
    SENS.sar_meas1_ctrl2.sar1_en_pad = (1 << channel);
    SENS.sar_meas1_ctrl2.meas1_start_sar = 0;
    SENS.sar_meas1_ctrl2.meas1_start_sar = 1;
    while(!(bool)SENS.sar_meas1_ctrl2.meas1_done_sar);
    return HAL_FORCE_READ_U32_REG_FIELD(SENS.sar_meas1_ctrl2, meas1_data_sar);
}

#ifdef ADC_CAL_USE
static inline uint16_t __attribute__((always_inline)) analogReadMilliVoltsFast(uint8_t channel)  {
    return adcApply(analogReadFast(channel) << ADC_CAL_SHIFT);
}
#endif

#ifdef __cplusplus
}
#endif
#endif
