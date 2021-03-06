*
* $Id: gsvert.F,v 1.1.1.1 2004/01/15 00:12:12 potekhin Exp $
*
* $Log: gsvert.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:12  potekhin
*
* Revision 1.1.1.1  1997/11/03 15:30:29  atlascvs
* Importing CERNLIB version 08.21.
*
* Revision 1.1.1.1  1995/10/24 10:21:19  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.21  by  S.Giani
*-- Author :
      SUBROUTINE GSVERT(V,NTBEAM,NTTARG,UBUF,NWBUF,NVTX)
C.
C.    ******************************************************************
C.    *                                                                *
C     *       Creates a new vertex bank                                *
C     *       Vertex is generated from tracks NTBEAM NTTARG            *
C     *       NVTX is the new vertex number                            *
C.    *                                                                *
C.    *    ==>Called by : <USER>, GUKINE,GUSTEP                        *
C.    *       Authors    R.Brun, F.Carena, M.Hansroul  *********       *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcbank.inc"
#include "geant321/gcnum.inc"
#include "geant321/gcpush.inc"
#include "geant321/gctrak.inc"
      DIMENSION V(3),UBUF(1)
C.
C.    ------------------------------------------------------------------
C.
      NVTX   = NVERTX + 1
      IF (JVERTX.EQ.0)THEN
         CALL MZBOOK(IXDIV,JVERTX,JVERTX,1,'VERT',NCVERT,NCVERT,1,2,0)
         IQ(JVERTX-5)=0
      ENDIF
      IF(NVTX.GT.IQ(JVERTX-2)) CALL MZPUSH(IXDIV,JVERTX,NPVERT,0,'I')
C
      CALL MZBOOK(IXDIV,JV,JVERTX,-NVTX,'VERT',1,1,9,3,0)
C
      IF(NWBUF.GT.0)THEN
         CALL MZBOOK(IXDIV,JUV,JV,-1,'VERU',0,0,NWBUF,3,0)
         IQ(JUV-5)=NVTX
         DO 3 I=1,NWBUF
   3     Q(JUV+I)=UBUF(I)
      ENDIF
      DO 4 I=1,3
   4  Q(JV + I) = V(I)
      Q(JV + 4) = TOFG
      Q(JV + 5) = NTBEAM
      Q(JV + 6) = NTTARG
      NTK=0
      IF(JKINE.GT.0)NTK=IQ(JKINE-2)
      IF(NTBEAM.GT.NTK)GO TO 90
      IF(NTBEAM.LT.0)GO TO 90
      IF(NTTARG.GT.NTK)GO TO 90
      IF(NTTARG.LT.0)GO TO 90
      IF(NTBEAM.NE.0)THEN
         JK = LQ(JKINE- NTBEAM)
         IF(JK.EQ.0)GO TO 90
         NVG = Q(JK + 7)
         NFREE=IQ(JK-1)-7-NVG
         IF(NFREE.LE.0)CALL MZPUSH(IXDIV,JK,0,2,'I')
         Q(JK + NVG + 8) = NVTX
         Q(JK + 7) = NVG + 1
      ENDIF
C
      IF(NTTARG.NE.0)THEN
         JK = LQ(JKINE- NTTARG)
         NVG = Q(JK + 7)
         NFREE=IQ(JK-1)-7-NVG
         IF(NFREE.LE.0)CALL MZPUSH(IXDIV,JK,0,2,'I')
         Q(JK + NVG + 8) = NVTX
         Q(JK + 7) = NVG + 1
      ENDIF
C
      NVERTX = NVTX
      IQ(JVERTX+1)=NVERTX
      GO TO 99
C
C             Error
C
  90  NVTX   = 0
  99  RETURN
      END
