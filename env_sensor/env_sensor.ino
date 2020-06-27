//Credits

/* 
 PPD42 
 code from Seeed Studio
 https://wiki.seeedstudio.com/Grove-Dust_Sensor/
*/ 

/* 
 MQ9 
 modified on 19 Feb 2019 
 by Saeed Hosseini 
 https://electropeak.com/learn/ 
 N.B. MQ9 requires 24-48 h warm-up, followed by calibration to determine R0 value (see mq9cal)
*/ 

/*
 * Electronics Projects Hub - https://electronicsprojectshub.com
 * 
@file     MQ135.cpp
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3
 */

//PPD42 particle counts - default PM10
int dustpin4 = 8; //P1
unsigned long durationP1;
unsigned long lowpulseoccupancyP1 = 0;
float ratioP1 = 0;
float concentrationP1 = 0;
unsigned long starttime;
unsigned long sampletime_ms = 3000;//sample 3s ;

//PPD42: add P2 to count 2.5-10 micron
int dustpin2 = 7; //P2
unsigned long durationP2;
unsigned long lowpulseoccupancyP2 = 0;
float ratioP2 = 0;
float concentrationP2 = 0;

//MQ9
float sensor_volt; 
float RS_gas; 
float ratio; 
//-Replace the name "R0" with the value of R0 in the demo of First Test -/ 
float R0 = 0.91; 
int sensorValue;

//MQ135
#include "MQ135.h"
#define ANALOGPIN 3 //Arduino pin-in
#define RZERO 206.85 //Rzero calibration value
MQ135 gasSensor = MQ135(ANALOGPIN); 
float rzero;
float ppm;
    
void setup() 
{
    //general
    Serial.begin(9600);
    
    //PPD42
    pinMode(dustpin4,INPUT);
    pinMode(dustpin2,INPUT);
    starttime = millis();//get the current time;

    //MQ135    
    rzero = gasSensor.getRZero();
    Serial.print("MQ135 RZERO calibration value: ");
    Serial.println(rzero);      

}

void loop() 
{
    //PPD42
    durationP1 = pulseIn(dustpin4, LOW);
    lowpulseoccupancyP1 = lowpulseoccupancyP1+durationP1;
    
    durationP2 = pulseIn(dustpin2, LOW);
    lowpulseoccupancyP2 = lowpulseoccupancyP2+durationP2;
    
    if ((millis()-starttime) > sampletime_ms)//if the sample time == 30s
    {
        ratioP1 = lowpulseoccupancyP1/(sampletime_ms*10.0);  // Integer percentage 0=>100
        concentrationP1 = 1.1*pow(ratioP1,3)-3.8*pow(ratioP1,2)+520*ratioP1+0.62; // using spec sheet curve
        Serial.print("lowpulseoccupancyP1 = ");
        Serial.println(lowpulseoccupancyP1);
        Serial.print("ratioP1 = ");
        Serial.println(ratioP1);
        Serial.print("concentrationP1 = ");
        Serial.println(concentrationP1); //1-10 micron count
        lowpulseoccupancyP1 = 0;

        ratioP2 = lowpulseoccupancyP2/(sampletime_ms*10.0);  // Integer percentage 0=>100
        Serial.print("lowpulseoccupancyP2 = ");
        Serial.println(lowpulseoccupancyP2);
        Serial.print("ratioP2 = ");
        Serial.println(ratioP2);
        Serial.print("concentrationP2 = ");
        Serial.println("no calculation implemented yet"); //1-10 micron count
                
        starttime = millis();

        //MQ9
        //Dropped pin 3 digital out and warning LED
        //place in PPD 42 loop to have same output timing
        sensorValue = analogRead(A0);
        sensor_volt = ((float)sensorValue / 1024) * 5.0; 
        RS_gas = (5.0 - sensor_volt) / sensor_volt; // Depend on RL on yor module 
        ratio = RS_gas / R0; // ratio = RS/R0 
        //------------------------------------------------------------/ 
        Serial.print("sensor_volt = "); 
        Serial.println(sensor_volt); 
        Serial.print("RS_ratio = "); 
        Serial.println(RS_gas); 
        Serial.print("Rs/R0 = "); 
        Serial.println(ratio);  

        //MQ135
        ppm = gasSensor.getPPM();
        Serial.print("CO2 concentration (ppm): ");
        Serial.println(ppm);


        //break
        Serial.print("\n\n");
    }
}
