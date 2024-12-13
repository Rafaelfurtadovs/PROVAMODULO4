#include <WiFi.h>
#include <HTTPClient.h>


#define green_led 41 // number of green led pin
#define red_led 40 // number of red led pin
#define yellow_led 9 // number of yellow led pin

const int buttonPin = 18;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status

const int ldrPin = 4;  // the number of the ldr pin
int threshold= -1;

void setup() {

  // initial setting of the pins for led control as (OUTPUTS)
  
  pinMode(green_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  pinMode(yellow_led, OUTPUT);

  // initialize entries
  pinMode(buttonPin, INPUT); // Initialize the pushbutton pin as an input
  pinMode(ldrPin, OUTPUT);
  
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);
  digitalWrite(yellow_led, LOW);

  Serial.begin(9600); // setting for debug by serial interface between ESP and the desktop with baud rate 9600

  WiFi.begin("Wokwi-GUEST", ""); // conection to open WiFi with SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Wifi conected sucessfully!"); // considerating that get off the loop above, ESP32 now is conected on WiFi (other option is putting this command on the if below)

  // Verifify button state
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Button pressed!");
  } else {
    Serial.println("Button not pressed!");
  }

  if(WiFi.status() == WL_CONNECTED){ // if esp32 is conected to wifi
    HTTPClient http;

    String serverPath = "wss://netwi.wokwi.com/"; // Endpoint for HTTP requisition

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // code to the result of the http requisition

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int ldrstatus = analogRead(ldrPin);

  if(ldrstatus <= threshold){
    Serial.print("its dark turn on led");
    digitalWrite(yellow_led, HIGH); 
    delay(1000);
    digitalWrite(yellow_led, LOW); 
    delay(1000);
    Serial.println(ldrstatus);
  }else{
    Serial.print("its bright turn off light");
    digitalWrite(green_led, HIGH); 
    delay(3000);
    digitalWrite(green_led, LOW); 
    digitalWrite(yellow_led, HIGH);
    delay(2000);
    digitalWrite(yellow_led, LOW);
    digitalWrite(red_led, HIGH);
    delay(5000);
    digitalWrite(red_led, LOW);
    Serial.println(ldrstatus);
    
  }
}