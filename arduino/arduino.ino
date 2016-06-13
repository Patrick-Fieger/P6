#include <Bridge.h>
#include <Servo.h>
#include <YunServer.h>
#include <YunClient.h>

YunServer server;
Servo myservo;
int pos = 0; 

void setup() {
  Bridge.begin();          // Let's activate the Yun Bridge...
  myservo.attach(9);
  // server.listenOnLocalhost();       // Listen for incoming connection only from localhost (no one from the external network could connect)
  server.begin();          // Let's acrivate the Yun server...

}

void loop() {
  Serial.println(analogRead(5));
  
  YunClient client = server.accept();  // Get clients coming from server
  if (client)                          // Is there a new client?
  {

    String command = client.readString();
    command.trim();        //kill whitespace

    if(command == "1"){
        myservo.write(90);              // tell servo to go to position in variable 'pos'
    }else if(command == "3"){
          myservo.write(180);              // tell servo to go to position in variable 'pos'
    }
    
    Serial.println(command);
    client.print(command);
    client.stop();                     // Close connection and free resources.
  }
  delay(50);
}


