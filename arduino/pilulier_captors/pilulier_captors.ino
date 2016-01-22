const int LED_PIL = 2;
const int LED_SALON = 4;
const int consoleOutput = 9600;

/* --------------- Capteur de contact --------------- */
/* Accrocher le détecteur au port analogique A0 */
const int contact_sensor = 3; //A3
boolean box_open = false;

/* Variables globales */
char command[500];
int s_clignote_pil = 0;
int s_clignote_salon = 0;


/* Initialize console output */
void initOutput() {
  Serial.begin(consoleOutput);
}

/* Initialize pins states */
void initPins() {
  pinMode(LED_PIL, OUTPUT);
  pinMode(LED_SALON, OUTPUT);
  pinMode(contact_sensor, INPUT);
}

/* Code Initialized Once */
void setup()
{
  initPins();
  initOutput();
}

void loop(){ // debut de la fonction loop()
  /*LED pour le warning */
  if (s_clignote_pil == 1)
    clignote(LED_PIL);
  if (s_clignote_salon == 1)
    clignote(LED_SALON);
  if (Serial.readBytes(command, 10) == 0) {
    *command = NULL;
  } else {
    char *p = command;
    char *str;
    //Découpage des commandes
    while ((str = strtok_r(p, ";", &p)) != NULL) {
      //actions
      if (strstr(str, "CPIL") > 0) {
        s_clignote_pil = 1;        
      }
      if (strstr(str, "CSALON") > 0) {
        s_clignote_salon = 1;        
      }
      if (strstr(str, "STOP") > 0) {
        s_clignote_pil = 0;
        s_clignote_salon = 0;        
      }
    }
  }
  
  /* Capteur de contact sur la pillbox pour détecter prise de médicaments */  
  if(box_open && analogRead(A3) == 0) {
    /* Boite fermée */
    box_open = false;
    Serial.println("Boite fermee"); 
    Serial.println(analogRead(A3));
    delay(500);
  }
  else if(!box_open && analogRead(A3) != 0) {
    /* Boite ouverte */
    box_open = true;    
    Serial.println("PillTaken");   
    //Serial.println(analogRead(A3));
    delay(500);
  }
}

void clignote(int led) {
  //Serial.print(led);
  //Serial.println(" clignote");
  digitalWrite(led,HIGH); // met la broche au niveau haut (5V) – allume la LED_PIL
  delay(200); // pause de 500 millisecondes (ms)
  digitalWrite(led,LOW); // met la broche au niveau bas (0V) – éteint la LED_PIL
  //delay(200); // pause de 500ms
}

