#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>
#include <FS.h>
#include <SPIFFS.h> 

// Paramètres du réseau Wi-Fi
const char* ssid = "Livebox-B8C0";
const char* password = "tAdwttJtCdKjhiZaY6";

// Paramètres du ruban LED
#define LED_PIN 27  // Broche de contrôle du ruban LED
#define NUM_LEDS 222  // Nombre de LEDs dans le ruban
CRGB leds[NUM_LEDS];

// Création de l'objet serveur web
WebServer server(80);

// Variables pour la couleur des LEDs
int red = 0;
int green = 0;
int blue = 0;

int r;
int g;
int b;

// Variables pour les animations
int animationMode = 0;
int brightness = 127;
int extinction;
double temps1 = 0;
double temps2 = 0;


void handleRoot();
void on_off();

void setBrightness();
void handleColor();
void setLedColor(int r, int g, int b);



void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi...");
  }

  if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  }

  // Configuration du ruban LED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.show();

  // Routes du serveur web
  server.on("/", handleRoot);

  server.on("/brightness", setBrightness);
  server.on("/switchState", on_off);

  server.on("/color", handleColor);
  /*server.on("/animation", handleAnimation);
  server.on("/eteindre", set_extinction);*/

  // Démarrage du serveur web
  server.begin();

  Serial.println("Serveur démarré !");
}

void loop() {
  server.handleClient();

  // Ajoutez ici le code pour les animations du ruban LED
  temps2 = millis();
  if(extinction == 1 && temps2 >= (temps1+900000)){
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
    FastLED.show();
    extinction = 0;
  }
}

