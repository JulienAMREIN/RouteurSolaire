// Routeur Solaire - V0.1 - Julien AMREIN, Licence GNU GPL V3

#include "EmonLib.h"
EnergyMonitor emon1;
byte valeurLedDimmer = 0;
byte valeurMaximumLed = 25;
byte ledPin = 9;

void setup()
{  
  Serial.begin(9600);

  emon1.voltage(2, 300, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(1, 57);       // Current: input pin, calibration.

}

void loop()
{
  //------------------------------------------------------------------------------------------------------------Calculs EmonLib

  emon1.calcVI(20,500);         // Calculate all. No.of half wavelengths (crossings), time-out

  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;             //extract Irms into Variable

  //------------------------------------------------------------------------------------------------------------Affichage sur le moniteur série pour controle sera effacé sur la version finale

  if (realPower > 0) // Si on consomme du courant depuis EDF
  {
    Serial.print("On consomme du courant depuis EDF     ");
  }

  if (realPower < 0) // Si on injecte du courant vers EDF
  {
    Serial.print("On injecte du courant vers EDF        ");
  }

  if (realPower == 0) // Si on est neutre sur la consommation EDF
  {
    Serial.print("Point d equilibre par rapport a EDF   ");
  }

  Serial.print(realPower);	       // Puissance apparente
  Serial.print(" signe          ");
  Serial.print(Irms);
  Serial.print(" Amperes        ");
  Serial.print(" valeur de la led");
  Serial.println(valeurLedDimmer);

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
    if(valeurLedDimmer <= valeurMaximumLed)
    {
      valeurLedDimmer += 1;
      analogWrite(ledPin, valeurLedDimmer);
    }
  }

  if (realPower == 0) // Si on est neutre sur la consommation EDF
  {
    delay(5000); // une fois au point d'équilibre injection / consomation , on fige le système pendant 5 secondes
  }

  //------------------------------------------------------------------------------------------------------------

}




