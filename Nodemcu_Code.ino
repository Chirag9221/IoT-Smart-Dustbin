// Add the Blynk Auth Token (this generated after adding device in Blynk App)
// Chnage the first three lines according to your project this just for example purpose
#define BLYNK_TEMPLATE_ID "TMPL3wq"                   
#define BLYNK_TEMPLATE_NAME "SmartBin"
#define BLYNK_AUTH_TOKEN "r22DYz9N_I3n5q4eJoBf"



#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MSI 3786";       // Name of your Mobile or Laptop Hotspot
char pass[] = "mynameischirag";     // password of  Mobile or Laptop Hotspot

const int trigPin = D2;     // Ultrasonic sensor trigger pin
const int echoPin = D3;     // Ultrasonic sensor echo pin
const int alertThreshold = 17;    // Threshold for sending an email alert (17cm) Once garbage is filled upto 17cm the notification will be send. So set threshold value accordingly.
const int binLength = 19;         // Height of the dustbin (19cm)

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  timer.setInterval(1000L, garbageLevelCheck); // Check garbage level every 10 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}

void garbageLevelCheck() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);

  // long distance = duration / 29 / 2;
  long distance = duration * 0.034 / 2;

  // Calculate and send the garbage level as a percentage
  
  int garbageLevelPercent = map(distance, binLength,0, 0, 100);

  // VirtualWrite helps you to print the values on the blynk UI
  Blynk.virtualWrite(V0, garbageLevelPercent);
  Blynk.virtualWrite(V1, distance);

  // print the distance calculated by ultrasonic senor (cm) and binlevel in percentage
  Serial.print("Garbage Level (cm): ");
  Serial.println(distance);
  Serial.print("Percentage of Garbage filled (%): ");
  Serial.println(garbageLevelPercent);

  // Check if the garbage level exceeds the alert threshold
  if (distance <= 3) {
    sendEmailAlert();
  }

}

void sendEmailAlert() {
  Serial.println("Sending Email Alert...");

  // Send an email using Blynk's email widget
  // Blynk.logEvent("RecipientEmail@example.com", "Garbage Level Alert", "The garbage is almost full. Please empty the dustbin.");
  Blynk.logEvent("dustbin_full", "Please clean the dustbin it's almost full");


  Serial.println("Email Alert Sent");
}

