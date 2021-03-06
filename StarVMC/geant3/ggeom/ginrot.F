*
* $Id: ginrot.F,v 1.1.1.2 2009/02/01 17:10:25 fisyak Exp $
*
* $Log: ginrot.F,v $
* Revision 1.1.1.2  2009/02/01 17:10:25  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:47:45  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:36  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.1.1.1  2002/07/24 15:56:25  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:39  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:17  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:51  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.29  by  S.Giani
*-- Author :
      SUBROUTINE GINROT(X,RMAT,XNEW)
C.
C.    ******************************************************************
C     *                                                                *
C     *        ROUTINE TO ROTATE THE VECTOR X BY THE INVERSE OF THE    *
C     *       ROTATION MATRIX RMAT AND STORE THE RESULT AS THE         *
C     *        VECTOR XNEW                                             *
C.    *                                                                *
C.    *    ==>Called by : GDFR3D,GDSSUR,GD3D3D,GFLCAR,GFLPHI,GFLRAD    *
C.    *                   GFLTHE                                       *
C.    *         Author  A.McPherson  *********                         *
C.    *                                                                *
C.    ******************************************************************
C.
      DIMENSION X(3),XNEW(3),RMAT(10)
C.
C.    ------------------------------------------------------------------
C.
      IF(RMAT(10).NE.0.0) THEN
         XNEW(1) = X(1)*RMAT(1) + X(2)*RMAT(4) + X(3)*RMAT(7)
         XNEW(2) = X(1)*RMAT(2) + X(2)*RMAT(5) + X(3)*RMAT(8)
         XNEW(3) = X(1)*RMAT(3) + X(2)*RMAT(6) + X(3)*RMAT(9)
*
      ELSE
         XNEW(1) = X(1)
         XNEW(2) = X(2)
         XNEW(3) = X(3)
*
      ENDIF
      END
