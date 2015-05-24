#include <math.h>

void setup()
{
    Serial.begin(9600);
    pinMode(DAC0,OUTPUT);
}

long int value=0;
int cnt=0;
char input;
int array[10];


void loop(){
  
    while(Serial.available()==1)
    {
      input=Serial.read();  
      
      if (input!='e'){
        array[cnt]= (int)input;
        cnt++;
      }
      else if(input=='e')
        { 
             for(int i=0;i<cnt;i++)
          {
            value+=(array[i]-48)*pow(10,cnt-i-1);
        }
          
          Serial.println(value)
          analogWrite(DAC0,value);
        value=0;
        cnt=0;
        }
        
    }
    
}
