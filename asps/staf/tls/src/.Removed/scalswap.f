	SUBROUTINE SCALSWAP(raw,swapped)
 
C+
C 
C FUNCTIONAL DESCRIPTION:	
C 
C    Swaps order of 2 integer*2 array elements for scaler swapping.
C 
C FORMAL PARAMETERS:
C  
C     raw = raw data, 2 2byte words
c	swapped = reordered 2byte words
C  
C [common blocks]
C [design]
C 
C-
	IMPLICIT NONE
	INTEGER*2 raw(2),swapped(2)
 
	swapped(1) = raw(2)
	swapped(2) = raw(1)
	RETURN
	END
