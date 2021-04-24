#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "math.h"
#include "Stepper.h"

Stepper::Stepper(uint8_t pin1,uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t speed){
  
  Pin1=pin1;
  Pin2=pin2;
  Pin3=pin3;
  Pin4=pin4;
  Speed=speed;

  gpio_init(Pin1);
  gpio_set_dir(Pin1, GPIO_OUT);

  gpio_init(Pin2);
  gpio_set_dir(Pin2, GPIO_OUT);

  gpio_init(Pin3);
  gpio_set_dir(Pin3, GPIO_OUT); 
  
  gpio_init(Pin4);
  gpio_set_dir(Pin4, GPIO_OUT);
}

void Stepper::SingleStep(int step) 
{ 
   switch(step)
   { 
   case 0: 
     gpio_put(Pin1, 1);  
     gpio_put(Pin2, 0); 
     gpio_put(Pin3, 0); 
     gpio_put(Pin4, 0); 
   break;  
   case 1: 
     gpio_put(Pin1, 1);  
     gpio_put(Pin2, 1); 
     gpio_put(Pin3, 0); 
     gpio_put(Pin4, 0); 
   break;  
   case 2: 
     gpio_put(Pin1, 0);  
     gpio_put(Pin2, 1); 
     gpio_put(Pin3, 0); 
     gpio_put(Pin4, 0); 
   break;  
   case 3: 
     gpio_put(Pin1, 0);  
     gpio_put(Pin2, 1); 
     gpio_put(Pin3, 1); 
     gpio_put(Pin4, 0); 
   break;  
   case 4: 
     gpio_put(Pin1, 0);  
     gpio_put(Pin2, 0); 
     gpio_put(Pin3, 1); 
     gpio_put(Pin4, 0); 
   break;  
   case 5: 
     gpio_put(Pin1, 0);  
     gpio_put(Pin2, 0); 
     gpio_put(Pin3, 1); 
     gpio_put(Pin4, 1); 
   break;  
     case 6: 
     gpio_put(Pin1, 0);  
     gpio_put(Pin2, 0); 
     gpio_put(Pin3, 0); 
     gpio_put(Pin4, 1); 
   break;  
   case 7: 
     gpio_put(Pin1, 1);  
     gpio_put(Pin2, 0); 
     gpio_put(Pin3, 0); 
     gpio_put(Pin4, 1); 
   break;  
   default: 
     gpio_put(Pin1, 0);  
     gpio_put(Pin2, 0); 
     gpio_put(Pin3, 0); 
     gpio_put(Pin4, 0); 
   break;  
   }
} 

void Stepper::MoveSteps(int steps)
{
    if(steps>0){
        for(int r=0;r<=steps;r++)
        {
            if(!isRunning)
              break;

            for(int x=0;x<=7;x++)
            {
                if(!isRunning)
                  break;
                SingleStep(x);                
                sleep_us(/*729*/ 900 * Speed);
            }
        }
    }else{
        for(int r = steps * -1; r >= 0; r--)
        {
            if(!isRunning)
              break;

            for(int x = 7; x >= 0; x--)
            {
              if(!isRunning)
                  break;
                SingleStep(x);                
                sleep_us(/*729*/ 900 * Speed);
            }
        }
    }
}

void Stepper::Stop()
{
  isRunning=false;
  gpio_put(Pin1, 0);  
  gpio_put(Pin2, 0); 
  gpio_put(Pin3, 0); 
  gpio_put(Pin4, 0);  
}

void Stepper::MoveAngle(int angle)
{
    isRunning=true;
    double s = 512.0 / (360.0  / angle ) ;
    int steps= (int)s;
    MoveSteps(steps);
    isRunning=false;
}

bool Stepper::IsRunning(){
  return isRunning;
}

uint8_t Stepper::GetSpeed()
{
  return Speed;
}

void Stepper::SetSpeed( uint8_t speed)
{
  Speed = speed;
}