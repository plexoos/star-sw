*
* $Id: mzchnb.F,v 1.1.1.1 2004/07/17 20:01:56 perev Exp $
*
* $Log: mzchnb.F,v $
* Revision 1.1.1.1  2004/07/17 20:01:56  perev
* STAR version of Geant321 TGeant3 etc
*
* Revision 1.1.1.1  2002/07/24 15:56:28  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:49  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:26  fca
* AliRoot sources
*
* Revision 1.1.1.1  1996/03/06 10:47:21  mclareni
* Zebra
*
*
#include "zebra/pilot.h"
      SUBROUTINE MZCHNB (LIX)

C-    Check that LIX is a link in the working space, in a link area,
C-    or even on a local variable, but not a link in a bank.
C-    This is used by routines which restore the input link
C-    from the user back to him, and which could therefor
C-    destroy bank material if a garbage collection happened

#include "zebra/mqsys.inc"
C--------------    End CDE                             --------------
      DIMENSION    LIX(9)
#if (defined(CERNLIB_QTRHOLL))&&(!defined(CERNLIB_A6M))
      DIMENSION    NAMESR(2)
      DATA  NAMESR / 4HMZCH, 4HNB   /
#endif
#if (defined(CERNLIB_QTRHOLL))&&(defined(CERNLIB_A6M))
      DATA  NAMESR / 6HMZCHNB /
#endif
#if !defined(CERNLIB_QTRHOLL)
      CHARACTER    NAMESR*8
      PARAMETER   (NAMESR = 'MZCHNB  ')
#endif


      K = LOCF (LIX(1)) - LQSTOR

      IF (K.LT.LQSTA(KQT+1))          RETURN
      IF (K.GE.LQEND(KQT+20))         RETURN

#include "zebra/qtrace.inc"

      NQCASE = 1
      NQFATA = 2
      IQUEST(11) = K
      IQUEST(12) = LIX(1)
#include "zebra/qtofatal.inc"
      END
*      ==================================================
#include "zebra/qcardl.inc"
