/***********************************************************************
 *
 * $Id: StV0MiniDst.cc,v 1.3 1999/07/30 15:01:13 genevb Exp $
 *
 * Author: Peter G. Jones, University of Birmingham, 04-Jun-1999
 *
 ***********************************************************************
 *
 * Description: V0 mini dst class
 *
 ***********************************************************************
 *
 * $Log: StV0MiniDst.cc,v $
 * Revision 1.3  1999/07/30 15:01:13  genevb
 * Switched from TObject to StHFillObject inheritance
 *
 * Revision 1.2  1999/07/26 19:17:24  jones
 * Added primary vertex position and v0 daughter DCA to the primary vertex
 *
 * Revision 1.1  1999/07/13 12:42:24  jones
 * *** empty log message ***
 *
 *
 ***********************************************************************/
#include "global/inc/phys_constants.h"
#include "StV0MiniDst.hh"
#include "StEvent/StGlobalTrack.h"
#include "StEvent/StV0Vertex.h"
#include "SystemOfUnits.h"
ClassImp(StV0MiniDst)

StV0MiniDst::StV0MiniDst() { 
}

StV0MiniDst::StV0MiniDst(StV0Vertex* v0Vertex,
			 StVertex*   primaryVertex) {
  mPrimVertex[0] = primaryVertex->position().x();
  mPrimVertex[1] = primaryVertex->position().y();
  mPrimVertex[2] = primaryVertex->position().z();
  mPosition[0] = v0Vertex->position().x();
  mPosition[1] = v0Vertex->position().y();
  mPosition[2] = v0Vertex->position().z();
  mDecayDistance = 
    sqrt(pow(mPosition[0] - mPrimVertex[0],2) +
	 pow(mPosition[1] - mPrimVertex[1],2) +
	 pow(mPosition[2] - mPrimVertex[2],2));

  mDcaDaughters = v0Vertex->dcaDaughters();
  mDcaParentToPrimVertex = v0Vertex->dcaParentToPrimaryVertex();
  mDcaPosToPrimVertex = v0Vertex->dcaDaughterToPrimaryVertex(positiveTrack);
  mDcaNegToPrimVertex = v0Vertex->dcaDaughterToPrimaryVertex(negativeTrack);
  mMomNegDaughter[0] = v0Vertex->momentumOfDaughter(negativeTrack).x();
  mMomNegDaughter[1] = v0Vertex->momentumOfDaughter(negativeTrack).y();
  mMomNegDaughter[2] = v0Vertex->momentumOfDaughter(negativeTrack).z();
  mMomPosDaughter[0] = v0Vertex->momentumOfDaughter(positiveTrack).x();
  mMomPosDaughter[1] = v0Vertex->momentumOfDaughter(positiveTrack).y();
  mMomPosDaughter[2] = v0Vertex->momentumOfDaughter(positiveTrack).z();

  this->Update();
}

StV0MiniDst::~StV0MiniDst() {
}

void StV0MiniDst::Update() { 
  // Calculate derived data members

       mDecayDistance = sqrt(pow(mPosition[0]-mPrimVertex[0],2) +
			     pow(mPosition[1]-mPrimVertex[1],2) +
			     pow(mPosition[2]-mPrimVertex[2],2));
  
    mPtot2PosDaughter = mMomPosDaughter[0]*mMomPosDaughter[0] +
                        mMomPosDaughter[1]*mMomPosDaughter[1] +
                        mMomPosDaughter[2]*mMomPosDaughter[2];

    mPtot2NegDaughter = mMomNegDaughter[0]*mMomNegDaughter[0] +
                        mMomNegDaughter[1]*mMomNegDaughter[1] +
                        mMomNegDaughter[2]*mMomNegDaughter[2];

                  mPx = mMomPosDaughter[0] + mMomNegDaughter[0];
                  mPy = mMomPosDaughter[1] + mMomNegDaughter[1];
                  mPz = mMomPosDaughter[2] + mMomNegDaughter[2];
                 mPt2 = mPx*mPx + mPy*mPy;
               mPtot2 = mPt2 + mPz*mPz;

       mMomPosAlongV0 = ( mMomPosDaughter[0]*mPx + 
			  mMomPosDaughter[1]*mPy +
			  mMomPosDaughter[2]*mPz ) / sqrt(mPtot2); 
       mMomNegAlongV0 = ( mMomNegDaughter[0]*mPx + 
			  mMomNegDaughter[1]*mPy + 
			  mMomNegDaughter[2]*mPz ) / sqrt(mPtot2);
}

float StV0MiniDst::alpha() {
  return (mMomPosAlongV0-mMomNegAlongV0)/
         (mMomPosAlongV0+mMomNegAlongV0);
}

float StV0MiniDst::ptArm() {
  return sqrt(mPtot2PosDaughter - mMomPosAlongV0*mMomPosAlongV0);
}

float StV0MiniDst::eLambda() {
  return sqrt(mPtot2+M_LAMBDA*M_LAMBDA);
}

float StV0MiniDst::eK0Short() {
  return sqrt(mPtot2+M_KAON_0_SHORT*M_KAON_0_SHORT);
}

float StV0MiniDst::ePosDaughterProton() {
  return sqrt(mPtot2PosDaughter+M_PROTON*M_PROTON);
}

float StV0MiniDst::eNegDaughterProton() {
  return sqrt(mPtot2NegDaughter+M_ANTIPROTON*M_ANTIPROTON);
}

float StV0MiniDst::ePosDaughterPion() {
  return sqrt(mPtot2PosDaughter+M_PION_PLUS*M_PION_PLUS);
}

float StV0MiniDst::eNegDaughterPion() {
  return sqrt(mPtot2NegDaughter+M_PION_MINUS*M_PION_MINUS);
}

float StV0MiniDst::massLambda() {
  return sqrt(pow(ePosDaughterProton()+eNegDaughterPion(),2)-mPtot2);
}

float StV0MiniDst::massAntiLambda() {
  return sqrt(pow(eNegDaughterProton()+ePosDaughterPion(),2)-mPtot2);
}

float StV0MiniDst::massK0Short() {
  return sqrt(pow(ePosDaughterPion()+eNegDaughterPion(),2)-mPtot2);
}

float StV0MiniDst::rapLambda() {
  float ela = eLambda();
  return 0.5*log((ela+mPz)/(ela-mPz));
}

float StV0MiniDst::rapK0Short() {
  float ek0 = eK0Short();
  return 0.5*log((ek0+mPz)/(ek0-mPz));
}

float StV0MiniDst::cTauLambda() {
  return massLambda()*mDecayDistance/sqrt(mPtot2);
}

float StV0MiniDst::cTauK0Short() {
  return massK0Short()*mDecayDistance/sqrt(mPtot2);
}

float StV0MiniDst::ptPosDaughter() {
  return sqrt(mPtot2PosDaughter-mMomPosDaughter[2]*mMomPosDaughter[2]);
}

float StV0MiniDst::ptotPosDaughter() {
  return sqrt(mPtot2PosDaughter);
}

float StV0MiniDst::ptNegDaughter() {
  return sqrt(mPtot2NegDaughter-mMomNegDaughter[2]*mMomNegDaughter[2]);
}

float StV0MiniDst::ptotNegDaughter() {
  return sqrt(mPtot2NegDaughter);
}

float StV0MiniDst::pt() {
  return sqrt(mPt2);
}

float StV0MiniDst::ptot() {
  return sqrt(mPtot2);
}

void StV0MiniDst::LearnMembers() {
  LearnMember("primVertex",(TMemOff) &StV0MiniDst::primVertex);
  LearnMember("decayDistance",(TMemOff) &StV0MiniDst::decayDistance);
  LearnMember("position",(TMemOff) &StV0MiniDst::position);
  LearnMember("dcaDaughters",(TMemOff) &StV0MiniDst::dcaDaughters);
  LearnMember("dcaParentToPrimVertex",(TMemOff) &StV0MiniDst::dcaParentToPrimVertex);
  LearnMember("dcaPosToPrimVertex",(TMemOff) &StV0MiniDst::dcaPosToPrimVertex);
  LearnMember("dcaNegToPrimVertex",(TMemOff) &StV0MiniDst::dcaNegToPrimVertex);
  LearnMember("momPosDaughter",(TMemOff) &StV0MiniDst::momPosDaughter);
  LearnMember("momNegDaughter",(TMemOff) &StV0MiniDst::momNegDaughter);
  LearnMember("alpha",(TMemOff) &StV0MiniDst::alpha);
  LearnMember("ptArm",(TMemOff) &StV0MiniDst::ptArm);
  LearnMember("eLambda",(TMemOff) &StV0MiniDst::eLambda);
  LearnMember("eK0Short",(TMemOff) &StV0MiniDst::eK0Short);
  LearnMember("ePosDaughterProton",(TMemOff) &StV0MiniDst::ePosDaughterProton);
  LearnMember("ePosDaughterPion",(TMemOff) &StV0MiniDst::ePosDaughterPion);
  LearnMember("eNegDaughterProton",(TMemOff) &StV0MiniDst::eNegDaughterProton);
  LearnMember("eNegDaughterPion",(TMemOff) &StV0MiniDst::eNegDaughterPion);
  LearnMember("massLambda",(TMemOff) &StV0MiniDst::massLambda);
  LearnMember("massAntiLambda",(TMemOff) &StV0MiniDst::massAntiLambda);
  LearnMember("massK0Short",(TMemOff) &StV0MiniDst::massK0Short);
  LearnMember("rapLambda",(TMemOff) &StV0MiniDst::rapLambda);
  LearnMember("rapK0Short",(TMemOff) &StV0MiniDst::rapK0Short);
  LearnMember("cTauLambda",(TMemOff) &StV0MiniDst::cTauLambda);
  LearnMember("cTauK0Short",(TMemOff) &StV0MiniDst::cTauK0Short);
  LearnMember("pt",(TMemOff) &StV0MiniDst::pt);
  LearnMember("ptot",(TMemOff) &StV0MiniDst::ptot);
  LearnMember("ptPosDaughter",(TMemOff) &StV0MiniDst::ptPosDaughter);
  LearnMember("ptotPosDaughter",(TMemOff) &StV0MiniDst::ptotPosDaughter);
  LearnMember("ptNegDaughter",(TMemOff) &StV0MiniDst::ptNegDaughter);
  LearnMember("ptotNegDaughter",(TMemOff) &StV0MiniDst::ptotNegDaughter);
}
