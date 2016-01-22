/* --------------- Capteur Ultrason --------------- */
int trig = 12;  // Digital 12
int echo = 11;  // Digital 11
long lecture_echo;
long mm = 0;

/* --------------- Capteur de contact --------------- */
/* Accrocher le détecteur au port analogique A0 */
int contact_sensor = 0; // A0
boolean detect_close_door = 0;

/* --------------- Photo resistance --------------- */
const char photo_resistance = 2; // A2
int photo_resistance_value;

/* Constante */
const int consoleOutput = 9600;
const int LED_PIN1 = 3;
const int LED_PIN2 = 4;

/* Variable globale */
bool getHauteur = false;
char command[500];

/* Initialize pins states */
void initPins() {
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(contact_sensor, INPUT);
  pinMode(photo_resistance,OUTPUT);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
}

/* Initialize console output */
void initOutput() {
  Serial.begin(consoleOutput);
}

/* Code Initialized Once */
void setup() 
{
 
  initPins();
  initOutput();
  Serial.println("FirstMessage");
  
  while(mm < 1580) {
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    lecture_echo = pulseIn(echo, HIGH);
    mm = lecture_echo;
  }
  
  Serial.print("MailboxHeight:");
  Serial.print(mm);
  Serial.println(";");
}

/* Code executed in a loop */
void loop() 
{
  /* Capteur de contact : porte boite aux lettres */
  if(analogRead(A0) == 0) {
    /* Porte ouverte */
    detect_close_door = true;
  }
  if(detect_close_door && analogRead(A0) != 0) {
    /* Porte fermée */
    detect_close_door = false;
    Serial.println("MailboxOpenDoor");
    getHauteur = true;
  }
  
  /* Photoresistance */
  photo_resistance_value = analogRead(photo_resistance);
  if(!detect_close_door && photo_resistance_value > 100) {
    Serial.println("MailboxLightDetected");
    getHauteur = true;
    delay(3000);
  }

  /* Capteur Ultrason */
  if(getHauteur) {
    getHauteur = false;
    for(int i = 1; i <= 2; i++) {
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig, LOW);
      lecture_echo = pulseIn(echo, HIGH);
      mm = lecture_echo;
      Serial.print("MailboxHeight:");
      Serial.print(mm);
      Serial.println(";");
      delay(1000);
    }
  }
  
  /* Commandes reçues */
  if(Serial.readBytes(command, 500) == 0){
	*command = NULL;
  }else{
	  char *p = command;
	  char *str;
	// Découpage des commandes
	  while ((str = strtok_r(p, ";", &p)) != NULL){
	   //actions
            //digitalWrite(LED_PIN, HIGH);
            
            if(strstr(str, "SET Led1 ON") > 0) {
              digitalWrite(LED_PIN1, HIGH);
              Serial.println("Led On");
            } 
            
            if(strstr(str, "SET Led1 OFF") > 0) {
              digitalWrite(LED_PIN1, LOW);
              //Serial.println("Led On");
            } 
            
            if(strstr(str, "SET Led2 ON") > 0) {
              digitalWrite(LED_PIN2, HIGH);
              Serial.println("Led On");
            } 
            
            if(strstr(str, "SET Led2 OFF") > 0) {
              digitalWrite(LED_PIN2, LOW);
              //Serial.println("Led On");
            } 
            
            
	}
  }

  delay(1000);
}
