// $Id: St_QATestTables_Maker.cxx,v 1.4 1999/09/29 16:46:26 kathy Exp $
// $Log: St_QATestTables_Maker.cxx,v $
// Revision 1.4  1999/09/29 16:46:26  kathy
// changed code so it would compile in .dev due to changes in DST tables - I even used cons instead of makel - wow! - I just changed variables or commented out some histograms that use now-non-existant variables so it would compile - later I will go through and redefine histograms as needed
//
// Revision 1.3  1999/09/23 16:04:31  kathy
// change paths for include files to standard way according to Yuri's request
//
// Revision 1.2  1999/09/02 21:47:22  kathy
// changed code so that it uses TMath functions so will compile on HP
//
// Revision 1.1  1999/08/13 17:16:29  kathy
// add new maker St_QATestTables_Maker in directory St_QA_Maker, written by Aya Ishihara, calculates and prints info about tables - used for QA
//

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// St_QATestTables_Maker class for Makers 
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include "TStyle.h"
#include "TCanvas.h"
#include "TObjString.h"
#include "PhysicalConstants.h"
#include <math.h>
#include "TMath.h"
#include "St_QATestTables_Maker.h"

#include "tables/St_dst_track_Table.h" 

#include "StChain.h"
#include "St_DataSetIter.h"


ClassImp(St_QATestTables_Maker)
     
     
//_____________________________________________________________________________



St_QATestTables_Maker::St_QATestTables_Maker(const char *name, const char *title):StMaker(name,title)
{ 
  
  
   cout << "\n" << " The constructor St_QATestTables_Maker's been called!" << "\n";

   number_events   = 0;

// For globtrk
   number_trk_glob      = 0;
   number_events   = 0;
   number_good_trk_glob = 0;
   nover_Pt_glob = 0;       
  
   sum_Pt_glob    = 0;
   sum_sq_Pt_glob = 0;
   mean_Pt_glob   = 0;
   rms_Pt_glob    = 0;

// For primtrk
   number_trk_prim      = 0;
   number_events_prim   = 0;
   number_good_trk_prim = 0;
   nover_Pt_prim = 0;       
  
   sum_Pt_prim    = 0;
   sum_sq_Pt_prim = 0;
   mean_Pt_prim   = 0;
   rms_Pt_prim    = 0;
 
} 





//_____________________________________________________________________________

St_QATestTables_Maker::~St_QATestTables_Maker(){


}

//_____________________________________________________________________________

Int_t St_QATestTables_Maker::Finish() {

  cout << " *** St_QATestTables_Maker::Finish *** "<<  endl;
 
  TestTablesFinish();

  return StMaker::Finish();
}

//_____________________________________________________________________________

Int_t St_QATestTables_Maker::Init(){

  cout << "\n*** St_QATestTables_Maker::Init *** "<<  endl;
 
  TestTablesInit();

 
  return StMaker::Init();

}
//_____________________________________________________________________________

Int_t St_QATestTables_Maker::Make(){
   St_DataSet *dst = GetDataSet("dst");
   cout << "*** St_QATestTables_Maker::Make *** "<<  endl;

   TestTables_testGlobtrk(dst);
   TestTables_testPrimtrk(dst);
      
   return kStOK;
}
//_____________________________________________________________________________

void St_QATestTables_Maker::TestTablesInit(){

  // What I want to do in Init():
  // open output files
  // set constants
  
}
//-----------------------------------------------------------------

void St_QATestTables_Maker::TestTables_testGlobtrk(St_DataSet *dst){
 
    number_events++;
   
    St_DataSetIter dstI(dst);
   
    St_dst_track *globtrk = (St_dst_track *) dstI["globtrk"];
 
  if (globtrk) {
 
      dst_track_st  *t   = globtrk->GetTable();
        number_trk_glob  = number_trk_glob + globtrk->GetNRows();
     
     for (Int_t i = 0; i < globtrk->GetNRows(); i++,t++)
       {
        
      if (t->iflag>0) {
         number_good_trk_glob++;
            
	 Float_t pT = -999.;
	 pT = 1./TMath::Abs(t->invpt);
 
         if (pT==-999.){ cout << " strange pT in globtrk " << endl; }
	 if (pT >= 0 && pT <= 5)
         {
         sum_Pt_glob = sum_Pt_glob + pT;
         Float_t sq_Pt = TMath::Power(pT , 2.);
         sum_sq_Pt_glob = sq_Pt + sum_sq_Pt_glob;
         }
         else
	   { nover_Pt_glob++; }
        }
       }
      }
    cout << " *** globtrk *** " << endl;
    cout << " sum of Pt      " << sum_Pt_glob      <<  endl;
    cout << " sum of Pt**2   " << sum_sq_Pt_glob   << endl;
    cout << " # of rows      " <<  globtrk->GetNRows() << endl;
    cout << " total # of trk " << number_trk_glob <<  endl;
    cout << " # of good trk  " << number_good_trk_glob << "\n\n\n" << endl;
}

//---------------------------------------------------------------------------------------
 
//-----------------------------------------------------------------

void St_QATestTables_Maker::TestTables_testPrimtrk(St_DataSet *dst){

 St_DataSetIter dstI(dst);

 St_dst_track *primtrk = (St_dst_track *) dstI["primtrk"];
  if (primtrk) {
    dst_track_st  *t   = primtrk->GetTable();
       number_trk_prim = number_trk_prim + primtrk->GetNRows();


    for (Int_t i = 0; i < primtrk->GetNRows(); i++,t++)
      {

      if (t->iflag>0) {
        number_good_trk_prim++;
	Float_t pT = -999.;
	pT = 1./TMath::Abs(t->invpt);

        if (pT==-999.){ cout << " strange pT in primtrk " << endl;} 
        if(pT >= 0 && pT <= 5) 
	  {
            sum_Pt_prim = sum_Pt_prim + pT;
            Float_t sq_Pt = TMath::Power(pT , 2.);
            sum_sq_Pt_prim = sq_Pt + sum_sq_Pt_prim;
          }
        else
	   { nover_Pt_prim++; }
      }
     }
    }
    cout << " ** primtrk*** " << endl;
    cout << " sum of Pt      " << sum_Pt_prim      <<  endl;
    cout << " sum of Pt**2   " << sum_sq_Pt_prim   << endl;
    cout << " # of rows      " <<  primtrk->GetNRows() << endl;
    cout << " total # of trk " << number_trk_prim <<  endl;
    cout << " # of good trk  " << number_good_trk_prim << "\n\n\n" << endl;
       
  
}
//_____________________________________________________________________________


void St_QATestTables_Maker::TestTablesFinish(){


    mean_Pt_glob = sum_Pt_glob/number_good_trk_glob;
    Float_t mean_sq_Pt_glob = sum_sq_Pt_glob/number_good_trk_glob;
    rms_Pt_glob = TMath::Sqrt(mean_sq_Pt_glob);

    cout << "\n" << " ************* Globtrk *************** " << endl;
    cout << " *                                   * " << endl;                     
    cout << " * total # of trk : " << number_trk_glob       << "            *" << endl;
    cout << " * # of good trk  : "  << number_good_trk_glob << "            *" << endl;
    cout << " * mean Pt        : " << mean_Pt_glob          << "         *" << endl;
    cout << " * sum of Pt      : " << sum_Pt_glob           << "            *" << endl;
    cout << " * RMS of Pt      : " << rms_Pt_glob           << "         *" << endl;
    cout << " * # of over range: " << nover_Pt_glob            << "              *" << endl;
    cout << " ************************************* " << endl;


    mean_Pt_prim = sum_Pt_prim/number_good_trk_prim;
    Float_t mean_sq_Pt_prim = sum_sq_Pt_prim/number_good_trk_prim;
    rms_Pt_prim = TMath::Sqrt(mean_sq_Pt_prim);

    cout << "\n" << " ************* primtrk *************** " << endl;
    cout << " *                                   * " << endl;                     
    cout << " * total # of trk : " << number_trk_prim       << "            *"      << endl;
    cout << " * # of good trk  : "  << number_good_trk_prim << "            *"      << endl;
    cout << " * mean Pt        : " << mean_Pt_prim          << "         *"         << endl;
    cout << " * sum of Pt      : " << sum_Pt_prim           << "           *"      << endl;
    cout << " * RMS of Pt      : " << rms_Pt_prim           << "         *"         << endl;
    cout << " * # of over range: " << nover_Pt_prim            << "               *" << endl;
    cout << " ************************************* "                               << endl;


}  



//---------------------------------------------------------------------------------------
void St_QATestTables_Maker::PrintInfo(){
  printf("**************************************************************\n");
  printf("* $Id: St_QATestTables_Maker.cxx,v 1.4 1999/09/29 16:46:26 kathy Exp $\n");
  //  printf("* %s    *\n",m_VersionCVS);
  printf("**************************************************************\n");
  if (Debug()) StMaker::PrintInfo();

}



































































































































