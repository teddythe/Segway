struct Gyroscope
{
  int x, y, z;
  int offset_x, offset_y, offset_z;
  int raw_x, raw_y, raw_z;    
  float angRate_x, angRate_y, angRate_z;
  float angle_x, angle_y, angle_z;
};

struct Gyroscope gyro;

float angle_x = 0.00, angle_z = 0.00;

void setup()
{
  analogReference(EXTERNAL);
  Serial.begin(9600);
  delay(100);
}

void loop()
{ 
  readAndCalculateGyroscope();

  Serial.print(angle_x); Serial.print("\t"); Serial.print(gyro.raw_x);
//  Serial.print(gyro.angle_z); Serial.print("\t");  
    
  Serial.print("\n");

  delay(50);
}

void readAndCalculateGyroscope()
{
  gyro.offset_x = 413;
//  gyro.offset_z = 369;
  
  gyro.x = analogRead(A4);
//  gyro.z = analogRead(A5);
  
  gyro.raw_x = gyro.x - gyro.offset_x;
//  gyro.raw_z = gyro.z - gyro.offset_z;
  
  gyro.raw_x = constrain(gyro.raw_x, -gyro.offset_x, gyro.offset_x);
//  gyro.raw_z = constrain(gyro.raw_z, -369, 369);  

  gyro.angRate_x = (float)(gyro.raw_x * 0.2267) * -0.05;  
//  gyro.angRate_z = (float)(gyro.raw_z) * 0.05;
  
  angle_x = angle_x + gyro.angRate_x;
//  gyro.angle_z = angle_z + gyro.angRate_z;
  
}  
