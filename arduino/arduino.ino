#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

const byte internalLED = 13;

YunServer server;

void setup() {
  pinMode(internalLED, OUTPUT);
  digitalWrite(internalLED, HIGH);     // Turn the led on to advise about bridge begin in progress
  Bridge.begin();          // Let's activate the Yun Bridge...
  ledBlink(150);           // ...bridge activation done (user sees a blink)
  // server.listenOnLocalhost();       // Listen for incoming connection only from localhost (no one from the external network could connect)
  server.begin();          // Let's acrivate the Yun server...
  ledBlink(150);           // ...server activation done (user sees a blink)
}

void loop() {
  YunClient client = server.accept();  // Get clients coming from server
  if (client)                          // Is there a new client?
  {

    String command = client.readString();
    command.trim();        //kill whitespace
    Serial.println(command);
    client.print(command);
    client.print(77);                  // Send a "77" to client (e.g. browser)
    client.stop();                     // Close connection and free resources.
    ledBlink(500);
  }
  delay(50);
}

void ledBlink(int delayTime) {
  digitalWrite(internalLED, HIGH); // Turn (or keep) the LED on
  delay(delayTime);
  digitalWrite(internalLED, LOW);  // Turn (or keep) the LED off
  delay(delayTime);
}
