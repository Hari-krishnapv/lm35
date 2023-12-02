#include <TimerOne.h>
#define led 13
const int sensor = A0;
bool LED_STATE = false;

void setup() {
  Serial.begin(9600);
       pinMode(13, OUTPUT);//Set the pin to be OUTPUT
  cli();                      //stop interrupts for till we make the settings
  /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
 
  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
  TCCR1B |= B00000100;        //Set CS12 to 1 so we get prescalar 256  
  
  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
  
  /*4. initially Set the value of register A to 0*/
  OCR1A = 0;             
     sei();                  //Enable back the interrupts
}
volatile int i=0;  //initilization of flag variables used to check values
int vout,t=0;

int readtemp(void)
{
  return  analogRead(A0);   //read analog values from A0 pin of controller
}
void loop() {

  while(i==0)

  {
     vout= readtemp();                 //vout variable hold the return value of readtemp()
     Serial.println(vout);
      if(vout >= 62)                  // check temperature values 
      t=1;                            // if value greater than 62 asssign t=1
      else
      t=0;                             // assign t=0
      
      //i =1;
      
  }
    
}



//With the settings above, this IRS will trigger each 500ms, if t=1 (temperature above 30 degree)
ISR(TIMER1_COMPA_vect){
  if(t == 1){
  OCR1A = 31249;                     //set value to conunt up to 31249        
  
  TCNT1  = 0;                         //First, set the timer back to 0 so it resets for next interrupt
  LED_STATE = !LED_STATE;             //Invert LED state
  digitalWrite(13,LED_STATE);         // write current state to LED
   i=0;                               //flag variable set 
}
if(t == 0)                            //this IRS will trigger each 250ms, if t=0 (temperature less than 30 degree)
{
  OCR1A = 15624;                      //set value to conunt up to 15654            
                                  
  TCNT1  = 0;                         //First, set the timer back to 0 so it resets for next interrupt
  LED_STATE = !LED_STATE;             //Invert LED state
  digitalWrite(13,LED_STATE); 
   i=0;                              ///flag variable set
}
}
