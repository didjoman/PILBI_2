
int inputPin =3;               // Pin d'entrée choisie pour surveillé le senseur PIR
int pirMotionState = LOW;       // Pour commencer, nous considérons qu'il n'y a pas de nouvement.
int val = 0;                    // Variable utilisée pour lire l'entrée Pin 2 (valeur du PIR)
 
void setup() {
   pinMode(inputPin, INPUT);     
   Serial.begin(9600);
}
 
void loop(){
  val = digitalRead(inputPin);  // Lire la valeur d'entrée
  if (val == HIGH) {           
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
}

