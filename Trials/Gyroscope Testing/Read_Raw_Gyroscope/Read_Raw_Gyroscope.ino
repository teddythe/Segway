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
  gyro = readGyroscope(gyro);  
}

void loop()
{ 
  Serial.print("x = "); Serial.print(gyro.angRate_x); Serial.print("\t"); Serial.print(analogRead(A4)); Serial.print("\n");
//  Serial.print("z = "); Serial.print(gyro.angRate_z); Serial.print("\t"); Serial.print(analogRead(A3)); Serial.print("\n");
    
  Serial.print("\n");  
  delay(1000);
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
