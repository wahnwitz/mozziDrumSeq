#include <MozziGuts.h>
#include <Metronome.h>
#include <mozzi_rand.h>
#include <Sample.h> 

#include "Voice1.h"
#include "Voice2.h"
#include "Voice3.h"
#include "SynthVars.h"
#include "Sequencer.h"

#define CONTROL_RATE 32


// Rotary encoder
int clockP = A9;
int data = A8;
int count = 0;
int c = HIGH;
int cLast = HIGH;
int d = HIGH;
// Rotry menu Poti
int MENUPOTI = A10;



/* function prototypes */
float* checkBPM(); /* Metro 1-2*/
void setSequencerLED(int);
void checkConfigMode();
void loop();
void playInstrument(short);


void setup ()  {
  Serial.begin(115200);
  //S1.CurrentStep = 1;
  //S2.CurrentStep = 1;
  //S3.CurrentStep = 1;
  // S1
  pinMode(2 , OUTPUT);
  pinMode(3 , OUTPUT);
  pinMode(4 , OUTPUT);  
  pinMode(5 , OUTPUT);
  // S2
  pinMode(6 , OUTPUT);
  pinMode(7 , OUTPUT);
  pinMode(8 , OUTPUT);
  pinMode(9 , OUTPUT);
  // S3
  pinMode(14 , OUTPUT);
  pinMode(15 , OUTPUT);
  pinMode(16 , OUTPUT);
  pinMode(17 , OUTPUT);
  
  startMozzi();
  Serial.println("Mozzi started");
  Metro_1.stop();
  Metro_2.stop();
  Metro_3.stop();
  Metro_1.start(100);  
  Metro_2.start(100);  
  Metro_3.start(100);  

  // Drum Voice 1-3 initial setup
  SetVoice1(20, 100, 255, 400, 60, 25); // 10ms attack, 100ms decay, full level, 600Hz, 50Hz, 25ms
  SetVoice2(20, 100, 255, 500, 50, 25); // 10ms attack, 100ms decay, full level, 600Hz, 50Hz, 25ms
  SetVoice3(30, 100, 255, 100, 50, 25); // 10ms attack, 100ms decay, full level, 600Hz, 50Hz, 25ms 

  // rotary encoder
  pinMode (clockP,INPUT_PULLUP);
  pinMode (data,INPUT_PULLUP);
}


void checkConfigMode(){ 
    Metro_1.setBPM(S1.BPM);
    Metro_2.setBPM(S2.BPM);    
    Metro_3.setBPM(S3.BPM);    

   attack = mozziAnalogRead(0); 
   decay  = mozziAnalogRead(1);  
   hertz1 = mozziAnalogRead(2);   
   hertz2 = mozziAnalogRead(3);

   // read the menu position for rotary encoder
   rotarypoti = mozziAnalogRead(MENUPOTI);

   m_attack = map(attack, 0, 1024, 1, 96);
   m_decay = map(decay, 0, 1024, 10, 1000);
   m_hertz1 = map(hertz1, 0, 1024, 10, 1000);
   m_hertz2 = map(hertz2, 0, 1024, 10, 480);
   m_rotarypoti = map(rotarypoti, 0, 1024, 1, 11);

   // Serial.println(m_rotarypoti);

   SetVoice3(m_attack, decay, 255, m_hertz1, hertz2, 25); // 10ms attack, 100ms decay, full level, 600Hz, 50Hz, 25ms

   S1B1 = digitalRead(18);
   if (S1B1 == HIGH) {
      if (S1.Step[0] == false) {
        S1.Step[0] = true;
      } else {
        S1.Step[0] = false;
      }

   } 
  
   S1B2 = digitalRead(19);
   if (S1B2 == HIGH) {
      if (S1.Step[1] == false) {
        S1.Step[1] = true;
      } else {
        S1.Step[1] = false;
      }
   }
   
   S1B3 = digitalRead(20);
   if (S1B3 == HIGH) {
      if (S1.Step[2] == false) {
        S1.Step[2] = true;
      } else {
        S1.Step[2] = false;
      }
   }
   
   S1B4 = digitalRead(21);
   if (S1B4 == HIGH) {
     if (S1.Step[3] == false) {
        S1.Step[3] = true;
      } else {
        S1.Step[3] = false;
      }
   }
}



void updateControl() 
{    
  //  static uint8_t bpm_speed = 80;
  //  only needed when changing BPM
  //  note_length_4t = (uint16_t)(60000) / bpm_speed;  // ( 4* 60 * 1000 / 4 ) / bpm
     
  checkConfigMode();    

  // Sequencer 1
  if(Metro_1.ready()) {                  
    processStep1(S1.CurrentStep);
    S1.CurrentStep++;
    if (S1.CurrentStep > S1.LastStep) 
      S1.CurrentStep = 1;      
  }

  // Sequencer 2
  if(Metro_2.ready()) {              
    processStep2(S2.CurrentStep);
    S2.CurrentStep++;
    if (S2.CurrentStep > S2.LastStep) 
      S2.CurrentStep = 1;
  }

  // Sequencer 3 
  if(Metro_3.ready()) {                  
    processStep3(S3.CurrentStep);
    S3.CurrentStep++;
    if (S3.CurrentStep > S3.LastStep) 
      S3.CurrentStep = 1;      
  }


/*
  c = digitalRead(clockP); // read pin A as clock
  d = digitalRead(data);
 
  if (c != cLast) {       // clock pin has changed value
   d = c^d;              // work out direction using an XOR
   if ( d ) {
     count++;
     Serial.print ("Right: ");
     Serial.println(count);
     delay(20);
   }else{
     count--;
     Serial.print ("Left: ");
     Serial.println(count);
     delay(20);
   }
   cLast = c;
 }
 */
 
}

  
AudioOutput_t updateAudio() {
  int16_t new_audio;

  // update kick drum envelope
  kick_wave_envelope.update();
  kick_wave_envelope2.update();
  kick_wave_envelope3.update();

  // change the frequency of the kick oscillator
  if( kick_audio_step_counter  ){
    kickOscil.setPhaseInc(kickGliss.next());
    kick_audio_step_counter--;
  } 

  if( kick_audio_step_counter2  ){
    kickOscil2.setPhaseInc(kickGliss2.next());
    kick_audio_step_counter2--;
  }

  if( kick_audio_step_counter3  ){
    kickOscil3.setPhaseInc(kickGliss3.next());
    kick_audio_step_counter3--;
  }

   new_audio = (int16_t) (
    ((
       (int32_t)( (int16_t)(uint8_t)kick_wave_envelope.next() * (int16_t)(int8_t)kickOscil.next() + 
       (int16_t)(uint8_t)kick_wave_envelope2.next() * (int16_t)(int8_t)kickOscil2.next() +
       (int16_t)(uint8_t)kick_wave_envelope3.next() * (int16_t)(int8_t)kickOscil3.next()
       )) 
 
    ) >> 2 );
    
  // mixing all oscillators & envelopes
 /*
 new_audio = (int16_t) (
    ((
       (int32_t)( (int16_t)(uint8_t)kick_wave_envelope.next() * (int16_t)(int8_t)kickOscil.next()  ) 
                                                             // 255 * 128 = 32640 == > / 4
    ) >> 2 )                                                 // scale down to signed 14 bit +-8192 (hifimode)
  );

 
 new_audio += (int16_t) (
    ((
       (int32_t)( (int16_t)(uint8_t)kick_wave_envelope2.next() * (int16_t)(int8_t)kickOscil2.next()  ) 
    ) >> 2 )                                                
  );
 
 new_audio += (int16_t) (
    ((
       (int32_t)( (int16_t)(uint8_t)kick_wave_envelope3.next() * (int16_t)(int8_t)kickOscil3.next() - 24 ) 
    ) >> 3 )                                                
  );

*/
  return new_audio;
}


void loop() {
  audioHook();
}
