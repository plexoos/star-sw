/***************************************************************************
 *
 * $Id: ParityEventCut.h,v 1.2 1999/10/15 01:56:55 lisa Exp $
 *
 * Author: Mike Lisa, Ohio State, lisa@mps.ohio-state.edu
 ***************************************************************************
 *
 * Description: part of STAR HBT Framework: StHbtMaker package
 *   A simple event-wise cut that selects on multiplicity and z-position
 *   of primary vertex           
 *
 ***************************************************************************
 **************************************************************************/

#ifndef ParityEventCut_hh
#define ParityEventCut_hh

#ifndef StMaker_H
#include "StMaker.h"
#endif

#include "StHbtMaker/Base/StHbtEventCut.h"

class ParityEventCut : public StHbtEventCut {

public:

  ParityEventCut(char* title = "Parity Cut", const int& nbins =50 , const float& Lo =-100, const float& Hi =100);
  ~ParityEventCut();

  void SetEventMult(const int& lo,const int& hi);
  void SetVertZPos(const float& lo, const float& hi);

  virtual StHbtString Report();

  virtual bool Pass(const StHbtEvent*);


  // note that these data are public-- the CorrFctn will access (increment) them...
  double RealQuantity;
  long nReals;
  double MixedQuantity;
  long nMixed;

  StHbt1DHisto* RealHisto();
  StHbt1DHisto* MixedHisto();

private:   // here are the quantities I want to cut on...

  int mEventMult[2];      // range of multiplicity
  float mVertZPos[2];     // range of z-position of vertex

  long mNEventsPassed;
  long mNEventsFailed;

  StHbt1DHisto* mReals;  
  StHbt1DHisto* mMixed;

  ClassDef(ParityEventCut, 0)

};

inline void ParityEventCut::SetEventMult(const int& lo, const int& hi){mEventMult[0]=lo; mEventMult[1]=hi;}
inline void ParityEventCut::SetVertZPos(const float& lo, const float& hi){mVertZPos[0]=lo; mVertZPos[1]=hi;}

inline StHbt1DHisto* ParityEventCut::RealHisto(){return mReals;}
inline StHbt1DHisto* ParityEventCut::MixedHisto(){return mMixed;}


#endif
