
struct Config {
  byte  CurrentSeqencer; // Links to S1 - S4
  byte  MaxSequencer;    // 4 yet..
  bool  ConfigMode;      // System is in config mode or not 
  
};

struct Voice {
  short  Attack;        // 20
  short  Decay;         // 100
  short  Level;         // 255
  short  Freq1;         // 400 
  short  Freq2;         // 60
  short  Milliseconds;  // 25  
};


struct Rotary {
  bool  Menu[10]; 
  bool  Sequencer[4];
};

/* Step to start */
byte  STEP = 1;
byte  STEPMAX = 4;

short  attack;
short  decay;
short  hertz1;
short  hertz2;
short  rotarypoti;
short  m_attack;
short  m_decay;
short  m_hertz1;
short  m_hertz2;
short  m_rotarypoti;

int   S1B1;
int   S1B2;
int   S1B3;
int   S1B4;

Metronome Metro_1(200);
Metronome Metro_2(200);
Metronome Metro_3(200);
