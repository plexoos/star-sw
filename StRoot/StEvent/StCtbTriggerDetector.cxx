/***************************************************************************
 *
 * $Id: StCtbTriggerDetector.cxx,v 2.6 2002/11/19 20:21:00 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StCtbTriggerDetector.cxx,v $
 * Revision 2.6  2002/11/19 20:21:00  ullrich
 * Added method to sum all mips.
 *
 * Revision 2.5  2001/04/05 04:00:47  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  2000/05/09 10:22:17  ullrich
 * Updated to cope with modified dst_TrgDet.idl
 *
 * Revision 2.3  1999/12/20 12:54:45  ullrich
 * Adapted changed in trigger table dst_TrgDet
 *
 * Revision 2.2  1999/10/28 22:24:58  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:44:28  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include "StCtbTriggerDetector.h"
#include "tables/St_dst_TrgDet_Table.h"

static const char rcsid[] = "$Id: StCtbTriggerDetector.cxx,v 2.6 2002/11/19 20:21:00 ullrich Exp $";

ClassImp(StCtbTriggerDetector)

StCtbTriggerDetector::StCtbTriggerDetector()
{
    int i, j, k;
    for(i=0; i<mMaxTrays; i++)
        for(j=0; j<mMaxSlats; j++)
            for(k=0; k<mMaxEventSamples; k++) {
                mMips[i][j][k] = 0;
                mTime[i][j][k] = 0;
            }

    for(i=0; i<mMaxAux; i++)
        for(j=0; j<mMaxEventSamples; j++)
            mAux[i][j] = 0;
    
    mNumberOfPreSamples = 0;
    mNumberOfPostSamples = 0;
}

StCtbTriggerDetector::StCtbTriggerDetector(const dst_TrgDet_st& t)
{
    int i, j, k;
    for(i=0; i<mMaxTrays; i++)
        for(j=0; j<mMaxSlats; j++)
            for(k=0; k<mMaxEventSamples; k++) {
                mMips[i][j][k] = t.nCtb[i][j][k];
                mTime[i][j][k] = t.timeCtb[i][j][k];
            }

    for(i=0; i<mMaxAux; i++)
        for(j=0; j<mMaxEventSamples; j++)
            mAux[i][j] = t.ctbaux[i][j];
    
    mNumberOfPreSamples = t.npre;
    mNumberOfPostSamples = t.npost;
}

StCtbTriggerDetector::~StCtbTriggerDetector() {/* noop */}

unsigned int
StCtbTriggerDetector::numberOfTrays() const {return mMaxTrays;}

unsigned int
StCtbTriggerDetector::numberOfSlats() const {return mMaxSlats;}

unsigned int
StCtbTriggerDetector::numberOfAuxWords() const {return mMaxAux;}

unsigned int
StCtbTriggerDetector::numberOfPreSamples() const {return mNumberOfPreSamples;}

unsigned int
StCtbTriggerDetector::numberOfPostSamples() const {return mNumberOfPostSamples;}

float
StCtbTriggerDetector::mips(unsigned int i, unsigned int j, unsigned int k) const
{
    return mMips[i][j][k];
}

double
StCtbTriggerDetector::mips(unsigned int evt) const
{
    double sum = 0;
    for (unsigned int i=1; i<mMaxTrays; i++) 
	for (unsigned int j=1; j<mMaxSlats; j++)
	    sum += mMips[i][j][evt];
    return sum;
}

char
StCtbTriggerDetector::time(unsigned int i, unsigned int j, unsigned int k) const
{
    return mTime[i][j][k];
}
    
float
StCtbTriggerDetector::aux(unsigned int i, unsigned int j) const
{
    return mAux[i][j];
}

void
StCtbTriggerDetector::setMips(unsigned int i, unsigned int j, unsigned int k, float val)
{
    mMips[i][j][k] = val;
}

void
StCtbTriggerDetector::setTime(unsigned int i, unsigned int j, unsigned int k, char val)
{
    mTime[i][j][k] = val;
}

void
StCtbTriggerDetector::setAux(unsigned int i, unsigned int j, float val)
{
    mAux[i][j] = val;
}

void
StCtbTriggerDetector::setNumberOfPreSamples(unsigned int val)
{
    mNumberOfPreSamples = val;
}

void
StCtbTriggerDetector::setNumberOfPostSamples(unsigned int val)
{
    mNumberOfPostSamples = val;
}

