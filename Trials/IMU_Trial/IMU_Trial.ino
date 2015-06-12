struct Accelerometer
{
  int x;  
  int offset_x;
  int raw_x;    
  float angle_x;
};

struct Gyroscope
{
  int x;
  int offset_x;
  int raw_x;
  float angRate_x;
  float angle_x;
};

struct Accelerometer accelero;
struct Gyroscope gyro;
float angle_x = 0.00;

// timer variables
int cycle_time;
long last_cycle = 0;

void setup()
{
  analogReference(EXTERNAL);  
  Serial.begin(9600);
  delay(100);
}

void loop()
{
  readAndCalculateAccelerometer();
  readAndCalculateGyroscope();
  
  angle_x = 0.98 * gyro.angle_x + 0.02 * accelero.angle_x;
  
  time_stamp();
  
  Serial.print(accelero.angle_x); Serial.print("\t"); 
  Serial.print(gyro.angle_x); Serial.print("\t");
  Serial.println(angle_x);
//  Serial.print("time: ");
//  Serial.println(cycle_time); // print the loop cycle time
}

void readAndCalculateAccelerometer()
{
  accelero.offset_x = 517; 
  accelero.x = analogRead(A0);
  accelero.raw_x = accelero.x - accelero.offset_x;
  accelero.raw_x = constrain(accelero.raw_x, -90, 90);
  accelero.angle_x = (float)(accelero.raw_x * 0.01);
}

void readAndCalculateGyroscope()
{
  gyro.offset_x = 397;
  gyro.x = analogRead(A1);
  gyro.raw_x = gyro.x - gyro.offset_x;
  gyro.raw_x = constrain(gyro.raw_x, -gyro.offset_x, gyro.offset_x);
//  gyro.raw_x = map(gyro.raw_x, -gyro.offset_x, gyro.offset_x, -90, 90);  
  gyro.angRate_x = (float)(gyro.raw_x * 0.0015) * -0.05;  
  gyro.angle_x = angle_x + gyro.angRate_x; 
}


void time_stamp(){
  // check to make sure it has been exactly 50 milliseconds since the last recorded time-stamp 
  while((millis() - last_cycle) < 50){
    delay(1);
  }
  // once the loop cycle reaches 50 mS, reset timer value and proceed
  cycle_time = millis() - last_cycle;
  last_cycle = millis(); 
}
