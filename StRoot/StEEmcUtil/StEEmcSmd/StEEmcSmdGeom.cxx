/*******************************************************************
 *
 * $Id: StEEmcSmdGeom.cxx,v 1.9 2004/01/29 15:26:10 jwebb Exp $
 *
 * Author: Wei-Ming Zhang
 *
 * Revisions:
 *
 * 01/28/04 Jason Webb -- StRoot independent code moved to a
 * separate EEmcSmdGeom class.  StEEmcSmdGeom now derives from 
 * that class, implementing functions useful for integrating
 * with Star.  See EEmcSmdGeom.{h,cxx} for further documentation.
 *
 *****************************************************************
 *
 * Description: Interface to EEMC-SMD database
 *
 *****************************************************************/

#include "StEEmcSmdGeom.h"
#include "PhysicalConstants.h"

ClassImp(StEEmcSmdGeom);

/////////////////////////////////////////////////////////////////////////////

// return index of a sector from a global point in a plane 

Int_t StEEmcSmdGeom::getEEmcISec(const Int_t iPlane, 
				 const StThreeVectorD& point) const {
  
  const TVector3 myPoint ( point[0], point[1], point[2] );
  Int_t val = EEmcSmdGeom::getEEmcISec( iPlane, myPoint );

  return val;
}

/////////////////////////////////////////////////////////////////////////////

// Returns a pointer to the strip which has the smallest DCA (distance of
// closest approach) to the specified point.

StructEEmcStrip* 
StEEmcSmdGeom::getDcaStripPtr(const Int_t iPlane, 
			      const Int_t iSec, 
			      const StThreeVectorD& point, 
			      Float_t* dca) {

  const TVector3 myPoint( point[0], point[1], point[2] );
  return EEmcSmdGeom::getDcaStripPtr( iPlane, iSec, myPoint, dca );

}

StructEEmcStrip* 
StEEmcSmdGeom::getDcaStripPtr(const Int_t iPlane, 
			      StThreeVectorD& point, 
			      Float_t* dca) {

  TVector3 myPoint( point[0], point[1], point[2] );
  return EEmcSmdGeom::getDcaStripPtr( iPlane, myPoint, dca );

}

/////////////////////////////////////////////////////////////////////////////

  // Given two strips (alternatively sector and strip Id's), return a 
  //   vector pointing to the center of the trapezoid formed by their
  //   crossing.  These functions may return non-physical locations,
  //   for instance, when a U,V pair does not cross within the 
  //   fiducial area of the detector.  Note: the z-component returned
  //   will be the average z of the U and V detector planes.

StThreeVectorD StEEmcSmdGeom::getIntersection ( Int_t iSec, 
						Int_t iUStrip, 
						Int_t iVStrip ) {

  TVector3 myPoint = EEmcSmdGeom::getIntersection(iSec,iUStrip,iVStrip);
  StThreeVectorD point = StThreeVectorD( myPoint[0], myPoint[1], myPoint[2] );
  return point;

}

StThreeVectorD StEEmcSmdGeom::getIntersection ( StructEEmcStrip *u, 
						StructEEmcStrip *v ) {

  TVector3 myPoint = EEmcSmdGeom::getIntersection(u,v);
  StThreeVectorD point = StThreeVectorD( myPoint[0], myPoint[1], myPoint[2] );
  return point;

}

/////////////////////////////////////////////////////////////////////////////

StThreeVectorD StEEmcSmdGeom::getstripEnd( const StructEEmcStrip strip, 
					   const Int_t endId ) {

  TVector3 myPoint = EEmcSmdGeom::getstripEnd( strip, endId );
  return StThreeVectorD( myPoint[0], myPoint[1], myPoint[2] );

}

/////////////////////////////////////////////////////////////////////////////

// methods for ITTF

// return phiMax and phiMax of a sector including empty sector 
pairD StEEmcSmdGeom::getEEmcSmdPhiMinMax(const Int_t iPlane, const Int_t iSec) 
{
     pairD phiMinMax;
     float phiMin, phiMax;
     //int iUV, antiClockUVId, clockUVId;
     int iUV, antiClockIUV, clockIUV;
     int antiClockISec, clockISec;

     iUV = kEEmcSmdMapUV[iPlane][iSec];

     if(iUV >= 0) {
           phiMin = getEEmcSector(iUV, iSec).phiMin;
           phiMax = getEEmcSector(iUV, iSec).phiMax;
     }
     else {  // emtry sector
       // find phiMax in anticlockwise adjacent sector 
	  if(iSec != 0) antiClockISec = iSec - 1;
	  else antiClockISec = 11; 
	  antiClockIUV = kEEmcSmdMapUV[iPlane][antiClockISec];

          phiMax = getEEmcSector(antiClockIUV,antiClockISec).phiMin;
	  // find phiMin in clockwise adjacent sector 
	  if(iSec != 11) clockISec = iSec + 1;
	  else clockISec = 0; 
	  clockIUV = kEEmcSmdMapUV[iPlane][clockISec];
	  phiMin=getEEmcSector(clockIUV,clockISec).phiMax;
     }
     phiMinMax.first = (double) phiMin;	     
     phiMinMax.second = (double) phiMax;	     

     return phiMinMax;
}


// return delta_phi of a sector including empty sector 

float StEEmcSmdGeom::getEEmcSmdDelPhi(const Int_t iPlane, const Int_t iSec) 
{
     float delPhi;
     pairD  phiMinMax = getEEmcSmdPhiMinMax(iPlane, iSec);
     delPhi = (float) phiMinMax.second - (float)phiMinMax.first;
     if(iSec  == kEEmcSmdSectorIdPhiCrossPi - 1) delPhi = 2*pi + delPhi; 

     return delPhi;
}



// return center phi of a sector including empty sector 
float StEEmcSmdGeom::getEEmcSmdCenterPhi(const Int_t iPlane, 
		                               const Int_t iSec)
{
     float centerPhi;
     pairD phiMinMax = getEEmcSmdPhiMinMax(iPlane, iSec);
     centerPhi = 0.5*((float) phiMinMax.second + (float)phiMinMax.first);
     if(iSec  == kEEmcSmdSectorIdPhiCrossPi - 1) {
	     if(centerPhi <= 0) centerPhi= M_PI + centerPhi; 
	     else centerPhi = M_PI - centerPhi; 
     }

     return centerPhi;
}

/////////////////////////////////////////////////////////////////////////////

void StEEmcSmdGeom::printSectorPhis(const Int_t iPlane, const Int_t iSec,
                                                              ostream& os ) {
  int iUV;
  iUV = kEEmcSmdMapUV[iPlane][iSec];

  os << "------EEmcSmdGeom::printPhis()------" << std::endl;
  os << " planeId = " << iPlane + 1 << " sectorId = " << iSec + 1 << std::endl;
  if(iUV >= 0) 
    os << " " <<  kEEmcSmdUVChar[iUV] << " Sector" << std::endl; 
  else  
    os << " Empty" << std::endl; 
  os << " delPhi = " << getEEmcSmdDelPhi(iPlane, iSec)/degree <<
    " " << "centerPhi = " << getEEmcSmdCenterPhi(iPlane, iSec)/degree 
     << std::endl;
  
}
