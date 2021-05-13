/*
 * sinusoid.c
 *
 *  Created on: May 7, 2021
 *      Author: BG0FNDN
 */
#include "stm32f4xx_hal.h"
#include <math.h>

#include "sinusoid.h"
#define OFFSET 2047
#define ANALOG_MAX_VALUE 4095
#define VCC 3300 // VCC = 3.3V => 3300mV
#define SAMPLE_TIME 1
#define PI 3.1415926

int
voltToValueConverter (int mV)
{
  double step = (VCC * 1.0) / ANALOG_MAX_VALUE;

  return (mV * 1.0 / step) - 1; // cuts the precision
}

int
sinToAnalogValue (double val, int diapasonValue) // sin() => 0 ; ANALOG_MAX_VALUE
{
  return (sin (val) * diapasonValue) + OFFSET + 0.5;
}

void
sinusoid (int mV, int Hz, DAC_HandleTypeDef *hdac)
{
  int T = 1000 / Hz;

  double step = 2 * PI / T;

  int diapasonValue = voltToValueConverter (mV);

  if (diapasonValue >= ANALOG_MAX_VALUE)
    {
      return;
    }

  for (double i = 0; i < 2 * PI; i += step)
    {
      HAL_DAC_SetValue (hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,
			sinToAnalogValue (i, diapasonValue));

      HAL_Delay (SAMPLE_TIME);
    }
}
