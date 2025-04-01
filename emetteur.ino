#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Définition des broches pour le module nRF24L01 (adapté pour ESP32 )
#define CE_PIN 4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
// Adresse de communication
const byte adresse[6] = "00001"; 

// Structure contenant les valeurs des joysticks
struct Commande {
  int joystick1X;
  int joystick1Y;
  int joystick2X;
  int joystick2Y;
};

Commande commande;

void setup() {
  // Démarrer la communication série
  Serial.begin(115200); 
  
  if (!radio.begin()) {
    Serial.println("Erreur : Module nRF24L01 non détecté !");
  } else {
    Serial.println("Module nRF24L01 initialisé ");
  }

  radio.openWritingPipe(adresse);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(100);

  Serial.println(radio.isChipConnected() ? " Module détecté" : " Module non détecté !");
  
  radio.stopListening();
}

void loop() {
  // Lecture des joysticks (utilisation des broches analogiques du Nano)
  commande.joystick1X = analogRead(A0); // Axe X du premier joystick
  commande.joystick1Y = analogRead(A1); // Axe Y du premier joystick
  commande.joystick2X = analogRead(A2); // Axe X du second joystick
  commande.joystick2Y = analogRead(A3); // Axe Y du second joystick

  Serial.print("J1X: "); Serial.print(commande.joystick1X);
  Serial.print(" | J1Y: "); Serial.print(commande.joystick1Y);
  Serial.print(" | J2X: "); Serial.print(commande.joystick2X);
  Serial.print(" | J2Y: "); Serial.print(commande.joystick2Y);
  
  // Envoi des données prises de la commande via nRF24L01
  bool ok = radio.write(&commande, sizeof(commande));

  if (ok) {
    Serial.println(" Données envoyées !");
  } else {
    Serial.println(" Échec de l'envoi !");
  }

  delay(100);
}
