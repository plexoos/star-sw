#ifndef StEbyeScaTagsMaker_HH
#define StEbyeScaTagsMaker_HH

///////////////////////////////////////////////////////////////////////////////
//
// StEbyeScaTagsMaker
//
// Description: 
//  Sample maker to access and analyze StEvent
//
// Environment:
//  Software developed for the STAR Detector at Brookhaven National Laboratory
//
// Author List: 
//  Jeff Reid, UW, 2/99
//
// History:
//
///////////////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include "StMaker.h"
#include "tables/ScaTag.h"

class StEvent;
class StRun;

class StEbyeScaTagsMaker : public StMaker {

private:
  Bool_t drawinit;
  Char_t collectionName[256];

  // Maker generates a tag
  ScaTag_st* theTag; //!

protected:
  void fillTag(StEvent& event, ScaTag_st& scaTag);    // does the actual work;
  float mtInverseSlope(double *mthisto, int ibegin, int istop); 

public:

  StEbyeScaTagsMaker(const Char_t *name="phi", const Char_t *title="phi");
  virtual ~StEbyeScaTagsMaker();
  virtual void Clear(Option_t *option="");
  virtual Int_t Init();
  virtual Int_t  Make();
  virtual void   PrintInfo();
  virtual Int_t  Finish();

  // Tag accessor
  ScaTag_st* tag();

  // output Tag info to screen
  void printTag(ostream& = cout);

  ClassDef(StEbyeScaTagsMaker, 1)
};

#endif
