struct Accelerometer
{
  long x, y, z;
  int offset_x, offset_y, offset_z;
  float acc_x, acc_y, acc_z;
  float angle_x, angle_y, angle_z;
  float R;
};

struct Accelerometer accelero;

struct Gyroscope
{
  long x, y, z;
  int offset_x, offset_y, offset_z;
  float angRate_x, angRate_y, angRate_z;
  float angle_x, angle_y, angle_z;
};

struct Gyroscope gyro;

void setup()
{
  analogReference(EXTERNAL);
  Serial.begin(9600);
  delay(100);
  accelero = readAccelerometer(accelero);  
  gyro = readGyroscope(gyro);
}

void loop()
{
  Serial.print("acc x = "); Serial.print(accelero.acc_x); Serial.print("\t"); Serial.print(analogRead(A0)); Serial.print("\n");
  Serial.print("gyro x = "); Serial.print(gyro.angRate_x); Serial.print("\t"); Serial.print(analogRead(A1)); Serial.print("\n");    
  delay(1000);
}

struct Accelerometer readAccelerometer(struct Accelerometer accelero)
{
  int i=0;
  accelero.x = 0; accelero.y = 0; accelero.z = 0;
  for (i=0; i<100; i++)
  {
    accelero.x += analogRead(A0);
//    accelero.y += analogRead(A);
//    accelero.z += analogRead(A);
    
    delay(50);
  }
  
  accelero.acc_x = accelero.x / 100;
  accelero.acc_y = accelero.y / 100;
  accelero.acc_z = accelero.z / 100;
  
  return accelero;
}

struct Gyroscope readGyroscope(struct Gyroscope gyro)
{ 
  int i=0;
  gyro.x = 0; gyro.z = 0;
  for (i=0; i<100; i++)
  {
    gyro.x += analogRead(A1);
//    gyro.z += analogRead(A3);    
    
    delay(50);
  }
  
  gyro.angRate_x = gyro.x / 100;
//  gyro.angRate_z = gyro.z / 100;
  
  return gyro;
}  
