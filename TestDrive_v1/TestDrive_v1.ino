#define DEBUG 0

// define "zero" values of accelerometer and gyroscope on a horizontal position to calibrate. Values are taken by running outer calibration program
#define ACCELEROMETER_OFFSET_X 514
#define GYROSCOPE_OFFSET_X 378
#define GYROSCOPE_SCALE_X 0.0015
#define SAMPLE_TIME 0.05  // the time of how long it has been since a previous gyroscope reading until the next reading
#define IR_THRESHOLD 300

// Analog input pins
const byte ACCELEROMETER_X = A0, GYROSCOPE_X = A1,
           STEER_POT = A2, GAIN_POT = A3,
           BAT_1 = A5, BAT_2 = A4;

// Digital I/O pins
const byte LWR_RED_LED = 10, GREEN_LED = 11, UPR_RED_LED = 12,
           UPRLEFT_IR = 4, UPRRIGHT_IR = 5, LWRLEFT_IR = 6, LWRRIGHT_IR = 7,
           BUZZER = 4,
           LEFT_HE = 2, RIGHT_HE = 3;

struct Accelerometer
{
  int x;  
  int offset_x;
  int raw_x;    
  float angle_x;
};
struct Accelerometer accelero;

struct Gyroscope
{
  int x;
  int offset_x;
  int raw_x;
  float angRate_x;
  float angle_x;
};
struct Gyroscope gyro;

float angle_x = 0.00;

int IR_value[4] = {0};

// engage button variables
int engage = false;
int engage_state = 1;

// motor speed variables
int motor_out = 0;
int motor_1_out = 0;
int motor_2_out = 0;
int m1_speed = 0;
int m2_speed = 0;
int output;

// potentiometer variables
int steer_val;
int steer_range = 7;
int steer_reading;
int gain_reading;
int gain_val;

// timer variables
int cycle_time;
long last_cycle = 0;

void setup()
{
  analogReference(EXTERNAL);
  
  pinMode(UPR_RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(LWR_RED_LED, OUTPUT);
//  pinMode(BUZZER, OUTPUT);
  
  Serial1.begin(9600);

  digitalWrite(UPR_RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(LWR_RED_LED, LOW);
  delay(1000);

  digitalWrite(UPR_RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(LWR_RED_LED, LOW);
  delay(1000);

  digitalWrite(UPR_RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(LWR_RED_LED, HIGH);
  delay(1000);

  digitalWrite(UPR_RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(LWR_RED_LED, LOW);
  delay(1000);

#if DEBUG
  Serial.begin(9600);
#endif     
}

void loop()
{
  sampleAccelerometer();
  sampleGyroscope();
  
  angle_x = 0.98 * gyro.angle_x + 0.02 * accelero.angle_x;
  
  read_pots();
  
  auto_level();
  
  update_motor_speed();
  
  time_stamp();
  
#if DEBUG  
  debug();
#endif
}

void sampleAccelerometer()
{
  accelero.offset_x = ACCELEROMETER_OFFSET_X; 
  accelero.x = analogRead(ACCELEROMETER_X);
  accelero.raw_x = accelero.x - accelero.offset_x;
  accelero.raw_x = constrain(accelero.raw_x, -90, 90);
  accelero.angle_x = (float)(accelero.raw_x * 0.01);
}

void sampleGyroscope()
{
  gyro.offset_x = GYROSCOPE_OFFSET_X;
  gyro.x = analogRead(GYROSCOPE_X);
  gyro.raw_x = gyro.x - gyro.offset_x;
  gyro.raw_x = constrain(gyro.raw_x, -gyro.offset_x, gyro.offset_x);
//  gyro.raw_x = map(gyro.raw_x, -gyro.offset_x, gyro.offset_x, -90, 90);  
  gyro.angRate_x = (float)(gyro.raw_x * GYROSCOPE_SCALE_X) * -SAMPLE_TIME;  
  gyro.angle_x = angle_x + gyro.angRate_x; 
}

void read_pots()
{
  // Read and convert potentiometer values
  // Steering potentiometer
  steer_reading = analogRead(STEER_POT); // We want to coerce this into a range between -1 and 1, and set that to steer_val
  steer_val = map(steer_reading, 510, 630, steer_range, -steer_range); 
  if (angle_x == 0.00)
  {
    gain_reading = 0;
  }
  // Gain potentiometer
  gain_reading = analogRead(GAIN_POT);
  gain_val = map(gain_reading, 0, 1023, 32, 64);
}

int readQD(const byte QRE1113_Pin)
{
  //Returns value from the QRE1113 
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode(QRE1113_Pin, OUTPUT);
  digitalWrite(QRE1113_Pin, HIGH);
  delayMicroseconds(10);
  pinMode(QRE1113_Pin, INPUT);

  long time = micros();

  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(QRE1113_Pin) == HIGH && micros() - time < 3000);
  int diff = micros() - time;

  return diff;
}

void auto_level()
{
  // enable auto-level turn On
  // read all four IR sensors
  for(int i=0;i<4;i++)
    IR_value[i] = readQD(i+UPRLEFT_IR);
    
  if (IR_value[0] < IR_THRESHOLD || IR_value[1] < IR_THRESHOLD || IR_value[2] < IR_THRESHOLD || IR_value[3] < IR_THRESHOLD)
//  if (IR_value[0] > IR_THRESHOLD || IR_value[1] > IR_THRESHOLD || IR_value[2] > IR_THRESHOLD || IR_value[3] > IR_THRESHOLD)  
    engage_state = LOW;
  else
    engage_state = HIGH;

  if (engage_state == HIGH)  // ga diinjek
  {
    engage = false;
    if (angle_x > 0.02)
    {
      digitalWrite(UPR_RED_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(LWR_RED_LED, HIGH);
    }
    else if (angle_x < -0.02)
    {
      digitalWrite(UPR_RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(LWR_RED_LED, LOW);
    }
    else
    {
      digitalWrite(UPR_RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(LWR_RED_LED, LOW);
    }
  }
  else
  { 
    if (engage == false)
    {
      if (angle_x < 0.02 && angle_x > -0.02)
        engage = true;
      else 
        engage = false;
    }
    else 
    {
      engage = true;  
    }
  }  
}

void update_motor_speed()
{
  // Update the motors

  if (engage == true)
  {
    if (angle_x < -0.4 || angle_x > 0.4)
    {
      motor_out = 0;
    } 
    else
    {
      output = (angle_x * -1000); // convert float angle back to integer format
      motor_out = map(output, -250, 150, -gain_val, gain_val); // map the angle
    }

    // assign steering bias
    if(motor_out > 0)
    {
      motor_1_out = motor_out - (steer_val); 
      motor_2_out = motor_out + (steer_val);
    }
    else if(motor_out < 0)
    {
      motor_1_out = motor_out + (steer_val); 
      motor_2_out = motor_out - (steer_val);
    }
    
    // test for and correct invalid values
    if(motor_1_out > 63)
    {
      motor_1_out = 63;
    }
    if(motor_1_out < -63)
    {
      motor_1_out = -63;
    }
    if(motor_2_out > 63)
    {
      motor_2_out = 63;
    }
    if(motor_2_out < -63)
    {
      motor_2_out = -63;
    }

    // assign final motor output values
    m1_speed = 64 + motor_1_out;
    m2_speed = 192 - motor_2_out;
  }

  else
  {
    m1_speed = 0;
    m2_speed = 0;
  }

  // write the final output values to the Sabertooth via SoftwareSerial
  Serial1.write(m1_speed);
  Serial1.write(m2_speed);
}

void time_stamp()
{
  // check to make sure it has been exactly 50 milliseconds since the last recorded time-stamp 
  while((millis() - last_cycle) < 50)
    delay(1);
    
  // once the loop cycle reaches 50 mS, reset timer value and proceed
  cycle_time = millis() - last_cycle;
  last_cycle = millis(); 
}

void debug()
{
  // Debug with the Serial monitor

//  Serial.print("Accel: ");  
//  Serial.print(accelero.angle_x);  // print the accelerometer angle
//  Serial.print("  ");   
//
//  Serial.print("Gyro: ");  
//  Serial.print(gyro.angle_x);  // print the gyro angle
//  Serial.print("  ");    

  Serial.print("Filtered: ");  
  Serial.print(angle_x);   // print the filtered angle
  Serial.print("  "); 

  Serial.print(IR_value[0]);
  Serial.print("  "); 
  Serial.print(IR_value[1]);
  Serial.print("  "); 
  Serial.print(IR_value[2]);
  Serial.print("  "); 
  Serial.print(IR_value[3]);
  Serial.print("  "); 

//  Serial.print(analogRead(STEER_POT));
//  Serial.print("  "); 
//  Serial.print("steer_val: ");
//  Serial.print(steer_val);
//  Serial.print("  "); 
//  Serial.print(analogRead(GAIN_POT));
//  Serial.print("  "); 
//  Serial.print(gain_val);
//  Serial.print("  ");  

//  Serial.print("m1/m2: ");
//  Serial.print(motor_1_out);
//  Serial.print("/");
//  Serial.print(motor_2_out);
//  Serial.print("  ");  
  
  Serial.print(" time: ");
  Serial.print(cycle_time); // print the loop cycle time
  Serial.println("    "); 

  
//   Serial.print("o/m: ");
//   Serial.print(output);
//   Serial.print("/");
//   Serial.print(motor_out);
//   Serial.println("  "); 
//   
//   
//   Serial.print("steer_reading: ");
//   Serial.print(steer_reading);
//   Serial.print("  "); 
//   

}
