#! /usr/local/bin/tcsh -f
set list = "linkdir share hp_ux102 i386_linux2 i386_redhat50 i386_redhat60 sun4x_55 sun4x_56"
foreach dir ($list)
#  chmod -R +w /afs/rhic/.asis/$dir; 
  gmake -f $STAR/mgr/asis.mk -r DIR=/afs/cern.ch/asis/$dir >& $dir.log &
end
#end of script

