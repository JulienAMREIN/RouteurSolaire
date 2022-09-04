// Routeur Solaire - V0.1 - Julien AMREIN, Licence GNU GPL V3

// La résistance testée sur le dispositif doit etre au minimum de [33 000 ohms] pour le photo-régulateur DIY

#include "EmonLib.h"

EnergyMonitor emon1;

byte ledPin = 9;                                          // Variable pour déterminer la pin de branchement de la led en PWM
byte valeurLedDimmer = 0;                                  // Variable de la valeur de sortie sur la pin "ledPin" en PWM pour faire varier l'intensité lumineuse et piloter le dimmer de 0 à 255
byte statusCourantLed = 0;                                // 0= initial   1=était en conso EDF   2=était en injection EDF

int delayChangementEtat = 5000;                           // Temps de maintient de la luminosité de la led lord d'une bascule injection/conso et conso/injection
byte valeurMaximumLed = 30;                               // Variable pour définir l'amplitude maximum de la lumière de la led qui pilote le dimmer
byte valeurIncrementationLed = 1;                         // Le pas d'incrémentation pour augmenter la luminosité de la LED et se rapprocher du seuil consomation depuis EDF
byte valeurDecrementationLed = 1;                         // Le pas de décrémentation pour diminuer la luminosité de la LED et stopper rapidement la consomation depuis EDF




void setup()
{  
  Serial.begin(9600);                                     // Initialisation pour le moniteur série, sera effacé dans la version finale

  emon1.voltage(2, 300, 1.7);                             // Tension: input pin, calibration, phase_shift
  emon1.current(1, 57);                                   // Courrant: input pin, calibration.

}

void loop()
{
  //------------------------------------------------------------------------------------------------------------Calculs EmonLib
  
  delay(1000);
  emon1.calcVI(20,500);                                   // Calculate all. No.of half wavelengths (crossings), time-out

  float realPower       = emon1.realPower;                //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;            //extract Apparent Power into variable
  float powerFActor     = emon1.powerFactor;              //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;                     //extract Vrms into Variable
  float Irms            = emon1.Irms;                     //extract Irms into Variable
 
  // On Consomme de l'électricité------------------------------------------------------------------------------------------------------------
  
  if (realPower > 0)                                     // Si on consomme du courant depuis EDF
  {

    if(statusCourantLed == 2)                    // Et si avant on injectai de l'électricité depuis EDF
    {                                            // Alors
      Serial.println("----------------------------------------------------------");
      Serial.print("Notification:   <BASCULE>    INJECTION >>> CONSOMMATION          ");
      Serial.println(valeurLedDimmer);
      Serial.println("----------------------------------------------------------");
    }
    Serial.print(valeurLedDimmer);
    Serial.println("          On consomme du courant depuis EDF               "); 

    if(valeurLedDimmer > 0)                              // Et si la LED du dimmer est allumée
    {                                                    // ALORS
      valeurLedDimmer -= valeurDecrementationLed;        // On diminue la valeur de la variable de luminosité de la led qui controle le dimmer d'une valeur de variable décrémentation

      if(valeurLedDimmer < 0)                            // On refuse une valeur négative pour la variable "valeurLedDimmer" en attribuant la valeur 0 le cas echeant.
      {
        valeurLedDimmer = 0;
      }
      analogWrite(ledPin, valeurLedDimmer);              // Et donc on fait diminuer la puissance autorisée dans le dimmer en baissant la lumière émise par la LED
    }

    if(valeurLedDimmer == 0)                             // Et si la LED du dimmer est éteinte
    {                                                    // ALORS
      delay(2000);                                       // on attends X secondes avant de continuer dans le code
    }

    statusCourantLed = 1;                                // Attribution de la valeur d'état précédent     0= initial   1=était en conso EDF   2=était en injection EDF

  }

  // On Injecte de l'électricité------------------------------------------------------------------------------------------------------------

  if (realPower < 0)                                     // Si on injecte du courant vers EDF
  {

    if(statusCourantLed == 1)                            // Et si avant on consommait de l'électricité depuis EDF
    {                                                    // Alors
      Serial.println("----------------------------------------------------------");
      Serial.print("Notification:   <BASCULE>    CONSOMMATION >>> INJECTION          ");
      Serial.println(valeurLedDimmer);
      Serial.println("----------------------------------------------------------");


      delay(delayChangementEtat);                        // on reste X secondes avant de continuer dans le code
    }


    Serial.print(valeurLedDimmer);
    Serial.println("          On injecte du courant vers EDF               ");



    if(valeurLedDimmer < valeurMaximumLed)               // Et si on est inferieur à la valeurMaximumLed
    {
      valeurLedDimmer += valeurIncrementationLed;        // On augmente la valeur de la variable de luminosité de la led qui controle le dimmer d'une valeur de variable incrémentation
      analogWrite(ledPin, valeurLedDimmer);              // Et donc on fait augmenter la puissance autorisée dans le dimmer en montant la lumière émise par la LED
    }

    if(valeurLedDimmer == valeurMaximumLed)              // Et si on est arrivé à la valeurMaximumLed
    {                                                    // Alors
      delay(2000);                                       // on attends X secondes avant de continuer dans le code
    }

    statusCourantLed = 2;                                // Attribution de la valeur d'état précédent     0= initial   1=était en conso EDF   2=était en injection EDF

  }

  // On est neutre en conso. ------------------------------------------------------------------------------------------------------------

  if (realPower == 0)                                    // Si on est neutre sur la consommation EDF
  {                                                      // cela signifie qu'on est au point d'équilibre injection / consomation
    Serial.print("Point d equilibre par rapport a EDF             ");
    valeurLedDimmer -= valeurDecrementationLed;          // On diminue la luminosité de la led qui controle le dimmer de X crans pour temporiser les variations densoleillement
    if(valeurLedDimmer < 0)                              // On refuse une valeur négative,en attribuant la valeur 0 le cas echeant.
    {
      valeurLedDimmer = 0;
    }
    analogWrite(ledPin, valeurLedDimmer);                // Et donc on fait diminuer la puissance autorisée dans le dimmer en baissant la lumière émise par la LED
    delay(2000);                                         // Puis on fige le système pendant X secondes pour limiter les variations intempestives (A tester pour validation)
  }

  //------------------------------------------------------------------------------------------------------------

}









