#include <Oscil.h> // oscillator template
#include <EventDelay.h> // for scheduling events
#include <ADSR.h> // ADSR envelope
#include <tables/sin512_int8.h> // sine wavetable for oscillator, length 512 is normally sufficient

// kick
ADSR <AUDIO_RATE, AUDIO_RATE> kick_wave_envelope3; // envelope for kickdrum
Oscil <512, AUDIO_RATE> kickOscil3; // oscillator for kickdrum, fixed length of 512 for easy wavetable switching

// for triggering the envelope
EventDelay noteDelay3;
// line for frequency shifts
Line <uint32_t> kickGliss3;
// counter of audio steps, needed as global variable
uint32_t kick_audio_steps3 = 0;
uint32_t kick_audio_step_counter3 = 0;
// start value for frequency shift, needed as global variable
uint32_t kick_gliss_start3;
// kick

void SetVoice3(uint16_t time1, uint16_t time2, uint8_t level, uint16_t f1, uint16_t f2, uint16_t time_gliss){
  uint16_t kickwave_time_A, kickwave_time_D, kickwave_time_S, kickwave_time_R;
  uint16_t kickwave_level_A, kickwave_level_D, kickwave_level_S, kickwave_level_R;
  uint32_t gliss_end;

     // set the wavetable
     kickOscil3.setTable(SIN512_DATA);

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
      
    kick_wave_envelope3.setTimes(kickwave_time_A,
                                kickwave_time_D,
                                kickwave_time_S,
                                kickwave_time_R );
                                 
    kick_wave_envelope3.setLevels(kickwave_level_A,
                                 kickwave_level_D,
                                 kickwave_level_S,
                                 kickwave_level_R );

     kick_gliss_start = kickOscil3.phaseIncFromFreq(f1);
     gliss_end        = kickOscil3.phaseIncFromFreq(f2);

     // number of steps at audiorate
     kick_audio_steps3 = (AUDIO_RATE / 800) *(time_gliss)   ; 
     
     // set the audio rate line for frequency sweep
     kickGliss3.set(kick_gliss_start, gliss_end, kick_audio_steps3);   

}
