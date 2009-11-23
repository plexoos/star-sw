// $Id: Wevent2009.h,v 1.1 2009/11/23 23:00:18 balewski Exp $
//
//*-- Author : Jan Balewski, MIT

//----------------------------
//------- W-reco event container
//----------------------------
#ifndef W_EVENT_2009_HH
#define W_EVENT_2009_HH

#include <TVector3.h>
//get L2
#include <StTriggerUtilities/L2Emulator/L2wAlgo/L2wResult2009.h>

#include <StMuDSTMaker/COMMON/StMuTrack.h>
#include "WanaConst.h"

//---------------
class WeveCluster { // info BTOW cluster
 public:
  float energy,ET, adcSum;
  int nTower; // with non-zero ADC>kSigPed
  int iEta, iPhi; // lower-left corner of the cluster, can be negative, L2-indexing convention
  TVector3 position; // 3D ln(E) weighted sume of tower positions
  WeveCluster() {clear();}

  void clear() {  position=TVector3(0,0,0);
  ET=energy=adcSum=0; nTower=0; iEta=iPhi=999;}
  void print( int flag=0){
    printf("     Cluster ET=%.1f E=%.1f GeV, sumAdc=%.0f nTw=%d iEta=%d, iPhi=%d XYZ(%.0f,%.0f,%.0f)cm\n",ET,energy,adcSum,nTower,iEta, iPhi,position.x(),position.y(),position.z());}
};
 


//---------------
class WevePointTower { // tower pointed by the track
 public:
  TVector3 R; // extrapolated position at tower entrance
  int id;
  int iEta, iPhi; // eta x phi bin using L2 indexing convention
  void clear() {    id=0; R=TVector3(0,0,0); iEta=iPhi=9999;}
  void print( int flag=0){
    printf("     pointed tower ID=%d; L2index: iEta=%d iPhi=%d; XYZ=(%.0f,%.0f,%.0f)cm\n",
	   id,iEta, iPhi, R.x(), R.y(), R.z());
  }
};

//---------------
class WeveEleTrack { // electron track info
 public:
  int isMatch2Cl; // result of cuts
  WevePointTower pointTower;
  const StMuTrack *glMuTrack, *prMuTrack;
  WeveCluster cluster,cl4x4;
  TVector3 primP; // primary momentum vector
  float nearTpcPT, nearEmcET, nearBtowET, nearEtowET; // (GeV/c), around prim track direction
  float awayTpcPT, awayEmcET, awayBtowET, awayEtowET; // (GeV/c), opposite in phi to  prim track direction
  float nearTotET, awayTotET; // (GeV), for nearCone 10 GeV is subtracted to avoid double counting
  int awayNTr,nearNTr;  // # tracks on away side  
  int awayNTow,nearNTow; // # towers on away aide  
  
  TVector3 maxAwayBtow,maxNearBtow;  // highest ET tower momentum vector  
  TVector3 maxAwayEtow,maxNearEtow;  // highest ET tower momentum vector 
  TVector3 maxAwayTr,maxNearTr;  // track vector for highest PT track on away side 
  float awayTotCone;    
  TVector3 awayConeAxis; // max of all away high ET vectors

  TVector3 hadronicRecoil; 

  WeveEleTrack() {clear();}
  
  void clear() {  pointTower.clear();
    cluster.clear();cl4x4.clear();  isMatch2Cl=false;  primP=TVector3(0,0,0);
    prMuTrack=glMuTrack=0; 
    awayTpcPT=nearTpcPT=nearTotET=awayTotET=nearEmcET=awayEmcET=nearBtowET=awayBtowET=nearEtowET=awayEtowET=0; awayNTr=awayNTow=nearNTr=nearNTow=0; 

    maxAwayTr=TVector3(0,0,0); maxNearTr=TVector3(0,0,0); maxAwayBtow=TVector3(0,0,0); maxNearBtow=TVector3(0,0,0); maxAwayEtow=TVector3(0,0,0); maxNearEtow=TVector3(0,0,0); awayConeAxis=TVector3(0,0,0); awayTotCone=0;
    hadronicRecoil=TVector3(0,0,0); } 
  
  void print( int flag=0){
    if(glMuTrack==0) {  printf("   Track NULL pointer???\n"); return;}
    printf("   Track glPT=%.1f GeV/c   isMatch2Cl=%d, nearTotET=%.1f, awayTotET=%.1f primPT=%.1f\n",
	   glMuTrack->pt(),isMatch2Cl,nearTotET, awayTotET,primP.Pt());
    pointTower.print(flag);
    cluster.print(flag);   
    TVector3 D=pointTower.R-cluster.position;
    printf("                XYZ(track-cluster):  |3D dist|=%.1fcm  delZ=%.1fcm\n",D.Mag(),D.z());
    printf("     4x4 :"); cl4x4.print(flag);    
    printf("     nearET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n",nearTpcPT,nearEmcET,nearBtowET,nearEtowET,nearTotET);
    printf("     awayET/GeV:    TPC=%.1f   Emc=%.1f (BTOW=%.1f ETOW=%.1f) sum=%.1f\n",awayTpcPT,awayEmcET,awayBtowET,awayEtowET,awayTotET);
    printf("     nearTr=%d nearTow=%d   awayTr=%d awayTow=%d \n",nearNTr,nearNTow,awayNTr,awayNTow);   
  }
};

//---------------
class WeveVertex { // info about vertex
 public:
  int id; // as store do muDst list
  float z; // cm
  float funnyRank;
  vector <WeveEleTrack> eleTrack;

  void clear() {
    id=-999; z=-999; funnyRank=-9999;
    eleTrack.clear();
  }
  
  void print( int flag=0){
    printf(" Vertex ID=%d Z=%.1f cm  nTrack=%d\n",id,z, eleTrack.size());
    for(uint i=0;i< eleTrack.size();i++) 
      eleTrack[i].print();
  }
  
};

//---------------
class WeveBEMC { // info about BEMC
 public:
  //raw BTOW/BPRS hits
  int   tileIn[mxBTile]; // 0 if no data
  float adcTile[mxBTile][mxBtow];
  float eneTile[mxBTile][mxBtow];
  int   statTile[mxBTile][mxBtow];

  //raw BSMD hits, both planes
  float adcBsmd[mxBSmd][mxBStrips];
  int  statBsmd[mxBSmd][mxBStrips];

  void clear() {
    memset(adcTile,0,sizeof(adcTile));
    memset(eneTile,0,sizeof(eneTile));
    memset(statTile,-1,sizeof(statTile));  // default all dead
    memset(tileIn,0,sizeof(tileIn)); // detector was On/Off
    memset(adcBsmd,0,sizeof(adcBsmd));
    memset(statBsmd,-1,sizeof(statBsmd));// default all dead
    //     memset(,0,sizeof());
  }

  void print( int flag=0){
    printf(" BTOW tower ADC>500 list: ");
    for(int i=0;i< mxBtow;i++) {
      if(adcTile[kBTow][i]<500) continue;
      int id=i+1;
      printf("id=%d adc=%.1f ene=%.1f;  ",id,adcTile[kBTow][i],eneTile[kBTow][i]);
    }    printf("\n");
    
    printf(" BSMDE tower ADC>200 list: ");
    for(int i=0;i< mxBStrips;i++) {
      if(adcBsmd[ kBSE][i]<200) continue;
      int id=i+1;
      int module= 1+i/150;
      printf("id=%d mod=%d adc=%.1f ;  ",id,module,adcBsmd[ kBSE][i]);
    }    printf("\n");
    
    if(flag&1) {//..................
      for(int i=0;i<120;i++) {
        int id=7+i*40;
        if(i%10==0) printf("\n  softID=%4d adc format  BTOW:BPRS=  ",id);
        printf("%.0f : %.0f, ", adcTile[kBTow][id-1], adcTile[kBPrs][id-1]);
      }
      printf("\n");
    }
  }// end of print
  
};

//--------------
class WeveETOW { // info about ETOW           
 public:
  
  //raw ETOW hit
  int etowIn;
  float adc[mxEtowSec*mxEtowSub][mxEtowEta]; //[phibin][etabin]
  float ene[mxEtowSec*mxEtowSub][mxEtowEta];
  int stat[mxEtowSec*mxEtowSub][mxEtowEta];
  
  void clear() {
    memset(adc,0,sizeof(adc));
    memset(ene,0,sizeof(ene));
    memset(stat,-1,sizeof(stat));  // default all dead
  }
  
};

//---------------
class Wevent2009 {
 public:
  // .....variables ....
  L2wResult2009 *l2algo;
  int l2bitET,l2bitRnd;
  int id; // eventID
  int bx7, bx48; // raw from muDst
  vector < WeveVertex> vertex;
  WeveBEMC bemc;
  WeveETOW etow;  
  // .... methods ....
  Wevent2009() { clear();}

  void clear() { 
    //printf("W2009event:clear()\n");
    id=0;
    l2bitET=l2bitRnd=0;
    bx7=bx48=-1;
    vertex.clear();
    bemc.clear();
    etow.clear(); 
  }
  
  //...........................
  void print( int flag=0, int isMC=0) {
    printf("\nmy W2009event ID=%d  L2Wbits: ET=%d rnd=%d;  bx7=%d bx48=%d nVert=%d\n",id,l2bitET,l2bitRnd,bx7,bx48, vertex.size());
    if(!isMC)L2wResult2009_print(l2algo);
    for(uint i=0;i< vertex.size();i++) vertex[i].print(flag);
    bemc.print(flag);
    
  }// end of PRINT
  
  
};

#endif


// $Log: Wevent2009.h,v $
// Revision 1.1  2009/11/23 23:00:18  balewski
// code moved spin-pool
//
