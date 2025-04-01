#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
// Adresse de communication
const byte adresse[6] = "00001"; 

// Structure des commandes reçues
struct Commande {
  int joystick1X;
  int joystick1Y;
  int joystick2X;
  int joystick2Y;
 
};

Commande commande;
//bool ack = true; // Réponse à envoyer à l'émetteur

void setup() {
  Serial.begin(115200);
 // SPI.begin();
  radio.begin();
  if (!radio.begin()){
    Serial.println(" Erreur : Module nRF24L01 non détecté !");
  }else{
    Serial.println("***********réglé***********");
  }
  Serial.begin(115200);
  Serial.println(radio.isChipConnected() ? " Module détecté" : " Module non détecté !");

  radio.openReadingPipe(1, adresse);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(100);
  radio.startListening();
  Serial.println(radio.isChipConnected() ? " Module détecté" : " Module non détecté !");

  Serial.println(" Récepteur prêt !");
}

void loop() {
  if (radio.available()) {
    radio.read(&commande, sizeof(commande));

    // Affichage des données reçues
    Serial.print("J1X: "); Serial.print(commande.joystick1X);
    Serial.print(" | J1Y: "); Serial.print(commande.joystick1Y);
    Serial.print(" | J2X: "); Serial.print(commande.joystick2X);
    Serial.print(" | J2Y: "); Serial.print(commande.joystick2Y);
  
  }
   // Envoi des données via nRF24L01
  bool ok = radio.write(&commande, sizeof(commande));

  if (ok) {
    Serial.println(" Données reçue !");
  } else {
    Serial.println(" Échec de réception !");
  }

}
