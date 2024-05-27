#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
 
int smallRawPin = D0;
int mediumRawPin = D1;
int largeRawPin = D2;
int smallRipePin = D3;
int mediumRipePin = D4;
int largeRipePin = D5;

#define RPWM  D6 // Right PWM pin (EN_R)
#define LPWM  D7 // Left PWM pin (EN_L)
#define R_EN  D8  // Right enable pin

#define WIFI_SSID "I'm in!"
#define WIFI_PASSWORD "connected"
#define API_KEY "ipi key"
#define DATABASE_URL "firebase url" 
 
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int smallRawCount = 0;  
int mediumRawCount = 0;  
int largeRawCount = 0;  
int smallRipeCount = 0;  
int mediumRipeCount = 0;  
int largeRipeCount = 0;  

boolean smallRawState = true;  
boolean mediumRawState = true;  
boolean largeRawState = true;  
boolean smallRipeState = true;  
boolean mediumRipeState = true;  
boolean largeRipeState = true;  

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int motor = 0;

void setup()  {  
  Serial.begin(9600);  
  pinMode(smallRawPin, INPUT);  
  pinMode(mediumRawPin, INPUT);  
  pinMode(largeRawPin, INPUT);  
  pinMode(smallRipePin, INPUT);  
  pinMode(mediumRipePin, INPUT);  
  pinMode(largeRipePin, INPUT);  
  pinMode(RPWM, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(R_EN, OUTPUT);
  digitalWrite(R_EN, HIGH);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    if (Firebase.signUp(&config, &auth, "", "")){
      Serial.println("ok");
      signupOK = true;
    }
    else{
      Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    config.token_status_callback = tokenStatusCallback;
    
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

}  

void smallRaw() {
    if (!digitalRead(smallRawPin) && smallRawState) {
      smallRawCount++;
      smallRawState = false;

      if (Firebase.RTDB.setInt(&fbdo, "raw/1/small", smallRawCount)) {
        Serial.print("Small Raw Count: ");
        Serial.println(smallRawCount, DEC);
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }

    if (digitalRead(smallRawPin )) {
      smallRawState = true;
    }
}
void smallRawChecker() {
    if (Firebase.RTDB.getInt(&fbdo, "raw/1/small")) {
      if (fbdo.dataType() == "int") {
        int firebaseValue = fbdo.intData();
        if (firebaseValue == 0 && smallRawCount != 0) {
          smallRawCount = 0;
          Serial.println("Small reset to 0 from Firebase");
        }
      }
    } else {
      Serial.println("Failed to get count from Firebase");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}

void mediumRaw() {
    if (!digitalRead(mediumRawPin) && mediumRawState) {
      mediumRawCount++;
      mediumRawState = false;

      if (Firebase.RTDB.setInt(&fbdo, "raw/1/medium", mediumRawCount)) {
        Serial.print("Medium Raw Count: ");
        Serial.println(mediumRawCount, DEC);
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }

    if (digitalRead(mediumRawPin )) {
      mediumRawState = true;
    }
}
void mediumRawChecker() {
    if (Firebase.RTDB.getInt(&fbdo, "raw/1/medium")) {
      if (fbdo.dataType() == "int") {
        int firebaseValue = fbdo.intData();
        if (firebaseValue == 0 && mediumRawCount != 0) {
          mediumRawCount = 0;
          Serial.println("Medium reset to 0 from Firebase");
        }
      }
    } else {
      Serial.println("Failed to get count from Firebase");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}

void largeRaw() {
    if (!digitalRead(largeRawPin) && largeRawState) {
      largeRawCount++;
      largeRawState = false;

      if (Firebase.RTDB.setInt(&fbdo, "raw/1/large", largeRawCount)) {
        Serial.print("Large Raw Count: ");
        Serial.println(largeRawCount, DEC);
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }

    if (digitalRead(largeRawPin )) {
      largeRawState = true;
    }
}
void largeRawChecker() {
    if (Firebase.RTDB.getInt(&fbdo, "raw/1/large")) {
      if (fbdo.dataType() == "int") {
        int firebaseValue = fbdo.intData();
        if (firebaseValue == 0 && largeRawCount != 0) {
          largeRawCount = 0;
          Serial.println("Large reset to 0 from Firebase");
        }
      }
    } else {
      Serial.println("Failed to get count from Firebase");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}

void smallRipe() {
    if (!digitalRead(smallRipePin) && smallRipeState) {
      smallRipeCount++;
      smallRipeState = false;

      if (Firebase.RTDB.setInt(&fbdo, "ripe/1/small", smallRipeCount)) {
        Serial.print("Small Ripe Count: ");
        Serial.println(smallRipeCount, DEC);
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }
    if (digitalRead(smallRipePin )) {
      smallRipeState = true;
    }
}
void smallRipeChecker() {
    if (Firebase.RTDB.getInt(&fbdo, "ripe/1/small")) {
      if (fbdo.dataType() == "int") {
        int firebaseValue = fbdo.intData();
        if (firebaseValue == 0 && smallRipeCount != 0) {
          smallRipeCount = 0;
          Serial.println("Small reset to 0 from Firebase");
        }
      }
    } else {
      Serial.println("Failed to get count from Firebase");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}

void mediumRipe() {
    if (!digitalRead(mediumRipePin) && mediumRipeState) {
      mediumRipeCount++;
      mediumRipeState = false;

      if (Firebase.RTDB.setInt(&fbdo, "ripe/1/medium", mediumRipeCount)) {
        Serial.print("Medium Ripe Count: ");
        Serial.println(mediumRipeCount, DEC);
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }
    if (digitalRead(mediumRipePin )) {
      mediumRipeState = true;
    }
}
void mediumRipeChecker() {
    if (Firebase.RTDB.getInt(&fbdo, "ripe/1/medium")) {
      if (fbdo.dataType() == "int") {
        int firebaseValue = fbdo.intData();
        if (firebaseValue == 0 && mediumRipeCount != 0) {
          mediumRipeCount = 0;
          Serial.println("Medium reset to 0 from Firebase");
        } 
      }
    } else {
      Serial.println("Failed to get count from Firebase");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}

void largeRipe() {
    if (!digitalRead(largeRipePin) && largeRipeState) {
      largeRipeCount++;
      largeRipeState = false;

      if (Firebase.RTDB.setInt(&fbdo, "ripe/1/large", largeRipeCount)) {
        Serial.print("Large Ripe Count: ");
        Serial.println(largeRipeCount, DEC);
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }
     if (digitalRead(largeRipePin )) {
      largeRipeState = true;
    }
}

void largeRipeChecker() {
    if (Firebase.RTDB.getInt(&fbdo, "ripe/1/large")) {
      if (fbdo.dataType() == "int") {
        int firebaseValue = fbdo.intData();
        if (firebaseValue == 0 && largeRipeCount != 0) {
          largeRipeCount = 0;
          Serial.println("Large reset to 0 from Firebase");
        }
      }
    } else {
      Serial.println("Failed to get count from Firebase");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}


void loop()  {  
 

 if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    //Update Count of every IR sensor
    smallRaw();
    mediumRaw();
    largeRaw();
    smallRipe();
    mediumRipe();
    largeRipe();

    //Check if value from firebase is empty of have value

    smallRawChecker();
    mediumRawChecker();
    largeRawChecker();
    smallRipeChecker();
    mediumRipeChecker();
    largeRipeChecker();

    if (Firebase.RTDB.getInt(&fbdo, "Controls/1/motor")){
      if (fbdo.dataType() == "int"){
      motor = fbdo.intData();
      digitalWrite(LPWM, LOW);  // Disable reverse direction
      analogWrite(RPWM, motor);   // Set speed (0-255), 128 for half speed
      Serial.print("Motor: ");
      Serial.println(motor);
      }
    }
    else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }

  }
}  



