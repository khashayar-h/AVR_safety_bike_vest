 /* 
    source: www.electroschematics.com
    You'll need to change the led pins and the codes 
    accordingly to your configuration and IR remote
    */
     
    #include "IRremote.h"
     
    int RECV_PIN = 7; // the pin where you connect the output pin of TSOP4838
    int led2 = 11;
    int led3 = 12;
    int led4 = 10;
    int itsONled[] = {0,0,0,0,0};
    /* the initial state of LEDs is OFF (zero) 
    the first zero must remain zero but you can 
    change the others to 1's if you want a certain
    led to light when the board is powered */

    #define code2  16724175 // code received from button B
    #define code3  16718055 // code received from button C
    #define code4  16743045 // code received from button D   
      
    IRrecv irrecv(RECV_PIN);
     
    decode_results results;
     
    void setup()
    {
      Serial.begin(9600);   // you can comment this line
      irrecv.enableIRIn();  // Start the receiver
      pinMode(led2, OUTPUT);
      pinMode(led3, OUTPUT);
      pinMode(led4, OUTPUT);


      cli();
      TCCR1A = 0; 
      TCCR1B = 0;
      OCR1A = 3624;  // = (target time / timer resolution) - 1 or 1 / 6.4e-5 - 1 = 15624
      //OCR1A = 15624>>1;  // divide by two >>EDIT added this line<<
      TCCR1B |= (1 << WGM12);// CTC mode on
      TCCR1B |= (1 << CS10);// Set CS10 and CS12 bits for 1024 prescaler:
      TCCR1B |= (1 << CS12);
      TIMSK1 |= (1 << OCIE1A);// timer compare intrupt
      sei(); 
      interrupts(); // enable all interrupts

      
    }
     
void loop() 
{
      if (irrecv.decode(&results)) {
        unsigned int value = results.value;
        switch(value) {
           
           case code2:
             if(itsONled[2] == 1) {
                digitalWrite(led2, LOW);
                itsONled[2] = 0;
             } else {
                 digitalWrite(led2, HIGH);
                 itsONled[4]=0;
                 digitalWrite(led4, LOW);
                 itsONled[2] = 1;
             }
              break;
              
           case code3:
             if(itsONled[3] == 1) {
                digitalWrite(led3, LOW);
                itsONled[3] = 0;
             } else {
                 digitalWrite(led3, HIGH);
                 itsONled[3] = 1;
             }
              break;    
              
                case code4:
             if(itsONled[4] == 1) {
                digitalWrite(led4, LOW);
                itsONled[4] = 0;
             } else {
                 digitalWrite(led4, HIGH);
                 itsONled[2]=0;
                 digitalWrite(led2, LOW); 
                 itsONled[4] = 1;
             }
              break;      
        }
        //Serial.println(value); // you can comment this line
        irrecv.resume(); // Receive the next value
      }
}


    
ISR(TIMER1_COMPA_vect)
{

  if(itsONled[2] == 1)
  {
     OCR1A = 3624; // preload timer
     digitalWrite(led2, digitalRead(led2) ^ 1); 
  }
  
  if(itsONled[4] == 1)
  {
     OCR1A = 3624; // preload timer
     digitalWrite(led4,digitalRead(led4) ^ 1);
  }

}
