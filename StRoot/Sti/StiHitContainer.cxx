//StiHitContainer.cxx
//M.L. Miller (Yale Software)
//03/01

#include "Stiostream.h"
#include <fstream>
#include <math.h>
#include <algorithm>
#include "Sti/Base/Messenger.h"
#include "Sti/Base/Filter.h"
#include "StiKalmanTrackNode.h"
#include "StiHit.h"
#include "StiPlacement.h"
#include "StiDetector.h"
#include "StiHitContainer.h"
using std::sort;
using std::find;
using std::lower_bound;
using std::upper_bound;
using std::stable_partition;
ostream& operator<<(ostream& os, const StiHit& hit);
ostream& operator<<(ostream&, const HitMapKey&);

StiHitContainer::StiHitContainer(const string & name, 
				 const string & description,
				 Factory<StiHit> *hitFactory)
  : Named(name),
    Described(description),
    _hitFactory(hitFactory)
{
  cout <<"StiHitContainer::StiHitContainer() -I- Started with name:"<<name<<endl;
  //_minPoint = new StiHit();
  //_maxPoint = new StiHit();
}

StiHitContainer::~StiHitContainer()
{
  cout <<"StiHitContainer::~StiHitContainer()"<<endl;
}

/*! Null implementation.  We provide this virtual function for the situation
  when StiHitContainer::instance() behaves polymorphically, e.g., when it
  actually points to a StiRootDrawableDetector object.  In that situation, a
  call to update() will propogate to the most derived class, allowing that
  class to perform necessary tasks (e.g., append hits to display).
 */
//void StiHitContainer::update()
//{}

/*! The time complexity of push_back has two components:\n
  1) The correct hit-vector must be retrieved (or inserted if it doesn't
  exist) from the map.  This portion is O(logN) where N is the number of
  hit-vectors in the map.  See the documentation of the hits() method for an
  estimate of N. \n
  2) The point must be added to the hit-vector.  This is guarunteed to be a
  constant time process, where the constant is determined by the vendor STL
  implementation.\n
  \warning A call to push_back() invalidates the sorted state of the
  container.  Thus, once all hits have been added to the container, then one
  must call sortHits().
 */
void StiHitContainer::add(StiHit* hit)
{
  const StiDetector* det = hit->detector();
  if (!det) 
    throw runtime_error("StiHitContainer::add() -E- Given hit has no associated detector");
  _key.refangle = det->getPlacement()->getNormalRefAngle();
  //_key.position = det->getPlacement()->getNormalRadius();
  _key.position = det->getPlacement()->getLayerRadius();
  _map[_key].theHitVec.push_back(hit);
  return;
}

void StiHitContainer::reset()
{
   HitMapToVectorAndEndType::iterator it;
   vector<StiHit*>::iterator iter;
   for (it=_map.begin(); it!=_map.end(); it++) 
     {
       vector<StiHit*> &hits = (*it).second.theHitVec;
       for (iter=hits.begin();iter!=hits.end();iter++)
	 {
	   (*iter)->setTimesUsed(0);
	 }
     }
}

/*! A call to clear() must call std::vector<StiHit*>::clear() for all vectors
  stored in the map.  Thus a call to clear is of O(N) *M where N is the number
  of keys in the map (see documenation of hits() for an estimate of N) and
  M is the time required to clear each vector.
 */
void StiHitContainer::clear()
{
  cout<<"StiHitContainer::clear() -I- Started"<<endl;
    HitMapToVectorAndEndType::iterator it;
    for (it=_map.begin(); it!=_map.end(); it++) 
			{
				(*it).second.theHitVec.clear();
				(*it).second.theEffectiveEnd = (*it).second.theHitVec.end();
			}
    cout<<"StiHitContainer::clear() -I- Done"<<endl;
}

/*! The time complexity of size is of O(logN) where N is the number of
  keys in the map.  For an estimate of N please see the documnation for
  hits() method.
*/
unsigned int StiHitContainer::size() const
{
    unsigned int thesize = 0;
    HitMapToVectorAndEndType::const_iterator it;
    for (it=_map.begin(); it!=_map.end(); it++) {
	thesize+=(*it).second.theHitVec.size();
    }
    return thesize;
}


vector<StiHit*>::iterator StiHitContainer::hitsBegin(const StiDetector* layer)
{
    _key.refangle = layer->getPlacement()->getNormalRefAngle();
    //_key.position = layer->getPlacement()->getNormalRadius();
    _key.position = layer->getPlacement()->getLayerRadius();
    return _map[_key].theHitVec.begin();
}

vector<StiHit*>::iterator StiHitContainer::hitsEnd(const StiDetector* layer)
{
    _key.refangle = layer->getPlacement()->getNormalRefAngle();
    //_key.position = layer->getPlacement()->getNormalRadius();
    _key.position = layer->getPlacement()->getLayerRadius();
    return _map[_key].theEffectiveEnd;
}


/*! Get hits specified by the filter condition implied by the given position 
    and search radius.

  The sub-volume is identified by the following algorithm:\n
  1) Identify the detector plane of intereset via the position and refAngle
  of the StiHit pointer passed. \n
  2) Find those hits that satisfy abs(hit->z-z_i)<deltaZ.  This is
  accomplished via a call to the STL algorithms lower_bound and
  upper_bound. \n
  3) Find thos hits that satisfy abs(hit->y-y_i)<deltaD.  This can only be
  accomplished via a linear search over those hits satisfying condition
  
  The time complexity of getHits has several components: \n
  1) The correct hit-vector must be retrieved from the map.  This is of
  O(logN) where N is the number of keys in the map (see documentation of
  hits() for an estimate of the size of N).\n
  2) The calls to lower_bound and upper_bound are each of O(logM) where M is
  the number of hits in the sorted vector. \n
  3) The linear search over those hits that satisfy criterion 2.  This is
  of O(P) where P is the number of points that passed criterion 2. \n

  This algorithm is easily modifiable to perform the search in the opposite
  order (search in y, then z instead of z, then y).  See the source code for
  the necessary conversion actions.
 */
vector<StiHit*> & StiHitContainer::getHits(StiHit& ref, double dY, double dZ, bool fetchAll)
{
  _selectedHits.clear();
  _key.refangle = ref.refangle();
  _key.position = ref.position();
	//cout << "StiHitContainer::getHits(StiHit& ref, double dY, double dZ, bool fetchAll) -I- " << endl;
	//cout << "   dY:"<<dY<<endl
	//<< "   dZ:"<<dZ<<endl;
  _minPoint.set(ref.position(),ref.refangle(),ref.y()-dY,ref.z()-dZ );
  _maxPoint.set(ref.position(),ref.refangle(),ref.y()+dY,ref.z()+dZ );
  vector<StiHit*>& tempvec = _map[_key].theHitVec;
  vector<StiHit*>::iterator& tempend = _map[_key].theEffectiveEnd;
  //Search first by distance along z
  _start = lower_bound(tempvec.begin(), tempend, &_minPoint, StizHitLessThan());
  if (_start!=tempend) 
    _stop = upper_bound(tempvec.begin(), tempend, &_maxPoint, StizHitLessThan());
  else 
    {
      _start = tempend;
      _stop  = _start;
    }
  //Now search over distance along d
  StiHit * hit;
  for (vector<StiHit*>::iterator cit=_start; cit!=_stop; cit++) 
    {
      hit = *cit;
      if (fabs( hit->y() - ref.y() ) < dY)
	{
	  if (fetchAll || (hit->timesUsed()==0 && hit->detector()->isActive()) )
	    _selectedHits.push_back(hit);
	}
    }
  return _selectedHits;
}

/*! This function calls the STL sort algorithm for each hit-vector in the
  map.  The StiHit objects are ordered via the struct StizHitLessThan.
  
  \note A call to push_back(StiHit*) invalidates the sorted state of the
  container.  Thus any number of x calls to push_back(StiHit*) must be
  followed by a call to sortHits().\n
  
  The time complexity of sortHits() has two components:\n
  1) The time to access each vector in the map.  This is of O(N) where N is
  the number of keys in the map (see documentation of hits() for an
  estimate of N). \n
  2) The time to sort an individual vector.  This is of O(M logM) where M
  is the number of hits in stored in the vector.
 */
void StiHitContainer::sortHits()
{
  HitMapToVectorAndEndType::iterator it;
  for (it=_map.begin(); it!=_map.end(); ++it) 
    {
      vector<StiHit*>& tempvec = (*it).second.theHitVec;
      sort(tempvec.begin(), tempvec.end(), StizHitLessThan());
      (*it).second.theEffectiveEnd =(*it).second.theHitVec.end();
    }
  return;
}

void StiHitContainer::partitionUsedHits()
{
    for (HitMapToVectorAndEndType::iterator it=_map.begin(); it!=_map.end(); ++it)
      {
      vector<StiHit*>& tempvec = (*it).second.theHitVec;
      vector<StiHit*>::iterator where =
        stable_partition(tempvec.begin(), tempvec.end(), StiHitIsUsed() );
      (*it).second.theEffectiveEnd = where;
    }
}

ostream& operator<<(ostream& os, const vector<StiHit*>& vec)
{
    for (vector<StiHit*>::const_iterator vit=vec.begin(); vit!=vec.end(); vit++) {
	os<<*(*vit)<<endl;
    }
    return os;
}

ostream& operator<<(ostream& os, const StiHitContainer& store)
{
    for (HitMapToVectorAndEndType::const_iterator it=store._map.begin(); it!=store._map.end(); it++) {
	os <<endl;
	os <<(*it).second.theHitVec;
    }
    return os;   
}


/// Get hits selected by the given filter. If no filter is given (i.e. filter==0)
//  then return all hits.
vector<StiHit*> & StiHitContainer::getHits()
{
  _selectedHits.clear();
  for(HitMapToVectorAndEndType::const_iterator iter= _map.begin(); iter !=_map.end(); iter++)
   {
      const vector<StiHit*> & t_hits = (*iter).second.theHitVec;
      for (vector<StiHit*>::const_iterator it=t_hits.begin();it!=t_hits.end();++it)
        _selectedHits.push_back(*it);
   }  
  return _selectedHits;
}

/// Get hits selected by the given filter. If no filter is given (i.e. filter==0)
//  then return all hits.
vector<StiHit*> & StiHitContainer::getHits(Filter<StiHit> & filter)
{
  //cout << "StiHitContainer::getHits(Filter<StiHit> * filter) -I- Started"<<endl;
  _selectedHits.clear();
  StiHit * hit;
  for(HitMapToVectorAndEndType::const_iterator iter= _map.begin(); iter !=_map.end(); iter++)
    {
    const vector<StiHit*> & t_hits = (*iter).second.theHitVec;
    for (vector<StiHit*>::const_iterator it=t_hits.begin();
         it!=t_hits.end();
         ++it)
      {
      hit = *it;
      if (filter.accept(hit)) _selectedHits.push_back(hit);
      }
    }
  return _selectedHits;
}


StiHit * StiHitContainer::getNearestHit(StiHit& ref, double dY, double dZ, bool fetchAll)
{ 
  StiHit* hit = 0;
  StiHit* closestHit = 0;
  double dMax = DBL_MAX;
  double dy, dz, d;
  vector<StiHit*> & hits = getHits(ref,dY,dZ,fetchAll);
  for (vector<StiHit*>::iterator iter=hits.begin();iter!=hits.end();++iter)
    {
      hit = *iter;
      dy  = hit->y() - ref.y();
      dz  = hit->z() - ref.z();
      d   = dy*dy + dz*dz;
      if ( d<dMax)
        {
        closestHit = hit;
        dMax       = d;
        }
    }
  return closestHit;
}

