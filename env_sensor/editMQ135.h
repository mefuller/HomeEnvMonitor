/**************************************************************************/
/*!
@file     MQ135.h
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.

@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/
#ifndef MQ135_H
#define MQ135_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/// The load resistance on the board
#define RLOAD 10.0
/// Calibration resistance at atmospheric CO2 level
#define RZERO 76.63
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 116.6020682
#define PARB 2.769034857

/// Parameters to model temperature and humidity dependence
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018

/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 397.13

class MQ135 {
 private:
  uint8_t _pin;

 public:
  MQ135(uint8_t pin);
  float getCorrectionFactor(float t, float h) {
    return CORA * t * t - CORB * t + CORC - (h-33.)*CORD;
  }
  float getResistance() {
    int val = analogRead(_pin);
    return ((1023./(float)val) * 5. - 1.)*RLOAD;
  }
  float getCorrectedResistance(float t, float h) {
    return getResistance()/getCorrectionFactor(t, h);
  }
  float getPPM() {
    return PARA * pow((getResistance()/RZERO), -PARB);
  }
  float getCorrectedPPM(float t, float h) {
    return PARA * pow((getCorrectedResistance(t, h)/RZERO), -PARB);
  }
  float getRZero() {
    return getResistance() * pow((ATMOCO2/PARA), (1./PARB));
  }
  float getCorrectedRZero(float t, float h) {
    return getCorrectedResistance(t, h) * pow((ATMOCO2/PARA), (1./PARB));
  }
};

#endif
