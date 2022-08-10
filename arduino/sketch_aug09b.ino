#define RELAY_1  D4 // the Arduino pin, which connects to the relay 1
#define RELAY_2  D7 // the Arduino pin, which connects to the relay 2
#define RELAY_3  D5 // the Arduino pin, which connects to the relay 3 D8
#define RELAY_4  D6 // the Arduino pin, which connects to the relay 4

void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(RELAY_1, HIGH);
  delay(1000);
  digitalWrite(RELAY_1, LOW);
  delay(200);
  digitalWrite(RELAY_2, HIGH);
  delay(1000);
  digitalWrite(RELAY_2, LOW);
  delay(200);
  digitalWrite(RELAY_3, HIGH);
  delay(1000);
  digitalWrite(RELAY_3, LOW);
  delay(200);
  digitalWrite(RELAY_4, HIGH);
  delay(1000);
  digitalWrite(RELAY_4, LOW);
  delay(200);
  
}
