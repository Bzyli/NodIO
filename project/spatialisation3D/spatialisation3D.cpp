/* ------------------------------------------------------------
name: "spatialisation3D"
Code generated with Faust 2.79.0 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif


class mydsp : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int IOTA0;
	float fVec0[128];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider1;
	float fConst2;
	
 public:
	mydsp() {
	}
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("filename", "spatialisation3D.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "spatialisation3D");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 0.1f * fConst0 + 1.0f;
		fConst2 = 0.00058309035f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(1.0f);
		fHslider1 = FAUSTFLOAT(0.5f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 128; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("spatialisation3D");
		ui_interface->addHorizontalSlider("angle", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider0, "unit", "m");
		ui_interface->addHorizontalSlider("distance", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = 1.0f / float(fHslider0);
		float fSlow1 = float(fHslider1);
		float fSlow2 = fConst2 * (1.0f - fSlow1);
		int iSlow3 = int(fSlow2);
		int iSlow4 = int(std::min<float>(fConst1, float(std::max<int>(0, iSlow3 + 1))));
		float fSlow5 = std::floor(fSlow2);
		float fSlow6 = fSlow2 - fSlow5;
		int iSlow7 = int(std::min<float>(fConst1, float(std::max<int>(0, iSlow3))));
		float fSlow8 = fSlow5 + (1.0f - fSlow2);
		float fSlow9 = fConst2 * fSlow1;
		int iSlow10 = int(fSlow9);
		int iSlow11 = int(std::min<float>(fConst1, float(std::max<int>(0, iSlow10 + 1))));
		float fSlow12 = std::floor(fSlow9);
		float fSlow13 = fSlow9 - fSlow12;
		int iSlow14 = int(std::min<float>(fConst1, float(std::max<int>(0, iSlow10))));
		float fSlow15 = fSlow12 + (1.0f - fSlow9);
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = fSlow0 * float(input0[i0]);
			fVec0[IOTA0 & 127] = fTemp0;
			output0[i0] = FAUSTFLOAT(fSlow8 * fVec0[(IOTA0 - iSlow7) & 127] + fSlow6 * fVec0[(IOTA0 - iSlow4) & 127]);
			output1[i0] = FAUSTFLOAT(fSlow15 * fVec0[(IOTA0 - iSlow14) & 127] + fSlow13 * fVec0[(IOTA0 - iSlow11) & 127]);
			IOTA0 = IOTA0 + 1;
		}
	}

};

#endif
