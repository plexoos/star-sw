/**************************************************************************
 * Copyright(c) 2001, STAR  Experiment at BNL, All rights reserved.       *
 *                                                                        *
 * Author: STAR Integrated Track Task Force                               *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 *                                                                        *
 **************************************************************************/

/**************************************************************************
 *                                                                        *
 * StiHitFactory					                  *
 *                                                                        *
 * Author:  Claude Pruneau, Wayne State University                        *
 * Created: April 2001                                                    *
 *                                                                        *
 * Description: A factory class used to provide the Sti tracking code     *
 * with StiHit instances upon the demand. The instances are created       *
 * all at once and never destroyed.                                       *
 *                                                                        *
 * Usage:                                                                 *
 *                                                                        *
 * The external interface consists essentially of two methods:            *
 *          StiHit *getHit()                                              *
 *          void    reset()                                               *
 *                                                                        *
 *  getHit() returns a pointer to a hit object. The objects already exist *
 *           so a pointer to an unused object has to been found and       *
 *           returned. This method is thus in principle quite fast.       *
 *                                                                        *
 *  reset()  declares that all hit objects currently in used are no longer*
 *           needed for the current event. The usage counter is reset     *
 *           and all objects are freed.                                   *
 *                                                                        *
 * Note (1)                                                               *
 * The original allocation of hits   has a size determined by the para-   *
 * meter "originalSize".                                                  *
 * The instances are created internally upon construction of the factory  *
 * object.  Note that if the original allocation is insufficient, new     *
 * instances will automatically be allocated and created. The new allo-   *
 * cation will have a size determined by the parameter "incrementalSize"  *
 * New allocations are permitted a finite number of times determined by   *
 * the value of "maxIncrementCount".                                      *
 *                                                                        *
 * Note (2)                                                               *
 * The factory is the owner of the hit instances. Do not destroy, or      *
 * deallocate the objects elsewhere than through the destructor of this   *
 * factory class.                                                         *
 *                                                                        *
 * Note (3)                                                               *
 * This factory is implemented as a class rather than a functor because   *
 * it might be possible to instantiate many instances of the factory      *
 * which serve different purposes. i.e. this class need not be a singleton*
 *                                                                        *
 *                                                                        *
 * Example:                                                               *
 *                                                                        *
 * main()                                                                 *
 * {                                                                      *
 *   StiHitFactory * hitFactory = new StiHitFactory() //using defaults    *
 *                                                                        *
 *   // some other code here                                              *
 *                                                                        *
 *   for (eventIndex=0;eventIndex<eventCountRequested;eventIndex++)       *
 *     {                                                                  *
 *       hitFactory->reset();  // tells the factory to free all hits      *
 *                                                                        *
 *       for (hitIndex=0;hitIndex<hitCount;hitIndex++)                    *
 *          {                                                             *
 *            // now copying hits from StEvent into StiHit                *
 *            StiHit * hit = hitFactory->getHit();                        *
 *            hit->x = xFromStEvent; //etc                                *
 *          }                                                             *
 *     }                                                                  *
 * }                                                                      *
 *                                                                        *
 **************************************************************************/
#include "StiHitFactory.h"

StiHitFactory::StiHitFactory(int original,
				 int incremental,
				 int maxIncrement)
  : StiObjectFactory("HitFactory",original,incremental,maxIncrement)
{
    createObjects(originalSize);
    currentSize = container.size();
}

StiHitFactory::~StiHitFactory()
{
    for (tobject_vector::iterator it=container.begin(); it!=container.end(); ++it) {
	delete (*it);
	(*it) = 0;
    }
}

StiHit * StiHitFactory::getHit()
{
    return dynamic_cast<StiHit*>( getObject() );
}
  
void StiHitFactory::createObjects(int n)
{
    //cout <<"StiHitFactory::createObjects() creating "<<n<<" objects"<<endl;
    for (int i=0;i<n; ++i) {
	container.push_back( new StiHit() );
    }
}

void StiHitFactory::print() const
{
    cout <<"StiHitFactory::print()"<<endl;
    for (tobject_vector::const_iterator it=container.begin(); it!=container.end(); ++it) {
	StiHit* temp = dynamic_cast<StiHit*>(*it);
	if (!temp) cout <<"StiHitFactory::print() Error! TObject* -> StiHit* cast failed!!"<<endl;
	else cout <<*temp<<endl;
    }
    return;
}
