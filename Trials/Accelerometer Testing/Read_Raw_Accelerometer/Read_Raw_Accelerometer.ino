struct Accelerometer
{
  long x, y, z;
  int offset_x, offset_y, offset_z;
  float acc_x, acc_y, acc_z;
  float angle_x, angle_y, angle_z;
  float R;
};

struct Accelerometer accelero;

void setup()
{
  analogReference(EXTERNAL);
  Serial.begin(9600);
  delay(100);
  accelero = readAccelerometer(accelero);  
}

void loop()
{
  Serial.print("x = "); Serial.print(accelero.acc_x); Serial.print("\t"); Serial.print(analogRead(A3)); Serial.print("\n");
  Serial.print("y = "); Serial.print(accelero.acc_y); Serial.print("\t"); //Serial.print(analogRead(A)); Serial.print("\n");
  Serial.print("z = "); Serial.print(accelero.acc_z); Serial.print("\t"); //Serial.print(analogRead(A)); Serial.print("\n");
    
  Serial.print("\n");  
  delay(1000);
}

struct Accelerometer readAccelerometer(struct Accelerometer accelero)
{
  int i=0;
  accelero.x = 0; accelero.y = 0; accelero.z = 0;
  for (i=0; i<100; i++)
  {
    accelero.x += analogRead(A3);
//    accelero.y += analogRead(A);
//    accelero.z += analogRead(A);
    
    delay(50);
  }
  
  accelero.acc_x = accelero.x / 100;
  accelero.acc_y = accelero.y / 100;
  accelero.acc_z = accelero.z / 100;
  
  return accelero;
}
