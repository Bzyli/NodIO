// PeakEQ.h
#include <AudioStream.h>
#include <math.h>

class PeakEQ : public AudioStream {
public:
    PeakEQ() : AudioStream(1, inputQueueArray) {
        setParameters(1000.0f, 1.0f, 0.0f); // Default: 1kHz, Q=1, 0dB
    }
    
    void setParameters(float freq, float Q, float gainDB) {
        if (freq == currentFreq && Q == currentQ && gainDB == currentGain) return;
        
        currentFreq = freq;
        currentQ = Q;
        currentGain = gainDB;
        
        // Calculate coefficients
        const float omega = 2.0f * 3.141592 * freq / AUDIO_SAMPLE_RATE_EXACT;
        const float alpha = sinf(omega) / (2.0f * Q);
        const float A = powf(10.0f, gainDB / 40.0f); // Divided by 40 for voltage gain
        
        const float a0 = 1.0f + alpha / A;
        const float a0inv = 1.0f / a0;
        
        b0 = (1.0f + alpha * A) * a0inv;
        b1 = (-2.0f * cosf(omega)) * a0inv;
        b2 = (1.0f - alpha * A) * a0inv;
        a1 = b1; // Same as b1 but with different a0
        a2 = (1.0f - alpha / A) * a0inv;
    }

    virtual void update(void) {
        audio_block_t *input, *output;
        
        input = receiveReadOnly();
        if (!input) return;
        
        output = allocate();
        if (!output) {
            release(input);
            return;
        }
        
        for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
            float in = input->data[i] * (1.0f / 32768.0f);
            float out = b0 * in + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
            
            // Update state variables
            x2 = x1; 
            x1 = in;
            y2 = y1;
            y1 = out;
            
            output->data[i] = constrain_int16(out * 32767.0f);
        }
        
        transmit(output);
        release(input);
        release(output);
    }

private:
    audio_block_t *inputQueueArray[1];
    float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f, a1 = 0.0f, a2 = 0.0f;
    float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f;
    float currentFreq = 0.0f, currentQ = 0.0f, currentGain = 0.0f;

    int constrain_int16(float value) {
        if (value > 32767.0f) return 32767;
        if (value < -32768.0f) return -32768;
        return (int) value;
    }
};