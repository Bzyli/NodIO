#ifndef faust_teensy_h_
#define faust_teensy_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "Audio.h"

#include "Additif.h"
#include "Sine.h"
#include "Echo.h"

class MyDsp : public AudioStream
{
  public:
    MyDsp(); // constructeur
    ~MyDsp(); // destructeur
    
    virtual void update(void);
    void setFreq(float freq);
    
  private:
    Sine sine;
    Additif additif;
    Echo echo;
    Echo echo0;
    Echo echo1;
    float amplitude;
    unsigned long lastUpdate;
};

#endif
