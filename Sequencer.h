
struct Sequencer {
  float   BPM;
  short   Voice;  // Voice 1 - 3
  short   CurrentStep;
  short   Steps;
  short   LastStep;
  short   Led[4];
  bool    Step[4]; // step N is set with woice X
  
};

Sequencer S1 = {140.0,  // BPM
                1,     // Voice
                1,     // Start Step     
                STEPMAX, // Steps
                STEPMAX, // Laststep 
                {2,3,4,5}, // LED Pins (Digital)
                {false, false, false, false} // Step 
                };    /*LED 2 - 5*/


Sequencer S2 = {140.0,  // BPM
                2,     // Voice
                1,     // Start Step     
                STEPMAX, // Steps
                STEPMAX, // Laststep 
                {6,7,8,9}, // LED Pins (Digital)
                {false, false, false, false} // Step 
                };    /*LED 2 - 5*/

Sequencer S3 = {140.0,  // BPM
                3,     // Voice
                1,     // Start Step     
                STEPMAX, // Steps
                STEPMAX, // Laststep 
                {22,23,24,25}, // LED Pins (Digital)
                {false, false, false, false} // Step 
                };    


void processStep1(short step ) {
  step--;
  digitalWrite(S1.Led[step] , HIGH);
  delay(10); 
  digitalWrite(S1.Led[step] , LOW);

  if (S1.Step[step]) {
     uint16_t note_length_4t = 200;

     if(noteDelay.ready()){
        // start event
        noteDelay.start(note_length_4t);

        // start the kick envelope
        kick_wave_envelope.noteOn();

        // start glitch-line   
        kickGliss.set(kick_gliss_start);   
        kick_audio_step_counter = kick_audio_steps;     
     }
  }
  
}


void processStep2(short step ) {
  step--;
  digitalWrite(S2.Led[step] , HIGH);
  delay(10); 
  digitalWrite(S2.Led[step] , LOW);

    if (S2.Step[step]) {
      uint16_t note_length_4t = 200;
    // Serial.println("Seq2 Step 2");

     if(noteDelay2.ready()){
        // start event
        noteDelay2.start(note_length_4t);

        // start the kick envelope
        kick_wave_envelope2.noteOn();

        // start glitch-line   
        kickGliss2.set(kick_gliss_start2);   
        kick_audio_step_counter2 = kick_audio_steps2;     
     }
  }
  
  
}



void processStep3(short step ) {
  step--;
  digitalWrite(S3.Led[step] , HIGH);
  delay(10); 
  digitalWrite(S3.Led[step] , LOW);

    uint16_t note_length_4t = 200;    

   if (S3.Step[step]) {
    
      if(noteDelay3.ready()) {
        
        noteDelay3.start(note_length_4t);

        //Serial.println("Note delay 3");
        //Serial.print("Attack: ");
        //Serial.println(hertz2);
        // start the kick envelope
        kick_wave_envelope3.noteOn();

        kickGliss3.set(kick_gliss_start3);   
        kick_audio_step_counter3 = kick_audio_steps3;     
     }
  
   }
}
