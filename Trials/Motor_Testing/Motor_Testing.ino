// Motor testing with Sabertooth 2x25 Simplified Serial

void setup()
{
  Serial2.begin(9600);
}

void loop()
{
  for (int i=64; i<=127; i++)
  {
    Serial2.write(i);
    Serial2.write(i+128);      
    delay(500);
  }
  
  for (int i=127; i>=64; i--)
  {
    Serial2.write(i);
    Serial2.write(i+128);      
    delay(500);
  }  
  
  for (int i=64; i>=1; i--)
  {
    Serial2.write(i);
    Serial2.write(i+128);      
    delay(500);
  }  
  
  for (int i=1; i<=64; i++)
  {
    Serial2.write(i);
    Serial2.write(i+128);      
    delay(500);
  }    
}
