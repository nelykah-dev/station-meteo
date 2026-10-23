#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// BROCHE DU CAPTEUR (D5 sur ton image)
#define DHTPIN 5          
#define DHTTYPE DHT11     

DHT dht(DHTPIN, DHTTYPE); 
WebServer server(80);     

// --- METS TES IDENTIFIANTS WI-FI ICI ---
const char* ssid = "SemaDjea 4G";      
const char* password = "Sema2018"; 

// Fonction pour fabriquer la page web
void envoyePageWeb() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Si la lecture échoue
  if (isnan(temp) || isnan(hum)) {
    temp = 0.0;
    hum = 0.0;
  }

  // Code HTML/CSS pour afficher la page comme sur la maquette
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Station Météorologique</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; background-color: #1e1e1e; color: white; text-align: center; margin: 0; padding: 20px; }";
  html += ".container { display: flex; justify-content: center; flex-wrap: wrap; gap: 20px; margin-top: 30px; }";
  html += ".card { background-color: white; color: black; border-radius: 15px; padding: 20px; width: 220px; box-shadow: 0 4px 8px rgba(0,0,0,0.3); }";
  html += ".card h2 { margin: 0 0 10px 0; font-size: 1.2rem; }";
  html += ".card p { font-size: 2rem; font-weight: bold; margin: 0; }";
  html += "footer { margin-top: 50px; font-size: 0.9rem; color: #aaa; }";
  html += "</style></head><body>";

  html += "<h1>Station Météorologique</h1>";
  html += "<div class='container'>";
  
  // Carte Température
  html += "<div class='card'><h2>Température</h2><p>" + String(temp, 1) + "°C</p></div>";
  
  // Carte Humidité
  html += "<div class='card'><h2>Humidité</h2><p>" + String((int)hum) + "%</p></div>";
  
  html += "</div>";
  html += "<footer>copyright votre nom 2025</footer>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connexion au Wi-Fi
  Serial.print("Connexion au WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connecté !");
  Serial.print("Adresse de ta station météo : http://");
  Serial.println(WiFi.localIP());

  server.on("/", envoyePageWeb);
  server.begin();
}

void loop() {
  server.handleClient(); // Écoute si quelqu'un ouvre la page web
  // Petit test de lecture toutes les 2 secondes
  static unsigned long timer = 0;
  if (millis() - timer > 2000) {
    timer = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    Serial.print("Temp: "); Serial.print(t);
    Serial.print(" | Hum: "); Serial.println(h);
  }
}