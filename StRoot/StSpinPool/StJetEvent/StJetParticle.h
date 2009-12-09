// -*- mode: c++ -*-
//
// Pibero Djawotho <pibero@tamu.edu>
// Texas A&M University
// 8 Dec 2009
//

#ifndef ST_JET_PARTICLE_H
#define ST_JET_PARTICLE_H

#include "TLorentzVector.h"
#include "StJetElement.h"

class StJetParticle : public StJetElement {
public:
  StJetParticle() {}

  float                     m() const;
  float                     e() const;
  int                     pdg() const;
  int                  status() const;
  TLorentzVector fourMomentum() const;

private:
  friend class StjeJetEventTreeWriter;

  float mM;
  float mE;
  int   mPdg;
  int   mStatus;

  ClassDef(StJetParticle,1);
};

inline float StJetParticle::m     () const { return mM     ; }
inline float StJetParticle::e     () const { return mE     ; }
inline int   StJetParticle::pdg   () const { return mPdg   ; }
inline int   StJetParticle::status() const { return mStatus; }

inline TLorentzVector StJetParticle::fourMomentum() const
{
  TLorentzVector p;
  p.SetPtEtaPhiE(mPt,mEta,mPhi,mE);
  return p;
}

#endif	// ST_JET_PARTICLE_H
