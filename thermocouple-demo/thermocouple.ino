#include <Arduino.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"

// for calculating mean of readings
const int numReadings = 25;

// for ADC calibration
int vref = 1100;

// assign pin A2 to temperature sensor
#define tempPin A0

void setup()
{
    Serial.begin(115200);

    // perform ADC calibration for ESP32
    esp_adc_cal_characteristics_t adc_chars;                                                                                            // declare a structure named adc_chars
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, adc_chars.vref, &adc_chars); // Define compensation of an ADC at a particular attenuation.
}

// main program
void loop()
{
    meanTempr = calcMean(tempPin);
    temperatureOut(meanTempr);
    outputReadings();
}

uint32_t calcMean(pin)
{
    // implement for loop to accumulate running sum
    int sumReadings = 0;
    for (int i = 0; i < numReadings; i++)
    {
        // sample from ADC
        sumReadings += analogRead(pin);

        // introduce delay between each sample
        delay(20);
    }
    // calc mean by diving running sum by total numner of readings
    uint32_t meanReadingInt = sumReadings / numReadings;

    return meanReadingInt;
}

void temperatureOut(int meanReadingInt)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        // to check reference voltage of ADC
        // Serial.printf("eFuse Vref:%u mV", adc_chars.vref);

        // convert analog value to some voltage value in (mV)
        float tempr = esp_adc_cal_raw_to_voltage(temprReadingInt, &adc_chars);
        // convert mV to V
        tempr = temper / 1000.0;

        /*
        THE FOLLOWING BLOCK OF CODE IS WHAT YOU NEED TO FIGURE OUT
        I HAVE PROVIDED AN EXAMPLE FOR THE THERMISTOR I HAVE USED IN THE PAST

        IT MAY NOT BE THE SAME FOR THE THERMOCOUPLE WE WILL BE USING ASK SOFTWARE LEAD FOR SPECS

        */

        // thermistors/thermocouples are basically variable resistors
        // finding value of thermistors resistance from the voltage divider:
        tempr = tempr / (3.289 - tempr) * 10000;

        /* using beta parameter to determine corresponding temperature
         B = 3976
         T @ 25C = 298.15K
         R @ 25C = 2252 ohms
        */
        tempr = 3976 / log(tempr / 2252 * exp(3976 / 298.15));
        // and convert to degrees celsius:
        tempr = tempr - 273.15;
    }
}

void outputReadings()
{
    // output readings
    Serial.print("esp_adc_cal_raw_to_voltage for temperature: ");
    Serial.println(esp_adc_cal_raw_to_voltage(temprReadingInt, &adc_chars));

    Serial.print("coeff_a: ");
    Serial.println(adc_chars.coeff_a);

    Serial.print("coeff_b: ");
    Serial.println(adc_chars.coeff_b);

    Serial.print("temperature reading: ");
    Serial.println(tempr);
    delay(1000);
}
