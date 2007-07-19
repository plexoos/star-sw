// -*- mode: C++ -*-

//
// Pibero Djawotho <pibero@indiana.edu>
// Indiana University
// 12 July 2007
//
// $Log: StPythiaEvent.h,v $
// Revision 1.1  2007/07/19 01:40:41  kocolosk
// use Pibero's StPythiaEvent class to supply mcAsymMaker results to user
//

#ifndef ST_PYTHIA_EVENT
#define ST_PYTHIA_EVENT

#include "TParticle.h"
#include "TClonesArray.h"

#include "tables/St_particle_Table.h"

class StPythiaEvent : public TObject {
public:
    StPythiaEvent();
    virtual ~StPythiaEvent();
    StPythiaEvent(const StPythiaEvent& other);
    StPythiaEvent& operator=(const StPythiaEvent& rhs); 

    enum GRSV { LO=0, NLO=1, STD=1, ZERO=2, MAX=3, MIN=4 };

    int runId() const;
    int eventId() const;
    int processId() const;
    TVector3& vertex();
    const TVector3& vertex() const;
    float s() const;
    float t() const;
    float u() const;
    float pt() const;
    float cosTheta() const;
    float x1() const;
    float x2() const;
    float Q2() const;
    float partonALL() const;
    float dF1(GRSV scenario) const;
    float dF2(GRSV scenario) const;
    float f1(GRSV scenario) const;
    float f2(GRSV scenario) const;
    float ALL(GRSV scenario) const;

    TClonesArray* particles();
    int numberOfParticles() const;
    TParticle* particle(int i);

    void Clear(Option_t* option = "");
    void setRunId(int id);
    void setEventId(int id);
    void setProcessId(int id);
    void setVertex(const TVector3& v);
    void setS(float s);
    void setT(float t);
    void setU(float u);
    void setPt(float pt);
    void setCosTheta(float cosTheta);
    void setX1(float x1);
    void setX2(float x2);
    void setPartonALL(float a);
    void setDF1(GRSV scenario, float val);
    void setDF2(GRSV scenario, float val);
    void setF1(GRSV scenario, float val);
    void setF2(GRSV scenario, float val);

    void addParticle(const particle_st& particle);

private:
    int mRunId;
    int mEventId;
    int mProcessId;
    TVector3 mVertex;
    float mS;
    float mT;
    float mU;
    float mPt;
    float mCosTheta;
    float mX1;
    float mX2;
    float mPartonALL;
    float mDF1[5];  //[LO][NLO][ZERO][MAX][MIN]
    float mDF2[5];  //[LO][NLO][ZERO][MAX][MIN]
    float mF1[2];   //[LO][NLO]
    float mF2[2];   //[LO][NLO]

    TClonesArray* mParticles;

    ClassDef(StPythiaEvent, 2);
};

inline int StPythiaEvent::runId() const { return mRunId; }
inline int StPythiaEvent::eventId() const { return mEventId; }
inline int StPythiaEvent::processId() const { return mProcessId; }
inline TVector3& StPythiaEvent::vertex() { return mVertex; }
inline const TVector3& StPythiaEvent::vertex() const { return mVertex; }
inline float StPythiaEvent::s() const { return mS; }
inline float StPythiaEvent::t() const { return mT; }
inline float StPythiaEvent::u() const { return mU; }
inline float StPythiaEvent::pt() const { return mPt; }
inline float StPythiaEvent::cosTheta() const { return mCosTheta; }
inline float StPythiaEvent::x1() const { return mX1; }
inline float StPythiaEvent::x2() const { return mX2; }
inline float StPythiaEvent::Q2() const { return mPt * mPt; }
inline float StPythiaEvent::partonALL() const { return mPartonALL; }
inline float StPythiaEvent::dF1(GRSV scenario) const { return mDF1[scenario]; }
inline float StPythiaEvent::dF2(GRSV scenario) const { return mDF2[scenario]; }

inline float StPythiaEvent::f1(GRSV scenario) const 
{
    if(scenario == LO) return mF1[0];
    return mF1[1];
}

inline float StPythiaEvent::f2(GRSV scenario) const 
{
    if(scenario == LO) return mF2[0];
    return mF2[1];
}

inline float StPythiaEvent::ALL(GRSV scenario) const
{
    switch(scenario) {
        case(LO):   return (mDF1[0]*mDF2[0]*mPartonALL) / (mF1[0]*mF2[0]);
        case(NLO):  return (mDF1[1]*mDF2[1]*mPartonALL) / (mF1[1]*mF2[1]);
        case(ZERO): return (mDF1[2]*mDF2[2]*mPartonALL) / (mF1[1]*mF2[1]);
        case(MAX):  return (mDF1[3]*mDF2[3]*mPartonALL) / (mF1[1]*mF2[1]);
        case(MIN):  return (mDF1[4]*mDF2[4]*mPartonALL) / (mF1[1]*mF2[1]);
        default:    return -999;
    }
}

inline TClonesArray* StPythiaEvent::particles() { return mParticles; }
inline int StPythiaEvent::numberOfParticles() const { return mParticles->GetEntriesFast(); }
inline TParticle* StPythiaEvent::particle(int i) { return (TParticle*)mParticles->At(i); }

inline void StPythiaEvent::addParticle(const particle_st& particle)
{
    new ((*mParticles)[mParticles->GetEntriesFast()])
        TParticle(particle.idhep,
        particle.isthep,
        particle.jmohep[0],
        particle.jmohep[1],
        particle.jdahep[0],
        particle.jdahep[1],
        TLorentzVector(particle.phep),
        TLorentzVector(particle.vhep));
}

inline void StPythiaEvent::Clear(Option_t* option)
{
    mRunId = 0;
    mEventId = 0;
    mProcessId = 0;
    mVertex.SetXYZ(0, 0, 0);
    mS = 0;
    mT = 0;
    mU = 0;
    mPt = 0;
    mCosTheta = 0;
    mX1 = 0;
    mX2 = 0;
    mPartonALL = 0;
    mDF1[0] = 0; mDF1[1] = 0; mDF1[2] = 0; mDF1[3] = 0; mDF1[4] = 0;
    mDF2[0] = 0; mDF2[1] = 0; mDF2[2] = 0; mDF2[3] = 0; mDF2[4] = 0;
    mF1[0] = 0; mF1[1] = 0;
    mF2[0] = 0; mF2[1] = 0;

    mParticles->Clear(option);
}

inline void StPythiaEvent::setRunId(int id) { mRunId = id; }
inline void StPythiaEvent::setEventId(int id) { mEventId = id; }
inline void StPythiaEvent::setProcessId(int id) { mProcessId = id; }
inline void StPythiaEvent::setVertex(const TVector3& v) { mVertex = v; }
inline void StPythiaEvent::setS(float s) { mS = s; }
inline void StPythiaEvent::setT(float t) { mT = t; }
inline void StPythiaEvent::setU(float u) { mU = u; }
inline void StPythiaEvent::setPt(float pt) { mPt = pt; }
inline void StPythiaEvent::setCosTheta(float cosTheta) { mCosTheta = cosTheta; }
inline void StPythiaEvent::setX1(float x1) { mX1 = x1; }
inline void StPythiaEvent::setX2(float x2) { mX2 = x2; }
inline void StPythiaEvent::setPartonALL(float a) { mPartonALL = a; }
inline void StPythiaEvent::setDF1(GRSV scenario, float val) { mDF1[scenario] = val; }
inline void StPythiaEvent::setDF2(GRSV scenario, float val) { mDF2[scenario] = val; }

inline void StPythiaEvent::setF1(GRSV scenario, float val) 
{
    if(scenario == LO) mF1[0] = val;
    mF1[1] = val;
}

inline void StPythiaEvent::setF2(GRSV scenario, float val) 
{
    if(scenario == LO) mF2[0] = val;
    mF2[1] = val;
}

#endif
