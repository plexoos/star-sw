/**********************************************************************
 *
 * $Id: StTpcElectronics.hh,v 1.2 1999/01/18 10:21:55 lasiuk Exp $
 *
 * Author: brian Nov 3, 1998
 *
 **********************************************************************
 *
 * Description:  Abstract Class interface for Electronics parameters
 *
 **********************************************************************
 *
 * $Log: StTpcElectronics.hh,v $
 * Revision 1.2  1999/01/18 10:21:55  lasiuk
 * add tau
 *
 * Revision 1.3  1999/02/24 19:32:11  lasiuk
 * tzero offset parameter added
 *
 * Revision 1.2  1999/01/18 10:21:55  lasiuk
 * add tau
 *
 * Revision 1.1  1998/11/10 17:12:06  fisyak
 * Put Brian trs versin into StRoot
 *
 * Revision 1.1  1998/11/04 18:52:25  lasiuk
 * Initial Revision
 *
 **********************************************************************/
#ifndef ST_TPC_ELECTRONICS_HH
#define ST_TPC_ELECTRONICS_HH

#include <iostream.h>

#include "StGlobals.hh"
#include "StTpcPadCoordinate.hh"

class StTpcElectronics {
public:
    virtual ~StTpcElectronics() {}
    //StTpcElectronics(const StTpcElectronics&);
    //StTpcElectronics& operator=(const StTpcElectronics&);

    // Analog Electronics
    virtual double    nominalGain()                    const = 0;
    virtual double    channelGain(int,int,int)         const = 0;
    virtual double    shapingTime()                    const = 0;
    virtual double    samplingFrequency()              const = 0;
    virtual double    tZero()                          const = 0;
    virtual double    tau()                            const = 0;

    // Digital Electronics
    virtual double adcConversion()                     const = 0;
    virtual double adcConversionCharge()               const = 0;
    virtual int    averagePedestal()                   const = 0;
    virtual int    pedestal(int,int,int,int)           const = 0;
    virtual int    pedestal(StTpcPadCoordinate&)       const = 0;

    // Diagnostic
    virtual void   print(ostream& = cout)              const = 0;
};    
#endif
