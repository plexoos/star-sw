**:>-----------------------------------------------------------------------
module    G2Tmain  is g2t converter
author    Pavel Nevski
created   22 april 98
*
* description: create and fill two new directories in the CWD:
*              Run   - with tables mapped to DETM family banks
*              Event - with track/vertex and hit tables
*              One level is assumed, names can be modified by DETP command
* Modifications 
* PN, 98/09/23: leave NTRACK in IQUEST(100) to allow analysis in Kuip
**:<-----------------------------------------------------------------------
#include "geant321/gcbank.inc"
#include "geant321/gcflag.inc"
#include "geant321/gcunit.inc"
#include "geant321/gcnum.inc"
#include "g2t_event.inc"
#include "g2t_vertex.inc"
#include "g2t_track.inc"
#include "g2t_hits.inc"
+cde,quest.
*
      structure GTTC { int version, int nsys , char edir(3), char rdir(3) }
      structure dete { onoff, char ctab, char spec, char csys, char cdet }
      Integer        G2T_MAIN,AgFHIT0,DUI_CDIR,TDM_MAP_TABLE
      Integer        TDM_NEW_TABLE,AMI_CALL,G2T_NEW_TABLE,AMI_MODULE_CALL
      External       TDM_NEW_TABLE,AMI_CALL,G2T_NEW_TABLE,AMI_MODULE_CALL
      Integer        i,j,ld,nhits,nnhits,Iprin
      Character*16   ctab,ctabo,names(3)
      Character      o*1,cdir*20,edir*20
      Record /G2T_EVENT_ST/  g2t_event
      save                   g2t_event
*
  entry  g2t 
  entry  g2t_start
* show Ntrack in Kuip
  IQUEST(100)=0

 begin 
 G2T_MAIN=0 
 if JVOLUM<=0 { print *,' G2T: no geometry loaded YET'; return; }
*
 If (First) Then
   first = .false.
   fill GTTC(1)           ! g2t control
     version = 1                  ! version number
     nsys    = 21                 ! number of subsystems
     edir    = {'Even','t',' '}   ! event output directory name 
     rdir    = {'Run ',' ',' '}   ! run output directory name 

   fill dete       ! star subsystem
     onoff = 1            ! usage flag
     ctab  = 'svt'        ! table name
     spec  = 'svt'        ! specification type
     csys  = 'SVTT'       ! Geant Subsystem
     cdet  = 'SVTD'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'svt'        ! table name
     spec  = 'svt'        ! specification type
     csys  = 'SVTT'       ! Geant Subsystem
     cdet  = 'SFSD'       ! Sensitive detector

   fill dete       ! star subsystem
     ctab  = 'tpc'        ! table name
     spec  = 'tpc'        ! specification type
     csys  = 'TPCE'       ! Geant Subsystem
     cdet  = 'TPAD'       ! Sensitive detector
   fill dete       ! star subsystem
     cdet  = 'TPAI'       ! Sensitive detector
   fill dete       ! star subsystem
     cdet  = 'TPAO'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'mwc'        ! table name
     spec  = 'mwc'        ! specification type
     csys  = 'TPCE'       ! Geant Subsystem
     cdet  = 'TMSE'       ! Sensitive detector

   fill dete       ! star subsystem
     ctab  = 'ftp'        ! table name
     spec  = 'ftp'        ! specification type
     csys  = 'FTPC'       ! Geant Subsystem
     cdet  = 'FSEN'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'ftp'        ! table name
     spec  = 'ftp'        ! specification type
     csys  = 'FTPC'       ! Geant Subsystem
     cdet  = 'FSEC'       ! Sensitive detector

   fill dete       ! star subsystem
     ctab  = 'ctb'        ! table name
     spec  = 'ctf'        ! specification type
     csys  = 'BTOF'       ! Geant Subsystem
     cdet  = 'BXSA'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'tof'        ! table name
     spec  = 'ctf'        ! specification type
     csys  = 'BTOF'       ! Geant Subsystem
     cdet  = 'BCSB'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'ctb'        ! table name
     spec  = 'ctf'        ! specification type
     csys  = 'BTOF'       ! Geant Subsystem
     cdet  = 'BCSA'       ! Sensitive detector

   fill dete       ! star subsystem
     ctab  = 'rch'        ! table name
     spec  = 'ctf'        ! specification type
     csys  = 'RICH'       ! Geant Subsystem
     cdet  = 'RGAP'       ! Sensitive detector

   fill dete       ! star subsystem
     ctab  = 'rch'        ! table name
     spec  = 'ctf'        ! specification type
     csys  = 'RICH'       ! Geant Subsystem
     cdet  = 'RCSI'       ! Sensitive detector

   fill dete       ! star subsystem
     ctab  = 'emc'        ! table name
     spec  = 'emc'        ! specification type
     csys  = 'CALB'       ! Geant Subsystem
     cdet  = 'CSUP'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'smd'        ! table name
     spec  = 'emc'        ! specification type
     csys  = 'CALB'       ! Geant Subsystem
     cdet  = 'CSDA'       ! Sensitive detector

   fill dete       ! star subsystem
     ctab  = 'eem'        ! table name
     spec  = 'emc'        ! specification type
     csys  = 'ECAL'       ! Geant Subsystem
     cdet  = 'ESCI'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'esm'        ! table name
     spec  = 'emc'        ! specification type
     csys  = 'ECAL'       ! Geant Subsystem
     cdet  = 'EXSE'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'zdc'        ! table name
     spec  = 'emc'        ! specification type
     csys  = 'ZCAL'       ! Geant Subsystem
     cdet  = 'QSCI'       ! Sensitive detector

   fill dete       ! star subsystem
     ctab  = 'vpd'        ! table name
     spec  = 'vpd'        ! specification type
     csys  = 'VPDD'       ! Geant Subsystem
     cdet  = 'VRAD'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'psc'        ! table name
     spec  = '   '        ! specification type
     csys  = 'PMDD'       ! Geant Subsystem
     cdet  = 'VRAD'       ! Sensitive detector
   fill dete       ! star subsystem
     ctab  = 'pgc'        ! table name
   endfill
*
 endif
*   
      G2T_MAIN = 0
      Use GTTC
*     map detm family banks:
      o    = CHAR(0)
      Cdir = gttc_rdir(1)//gttc_rdir(2)//gttc_rdir(3)
      Ld=0; Do i=1,12 { if (Cdir(i:i)==' '|Cdir(i:i)==o) Break; Ld=i; }
      If (Ld>0&Cdir!='none')  Call AGSTRUT (' ',Cdir(1:ld))
*
      Cdir = gttc_edir(1)//gttc_edir(2)//gttc_edir(3)
      Ld=0; Do i=1,12 { if (Cdir(i:i)==' '|Cdir(i:i)==o) Break; Ld=i; }
      edir = Cdir(1:ld)//o;  if (ld>0) Call TDM_CLEAR_ALL(edir)

      IQUEST(1)=IEOTRI+1
      if (.not. ( NTRACK>0 & NVERTX>0 & IEOTRI==0) ) then
         prin1 IQUEST(1); (' g2t quiting with IQUEST(1)=',i7)
         prin1 NTRACK,NVERTX,IEOTRI; (' NTRACK,NVERTX,IEOTRI=',3i7)
         return
      endif
*     map ghea_* headers and  hepe_* particle tables:
      call agstrut('/evnt/@HEPE',Edir)
      call agstrut('/evnt/@GHEA',Edir)

      i = TDM_map_table(edir,'g2t_event'//o,g2t_event_spec//o,1,g2t_event)
      if (ld>0) i = DUI_CDIR (edir)
      i = TDM_NEW_TABLE('g2t_vertex'//o,G2T_VERTEX_SPEC//o,NVERTX)
      i = TDM_NEW_TABLE('g2t_track'//o, G2T_TRACK_SPEC//o, NTRACK)

      names(1)='g2t_vertex'//o
      names(2)='g2t_track'//o

      i = AMI_MODULE_CALL ('g2t_get_kine'//o,2,names)
      prin5 i; (' g2tmain: get_kine done with i=',i6)

      Use GTTC
      ctabo = ' '
      do j=1,GTTC_Nsys

        use dete(j) 

        call  GFNHIT  (dete_Csys(1:3)//'H',dete_Cdet,Nhits)
        Check dete_onoff>0 & Nhits>0
        ctab='g2t_'//dete_ctab(1:3)//'_hit'//o
        prin3 ctab,dete_Csys,dete_Cdet,nhits
        (' in G2Tmain: found ',3(1x,a),'   Nhits = ',i6)

        if (ctabo .ne. ctab ) nnhits = 0
        ctabo  = ctab
        nnhits = nnhits + Nhits
        i = G2T_NEW_TABLE (ctab, dete_spec, nnhits)
        prin5 i; (' g2tmain ===> tdm table for g2t_hits = ',i6)
        
        Check agfhit0 (dete_Csys,dete_Cdet) == ok
        names(3)=ctab
        i = AMI_CALL ('g2t_get_hits'//o,3,names)
        prin5 i; (' g2tmain ===> ami_call g2t_get_hits  = ',i6)

      enddo

      if (ld>0) i = DUI_CDIR('..'//o)
*
      call g2t_get_event(g2t_event)
      IQUEST(1)   = IEOTRI
      IQUEST(100) = NTRACK
      G2T_MAIN = i
      END


      subroutine g2t_get_event(g2t_event)
      implicit   none
#include "g2t_event.inc"
      Integer                TDM_MAP_TABLE
      Integer                NwHEAD,IHEAD(100),Nu,IBUF(100),i,j,in,ip,jn,jp
      Integer                Ntra,IRUN,Ievt,IwtFL,Istat,IDPDG,MOTH(2),IDAU1
      Real                   VERT(4),PP(4),TIME,AMASS,WEIGH
      Character              GeName*8
      INTEGER                NUMPRIM,NUMFINS
      common /g2tevtcnt/     NUMPRIM,NUMFINS 
      Record /G2T_EVENT_ST/  g2t_event
*
*     make g2t_event:
      call GFHEAD  (NwHead,IHEAD,Nu,IBUF)
      call AgNZGETE(1,1,Ntra,IRUN,IEVT,GeName,VERT,IWTFL,WEIGH)
      call AgNZGETP(1,1,1,Istat,IDPDG,PP,AMASS,MOTH,TIME,IDAU1,VERT)

      call VZERO(g2t_event.merge_info,  10)
      call VZERO(g2t_event.polarization,10)
      g2t_event.ge_rndm(1)         = IHEAD(3)
      g2t_event.ge_rndm(2)         = IHEAD(4)
      g2t_event.n_event            = IHEAD(2)

      if (Istat==10) then
        g2t_event.eg_label         = VERT(2)    ! run number
        g2t_event.event_type       = VERT(1)    ! event number
        i= AMASS                                ! Awest.Aeast
        j=(AMASS-i)*1000                        !
        if (i<=100) { in=0;   ip=i;     }       ! west
        if (j<=100) { jn=0;   jp=j;     }       ! east
        if (i> 100) { in=100*i/197; ip=i-in; }  ! west
        if (j> 100) { jn=100*j/197; jp=j-jn; }  ! east
        g2t_event.n_part_neut_east = jn  
        g2t_event.n_part_neut_west = in
        g2t_event.n_part_prot_east = jp
        g2t_event.n_part_prot_west = ip
        g2t_event.n_run            = MOTH(1)
        g2t_event.b_impact         = PP(1)
        g2t_event.phi_impact       = PP(2)
      endif
      g2t_event.n_track_eg_fs      = NUMPRIM
      g2t_event.n_track_prim       = NUMFINS
      g2t_event.prim_vertex_p      = 1
      g2t_event.time_offset        = 0

      END


      Integer FUNCTION G2T_NEW_TABLE(name,spec,L)
      implicit   none
      character  name*(*),spec*(*),o*1
      integer    TDM_NEW_TABLE,i,L 

*include "PAM.inc"
#include "g2t_hits.inc"
#include "g2t_svt_hit.inc"
#include "g2t_tpc_hit.inc"
#include "g2t_mwc_hit.inc"
#include "g2t_ctf_hit.inc"
#include "g2t_emc_hit.inc"
#include "g2t_smd_hit.inc"
#include "g2t_eem_hit.inc"
#include "g2t_esm_hit.inc"
#include "g2t_ftp_hit.inc"
#include "g2t_vpd_hit.inc"

      i = 0
      o = char(0)  
      if (spec=='svt') { i=TDM_NEW_TABLE (name, g2t_svt_hit_spec//o, L) } 
  elseif (spec=='tpc') { i=TDM_NEW_TABLE (name, g2t_tpc_hit_spec//o, L) } 
  elseif (spec=='mwc') { i=TDM_NEW_TABLE (name, g2t_mwc_hit_spec//o, L) } 
  elseif (spec=='ctf') { i=TDM_NEW_TABLE (name, g2t_ctf_hit_spec//o, L) } 
  elseif (spec=='emc') { i=TDM_NEW_TABLE (name, g2t_emc_hit_spec//o, L) } 
  elseif (spec=='smd') { i=TDM_NEW_TABLE (name, g2t_smd_hit_spec//o, L) } 
  elseif (spec=='eem') { i=TDM_NEW_TABLE (name, g2t_eem_hit_spec//o, L) } 
  elseif (spec=='esm') { i=TDM_NEW_TABLE (name, g2t_esm_hit_spec//o, L) } 
  elseif (spec=='ftp') { i=TDM_NEW_TABLE (name, g2t_ftp_hit_spec//o, L) } 
  elseif (spec=='vpd') { i=TDM_NEW_TABLE (name, g2t_vpd_hit_spec//o, L) }
  else                 { i=TDM_NEW_TABLE (name, g2t_hits_spec//o,    L) }
  G2T_NEW_TABLE=i
  end


