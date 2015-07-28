/// \File StiKNNSeedFinder.h
/// \author Victor Perev
/// It Is Adaptor for StvKNNSeedFinder
#ifndef StiKNNSeedFinder_HH
#define StiKNNSeedFinder_HH
#include "Stiostream.h"
#include <vector>
#include "Sti/StiTrackFinder.h"
class myStvKNSeedFinder;
class StiKNNSeedFinder : public StiTrackFinder
{
public:
  StiKNNSeedFinder();
  virtual ~StiKNNSeedFinder();
  StiTrack* findTrack(double rMin=0);
  virtual void reset();
//		empty methods
  virtual void initialize(){}
  virtual void findTracks(){}
  virtual bool find(StiTrack*, int, double){return 0;}
  virtual void clear();
  virtual Filter<StiTrack>* getTrackFilter(){return 0;}

protected:
vector<StiHit*>        _seedHits;
double fRxyMin;
myStvKNSeedFinder *fStvKNSeedFinder;
#if 0
ClassDef(StiKNNSeedFinder,0)
#endif
};


#endif

