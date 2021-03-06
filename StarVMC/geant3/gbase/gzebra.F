*
* $Id: gzebra.F,v 1.2 2010/02/15 19:38:39 jwebb Exp $
*
* $Log: gzebra.F,v $
* Revision 1.2  2010/02/15 19:38:39  jwebb
* Added compile time switch so that zebra initializes silently.
*
* Revision 1.1.1.2  2009/02/01 17:10:19  fisyak
* Merge with .DEV2
*
* Revision 1.1.1.1  2008/12/10 20:47:00  fisyak
* Merge with macos version
*
* Revision 1.1.1.1  2005/05/25 22:37:26  fisyak
* Alice version of geant3 (-minicern)
*
* Revision 1.2  2003/11/28 11:23:55  brun
* New version of geant321 with all geant3 routines renamed from G to G3
*
* Revision 1.1.1.1  2002/07/24 15:56:24  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:37  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:16  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:11  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.19  by  S.Giani
*-- Author :
      SUBROUTINE G3ZEBRA(NZEB)
C.
C.    ******************************************************************
C.    *                                                                *
C.    *       Routine to initialise ZEBRA store (//)                   *
C.    *                                                                *
C.    *    ==>Called by : <USER>                                       *
C.    *       Author    R.Brun  *********                              *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcbank.inc"
#define  __SILENT__
C.
C.    ------------------------------------------------------------------
C.
      NZEBRA=NZEB
#ifdef __SILENT__
      CALL MZEBRA(-2) 
#else
      CALL MZEBRA(-1) 
#endif 
      CALL MZSTOR(IXSTOR,'/GCBANK/',' ',FENDQ,LQ,LR1,WS,LQ(KWWORK+100)
     +            ,LQ(NZEBRA-30))
      CALL MZLOGL(IXSTOR,0)
C
      RETURN
      END
