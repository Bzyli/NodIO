/* ------------------------------------------------------------
name: "compresseur"
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
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fRec1[2];
	float fVec0[2];
	int iRec0[2];
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fHslider4;
	float fRec3[2];
	float fVec1[2];
	int iRec2[2];
	
 public:
	mydsp() {
	}
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("envelopes.lib/ar:author", "Yann Orlarey, Stéphane Letz");
		m->declare("envelopes.lib/author", "GRAME");
		m->declare("envelopes.lib/copyright", "GRAME");
		m->declare("envelopes.lib/license", "LGPL with exception");
		m->declare("envelopes.lib/name", "Faust Envelope Library");
		m->declare("envelopes.lib/version", "1.3.0");
		m->declare("filename", "compresseur.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("name", "compresseur");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(-1e+01f);
		fHslider1 = FAUSTFLOAT(0.01f);
		fHslider2 = FAUSTFLOAT(0.1f);
		fHslider3 = FAUSTFLOAT(4.0f);
		fHslider4 = FAUSTFLOAT(6.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec1[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fVec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			iRec0[l2] = 0;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			iRec2[l5] = 0;
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
		ui_interface->openVerticalBox("compresseur");
		ui_interface->declare(&fHslider1, "s", "");
		ui_interface->addHorizontalSlider("Attack", &fHslider1, FAUSTFLOAT(0.01f), FAUSTFLOAT(0.001f), FAUSTFLOAT(0.1f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider4, "dB", "");
		ui_interface->addHorizontalSlider("Makeup Gain", &fHslider4, FAUSTFLOAT(6.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("Ratio", &fHslider3, FAUSTFLOAT(4.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->declare(&fHslider2, "s", "");
		ui_interface->addHorizontalSlider("Release", &fHslider2, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.01f), FAUSTFLOAT(0.5f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider0, "dB", "");
		ui_interface->addHorizontalSlider("Threshold", &fHslider0, FAUSTFLOAT(-1e+01f), FAUSTFLOAT(-4e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = float(fHslider0);
		float fSlow1 = std::max<float>(1.0f, fConst0 * float(fHslider1));
		float fSlow2 = 1.0f / fSlow1;
		float fSlow3 = 1.0f / std::max<float>(1.0f, fConst0 * float(fHslider2));
		float fSlow4 = 1.0f / float(fHslider3);
		float fSlow5 = std::pow(1e+01f, 0.05f * float(fHslider4));
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = float(input0[i0]);
			fRec1[0] = fConst1 * std::fabs(fTemp0) + fConst2 * fRec1[1];
			float fTemp1 = std::log10(fRec1[0]);
			fVec0[0] = fTemp1;
			float fTemp2 = 2e+01f * fVec0[1];
			float fTemp3 = 2e+01f * fTemp1;
			iRec0[0] = (iRec0[1] + (iRec0[1] > 0)) * (fTemp3 <= fTemp2) + (fTemp3 > fTemp2);
			float fTemp4 = float(iRec0[0]);
			float fTemp5 = std::max<float>(0.0f, std::min<float>(fSlow2 * fTemp4, fSlow3 * (fSlow1 - fTemp4) + 1.0f));
			output0[i0] = FAUSTFLOAT(fSlow5 * fTemp0 * ((fTemp5 > fSlow0) ? std::pow(1e+01f, 0.05f * (fSlow0 + fSlow4 * (fTemp5 - fSlow0))) : 1.0f));
			float fTemp6 = float(input1[i0]);
			fRec3[0] = fConst1 * std::fabs(fTemp6) + fConst2 * fRec3[1];
			float fTemp7 = std::log10(fRec3[0]);
			fVec1[0] = fTemp7;
			float fTemp8 = 2e+01f * fVec1[1];
			float fTemp9 = 2e+01f * fTemp7;
			iRec2[0] = (iRec2[1] + (iRec2[1] > 0)) * (fTemp9 <= fTemp8) + (fTemp9 > fTemp8);
			float fTemp10 = float(iRec2[0]);
			float fTemp11 = std::max<float>(0.0f, std::min<float>(fSlow2 * fTemp10, fSlow3 * (fSlow1 - fTemp10) + 1.0f));
			output1[i0] = FAUSTFLOAT(fSlow5 * fTemp6 * ((fTemp11 > fSlow0) ? std::pow(1e+01f, 0.05f * (fSlow0 + fSlow4 * (fTemp11 - fSlow0))) : 1.0f));
			fRec1[1] = fRec1[0];
			fVec0[1] = fVec0[0];
			iRec0[1] = iRec0[0];
			fRec3[1] = fRec3[0];
			fVec1[1] = fVec1[0];
			iRec2[1] = iRec2[0];
		}
	}

};

#endif
