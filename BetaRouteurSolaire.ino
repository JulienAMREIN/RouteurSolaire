// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance
int valeurLedDimmer = 1;
int ledPin = 9;

void setup()
{  
  Serial.begin(9600);

  emon1.voltage(2, 300, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 57);       // Current: input pin, calibration.

}

void loop()
{
  emon1.calcVI(20,500);         // Calculate all. No.of half wavelengths (crossings), time-out

  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;             //extract Irms into Variable

  Serial.print(realPower);	       // Puissance apparente
  Serial.print(" signe          ");
  Serial.print(Irms);		       // Irms
  Serial.print(" Amperes        ");
  Serial.print(valeurLedDimmer);
  Serial.println(" valeur de la led");
  
 
//------------------------------------------------------------------------------------------------------------
  if (realPower > 0) // Si on consomme du courant depuis EDF, on diminue la puissance autorisée dans le dimmer
  {
    if(valeurLedDimmer > 0)
    {
      valeurLedDimmer -= 1;
      analogWrite(ledPin, valeurLedDimmer);

    }

  }


  if (realPower < 0) // Si on injecte du courant vers EDF,on augmente la puissance autorisée dans le dimmer
  {
    if(valeurLedDimmer <= 25)
    {
      valeurLedDimmer += 1;
      analogWrite(ledPin, valeurLedDimmer);

    }

  }

//------------------------------------------------------------------------------------------------------------



}

