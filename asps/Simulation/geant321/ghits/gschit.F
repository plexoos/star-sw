*
* $Id: gschit.F,v 1.1.1.1 2004/01/15 00:12:11 potekhin Exp $
*
* $Log: gschit.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:11  potekhin
*
* Revision 1.2  1998/02/10 13:05:05  atgeant3
* *** empty log message ***
*
*
#include "geant321/pilot.h"
      SUBROUTINE GSCHIT(ISET,IDET,ITRA,NUMBV,HITS,NHSUM,IHIT)
C.
C.    ******************************************************************
C.    *                                                                *
C.    *       Store HITS values into detector IUDET of set IUSET       *
C.    *                                                                *
C.    *       NUMBV  volume numbers                                    *
C.    *       HITS  array of values for the elements of current hit    *
C.    *       ITRA  track number associated to this hit                *
C.    *       IHIT  output parameter containing the hit number         *
C.    *             If IHIT=0  hit has not been stored                 *
C.    *                                                                *
C.    *       Same action as GSAHIT but in case the physical volume    *
C.    *       specified by NUMBV contains already some hit for the     *
C.    *       same track, then the routine will sum up the last NHSUM  *
C.    *       elements of the hit.                                     *
C.    *        In order to use that routine , no packing must be       *
C.    *        specified for these NHSUM last hits.                    *
C.    *        If NHSUM.LE.0 then GSCHIT is the same as GSAHIT.        *
C.    *                                                                *
C.    *    ==>Called by : <USER>, GUSTEP                               *
C.    *       Authors    R.Brun, M.Maire  *********                    *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcbank.inc"
#include "geant321/gcvolu.inc"
#include "geant321/gcunit.inc"
      COMMON/GCLOCA/NLOCAL(2),JS,JD,JDH,JH,JHD,LOCAL(15)
      DIMENSION NUMBV(*),HITS(*)
      PARAMETER (MAXINT=2147483647)
      SAVE NMESS
      DATA NMESS/0/
C.
C.    ------------------------------------------------------------------
C.
      CALL GSAHIT(ISET,IDET,ITRA,NUMBV,HITS,IHIT)
      IF (IHIT.LE.1)  GO TO 999
      IF (NHSUM.LE.0) GO TO 999
C
      NV=IQ(JD+1)
      NH=IQ(JD+3)
      NW=NV+NH+1
      JNEWH = JHD+(IHIT-1)*NW
      JCURR = JNEWH
C
      DO 30 I=1,IHIT-1
         JCURR = JCURR-NW
C
C             Check if track number is ITRA
C
         IF (ITRA.NE.IQ(JCURR+1)) GO TO 999
C
C             Check if volume numbers are the same
C
         DO 10 J=2,NV+1
            IF (IQ(JCURR+J).NE.IQ(JNEWH+J)) GO TO 30
   10    CONTINUE
C
C             Volumes are the same. Now sum the last NHSUM hits
C
         DO 20 K=NW,NW-NHSUM+1,-1
            IF (IQ(JCURR+K).GT.MAXINT-IQ(JNEWH+K)) THEN
               NMESS=NMESS+1
               IF (NMESS.LT.3) THEN
                  WRITE (CHMAIL,10000) IQ(JSET+ISET),IQ(JS+IDET)
10000             FORMAT(' ***** GSCHIT OVERFLOW WARNING FOR SET= ',
     +                                               A4,' DET= ',A4)
                  CALL GMAIL(0,0)
               ENDIF
               GO TO 999
            ELSE
               IQ(JCURR+K)=IQ(JCURR+K)+IQ(JNEWH+K)
            ENDIF
   20    CONTINUE
C
C             Remove temporarily stored hit
C
*****    IHIT=IHIT-1
         IHIT=IHIT-I
         IQ(JH+IDET)=IQ(JH+IDET)-NW
         GO TO 999
   30 CONTINUE
C
  999 CONTINUE
      END
