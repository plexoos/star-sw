// $Id: StvHitLoader.cxx,v 1.12 2012/04/27 00:22:40 perev Exp $
/*!
\author V Perev 2010  

A StvHitLoader loads  Stv hits.
<br>
Main tasks:				
<ul>
<li> Loop over StHits;			
<li> Make & fill according StvHits;				
<li> Fill StTGeoHelpr containers by StvHits;		
</ul>
 

*/
#include <Stiostream.h>
#include <math.h>
#include <string>
#include "TGeoManager.h"
#include "StvHitLoader.h"
#include "Stv/StvHit.h"
#include "Stv/StvToolkit.h"
#include "StarVMC/GeoTestMaker/StTGeoHelper.h"
#include "StEvent.h"
#include "StHit.h"
#include "StEventUtilities/StEventHelper.h"
#include "StEventUtilities/StEventHitIter.h"
#include "Stv/StvDraw.h"
#include "Stv/StvStl.h"

ClassImp(StvHitLoader)
//_____________________________________________________________________________
StvHitLoader::StvHitLoader(const char *name) : TNamed(name,"")

{
  mHitIter = new StEventHitIter();
}

//_____________________________________________________________________________
StvHitLoader::~StvHitLoader() 
{
  delete mHitIter;
}

//_____________________________________________________________________________
void StvHitLoader::Clear(const char*)
{
  StTGeoHelper::Inst()->ClearHits();
}

//_____________________________________________________________________________
Int_t StvHitLoader::Finish()
{
  return 0;
}

//_____________________________________________________________________________
Int_t StvHitLoader::Init()
{
   int nDet=0;
   for (int id=1; id<kMaxDetectorId; id++){
     if (!StTGeoHelper::Inst()->IsActive((StDetectorId)id)) continue;
     mHitIter->AddDetector((StDetectorId)id);
     nDet++;
   }
   return nDet;
}

//_____________________________________________________________________________
int StvHitLoader::LoadHits(const StEvent *stev)
{
enum {kFCF_CHOPPED=256		// 0x100 cluster is chopped from its neighbour: OFFLINE use only
     ,kFCF_SANITY =512};	// 0x200 cluster extents not sane
static int nCall=0; nCall++;
static int myGraph=0;

StvDraw *myDraw=0;
StvHits *myHits=0;
if (myGraph) { //create canvas
  myDraw = new StvDraw();
  myHits = new StvHits;
}

  mHitIter->Reset(stev);
  const StHit *stHit=0;
  StDetectorId didOld = kUnknownId;
  int nTotHits = 0,nTotHitz=0, nHits=0,nHitz=0;

  for (; ; ++(*mHitIter)) {
    stHit=*(*mHitIter);
    StDetectorId did = mHitIter->DetectorId();
    
    if (did != didOld || !stHit) {
      if (didOld) {
        Info("LoadHits","Loaded  %d good and failed %d %s hits"
	    ,nHits,nHitz,StTGeoHelper::DetName(didOld));
      }
      if (!stHit) break;
      Info("LoadHits","Start %s hits",StTGeoHelper::DetName(did));
      nHits=0; nHitz=0;
    }
    if (stHit->flag() & kFCF_CHOPPED || stHit->flag() & kFCF_SANITY) continue; // ignore hits marked by AfterBurner as chopped o
    StvHit *stiHit = MakeStvHit(stHit,mHitIter->UPath());

if (myHits) (*myHits)+= stiHit;   

    if (stiHit) {nHits++;nTotHits++;}  
    else 	{nHitz++;nTotHitz++;
      if (did == kTpcId) TpcHitTest(stHit);} 
    didOld = did; 
  }
  int nIniHits = StTGeoHelper::Inst()->InitHits();
  assert(nTotHits==nIniHits);
  Info("LoadHits","Loaded %d good and failed %d of all hits",nTotHits,nTotHitz);
if (myDraw) {myDraw->Hits(*myHits,kUnusedHit); myDraw->Wait();}
  return nTotHits;
}
//_____________________________________________________________________________
StvHit *StvHitLoader::MakeStvHit(const StHit *stHit,UInt_t upath)
{
   StvHit *stiHit = StvToolkit::Inst()->GetHit();
   StDetectorId did = stHit->detector();
   UInt_t hard = stHit->hardwarePosition();
   if (!hard) hard = upath;
   StThreeVectorF v3f = stHit->position();
   const float *xyz = v3f.xyz();
   int seed = 1;
   if (did == kTpcId) {
//   enum {zPrompt = 205,rMiddle=124};
     enum {zPrompt = 205,rMiddle=0};
     hard <<=1; hard |= (fabs(xyz[2]) > zPrompt);
     if (xyz[0]*xyz[0]+xyz[1]*xyz[1] >rMiddle*rMiddle) seed = 1;
   }
   hard *= (uint)kMaxDetectorId; hard+=(uint)did;
   
   const StHitPlane *hp = StTGeoHelper::Inst()->AddHit(stiHit,xyz,hard,seed);
   if (!hp) { StvToolkit::Inst()->FreeHit(stiHit);return 0;}
   
   stiHit->set(hp,stHit,xyz);
   return stiHit;
}
//_____________________________________________________________________________
int StvHitLoader::TpcHitTest(const StHit *stHit)
{
  enum {nbpads = 73,maxpads=100};
  int tpads[maxpads]   =   {  1, 1, 1, 2, 2, 2, 3, 3, 3, 4,
			      4, 4, 5, 5, 5, 6, 6, 6, 7, 7,
			      7, 8, 8, 8, 9, 9, 9,10,10,10,
			     11,11,11,12,12,12,13,13,13,14,
			     14,15,16,17,18,19,20,21,22,23,
			     24,25,26,27,28,29,30,31,32,33,
			     34,35,36,37,38,39,40,41,42,43,
			     44,45,45};                    

  int isdets[maxpads]   =  { 1, 0, 2, 1, 0, 2, 1, 0, 2, 1,
			     0, 2, 1, 0, 2, 1, 0, 2, 1, 0,
			     2, 1, 0, 2, 1, 0, 2, 1, 0, 2,
			     1, 0, 2, 1, 0, 2, 1, 0, 2, 1,
			     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			     0, 0, 2};                    


  StThreeVectorF v3f = stHit->position();
  TGeoNode *node = gGeoManager->FindNode(v3f[0],v3f[1],v3f[2]);
  assert(node);
  if (strncmp("TPA",node->GetName(),3)) return -1;
  int numbv[3];
  for (int i=0;i<3;i++) {
    node = gGeoManager->GetMother(i);
    numbv[2-i] = node->GetNumber();
  }
//int tpgv  = numbv[0];
//int tpss  = numbv[1];
//int sector= tpss+12*(tpgv-1) ;
  int tpad  = numbv[2];
  int isdet = 0;

  if (tpad > nbpads) tpad -= nbpads;
  isdet = isdets[tpad-1];
  tpad  = tpads [tpad-1];
  if (isdet) {
    Warning("TpcHitTest","TpcHit(%g,%g,%g) isdet=%d WRONG WRONG WRONG"
           ,v3f[0],v3f[1],v3f[2], isdet);
  }
  return isdet;
}
    
    
    
  
  
