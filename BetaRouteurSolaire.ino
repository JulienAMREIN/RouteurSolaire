// Routeur Solaire - V0.1 - Julien AMREIN, Licence GNU GPL V3

// La résistance testée sur le dispositif doit etre au minimum de 1000 ohms

#include "EmonLib.h"
EnergyMonitor emon1;
int valeurLedDimmer = 0;                                 // Variable de la valeur de sortie sur la pin "ledPin" en PWM pour faire varier l'intensité lumineuse et piloter le dimmer de 0 à 255
byte valeurMaximumLed = 25;                               // Variable pour définir l'amplitude maximum de la lumière de la led qui pilote le dimmer
byte ledPin = 9;                                          // Variable pour déterminer la pin de branchement de la led en PWM
byte statusCourantLed = 0;                                // 0= initial 1=baisse de la luminositée de la led 2=augmentation de la luminositée de la led

void setup()
{  
  Serial.begin(9600);                                     // Initialisation pour le moniteur série, sera effacé dans la version finale

  emon1.voltage(2, 300, 1.7);                             // Tension: input pin, calibration, phase_shift
  emon1.current(1, 57);                                   // Courrant: input pin, calibration.

}

void loop()
{
  //------------------------------------------------------------------------------------------------------------Calculs EmonLib

  emon1.calcVI(20,500);                                   // Calculate all. No.of half wavelengths (crossings), time-out

  float realPower       = emon1.realPower;                //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;            //extract Apparent Power into variable
  float powerFActor     = emon1.powerFactor;              //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;                     //extract Vrms into Variable
  float Irms            = emon1.Irms;                     //extract Irms into Variable

  //------------------------------------------------------------------------------------------------------------Affichage sur le moniteur série pour controle sera effacé sur la version finale

  if (realPower > 0)                                      // Si on consomme du courant depuis EDF
  {
    Serial.print("On consomme du courant depuis EDF     ");
  }

  if (realPower < 0)                                      // Si on injecte du courant vers EDF
  {
    Serial.print("On injecte du courant vers EDF        ");
  }

  if (realPower == 0)                                     // Si on est neutre sur la consommation EDF
  {
    Serial.print("Point d equilibre par rapport a EDF   ");
  }

  Serial.print(realPower);	                          // Puissance apparente: signée positif/négatif
  Serial.print(" signe          ");
  Serial.print(Irms);
  Serial.print(" Amperes        ");
  Serial.print(" valeur de la led");
  Serial.println(valeurLedDimmer);

  //------------------------------------------------------------------------------------------------------------
  if (realPower > 0)                                     // Si on consomme du courant depuis EDF
  {
    if(valeurLedDimmer > 0)                              // Et si on est pas déja à la valeur minimum du dimmer
    {
      valeurLedDimmer -= 3;                              // On diminue la luminosité de la led qui controle le dimmer
      if(valeurLedDimmer < 0){valeurLedDimmer = 0;}      // On refuse une valeur négative.
      analogWrite(ledPin, valeurLedDimmer);              // Et donc on diminue la puissance autorisée dans le dimmer
          delay(2000);
    }
  }

  if (realPower < 0)                                     // Si on injecte du courant vers EDF
  {
    if(valeurLedDimmer <= valeurMaximumLed)              // Et si on est pas déja à la valeurMaximumLed
    {
      valeurLedDimmer += 1;                              // On augmente la luminosité de la led qui controle le dimmer
      analogWrite(ledPin, valeurLedDimmer);              // Et donc on augmente la puissance autorisée dans le dimmer
    }
  }

  if (realPower == 0)                                    // Si on est neutre sur la consommation EDF
  {                                                      // cela signifie qu'on est au point d'équilibre injection / consomation
    valeurLedDimmer -= 3;                                // On diminue la luminosité de la led qui controle le dimmer d'un cran pour temporiser les variations densoleillement
          if(valeurLedDimmer < 0){valeurLedDimmer = 0;}      // On refuse une valeur négative.
    analogWrite(ledPin, valeurLedDimmer);                // Et donc on diminue la puissance autorisée dans le dimmer
    delay(2000);                                         // Puis on fige le système pendant 5 secondes pour limiter les variations intempestives (A tester pour validation)
  }

  //------------------------------------------------------------------------------------------------------------

}






