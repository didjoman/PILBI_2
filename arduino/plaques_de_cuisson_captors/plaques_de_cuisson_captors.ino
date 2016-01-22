/* Constante */
const int consoleOutput = 9600;

/* Bouton poussoir */
const int buttonPin = 2;  // Port digital 2
int buttonState = 0;      // Etat courant du bouton

/* Led */
const int ledPin = 11;                  // Port digital 13
const int ALARM_HOTEPLATE_LED_PIN = 8;  // Port digital 8

bool ledState = false;    // Etat courant de la led

/* Presence Sensor */
const int pirPin = 3;
int pirMotionState = LOW;       // Pour commencer, nous considérons qu'il n'y a pas de nouvement.
int pirValue = 0;               // Variable utilisée pour lire l'entrée Pin 2 (valeur du PIR)

/* Capteur de pression */
const int panPin = A0;
int panValue;
bool isWeighted = false;

/* Dernier état envoyé */
bool lastState[] = {false, false};

/* Variables globales */
char command[500];
bool firstRead = true;

/* Initialize console output */
void initOutput() {
  Serial.begin(consoleOutput);
}

/* Initialize pins states */
void initPins() {
  pinMode(ledPin, OUTPUT);
  pinMode(ALARM_HOTEPLATE_LED_PIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(pirPin, INPUT);
}

/* Code Initialized Once */
void setup() 
{
  initPins();
  initOutput();
}

/* Code executed in a loop */
void loop() 
{
  /* On - Off */
  buttonState = digitalRead(buttonPin);

  if(buttonState == HIGH) {
    if(ledState) {
      digitalWrite(ledPin, LOW);
      ledState = false;
    } else {
      digitalWrite(ledPin, HIGH);
      ledState = true;
    }
  }

  /* Weight */
  panValue = analogRead(panPin);
  /*Serial.print("poids : ");
  Serial.println(panValue);*/
  if(panValue > 0) {
    isWeighted = true;
  } else {
    isWeighted = false;
  }

  /* Send information */
  if(ledState && isWeighted != lastState[1]) {
    if(isWeighted) {
      Serial.println("HotWithPan");
    } else {
      Serial.println("HotWithoutPan");
    }
  } else if(ledState && firstRead) {
    firstRead = false;
    if(isWeighted) {
      Serial.println("HotWithPan");
    } else {
      Serial.println("HotWithoutPan");
    }
  } else if(!ledState && !firstRead) {
    firstRead = true;
    Serial.println("HotplateOff");
  }
  
  /* Manage Presence */
  pirValue = digitalRead(pirPin);  // Lire la valeur d'entrée
  if (pirValue == HIGH) {           
    if (pirMotionState == LOW) {
      // Si l'état du PIR est LOW, comme nous venons de détecter un mouvement
      // nous changeons l'état pirMotionState à HIGH (mouvement détecté)
      Serial.println("PresenceDetected");
      // Nous affichons le message seulement quand la sortie change
      pirMotionState = HIGH;
    }
  } else {
    if (pirMotionState == HIGH){
      // Si l'état PIR est HIGH (mouvement détecté) et que la sortie du PIR
      // passe à LOW, c'est que le mouvement à cessé! 
      // Nous affichons le message que pour le changement d'état (détection mouvement)
      pirMotionState = LOW;
    }
  }
  
  /* Update current state */
  lastState[0] = ledState;
  lastState[1] = isWeighted;
  
  /* Receive commands */
  if(Serial.readBytes(command, 500) == 0){
	*command = NULL;
  }else{
	  char *p = command;
	  char *str;
	// Découpage des commandes
	  while ((str = strtok_r(p, ";", &p)) != NULL){
	   //actions
            //digitalWrite(LED_PIN, HIGH);
            
            if(strstr(str, "SET AlarmHotplateLed ON") > 0) {
              digitalWrite(ALARM_HOTEPLATE_LED_PIN, HIGH);
              Serial.println("AlarmHotplateLed On");
            } 
            
            if(strstr(str, "SET AlarmHotplateLed OFF") > 0) {
              digitalWrite(ALARM_HOTEPLATE_LED_PIN, LOW);
              Serial.println("AlarmHotplateLed Off");
            }          
          
            if(strstr(str, "SET HotplateLed ON") > 0) {
              digitalWrite(ledPin, HIGH);
              Serial.println("HotplateLed On");
            } 
            
            if(strstr(str, "SET HotplateLed OFF") > 0) {
              digitalWrite(ledPin, LOW);
              Serial.println("HotplateLed Off");
              ledState = false;
            }    
            
	}
  }

  //delay(250);
}

