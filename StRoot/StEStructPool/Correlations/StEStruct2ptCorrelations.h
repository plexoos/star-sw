 /**********************************************************************
 *
 * $Id: StEStruct2ptCorrelations.h,v 1.14 2009/05/08 00:09:54 prindle Exp $
 *
 * Author: Jeff Porter adaptation of Aya's 2pt-analysis
 *
 **********************************************************************
 *
 * Description:  Analysis code for 2pt-analysis. 
 *    The analysis runs as follows:
 *       1D and 2D arrays (in yt,eta,phi) are setup
 *       and filled for each of the 8 pair types:
 *       Sibling (++, +-, -+, --)
 *       Mixed   (++, +-, -+, --)
 *       Particle id is done via dEdx and introduced into the analysis via cut-bins.
 *       Order particles so pi always come before K before p and plus comes before
 *       minus. 2D histograms will not be guaranteed to be symmetric.
 *       The 2D versions are additionally divided into z-vertex (via the StEStructBuffer)
 *       After arrays are filled (looped over all events/job), Histograms are 
 *       created, filled, and written out to the data file for further
 *       processing.
 *
 *       Note that if we find charge symmetry we can form LS, US, CD and CI
 *       combinations using these histograms.
 *
 *
 ***********************************************************************/
#ifndef __STESTRUCT2PTCORRELATIONS__H
#define __STESTRUCT2PTCORRELATIONS__H


#include "TROOT.h"
#include "TRandom2.h"
#include "StEStructPool/AnalysisMaker/StEStructAnalysis.h"
#include "StEStructPool/AnalysisMaker/StEStructQAHists.h"
#include "StEStructPool/EventMaker/StEStructCentrality.h"
#include "StEStructPairCuts.h"
#include "StEStructBinning.h"
#include "StEStructBuffer.h"

class TFile;
class TH1;
class TH2;
class TH1F;
class TH2F;
class TH1D;
class TH2D;
class StEStructEvent;
class StTimer;
#define _MAX_ZVBINS_ 30

class StEStruct2ptCorrelations: public StEStructAnalysis {

 protected:

  // Event-level hists needed for normalization and pt-correlations
  TH1D** mHNEventsSib;
  TH1D** mHNEventsMix;
  TH1D** mHNEventsPosSib;
  TH1D** mHNEventsPosMix;
  TH1D** mHNEventsNegSib;
  TH1D** mHNEventsNegMix;
  TH1D*  mHptAll;
  TH2D*  mHmix;
  TH1D*  mHcb;  // my local hist for cutbin usage
  TH1D **mHMeanPtP;
  TH1D **mHMeanPtM;
  TH1D **mHMeanYtP;
  TH1D **mHMeanYtM;
  TH1D **mHEtaP;
  TH1D **mHEtaM;

  // HBT parameters
  qBins * mQinv[8]; //!  1D
  TH1D ** mHQinv[8];//!  1D hist
  qBins * mNQinv[8]; //!  1D
  TH1D ** mHNQinv[8];//!  1D hist

  //-> X vs X 
  ytBins  **mYtYt[8];     //! YtYt are controlled by bit 9 of manalysisMode
  ytBins  **mNYtYt[8];    //! Npair for eta_delta weight errors

  etaBins **mEtaEta[8];   //! EtaEta, PhiPhi are controlled by bit 6 of manalysisMode
  etaBins **mPrEtaEta[8]; //! weight = pt1*pt2
  etaBins **mPaEtaEta[8]; //! weight = pt1
  etaBins **mPbEtaEta[8]; //! weight = pt2
  phiBins **mPhiPhi[8];   //!
  phiBins **mNPhiPhi[8];  //! Npair for eta_delta weight errors
  phiBins **mPrPhiPhi[8]; //!  "
  phiBins **mPaPhiPhi[8]; //!  "
  phiBins **mPbPhiPhi[8]; //!  "

  TH2D ** mHYtYt[8];     //!
  TH2D ** mHNYtYt[8];    //! Npair for eta_delta weight errors

  TH2D ** mHEtaEta[8];   //!
  TH2D ** mHPrEtaEta[8]; //!
  TH2D ** mHPaEtaEta[8]; //!
  TH2D ** mHPbEtaEta[8]; //!
  TH2D ** mHPhiPhi[8];   //!
  TH2D ** mHNPhiPhi[8];  //! Npair for eta_delta weight errors
  TH2D ** mHPrPhiPhi[8]; //!
  TH2D ** mHPaPhiPhi[8]; //!
  TH2D ** mHPbPhiPhi[8]; //!

  // Delta Y vs Delta X
  dphiBins **mJtDEtaDPhi[8]; //!
  dphiBins **mPrJtDEtaDPhi[8]; //!
  dphiBins **mPaJtDEtaDPhi[8]; //!
  dphiBins **mPbJtDEtaDPhi[8]; //!

  TH2D ** mHJtDEtaDPhi[8];
  TH2D ** mHPrJtDEtaDPhi[8];
  TH2D ** mHPaJtDEtaDPhi[8];
  TH2D ** mHPbJtDEtaDPhi[8];

  // Sum Y vs Delta X. These are controlled by bit 7 of manalysisMode
  dytBins  **mAtSYtDYt[8];     //! smt array of dmt bins
  dytBins  **mAtNSYtDYt[8];    //! Npair for eta_delta weight errors
  dphiBins **mJtSEtaDPhi[8];   //! 
  dphiBins **mJtNSEtaDPhi[8];  //!  Npair for eta_delta weight errors
  dphiBins **mPrJtSEtaDPhi[8]; //! 
  dphiBins **mPaJtSEtaDPhi[8]; //! 
  dphiBins **mPbJtSEtaDPhi[8]; //! 

  TH2D ** mHAtSYtDYt[8];
  TH2D ** mHAtNSYtDYt[8];//! Npair for eta_delta weight errors
  TH2D ** mHJtSEtaDPhi[8];//!
  TH2D ** mHJtNSEtaDPhi[8];//! Npair for eta_delta weight errors
  TH2D ** mHPrJtSEtaDPhi[8];//!
  TH2D ** mHPaJtSEtaDPhi[8];//!
  TH2D ** mHPbJtSEtaDPhi[8];//!

  // TPC Separation. These are controlled by bit 5 of manalysisMode
  TPCSepBins *mTPCAvgTSep[8];  //1D
  TPCSepBins *mTPCAvgZSep[8];
  TPCSepBins *mTPCEntTSep[8];
  TPCSepBins *mTPCEntZSep[8];
  TPCSepBins *mTPCMidTSep[8];
  TPCSepBins *mTPCMidZSep[8];
  TPCSepBins *mTPCExitTSep[8];
  TPCSepBins *mTPCExitZSep[8];

  TPCSepBins *mTPCMidTdptP[8]; //! needed to differentiate by sign of deltaPt   
  TPCSepBins *mTPCMidTdptN[8]; //! to evaluate pair crossing cut   
  TPCSepBins *mTPCMidZdptP[8];
  TPCSepBins *mTPCMidZdptN[8];

  TH1D **  mHTPCAvgTSep[8];
  TH1D **  mHTPCAvgZSep[8];
  TH1D **  mHTPCEntTSep[8];
  TH1D **  mHTPCEntZSep[8];
  TH1D **  mHTPCMidTSep[8];
  TH1D **  mHTPCMidZSep[8];
  TH1D **  mHTPCExitTSep[8];
  TH1D **  mHTPCExitZSep[8];

  TH1D **  mHTPCMidTdptP[8];
  TH1D **  mHTPCMidTdptN[8];
  TH1D **  mHTPCMidZdptP[8];
  TH1D **  mHTPCMidZdptN[8];

  TPCSepBins **mTPCAvgTZ[8];  //2D
  TPCSepBins **mTPCEntTZ[8];
  TPCSepBins **mTPCMidTZ[8];
  TPCSepBins **mTPCMidTZC[8];
  TPCSepBins **mTPCMidTZNC[8];
  TPCSepBins **mTPCExitTZ[8];
  dptBins **mTPCEntTdpt[8];  // T vs delta-Pt; for joint hists, use bin type of y axis
  dptBins **mTPCMidTdpt[8];
  dptBins **mTPCExitTdpt[8];

  TH2D **  mHTPCAvgTZ[8];
  TH2D **  mHTPCEntTZ[8];
  TH2D **  mHTPCMidTZ[8];
  TH2D **  mHTPCMidTZC[8];
  TH2D **  mHTPCMidTZNC[8];
  TH2D **  mHTPCExitTZ[8];
  TH2D **  mHTPCEntTdpt[8];
  TH2D **  mHTPCMidTdpt[8];
  TH2D **  mHTPCExitTdpt[8];

  char* bName[8];
  char* bTitle[8];

  // generic histogram create functions to simplify the code
  //

  void createHist2D(TH2D*** h, const char* name, int iknd, int icut, int zcut, int numCuts, int nx, float xmin, float xmax, int ny, float ymin, float ymax);
  void createHist1D(TH1D*** h, const char* name, int iknd, int icut, int zcut, int numCuts, int nx, float xmin, float xmax);
  void createHist1D(TH1F*** h, const char* name, int iknd, int icut, int zcut, int numCuts, int nx, float xmin, float xmax);
  void  moveEvents();
  void  initInternalData();
  int   bufferIndex();


 public:

  int  manalysisMode; //! simple enumeration of analyses ...
  bool mskipPairCuts; //!
  bool mdoPairCutHistograms; //!
  bool mdoPairDensityHistograms; 
  bool mskipEtaDeltaWeight; //!
  bool mdoInvariantMassHistograms; //!
  bool mdoFillEtaEta; //!
  bool mdoFillSumHistograms; //!
  bool mdontFillMeanPt; //!
  bool mdontFillYtYt; //!
  bool mInit;  //! found need when overridding this class
  bool mDeleted;//! "     " ...
  bool mHistosWritten;//! "     " ...

  StEStructEvent*        mCurrentEvent;  //! pointer to EStruct2pt data 
  StEStructPairCuts*     mPairCuts;      //! for pairs kine + all paircuts
  StEStructQAHists*      mQAHists;       //! for QA histogramming
  bool                   mlocalQAHists;  //! toggle needed for who writes output
  char*     moutFileName;   //!
  char*     mqaoutFileName; //!
  StTimer*  mtimer;         //!
  StEStructEvent*     mMixingEvent;  //! dummy      //  Previous Event Stored 

  // *** had a problem using constants here (dyn. libs wouldn't load), doing this for now...
  int kNumBuffers;
  float kZBuffMin, kZBuffMax; // Read from Cuts file. Default +/- 75cm if not found.
  float kBuffWidth;           // Set to 5 cm in Initr().
  StEStructBuffer mbuffer[_MAX_ZVBINS_];  // kNumBuffers slices in z-vertex from kZBuffMin to +kZBuffMax

  int mZBufferCutBinning;  // If true each z-buffer gets its own but bins.
  //  int             mbuffCounter[_MAX_ZVBINS_];
  TRandom2 mr2;

  //-> (pre) histograms & histograms for analysis.
  // All are arrays of 8 for 8 charged sign and combinatoric types;
  // 0,1,2,3 for Sibling (++,+-,-+,--)
  // 4,5,6,7 for Mixed   (++,+-,-+,--)
  
  int numPairs[8];
  int numPairsProcessed[8];
  int mpossiblePairs[8];
  int mInterestingPair;

  StEStruct2ptCorrelations(int mode=0);
  StEStruct2ptCorrelations(StEStructPairCuts* pcuts, int mode=0);
  virtual ~StEStruct2ptCorrelations();

  StEStructPairCuts* getPairCuts();
  void  setAnalysisMode(int mode);
  void  setCutFile(const char* cutFileName);  
  void  setZBuffLimits(StEStructCuts* cuts);// const char* cutFileName );
  void  setZBufferBinning(int zBinning);
  void  setQAHists(StEStructQAHists* qaHists);
  void  setPairCuts(StEStructPairCuts* cuts);

  //---> support of interface  
  void  setOutputFileName(const char* outFileName);
  void  setQAOutputFileName(const char* qaoutFileName);
  bool  doEvent(StEStructEvent* p);

  void  init();
  void  cleanUp();
  void  finish();

  virtual void  fillHistograms();
  virtual void  writeHistograms();
    void   writeQAHists(TFile * tf);
    void   writeDiagnostics();

    void  initArrays();
    void  deleteArrays();
    void  initHistograms();
    void  deleteHistograms();


  // analysis specific functions 
  bool  makeSiblingAndMixedPairs();
  virtual void  makePairs(StEStructEvent* e1, StEStructEvent* e2, int j);

  int   getNumPairs(int i){ return numPairs[i]; };
  int   getNumPairsProcessed(int i){ return numPairsProcessed[i]; };
  int   getPossiblePairs(int i){ return mpossiblePairs[i]; };
  int   setInterestingPair(int interest);
  int   getInterestingPair();

  void logStats(ostream& os);

    ClassDef(StEStruct2ptCorrelations,1)
};   

inline void StEStruct2ptCorrelations::setAnalysisMode(int mode){ manalysisMode=mode;};

inline void StEStruct2ptCorrelations::setCutFile(const char* cutFileName){
  if(!mPairCuts) mPairCuts=new StEStructPairCuts;
  mPairCuts->setCutFile(cutFileName);
  mPairCuts->loadCuts();
}

inline void StEStruct2ptCorrelations::setOutputFileName(const char* fName){
  if(!fName) return;
  moutFileName=new char[strlen(fName)+1];
  strcpy(moutFileName,fName);
}

inline void StEStruct2ptCorrelations::setZBufferBinning(int zBinning){
  mZBufferCutBinning = zBinning;
}

inline void StEStruct2ptCorrelations::setQAHists(StEStructQAHists* qahists){
  mQAHists = qahists;
}

inline void StEStruct2ptCorrelations::setPairCuts(StEStructPairCuts* pcuts){
  mPairCuts=pcuts;
}

inline void StEStruct2ptCorrelations::setQAOutputFileName(const char* fName){
  if(!fName) return;
  mqaoutFileName=new char[strlen(fName)+1];
  strcpy(mqaoutFileName,fName);
}

inline StEStructPairCuts* StEStruct2ptCorrelations::getPairCuts() {
  return mPairCuts;
}
inline int StEStruct2ptCorrelations::setInterestingPair(int interest) {
  return mInterestingPair = interest;
}
inline int StEStruct2ptCorrelations::getInterestingPair() {
  return mInterestingPair;
}

inline void StEStruct2ptCorrelations::logStats(ostream& os){
  char* htp[]={"SibPP","SibPM","SibMP","SibMM","MixPP","MixPM","MixMP","MixMM"};
  for(int i=0;i<8;i++){
    os<<"<pairType>"<<htp[i]<<" "<<endl;;
   os<<"   <processStat \"possiblePairs\">"<<getPossiblePairs(i);
   os<<"</processStat> "<<endl;;
   os<<"   <processStat \"inputPairs\">"<<getNumPairs(i);
   os<<"</processStat> "<<endl;;
   os<<"   <processStat \"outputPairs\">"<<getNumPairsProcessed(i);
   os<<"</processStat> "<<endl;
   os<<"</pairType>"<<endl;
  }
};



#endif


/***********************************************************************
 *
 * $Log: StEStruct2ptCorrelations.h,v $
 * Revision 1.14  2009/05/08 00:09:54  prindle
 * In 2ptCorrelations we added switches to select blocks of histograms to fill.
 * (See constructor in StEStruct2ptCorrelations.cxx)
 * Use a brute force method for checking crossing cuts. I had too many corner
 * cases with my clever check.
 * In Binning, change Yt limit and add methods for accessing number of histogram bins
 * to use (used in Support)
 *
 * Revision 1.13  2008/03/19 22:06:00  prindle
 * Added doInvariantMass flag.
 * Added some plots in pairDensityHistograms.
 * SetZOffset used to only be done when doPairDensity was true.
 * Moved creating/copying pairDensity histograms to same place as other histograms.
 * Added cutBinHistMode
 * mode3 neck was defined as yt1<2.2 && yt2<2.2 (and not soft)
 *            now is        1.8<yt1<2.2  && 1.8<yt2<2.2
 * Added gooddzdxy, Merging2 and Crossing2 to pair cuts.
 *
 * Revision 1.12  2007/11/26 19:55:23  prindle
 * In 2ptCorrelations: Support for keeping all z-bins of selected centralities
 *                     Change way \hat{p_t} is calculated for parent distributions in pid case.
 *    Binning          Added parent binning (for \hat{p_t}
 *    CutBin:          Mode 5 extensively modified.
 *                     Added invariant mass cuts (probably a bad idea in general.)
 *
 * Revision 1.11  2007/05/27 22:45:01  msd
 * Added new cut bin modes 2 (soft/hard SS/AS), 6 (z-vertex binning), and 7 (modes 2*6).
 * Fixed bug in merging cut.
 * Added a few histograms to 2pt corr.
 *
 * Revision 1.10  2006/04/27 22:40:36  porter
 * 3 changes: 1) added npair hists for errors needed with eta_delta weighting
 * 2) commented out a few histograms to trim memory usage
 * 3) changed all hists to double precision (reflected in createHists member functions)
 *
 * Revision 1.9  2006/04/06 01:01:17  prindle
 *
 *   New mode in CutBin, 5, to do pid correlations. There is still an issue
 * of how to set the pt ranges allowed for the different particle types.
 * For data we probably want to restrict p to below 1GeV for pi and K, but
 * for Hijing and Pythia we can have perfect pid. Currently cuts are type
 * into the code (so you have to re-compile to change them.)
 *
 *   In the Correlations code I split -+ from +- and am keeping track of
 * pt for each cut bin. These required changes in the Support code.
 *
 * Revision 1.8  2006/04/04 22:10:09  porter
 * a handful of changes (specific to correlations)
 *  - added StEStructQAHists so that if NOT input frm Maker, each analysis has its own
 *  - used ability to get any max,min val from the cut class - or z-vertex binning
 *  - put z-vertex binning into 1 place
 *  - switched back 1st line of pair cut method to keep pair if good, not to reject if bad.
 *  - Pair cut object is now pointer in correlations
 *  - some diagnostic printouts available from macro
 *  - Duncan's delta-phi binning change
 *
 * Revision 1.7  2006/02/22 22:05:13  prindle
 * Removed all references to multRef (?)
 * Added cut mode 5 for particle identified correlations.
 * Other cut modes should be same as before
 *
 * Revision 1.6  2005/09/14 17:14:20  msd
 * Large update, added new pair-cut system, added pair density plots for new analysis mode (4), added event mixing cuts (rewrote buffer for this)
 *
 * Revision 1.5  2005/09/07 20:21:15  prindle
 *
 *   2ptCorrelations: Rearranged array/histogram initialization/destruction.
 *                    Now histograms are only allocated at end of job,
 *                    just before they are filled then written.
 *
 * Revision 1.4  2005/03/03 01:30:43  porter
 * updated StEStruct2ptCorrelations to include pt-correlations and removed
 * old version of pt-correlations from chunhuih (StEStruct2ptPtNbar)
 *
 * Revision 1.3  2004/09/16 23:37:25  chunhuih
 *
 * changed a number of methods to be virtual, so that its behavior can
 * be dynamically changed.
 *
 * Revision 1.2  2004/06/25 03:11:49  porter
 * New cut-binning implementation and modified pair-cuts for chunhui to review
 *
 * Revision 1.1  2003/10/15 18:20:46  porter
 * initial check in of Estruct Analysis maker codes.
 *
 *
 *********************************************************************/





