struct Accelerometer
{
  int x, y, z;  
  int offset_x, offset_y, offset_z;
  int raw_x, raw_y, raw_z;    
  float angle_x, angle_y, angle_z;
  float R;
};

struct Accelerometer accelero;

void setup()
{
  analogReference(EXTERNAL);  
  Serial.begin(9600);
  delay(100);
}

void loop()
{
  accelero = readAndCalculateAccelerometer(accelero);
  
  Serial.print(accelero.angle_x); Serial.print("\t");
//  Serial.print(accelero.angle_y); Serial.print("\t");
//  Serial.print((accelero.angle_z); Serial.print("\t");  
    
  Serial.print("\n");

  delay(50);
}

struct Accelerometer readAndCalculateAccelerometer(struct Accelerometer accelero)
{
  accelero.offset_x = 517;
  accelero.offset_y = 514;
  accelero.offset_z = 613;
  
  accelero.x = analogRead(A3);
//  accelero.y = analogRead(A);
//  accelero.z = analogRead(A);
  
  accelero.raw_x = accelero.x - accelero.offset_x;
//  accelero.raw_y = accelero.y - accelero.offset_y;
//  accelero.raw_z = accelero.z - accelero.offset_z;
  
  accelero.raw_x = constrain(accelero.raw_x, -90, 90);
//  accelero.raw_y = constrain(accelero.raw_y, -90, 90);
//  accelero.raw_z = constrain(accelero.raw_z, -90, 90);  
  
  accelero.angle_x = (float)(accelero.raw_x);
//  accelero.angle_y = (float)(accelero.raw_y);  
//  accelero.angle_z = (float)(accelero.raw_z);  

  /*accelero.acc_x = (accelero.x - accelero.offset_x) / 102.3;
  accelero.acc_y = (accelero.y - accelero.offset_y) / 102.3;
  accelero.acc_z = (accelero.z - accelero.offset_z) / 102.3;  
  accelero.acc_z++;

  accelero.R = sqrt(pow(accelero.acc_x,2)+pow(accelero.acc_y,2)+pow(accelero.acc_z,2)); //the force vector
  accelero.angle_x = acos(accelero.acc_x/accelero.R) * (180 / PI) - 90;
  accelero.angle_y = acos(accelero.acc_y/accelero.R) * (180 / PI) - 90;
    
  if(accelero.acc_z < 0) //360 degrees
  {
    if(accelero.angle_x < 0)
      accelero.angle_x = -180-accelero.angle_x;
    else
      accelero.angle_x = 180-accelero.angle_x;
    
    if(accelero.angle_y < 0)
      accelero.angle_y = -180-accelero.angle_y;
    else
      accelero.angle_y = 180-accelero.angle_y;
  }*/
  
  return accelero;  
}
