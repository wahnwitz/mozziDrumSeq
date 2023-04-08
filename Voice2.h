#include <Oscil.h> // oscillator template
#include <EventDelay.h> // for scheduling events
#include <ADSR.h> // ADSR envelope
#include <tables/sin512_int8.h> // sine wavetable for oscillator, length 512 is normally sufficient

// kick
ADSR <AUDIO_RATE, AUDIO_RATE> kick_wave_envelope2; // envelope for kickdrum
Oscil <512, AUDIO_RATE> kickOscil2; // oscillator for kickdrum, fixed length of 512 for easy wavetable switching
// for triggering the envelope
EventDelay noteDelay2;
// line for frequency shifts
Line <uint32_t> kickGliss2;
// counter of audio steps, needed as global variable
uint32_t kick_audio_steps2 = 0;
uint32_t kick_audio_step_counter2 = 0;
// start value for frequency shift, needed as global variable
uint32_t kick_gliss_start2;
// kick

void SetVoice2(uint16_t time1, uint16_t time2, uint8_t level, uint16_t f1, uint16_t f2, uint16_t time_gliss){
  uint16_t kickwave_time_A, kickwave_time_D, kickwave_time_S, kickwave_time_R;
  uint16_t kickwave_level_A, kickwave_level_D, kickwave_level_S, kickwave_level_R;
  uint32_t gliss_end;

     // set the wavetable
     kickOscil2.setTable(SIN512_DATA);

     // set wave envelope timings
     kickwave_time_A = time1;

     // exponential decay
     kickwave_time_D = time2 >> 3; // 1/8
     kickwave_time_S = time2 >> 2; // 1/4
     kickwave_time_R = kickwave_time_D + kickwave_time_S + kickwave_time_S;  // 5/8


     // set wave envelope levels
     kickwave_level_A = level;
     
     // exponential decay
     kickwave_level_D = level >> 1;
     kickwave_level_S = level >> 3;
     kickwave_level_R = 0;

    // set the envelopes : timing & levels
      
    kick_wave_envelope2.setTimes(kickwave_time_A,
                                kickwave_time_D,
                                kickwave_time_S,
                                kickwave_time_R );
                                 
    kick_wave_envelope2.setLevels(kickwave_level_A,
                                 kickwave_level_D,
                                 kickwave_level_S,
                                 kickwave_level_R );

     kick_gliss_start = kickOscil2.phaseIncFromFreq(f1);
     gliss_end        = kickOscil2.phaseIncFromFreq(f2);

     // number of steps at audiorate
     kick_audio_steps2 = (AUDIO_RATE / 800) *(time_gliss)   ; 
     
     // set the audio rate line for frequency sweep
     kickGliss2.set(kick_gliss_start, gliss_end, kick_audio_steps2);   

}
