#include <Stiostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Stv/StvHit.h"
#include "Stv/StvNode.h"
#include "Stv/StvTrack.h"
#include "Stv/StvDraw.h"
#include "Stv/StvToolkit.h"
#include "StarVMC/GeoTestMaker/StTGeoHelper.h"
int StvTrack::mgId=0;

//______________________________________________________________________________
StvTrack::StvTrack()
{
  mId = -1; 		
}

//______________________________________________________________________________
StvTrack::~StvTrack()
{
  unset(); 		
}
//______________________________________________________________________________
void StvTrack::reset()
{
  mId = ++mgId;
  mPrimary=0;
  clear();
}
//______________________________________________________________________________
void StvTrack::unset()
{
  mId = -1;
  StvToolkit *kit = StvToolkit::Inst();
  for (StvNodeIter it = begin();it != end();++it) 
  {
    StvNode *node = *it;kit->FreeNode(node);
  }
  clear();
}
//______________________________________________________________________________
int StvTrack::GetNPoss(StDetectorId detectorId) const
{
  int n=0;
  for (StvNodeConstIter it = begin(); it !=end();++it) {
    const StvNode *node = *it; 
    const StHitPlane *hp = node->GetHitPlane();
    if (!hp) continue;
    if (detectorId && hp->GetDetId()!=detectorId) continue;
    n++;
  }
  return n;
}   
//______________________________________________________________________________
int StvTrack::GetNHits(StDetectorId detectorId) const
{
  int n=0;
  for (StvNodeConstIter it = begin(); it !=end();++it) {
    const StvNode *node = *it; 
    const StHitPlane *hp = node->GetHitPlane();
    if (!hp) continue;
    if (detectorId && hp->GetDetId()!=detectorId) continue;
    if (!node->GetHit()) continue;
    if (!node->GetXi2()>1000) continue;
    n++;    
  }
  return n;
}   
//______________________________________________________________________________
StvNode *StvTrack::GetNode(EPointType noTy)
{
  StvNode *node=0;int n=0;
  if (noTy!=kLastPoint) {
    for (StvNodeIter it = begin();it != end();++it) {
      node = *it; n++;
      switch(noTy) {
        case kDcaPoint: if (n>2) return 0;
	  if (node->GetType()==StvNode::kDcaNode) {return node;} 
          break;

        case kPrimPoint: 
	  if (node->GetType()==StvNode::kPrimNode) {return node;} 
          return 0;

        case kFirstPoint:
	  if (node->GetType()!=StvNode::kRegNode) 	break;
	  if (!node->GetHit()) 		 		break;
	  if (!node->GetXi2()>1000) 		 	break;
          return node;
        default: assert("Wrong Node type" && 0);
       }//end switch
     }//end loop
   } else {
    for (StvBakwNodeIter it = rbegin();it != rend();++it) {
      node = *it;
      if (node->GetType()!=StvNode::kRegNode) 	continue;
      if (!node->GetHit()) 		 	continue;
      if (!node->GetXi2()>1000) 		continue;
      return node;
    }
  }// end if
  return 0;

}
//______________________________________________________________________________
const StvNode *StvTrack::GetNode(EPointType noTy) const
{ return ((StvTrack*)this)->GetNode(noTy); }
//______________________________________________________________________________
int StvTrack::CountHits(StvHitCount &cnt) const
{
  cnt.Clear();
  for (StvNodeConstIter it = begin();it != end();++it) 
  {
    StvNode *node = *it;
    if (node->GetType()!= StvNode::kRegNode) 	continue;
    if (!node->GetHitPlane()) 			continue;
    if (node->GetXi2()<1000) {cnt.AddHit();} else {cnt.AddNit();}
  }
  return cnt.nGoodHits;
}
//_____________________________________________________________________________
double StvTrack::GetLength(EPointType ept) const
{
  const StvNodePars *pre=0;
  double len = 0;
  for (StvNodeConstIter it = begin();it != end();++it) 
  {
    StvNode *node = *it;
    if (!pre) {
      StvNode::ENodeType ty = node->GetType();
      if (ty == StvNode::kPrimNode) {
        if (ept!=kPrimPoint) 		continue;
        pre = &(node->GetFP(2)); 	continue;
      }
      if (ty == StvNode::kDcaNode) {
        if (ept!=kDcaPoint) 		continue;
        pre = &(node->GetFP(2)); 	continue;
      }
      if (ty != StvNode::kRegNode) 	continue;
      if (node->GetXi2()>1000) 		continue;
        pre = &(node->GetFP(2)); 	continue;
    }
    const double *x1 = &pre->_x;
    const double *x2 = &node->GetFP(2)._x;
    double dlen = sqrt(pow(x1[0]-x2[0],2) + pow(x1[1]-x2[1],2));
    double curv = 0.5*fabs(pre->_curv+node->GetFP(2)._curv);
    double dsin = (0.5*dlen*curv);
    if (dsin>0.9) dsin=0.9;
    dlen = (dsin<0.1)? dlen*(1.+dsin*dsin/6) : 2*asin(dsin)/curv; 
    len +=sqrt(dlen*dlen + pow(x1[2]-x2[2],2));
    pre = &(node->GetFP(2)); 	
  }
  return len;

}
//_____________________________________________________________________________
double StvTrack::GetXi2() const
{
  double Xi2 = 0;
  int nd=0;
  for (StvNodeConstIter it = begin();it != end();++it) 
  {
    const StvNode *node = *it;
    if (!node->GetHit()) continue;
    if (node->GetXi2()>1000) continue;
    nd++; Xi2+=node->GetXi2();
  }
  nd = nd*2-5;
 
  return (nd>0)? Xi2/nd:0;

}
//_____________________________________________________________________________
int StvTrack::ReleaseHits()
{
  int nd=0;
  for (StvNodeIter it = begin();it != end();++it) 
  {
    StvNode *node = *it;
    StvHit *hit = node->GetHit();
    if (!hit) 			continue;
    if (!node->GetDetId())	continue;
    hit->setTimesUsed(0);
    node->SetHit(0);
    nd++;
  }
  return nd;
}
//_____________________________________________________________________________
void StvTrack::Show() const
{
StvHits   showHits;
StvPoints showTrak;
  for (StvNodeConstIter it = begin();it != end();++it) 
  {
    const StvNode *node = *it;
    const StvHit  *hit  = node->GetHit();
    showTrak += node->GetFP().P;
    if (hit) showHits+=(StvHit*)hit;
  }
  StvDraw::Inst()->Trak(showTrak,kGlobalTrack);
  StvDraw::Inst()->Hits(showHits,kUsedHit    );
}
//_____________________________________________________________________________
//_____________________________________________________________________________
//_____________________________________________________________________________
void StvHitCount::AddHit()
{
    nPossHits++;  nTotHits++;nContHits++;
    if (nContNits) {
      if (nContNits>kContNits) nSeqLong++;
      nContNits=0;nSeqNits++;
    }
}

//_____________________________________________________________________________
void StvHitCount::AddNit()
{
    nPossHits++;nContNits++;
    if (nContHits) {
      if (nContHits<kContHits) {nSeqShort++;} else { nGoodHits+=nContHits;}
      nContHits=0;nSeqHits++;
    }
}
//_____________________________________________________________________________
int StvHitCount::Reject()
{
    if (nContNits) {
      if (nContNits>kContNits) nSeqLong++;
      nContNits=0;nSeqNits++;
    }
    if (nContHits) {
      if (nContHits<kContHits) {nSeqShort++;} else { nGoodHits+=nContHits;}
      nContHits=0;nSeqHits++;
    }
  return nGoodHits<kGoodHits && nTotHits<kTotHits;
}
//_____________________________________________________________________________
int StvHitCount::Skip() const
{
  return (nContNits>kContNits) ;
}
//_____________________________________________________________________________
void StvTrack::Print(const char *opt) const
{
  if (!opt) opt = "";
  printf("Track %p\n",(void*)this);

  int n=0;
  for (StvNodeConstIter it=begin();it!=end();++it) {
    const StvNode *node = (*it);
    const StvHit *hit = node->GetHit();
    if (!hit && strchr(opt,'H')) continue;
    n++;printf("%3d - ",n);
    node->Print(opt);
  }
}
