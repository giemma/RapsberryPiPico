#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "math.h"
#include "pico/multicore.h"
#include "../StepperLibrary/Stepper.h"
#include "time.h"

Stepper stepper(0,1,2,3,1);

void RunExample(){
  stepper.MoveAngle(90);
  sleep_ms(500);

  stepper.MoveAngle(-90);
  sleep_ms(500);

  uint64_t startTime;
  uint64_t endTime;
  startTime =  time_us_64 ();
  
  stepper.MoveAngle(360);

  endTime =  time_us_64 ();
  float runSecs = (float)(endTime - startTime) / 1000000.0;
  printf("Secs: %9.3f\n");

  stepper.Stop();
}

uint64_t startTime;
uint64_t endTime;
float runSecs;

void Rotate(){  
  startTime =  time_us_64 ();  
  stepper.MoveAngle(360);
  endTime =  time_us_64 ();
  runSecs = (float)(endTime - startTime) / 1000000.0;
  printf("Seconds: %f\n", runSecs);
  sleep_ms(200);  

  startTime =  time_us_64 ();  
  stepper.MoveAngle(-360);
  endTime =  time_us_64 ();
  runSecs = (float)(endTime - startTime) / 1000000.0;
  printf("Seconds: %f\n", runSecs);
  sleep_ms(200); 

}

int main() {
  stdio_init_all();
  sleep_ms(5000);  

  while(1){
    /*multicore_launch_core1(RunExample);
    sleep_ms(20000);   
    multicore_reset_core1();*/

    for(int i=1; i<1000; i+=1)
    {
      Rotate();
    }

  }    
}



