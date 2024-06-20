const int IR1 = 2; //detects bottle entering carrier
const int IR2 = 4; //detects bottole entering refilling station
const int IR3 = 6; //detects bottle entering capping station
const int IR4 = 7; //detects capping motor reaches the bottle
//const int IR5 = 13; //for detecting whether carrier reached initial position
const int buzzer=13;

const int relay1 = 3; //for controlling conveyor motor
const int relay2 = 5; //for controlling water pump
const int relay3 = 8; //for controlling supply to 12v relay
const int relay4 = 9; //for controlling air pump
const int relay5 = 10; //for controlling gear motor and solenoid


const int step = 11;
const int direction = 12;
const int stepper_delay = 5;

int ir1_reading, ir2_reading, ir3_reading, ir4_reading, steps;


void moveStepper () {
  digitalWrite(11, HIGH);
  delayMicroseconds(1000); 
  digitalWrite(step,LOW); 
  delayMicroseconds(1000); 
}


void beep () {
  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
}


void setup() {
  Serial.begin(9600); 

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  //pinMode(IR5, INPUT);

  pinMode(buzzer,OUTPUT);

  pinMode(relay1,OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);

  pinMode(step, OUTPUT);
  pinMode(direction, INPUT);

  digitalWrite(direction, LOW);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(step, LOW);
}

void loop() {
  steps = 0;

  digitalWrite(relay1, LOW); //conveyor starts
  Serial.println("Conveyor rotating");

  ir1_reading = digitalRead(IR1);
  while (ir1_reading == 1) { //loop ends when bottle is detected
    ir1_reading = digitalRead(IR1);
    delayMicroseconds(100);
  }

  delay(500); //to let the bottle slide into the rotating disk
  
  digitalWrite(relay1, HIGH); // conveyor stops

  Serial.println("Bottle reached Carrier");
  Serial.println("Conveyor motor stopped");
  Serial.println("Stepper motor started rotating");

  beep();
  ir2_reading = digitalRead(IR2);
  while (ir2_reading == 1) { //loop ends when bottle reaches refilling station
    ir2_reading = digitalRead(IR2);
    moveStepper();
    steps+=1;
    delay(stepper_delay);
  }
  beep();

  //stop stepper motor for fixed delay
  Serial.println("Stepper motor stopped rotating");
  Serial.println("Reached refilling station");
  Serial.println("Water pump starts");

  digitalWrite(relay2, LOW); //for starting water pump (connect to nc)

  delay(5000); //test and give accurate value (fixed delay for water filling)
  
  digitalWrite(relay2, HIGH); //water pump stops
  Serial.println("Water pump stops");

  //start stepper motor after refilling
  Serial.println("Stepper motor started rotating");

  beep();
  ir3_reading = digitalRead(IR3);
  while (ir3_reading == 1) { //loop ends when bottle reaches capping station
    ir3_reading = digitalRead(IR3);
    moveStepper();
    steps+=1;
    delay(stepper_delay);
  }
  beep();

  Serial.println("Reached Capping Station");
  Serial.println("Stepper Motor Stops");
  
  //lock the solenoid valve
  digitalWrite(relay3, LOW);
  digitalWrite(relay5, LOW); //gear motor connected to common
  
  Serial.println("Solenoid valve closes");

  //start air pump
  digitalWrite(relay4, LOW);

  Serial.println("Air pump starts");

  //when ir4 senses, stop air pump, then start gear motor
  ir4_reading = digitalRead(IR4);
  while (ir4_reading == 1) { //loop ends when capping motor reaches near the bottle
    ir4_reading = digitalRead(IR4);
    delayMicroseconds(100);
  }

  //stop air pump
  digitalWrite(relay4, HIGH);
  Serial.println("Cap came near bottle, Air pump Stops");

  //give 12v to common of relay 5 (gear motor starts)
  digitalWrite(relay3, HIGH);
  Serial.println("Capping Motor starts");

  delay(5000); // delay for capping process

  //after fixed delay 
  digitalWrite(relay5, HIGH);

  beep();
  Serial.println("Capping Motor stops");
  Serial.print("Steps taken: ");
  Serial.println(steps);

  int steps_to_initial = 610-steps;

  for(int i=0; i<=steps_to_initial; i++) {
    moveStepper();
    delay(stepper_delay);
  }
  Serial.println("Reached initial position");
  beep();

}
