////////////////////////////////////////////////////////////////////////
//
//   $Id: StFtpcGasUtilities.cc,v 1.19 2006/07/19 11:59:39 jcs Exp $
//
//   StFtpcGasUtilities
//
//   Author:  Janet Seyboth     10/30/2003
//
//   Function: adjust pressure and ftpc gas temperature event by event
//
////////////////////////////////////////////////////////////////////////
//
//   $Log: StFtpcGasUtilities.cc,v $
//   Revision 1.19  2006/07/19 11:59:39  jcs
//   additional body temperatures now available starting with runNumber 5009073
//
//   Revision 1.18  2006/06/23 07:56:55  jcs
//   Starting in Y2006 use only the FTPC Extra Temperature readings plus an adjustment, if necessary, to determine the average gas temperature
//
//   Revision 1.17  2005/03/30 11:40:35  jcs
//   Calculate the average temperature correctly for year 2005+ runs when only one set of temperature readings (body or extra) is available
//
//   Revision 1.16  2005/03/23 14:32:28  jcs
//   additional changes for using body + extra temperatures starting with y2005
//
//   Revision 1.15  2005/03/14 22:57:18  jcs
//   clean up code
//   use body + extra temperature readings starting with y2005
//
//   Revision 1.14  2005/01/17 15:58:05  jcs
//   use all 6 ftpc west body temperature readings for 2005 run
//
//   Revision 1.13  2004/09/17 08:55:40  jcs
//   add corrections for processing 2004 pp data
//
//   Revision 1.12  2004/09/15 10:27:59  jcs
//   correct error in averageTemperatureEast comment and output statement (body3,4,5 are used NOT 1,3,4)
//
//   Revision 1.11  2004/07/20 18:30:17  jcs
//   change temperature output information
//
//   Revision 1.10  2004/07/18 14:12:45  jcs
//   use adjustAverageWest/East from database
//   always output temperature calculation information since this is a critical value for the FTPC
//
//   Revision 1.9  2004/05/26 10:32:32  jcs
//   For all runs on or after 2004-01-25, we use only three temperature readings per FTPC
//   + and adjustment to compute the averageBodyTemperature
//   (this change is provisional - the adjustment must be added to the database)
//
//   Revision 1.8  2004/05/07 12:07:06  jcs
//   remove double code in averageTemperatureEast
//
//   Revision 1.7  2004/04/14 16:48:21  jcs
//   turn off DEBUG
//
//   Revision 1.6  2004/04/14 14:55:08  jcs
//   temporarily turn on debug
//   don't use flaky body temperature readings
//
//   Revision 1.5  2004/03/11 22:26:26  jcs
//   activate reading of additional body temperatures
//
//   Revision 1.4  2004/03/09 20:58:00  jcs
//   undo activation of additional body temperatures - they are not getting
//   thru from offline database to here
//
//   Revision 1.3  2004/03/09 20:22:27  jcs
//   activate use of additional body temperatures for y2004
//
//   Revision 1.2  2004/02/02 17:29:40  jcs
//   dbDate test works for y2004, now only print message for DEBUG
//
//   Revision 1.1  2003/11/13 14:12:17  jcs
//   move pressure and gas corrections from StFtpcClusterMaker.cxx to StFtpcGasUtilities
//
//
////////////////////////////////////////////////////////////////////////

#include "StFtpcParamReader.hh"
#include "StFtpcDbReader.hh"

#include "StFtpcGasUtilities.hh"
#include "StMessMgr.h"

#ifndef DEBUG
#define DEBUG 0
#endif


StFtpcGasUtilities::StFtpcGasUtilities(StFtpcParamReader   *paramReader,
				       StFtpcDbReader      *dbReader,
				       StDetectorDbFTPCGas *gas,
                                       St_ftpcTemps        *temps)
{				 
  mParam = paramReader;
  mDb    = dbReader;
  mGas   = gas;
  mTemps = temps;
}  


StFtpcGasUtilities::~StFtpcGasUtilities() {
}

Int_t StFtpcGasUtilities::barometricPressure() {
   if (mGas->getBarometricPressure() >= mDb->minPressure() && mGas->getBarometricPressure() <= mDb->maxPressure()) {
      gMessMgr->Info() <<"Change normalizedNowPressure from "<<mParam->normalizedNowPressure()<<" to "<<mGas->getBarometricPressure()<<endm;
      mParam->setNormalizedNowPressure(mGas->getBarometricPressure());
      return kStOK;
   }
   else {
      gMessMgr->Info() << "Invalid value ("<<mGas->getBarometricPressure()<<") from offline database for barometric pressure - using previous value ("<<mParam->normalizedNowPressure()<<")"<<endm;
      return kStWarn;
   }
}   

 
Int_t StFtpcGasUtilities::averageTemperatureWest(Int_t dbDate, Int_t runNumber) {
	
   Int_t numberBodyTemperaturesWest = 0;
   Float_t averageBodyTemperatureWest = 0.0;
   Float_t averageGasTempWest = 0.0;


   // Starting Y2006, calculate FTPC West gas temperature from extra temperatures only 
   // and adjust with mDb->adjustAverageWest()
   if ( dbDate >= 20051030 ) {
     
     ftpcTemps_st* tempT = mTemps->GetTable();

     Int_t numberExtraTempsWest = 0;
     Float_t averageExtraTempsWest = 0.0;

     if (tempT->extra1West >= mDb->minGasTemperature() && tempT->extra1West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra1West;
        numberExtraTempsWest++;
        cout<<tempT->extra1West<<"(extra1West)";
     }
     //if (tempT->extra2West >= mDb->minGasTemperature() && tempT->extra2West <= mDb->maxGasTemperature() ) {
     //   averageExtraTempsWest += tempT->extra2West;
     //   numberExtraTempsWest++;
     // cout<<" + "<<tempT->extra2West<<"(extra2West)";
     //}
     if (tempT->extra3West >= mDb->minGasTemperature() && tempT->extra3West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra3West;
        numberExtraTempsWest++;
        cout<<" + "<<tempT->extra3West<<"(extra3West)";
     }
     if (tempT->extra4West >= mDb->minGasTemperature() && tempT->extra4West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra4West;
        numberExtraTempsWest++;
        cout<<" + "<<tempT->extra4West<<"(extra4West)";
     }
     if (tempT->extra5West >= mDb->minGasTemperature() && tempT->extra5West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra5West;
        numberExtraTempsWest++;
        cout<<" + "<<tempT->extra5West<<"(extra5West)";
     }
     if (tempT->extra6West >= mDb->minGasTemperature() && tempT->extra6West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra6West;
        numberExtraTempsWest++;
        cout<<" + "<<tempT->extra6West<<"(extra6West)";
     }

     // print out average extra temperature west
     if (numberExtraTempsWest > 0) {
        cout<<" = "<<averageExtraTempsWest<<endl;
        cout<<"averageExtraTempsWest = "<<averageExtraTempsWest<<"/"<<numberExtraTempsWest<<" = "<<averageExtraTempsWest/numberExtraTempsWest<<endl;
     }
    
     if ( averageExtraTempsWest != 0 ) {
        averageGasTempWest =  averageExtraTempsWest/numberExtraTempsWest + mDb->adjustAverageWest();
        cout<<"setGasTemperatureWest = averageGasTempWest = "<<averageExtraTempsWest<<"/"<<numberExtraTempsWest<<" + "<<mDb->adjustAverageWest()<<"  = "<<averageExtraTempsWest/numberExtraTempsWest + mDb->adjustAverageWest()<<endl;
        mParam->setGasTemperatureWest(averageGasTempWest);
        return kStOK;
     }
     else {
        cout<<"No FTPC West Extra temperatures found for run number "<<runNumber<<"dbDate "<<dbDate<<endl;
        if (mParam->gasTemperatureWest() != 0) cout<<"Using defaultTemperatureWest = "<<mParam->gasTemperatureWest()<<endl; 
        return kStWarn;
     }	  
   }

   // For Y2005 calculate FTPC West gas temperature from body temperatures 
   // plus extra temperature readings
   // (the extra temperature readings are available as of 2005-01-01)

   if ( dbDate >= 20050101 && dbDate < 20051030 ) {

     cout<<" dbDate = "<<dbDate<<"(>2005-01-01) Use FTPC West body temperatures + FTPC West extra temperature readings 1,3-6 (2 is unstable in the year 2005 run)"<<endl;

     if (mGas->getBody1West() >= mDb->minGasTemperature() && mGas->getBody1West() <= mDb->maxGasTemperature()) {      
         averageBodyTemperatureWest += mGas->getBody1West();
         numberBodyTemperaturesWest++;
         cout<<mGas->getBody1West()<<"(body1West)";
     }		 
     if (mGas->getBody2West() >= mDb->minGasTemperature() && mGas->getBody2West()<= mDb->maxGasTemperature() ) {
          averageBodyTemperatureWest += mGas->getBody2West();
          numberBodyTemperaturesWest++;
          cout<<" + "<<mGas->getBody2West()<<"(body2West)";
     }		 
     if (mGas->getBody3West() >= mDb->minGasTemperature() && mGas->getBody3West()<= mDb->maxGasTemperature() ) {
          averageBodyTemperatureWest += mGas->getBody3West();
          numberBodyTemperaturesWest++;
          cout<<" + "<<mGas->getBody3West()<<"(body3West)";
     }		 
     if (mGas->getBody4West() >= mDb->minGasTemperature() && mGas->getBody4West() <= mDb->maxGasTemperature() ) {
          averageBodyTemperatureWest += mGas->getBody4West();
          numberBodyTemperaturesWest++;
          cout<<" + "<<mGas->getBody4West()<<"(body4West)";
     }		 
     if (mGas->getBody5West() >= mDb->minGasTemperature() && mGas->getBody5West() <= mDb->maxGasTemperature() ) {
          averageBodyTemperatureWest += mGas->getBody5West();	 
          numberBodyTemperaturesWest++;
          cout<<" + "<<mGas->getBody5West()<<"(body5West)";
     }  
     if (mGas->getBody6West() >= mDb->minGasTemperature() && mGas->getBody6West() <= mDb->maxGasTemperature() ) {
          averageBodyTemperatureWest += mGas->getBody6West();
          numberBodyTemperaturesWest++;
          cout<<" + "<<mGas->getBody6West()<<"(body6West)";
     }  


     // print out average body temperature west
     if (numberBodyTemperaturesWest > 0) {
        cout<<" = "<<averageBodyTemperatureWest<<endl;
        cout<<"averageBodyTemperatureWest = "<<averageBodyTemperatureWest<<"/"<<numberBodyTemperaturesWest<<" = "<<averageBodyTemperatureWest/numberBodyTemperaturesWest<<endl;
     }
     
     ftpcTemps_st* tempT = mTemps->GetTable();

     Int_t numberExtraTempsWest = 0;
     Float_t averageExtraTempsWest = 0.0;

     if (tempT->extra1West >= mDb->minGasTemperature() && tempT->extra1West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra1West;
        numberExtraTempsWest++;
        cout<<tempT->extra1West<<"(extra1West)";
     }
     //if (tempT->extra2West >= mDb->minGasTemperature() && tempT->extra2West <= mDb->maxGasTemperature() ) {
     //   averageExtraTempsWest += tempT->extra2West;
     //   numberExtraTempsWest++;
     // cout<<" + "<<tempT->extra2West<<"(extra2West)";
     //}
     if (tempT->extra3West >= mDb->minGasTemperature() && tempT->extra3West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra3West;
        numberExtraTempsWest++;
        cout<<" + "<<tempT->extra3West<<"(extra3West)";
     }
     if (tempT->extra4West >= mDb->minGasTemperature() && tempT->extra4West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra4West;
        numberExtraTempsWest++;
        cout<<" + "<<tempT->extra4West<<"(extra4West)";
     }
     if (tempT->extra5West >= mDb->minGasTemperature() && tempT->extra5West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra5West;
        numberExtraTempsWest++;
        cout<<" + "<<tempT->extra5West<<"(extra5West)";
     }
     if (tempT->extra6West >= mDb->minGasTemperature() && tempT->extra6West <= mDb->maxGasTemperature() ) {
        averageExtraTempsWest += tempT->extra6West;
        numberExtraTempsWest++;
        cout<<" + "<<tempT->extra6West<<"(extra6West)";
     }

     // print out average extra temperature west
     if (numberExtraTempsWest > 0) {
        cout<<" = "<<averageExtraTempsWest<<endl;
        cout<<"averageExtraTempsWest = "<<averageExtraTempsWest<<"/"<<numberExtraTempsWest<<" = "<<averageExtraTempsWest/numberExtraTempsWest<<endl;
     }
    
     if ( (averageBodyTemperatureWest + averageExtraTempsWest) != 0 ) {
        averageGasTempWest = (averageBodyTemperatureWest + averageExtraTempsWest)/(numberBodyTemperaturesWest + numberExtraTempsWest) + mDb->adjustAverageWest();
        cout<<"setGasTemperatureWest = averageGasTempWest = ("<<averageBodyTemperatureWest<<" + "<<averageExtraTempsWest<<")/("<<numberBodyTemperaturesWest<<" + "<<numberExtraTempsWest<<") + "<<mDb->adjustAverageWest()<<"  = "<<(averageBodyTemperatureWest+averageExtraTempsWest)/(numberBodyTemperaturesWest+numberExtraTempsWest) + mDb->adjustAverageWest()<<endl;
        mParam->setGasTemperatureWest(averageGasTempWest);
        return kStOK;
     }
     else {
        cout<<"No FTPC West body temperatures or FTPC Extra temperatures found for run number "<<runNumber<<"dbDate "<<dbDate<<endl;
        if (mParam->gasTemperatureWest() != 0) cout<<"Using defaultTemperatureWest = "<<mParam->gasTemperatureWest()<<endl; 
        return kStWarn;
     }	  
   }
        
   // Year2004: from runs after run 5027147 on 2004-01-27 to the end of run on  2004-05-15, only Body1, Body3 and Body4 temperature readings are useable for FTPC West
   // The averageBodyTemperatureWest must be adjusted since only 3 instead of 6 temperature readings are used
   if ( runNumber > 5027147 && runNumber < 5136001 ) {
      cout<<"runNumber = "<<runNumber<<" > 5027147 && <= 5136001: only Body1, Body3 and Body4 are useable for FTPC West. The averageBodyTemperatureWest is adjusted by  "<<mDb->adjustAverageWest()<<endl;
      if (mGas->getBody1West() >= mDb->minGasTemperature() && mGas->getBody1West() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureWest += mGas->getBody1West();	 
	 numberBodyTemperaturesWest++;
	 cout<<mGas->getBody1West()<<"(body1West)";
      }  
      if (mGas->getBody3West() >= mDb->minGasTemperature() && mGas->getBody3West() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureWest += mGas->getBody3West();	 
	 numberBodyTemperaturesWest++;
	 cout<<" + "<<mGas->getBody3West()<<"(body3West)";
      }  
      if (mGas->getBody4West() >= mDb->minGasTemperature() && mGas->getBody4West() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureWest += mGas->getBody4West();	 
	 numberBodyTemperaturesWest++;
	 cout<<" + "<<mGas->getBody4West()<<"(body4West)";
      }  
   }  

   else {
      if (mGas->getBody1West() >= mDb->minGasTemperature() && mGas->getBody1West() <= mDb->maxGasTemperature()) {      
	 averageBodyTemperatureWest += mGas->getBody1West();
	 numberBodyTemperaturesWest++;
	 cout<<mGas->getBody1West()<<"(body1West)";
      }		 
      if (mGas->getBody2West() >= mDb->minGasTemperature() && mGas->getBody2West()<= mDb->maxGasTemperature() ) {
	 averageBodyTemperatureWest += mGas->getBody2West();
	 numberBodyTemperaturesWest++;
	 cout<<" + "<<mGas->getBody2West()<<"(body2West)";
      }		 
      if (mGas->getBody3West() >= mDb->minGasTemperature() && mGas->getBody3West()<= mDb->maxGasTemperature() ) {
	 averageBodyTemperatureWest += mGas->getBody3West();
	 numberBodyTemperaturesWest++;
	 cout<<" + "<<mGas->getBody3West()<<"(body3West)";
      }		 
      if (mGas->getBody4West() >= mDb->minGasTemperature() && mGas->getBody4West() <= mDb->maxGasTemperature() ) {
	 averageBodyTemperatureWest += mGas->getBody4West();
	 numberBodyTemperaturesWest++;
	 cout<<" + "<<mGas->getBody4West()<<"(body4West)";
      }		 
      // starting with runNumber 5009073 there are 2 additional body temperature sensors
      if ( runNumber >= 5009073 ) {
         cout<<"(for runNumber >= 5009073 activate additional body temperature sensors) ";
         if (mGas->getBody5West() >= mDb->minGasTemperature() && mGas->getBody5West() <= mDb->maxGasTemperature() ) {
            averageBodyTemperatureWest += mGas->getBody5West();	 
	    numberBodyTemperaturesWest++;
	    cout<<" + "<<mGas->getBody5West()<<"(body5West)";
	 }  
         if (mGas->getBody6West() >= mDb->minGasTemperature() && mGas->getBody6West() <= mDb->maxGasTemperature() ) {
	    averageBodyTemperatureWest += mGas->getBody6West();
	    numberBodyTemperaturesWest++;
	    cout<<" + "<<mGas->getBody6West()<<"(body6West)";
	  }  
      }  
   }      

   // calculate average body temperature west
  if (numberBodyTemperaturesWest > 0) {
     cout<<" = "<<averageBodyTemperatureWest<<endl;
     cout<<"averageBodyTemperatureWest = "<<averageBodyTemperatureWest<<"/"<<numberBodyTemperaturesWest<<" = ";
     averageBodyTemperatureWest = averageBodyTemperatureWest/numberBodyTemperaturesWest;
     cout<<averageBodyTemperatureWest<<endl;
     cout<<"setGasTemperatureWest = "<<averageBodyTemperatureWest<<" (averageBodyTemperatureWest) + "<<mDb->adjustAverageWest()<<" (adjustAverageWest) = "<<averageBodyTemperatureWest + mDb->adjustAverageWest()<<endl;	  
     mParam->setGasTemperatureWest(averageBodyTemperatureWest + mDb->adjustAverageWest()); 
     return kStOK;
  }   
  //  if no body temperature readings return warning
  else {
     cout<<"No FTPC West body temperatures found for "<<dbDate<<endl;
     if (mParam->gasTemperatureWest() != 0) cout<<"Using defaultTemperatureWest = "<<mParam->gasTemperatureWest()<<endl; 
      return kStWarn;
  }	  
}


Int_t StFtpcGasUtilities::averageTemperatureEast(Int_t dbDate, Int_t runNumber) {
	
   Int_t numberBodyTemperaturesEast = 0;
   Float_t averageBodyTemperatureEast = 0.0;
   Float_t averageGasTempEast = 0.0;

   // Starting Y2006, calculate FTPC East gas temperature from extra temperatures only 
   // and adjust with mDb->adjustAverageEast()

   if ( dbDate >= 20051030 ) {
     
    ftpcTemps_st* tempT = mTemps->GetTable();

    Int_t numberExtraTempsEast = 0;
    Float_t averageExtraTempsEast = 0.0;

    if (tempT->extra1East >= mDb->minGasTemperature() && tempT->extra1East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra1East;
        numberExtraTempsEast++;
        cout<<tempT->extra1East<<"(extra1East)";
    }
    if (tempT->extra2East >= mDb->minGasTemperature() && tempT->extra2East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra2East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra2East<<"(extra2East)";
    }
    if (tempT->extra3East >= mDb->minGasTemperature() && tempT->extra3East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra3East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra3East<<"(extra3East)";
    }
    if (tempT->extra4East >= mDb->minGasTemperature() && tempT->extra4East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra4East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra4East<<"(extra4East)";
    }
    if (tempT->extra5East >= mDb->minGasTemperature() && tempT->extra5East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra5East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra5East<<"(extra5East)";
    }
    if (tempT->extra6East >= mDb->minGasTemperature() && tempT->extra6East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra6East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra6East<<"(extra6East)";
    }
    if (tempT->extra7East >= mDb->minGasTemperature() && tempT->extra7East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra7East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra7East<<"(extra7East)";
    }

    // print out average extra temperature east
    if (numberExtraTempsEast > 0) {
        cout<<" = "<<averageExtraTempsEast<<endl;
        cout<<"averageExtraTempsEast = "<<averageExtraTempsEast<<"/"<<numberExtraTempsEast<<" = "<<averageExtraTempsEast/numberExtraTempsEast<<endl;
    }

    if ( averageExtraTempsEast != 0 ) {
        averageGasTempEast =  averageExtraTempsEast/numberExtraTempsEast + mDb->adjustAverageEast();
        cout<<"setGasTemperatureEast = averageGasTempEast = "<<averageExtraTempsEast<<"/"<<numberExtraTempsEast<<" + "<<mDb->adjustAverageEast()<<"  = "<<averageExtraTempsEast/numberExtraTempsEast + mDb->adjustAverageEast()<<endl;
        mParam->setGasTemperatureEast(averageGasTempEast);
        return kStOK;
     }
     else {
        cout<<"No FTPC East Extra temperatures found for run number "<<runNumber<<"dbDate "<<dbDate<<endl;
        if (mParam->gasTemperatureEast() != 0) cout<<"Using defaultTemperatureEast = "<<mParam->gasTemperatureEast()<<endl; 
        return kStWarn;
     }	  
   }

   // For Y2005 calculate FTPC East gas temperature from body temperatures 
   // plus extra temperature readings
   // (the extra temperature readings are available as of 2005-01-01)

   if ( dbDate >= 20050101 && dbDate < 20051030 ) {

     cout<<" dbDate = "<<dbDate<<"(>2005-01-01) Use FTPC East body temperatures + FTPC East extra temperature readings 1-7"<<endl;
        
     if (mGas->getBody1East() >= mDb->minGasTemperature() && mGas->getBody1East() <= mDb->maxGasTemperature()) {      
         averageBodyTemperatureEast += mGas->getBody1East();
         numberBodyTemperaturesEast++;
         cout<<mGas->getBody1East()<<"(body1East)";
     }		 
     if (mGas->getBody2East() >= mDb->minGasTemperature() && mGas->getBody2East()<= mDb->maxGasTemperature() ) {
         averageBodyTemperatureEast += mGas->getBody2East();
         numberBodyTemperaturesEast++;
         cout<<" + "<<mGas->getBody2East()<<"(body2East)";
     }		 
     if (mGas->getBody3East() >= mDb->minGasTemperature() && mGas->getBody3East()<= mDb->maxGasTemperature() ) {
         averageBodyTemperatureEast += mGas->getBody3East();
         numberBodyTemperaturesEast++;
         cout<<" + "<<mGas->getBody3East()<<"(body3East)";
     }		 
     if (mGas->getBody4East() >= mDb->minGasTemperature() && mGas->getBody4East() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureEast += mGas->getBody4East();
         numberBodyTemperaturesEast++;
         cout<<" + "<<mGas->getBody4East()<<"(body4East)";
     }		 
     if (mGas->getBody5East() >= mDb->minGasTemperature() && mGas->getBody5East() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureEast += mGas->getBody5East();	 
         numberBodyTemperaturesEast++;
         cout<<" + "<<mGas->getBody5East()<<"(body5East)";
     }  
     if (mGas->getBody6East() >= mDb->minGasTemperature() && mGas->getBody6East() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureEast += mGas->getBody6East();
         numberBodyTemperaturesEast++;
         cout<<" + "<<mGas->getBody6East()<<"(body6East)";
     }  

     // print out average body temperature east
    if (numberBodyTemperaturesEast > 0) {
       cout<<" = "<<averageBodyTemperatureEast<<endl;
       cout<<"averageBodyTemperatureEast = "<<averageBodyTemperatureEast<<"/"<<numberBodyTemperaturesEast<<" = "<<averageBodyTemperatureEast/numberBodyTemperaturesEast<<endl;
    }
     
    ftpcTemps_st* tempT = mTemps->GetTable();

    Int_t numberExtraTempsEast = 0;
    Float_t averageExtraTempsEast = 0.0;

    if (tempT->extra1East >= mDb->minGasTemperature() && tempT->extra1East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra1East;
        numberExtraTempsEast++;
        cout<<tempT->extra1East<<"(extra1East)";
    }
    if (tempT->extra2East >= mDb->minGasTemperature() && tempT->extra2East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra2East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra2East<<"(extra2East)";
    }
    if (tempT->extra3East >= mDb->minGasTemperature() && tempT->extra3East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra3East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra3East<<"(extra3East)";
    }
    if (tempT->extra4East >= mDb->minGasTemperature() && tempT->extra4East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra4East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra4East<<"(extra4East)";
    }
    if (tempT->extra5East >= mDb->minGasTemperature() && tempT->extra5East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra5East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra5East<<"(extra5East)";
    }
    if (tempT->extra6East >= mDb->minGasTemperature() && tempT->extra6East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra6East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra6East<<"(extra6East)";
    }
    if (tempT->extra7East >= mDb->minGasTemperature() && tempT->extra7East <= mDb->maxGasTemperature() ) {
        averageExtraTempsEast += tempT->extra7East;
        numberExtraTempsEast++;
        cout<<" + "<<tempT->extra7East<<"(extra7East)";
    }

    // print out average extra temperature east
    if (numberExtraTempsEast > 0) {
        cout<<" = "<<averageExtraTempsEast<<endl;
        cout<<"averageExtraTempsEast = "<<averageExtraTempsEast<<"/"<<numberExtraTempsEast<<" = "<<averageExtraTempsEast/numberExtraTempsEast<<endl;
    }

    if ( (averageBodyTemperatureEast + averageExtraTempsEast) != 0 ) {
        averageGasTempEast = (averageBodyTemperatureEast + averageExtraTempsEast)/(numberBodyTemperaturesEast + numberExtraTempsEast) + mDb->adjustAverageEast();
        cout<<"setGasTemperatureEast = averageGasTempEast = ("<<averageBodyTemperatureEast<<" + "<<averageExtraTempsEast<<")/("<<numberBodyTemperaturesEast<<" + "<<numberExtraTempsEast<<") + "<<mDb->adjustAverageEast()<<"  = "<<(averageBodyTemperatureEast+averageExtraTempsEast)/(numberBodyTemperaturesEast+numberExtraTempsEast) + mDb->adjustAverageEast()<<endl;
        mParam->setGasTemperatureEast(averageGasTempEast);
        return kStOK;
     }
     else {
        cout<<"No FTPC East body temperatures or FTPC Extra temperatures found for run number "<<runNumber<<"dbDate "<<dbDate<<endl;
        if (mParam->gasTemperatureEast() != 0) cout<<"Using defaultTemperatureEast = "<<mParam->gasTemperatureEast()<<endl; 
        return kStWarn;
     }	  
   }
     
   // Year2004: from runs after run 5027147 on 2004-01-27 to run 5093053, the last run of the AuAu 62GeV run on  2004-04-02, only Body3, Body4 and Body5 temperature readings are useable for FTPC East
   // The averageBodyTemperatureEast must be adjusted since only 3 instead of 6 temperature readings are used
   if ( runNumber > 5027147 && runNumber <= 5093053 ) {
      cout<<"runNumber = "<<runNumber<<" > 5027147 && <= 5093053: only Body3, Body4 and Body5 are useable for FTPC East. The averageBodyTemperatureEast is adjusted by  "<<mDb->adjustAverageEast()<<endl;
      if (mGas->getBody3East() >= mDb->minGasTemperature() && mGas->getBody3East() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureEast += mGas->getBody3East();	 
	 numberBodyTemperaturesEast++;
	 cout<<mGas->getBody3East()<<"(body3East)";
      }  
      if (mGas->getBody4East() >= mDb->minGasTemperature() && mGas->getBody4East() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureEast += mGas->getBody4East();	 
	 numberBodyTemperaturesEast++;
	 cout<<" + "<<mGas->getBody4East()<<"(body4East)";
      }  
      if (mGas->getBody5East() >= mDb->minGasTemperature() && mGas->getBody5East() <= mDb->maxGasTemperature() ) {
         averageBodyTemperatureEast += mGas->getBody5East();	 
	 numberBodyTemperaturesEast++;
	 cout<<" + "<<mGas->getBody5East()<<"(body5East)";
      }  
   }  

   else {

      if (mGas->getBody1East() >= mDb->minGasTemperature() && mGas->getBody1East() <= mDb->maxGasTemperature()) {      
		 averageBodyTemperatureEast += mGas->getBody1East();
		 numberBodyTemperaturesEast++;
	         cout<<mGas->getBody1East()<<"(body1East)";
      }		 
      if (mGas->getBody2East() >= mDb->minGasTemperature() && mGas->getBody2East()<= mDb->maxGasTemperature() ) {
		 averageBodyTemperatureEast += mGas->getBody2East();
		 numberBodyTemperaturesEast++;
	         cout<<" + "<<mGas->getBody2East()<<"(body2East)";
      }		 
      if (mGas->getBody3East() >= mDb->minGasTemperature() && mGas->getBody3East()<= mDb->maxGasTemperature() ) {
		 averageBodyTemperatureEast += mGas->getBody3East();
		 numberBodyTemperaturesEast++;
	         cout<<" + "<<mGas->getBody3East()<<"(body3East)";
      }		 
      if (mGas->getBody4East() >= mDb->minGasTemperature() && mGas->getBody4East() <= mDb->maxGasTemperature() ) {
		 averageBodyTemperatureEast += mGas->getBody4East();
		 numberBodyTemperaturesEast++;
	         cout<<" + "<<mGas->getBody4East()<<"(body4East)";
      }		 
      // starting with runNumber 5009073 there are 2 additional body temperature sensors
      if ( runNumber >= 5009073 ) {
         cout<<"(for runNumber >= 5009073 activate additional body temperature sensors) ";
         if (mGas->getBody5East() >= mDb->minGasTemperature() && mGas->getBody5East() <= mDb->maxGasTemperature() ) {
            averageBodyTemperatureEast += mGas->getBody5East();	 
	    numberBodyTemperaturesEast++;
	    cout<<" + "<<mGas->getBody5East()<<"(body5East)";
	 }  
         if (mGas->getBody6East() >= mDb->minGasTemperature() && mGas->getBody6East() <= mDb->maxGasTemperature() ) {
	    averageBodyTemperatureEast += mGas->getBody6East();
	    numberBodyTemperaturesEast++;
	    cout<<mGas->getBody6East()<<"(body6East)";
	 } 
       }  
   }      

      // calculate average body temperature east
      if (numberBodyTemperaturesEast > 0) {
        cout<<" = "<<averageBodyTemperatureEast<<endl;
	cout<<"averageBodyTemperatureEast = "<<averageBodyTemperatureEast<<"/"<<numberBodyTemperaturesEast<<" = ";
        averageBodyTemperatureEast = averageBodyTemperatureEast/numberBodyTemperaturesEast;
	cout<<averageBodyTemperatureEast<<endl;
        cout<<"setGasTemperatureEast = "<<averageBodyTemperatureEast<<" (averageBodyTemperatureEast) + "<<mDb->adjustAverageEast()<<" (adjustAverageEast) = "<<averageBodyTemperatureEast + mDb->adjustAverageEast()<<endl;	  
        mParam->setGasTemperatureEast(averageBodyTemperatureEast + mDb->adjustAverageEast()); 
        return kStOK;
     }   
     //  if no body temperature readings return warning
     else {
        cout<<"No FTPC East body temperatures found for "<<dbDate<<endl;
        if (mParam->gasTemperatureEast() != 0) cout<<"Using defaultTemperatureEast = "<<mParam->gasTemperatureEast()<<endl; 
        return kStWarn;
     }	  
}

Int_t StFtpcGasUtilities::defaultTemperatureWest(Int_t dbDate,Bool_t SVT_On) {
    if ( !SVT_On) {
       mParam->setGasTemperatureWest(mDb->defaultTemperatureWest());
       cout<<"No valid body temperatures available for FTPC West; Initialize to defaultTemperatureWest (for dbDate = "<<dbDate<<" SVT off) = "<<mParam->gasTemperatureWest()<<endl;
    }
    if (SVT_On) {
	if (dbDate < 20021105) {
	   // for year 2001 data (AuAu,pp) FTPC west gas temperature is higher when SVT on
           mParam->setGasTemperatureWest(mDb->defaultTemperatureWest() + mDb->temperatureDifference());
           cout<<"No valid body temperatures available for FTPC West; Initialize to mDb->defaultTemperatureWest() + mDb->temperatureDifference() (for year2001 data,SVT on) = "<<mParam->gasTemperatureWest()<<endl;
	}	   
        if (dbDate >= 20021105) { 
           // starting in year 2003,the FTPC west gas temperature is not effected by SVT
           mParam->setGasTemperatureWest(mDb->defaultTemperatureWest());
           cout<<"No valid body temperatures available for FTPC West; Initialize to mDb->defaultTemperatureWest() (as of year2003, SVT on) = "<<mParam->gasTemperatureWest()<<endl;
	}
  }	  
  return kStOK;
}    

Int_t StFtpcGasUtilities::defaultTemperatureEast(Int_t dbDate,Bool_t SVT_On) {
    if ( !SVT_On) {
       mParam->setGasTemperatureEast(mDb->defaultTemperatureEast());
       cout<<"No valid body temperatures available for FTPC East; Initialize to defaultTemperatureEast (for dbDate = "<<dbDate<<" SVT off) = "<<mParam->gasTemperatureEast()<<endl;
    }
    if (SVT_On) {
	if (dbDate < 20021105) {
           // for year 2001 data (dAu,pp) FTPC east gas temperature is not effected by SVT
           mParam->setGasTemperatureEast(mDb->defaultTemperatureEast());
           cout<<"No valid body temperatures available for FTPC East; Initialize to mDb->defaultTemperatureEast() (for year2001 data, SVT on) = "<<mParam->gasTemperatureEast()<<endl;
	}	   
        if (dbDate >= 20021105) { 
           // starting in year 2003 data the FTPC east gas temperature is higher when SVT on
           mParam->setGasTemperatureEast(mDb->defaultTemperatureEast() + mDb->temperatureDifference());
           cout<<"No valid body temperatures available for FTPC East; Initialize to mDb->defaultTemperatureEast() + mDb->temperatureDifference() (as of year2003,SVT on) = "<<mParam->gasTemperatureEast()<<endl;
	}
    }	
  return kStOK;
  }	  
