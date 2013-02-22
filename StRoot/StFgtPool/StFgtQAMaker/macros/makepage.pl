#!/usr/bin/perl

$anadir="/ldaphome/akio/fgt";
$datadir="/evp/a";
$year=14;
$start = `date -d "Feb 6 00:00:00" +%s`; $start=~s/\n//g;

$yday=0;
$day=0;
$debug=0;
$submit=0;

$out = `ps -elf | grep $anadir/makepage.pl | grep -v grep | grep -v emacs | grep -v csh | wc -l`;
$out=~s/\n//g;
if($debug) {print("ps reports $out $anadir/makepage.pl jobs found\n");}
if ($out > 1) {
    print("There are other $anadir/makepage.pl running. Stop\n");
    `ps -elf | grep $anadir/makepage.pl | grep -v grep | grep -v emacs | grep -v csh`;
    exit;
}else{
    print("No $anadir/makepage.pl running. Go ahead\n");
}

if($#ARGV>-1) {
    if($ARGV[0] eq "all") {$yday=-1;} 
    elsif($ARGV[0] eq "0") {$yday=0;} 
    else {
	$yday=$ARGV[0]; $day=substr($yday,2,3); $year=substr($yday,0,2); 
	if($yday<14000 || $yday>20000) {print("yday=$yday is out of range\n"); exit; }
    }    
}
if($#ARGV>0) {
    if($ARGV[1] eq "submit") {$submit=1;}
    if($ARGV[1] eq "run")    {$submit=2;}
}
print("year=$year day=$day submit=$submit\n");

if (-e "$anadir/html")     {} else {`mkdir $anadir/html`;}
if (-e "$anadir/db")       {} else {`mkdir $anadir/db`;}
$today = ` date +%s`;             $today=~s/\n//g;
$dtoday = ` date +"%Y %b %d %a"`; $dtoday=~s/\n//g;                      
$itoday = ` date +%j`;            $itoday=~s/\n//g;          
if($debug) {print "today = $today $dtoday $itoday\n";}

$dstart=`date -d \"UTC 1970-01-01 $start secs\" +"%Y %b %d %a"`;  $dstart=~s/\n//g;
$istart=`date -d \"UTC 1970-01-01 $start secs\" +%j`;  $istart=~s/\n//g;
if($debug) {print "start = $start $dstart $istart\n";}

open(OUT,"> $anadir/html/index.php");
print(OUT "<HTML><head><META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html\">");
print(OUT "<title>FGT Monitor</title></header><BODY><meta http-equiv=\"Refresh\" content=\"60\">\n");
print(OUT "<?php require('links.php'); links(); ?>\n");

print(OUT "<H1>FGT Monitor</H1>\n");
print(OUT "<table border=1><tr><td>day</td><td>date</td><td># of runs</td><td># of runs with FGT</td></tr>\n");

$diff = 60*60*24+1;
for ($d = $today; $d>=$start; $d-=60*60*24){
    $dd=`date -d \"UTC 1970-01-01 $d secs\" +"%Y %b %d %a"`;
    $ddd=`date -d \"UTC 1970-01-01 $d secs\" +"%b %d"`;
    $id=`date -d \"UTC 1970-01-01 $d secs\" +%j`;
    $dd=~s/\n//g;
    $id=~s/\n//g;
    $id="$year$id";    
    print("id=$id   ");
    
    if (-e "$anadir/$id") {} else {`mkdir $anadir/$id`;}
    if ($d == $today) {`runs $id`;}
    if (-e "$anadir/$id/run.txt") {} else {`runs $id`;}
    
    $nrun=`wc -l $id/run.txt | cut -d " " -f 1`;
    $nrun=~s/\n//g;
    $nfgt=`wc -l $id/fgtrun.txt | cut -d " " -f 1`;
    $nfgt=~s/\n//g;
    printf("nrun=%4d nfgtrun=%4d\n",$nrun,$nfgt);

    if($nfgt==0) {next;}    
    print(OUT "<tr><td><a href=\"$id.php\">$id</a></td><td>$dd</td><td>$nrun</td><td>$nfgt</td>\n");    

    $diff=$today-$d;
    if($diff<60*60*24) {$isToday=1;}
    else {$isToday=0;}
    # if($debug) {print("isToday=$isToday\n");}
    if($yday==-1 || ($yday==0 && $isToday==1) || $yday==$id){	
	open(RUNS,"$anadir/$id/run.txt");
	@runlogdb=<RUNS>;
	%runlist=();
	@runs=();
	$nrun=0;
	foreach $_ (@runlogdb){
	    @rundb=split;
	    $run=$rundb[0];
	    $runlist{$run}=1;
	    $runs[$nrun]=$run;
	    $nrun++;
	}
	print("Found $nrun runs in DB\n");
	open(FRUNS,"$anadir/$id/fgtrun.txt");
	@frunlogdb=<FRUNS>;
	$nfrun=0;
	foreach $_ (@frunlogdb){
	    @frundb=split;
	    $frun=$frundb[0];
	    $runlist{$frun}=2;
	    $fruns[$nfrun]=$frun;
	    $nfrun++;
	}
	print("Found $nfrun runs with FGT\n");
	@sortruns=sort(@fruns);
	@revruns=reverse(@sortruns);
	
	$plotdir="$anadir/www/$id/";
	if (-e "$plotdir") {} else {`mkdir -p $plotdir`;}
	
	print("Creating $id.php\n");
	open(OUT1,"> $anadir/html/$id.php");
	
	print(OUT1 "<HTML><head><META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html\">");
	print(OUT1 "<title>FGT Monitor Day$id</title></header><BODY>\n");	    
	print(OUT1 "<?php require('links.php'); links(); ?>\n");
	
	print(OUT1 "<H1>FGT Monitor Day$id ($dd)</H1>\n");
	print(OUT1 "<a href=\"index.php\">Back to day list</a>\n");
	print(OUT1 "<table border=1><tr><td>Run</td><td>Time</td><td>Length[sec]</td><td>Config</td><td>Type</td>\n");
	print(OUT1 "<td>pdf</td><td>Plots</td><td>ADCvsTB</td><td>Trace</td><td>NHitStrip</td><td>PhiHits</td><td>RHits</td><td>NCluster</td><td>ClusterSize</td><td>Charge</td><td>ChargeAsy</td><td>XY</td></tr>\n");
	
	$tgt=" onclick=\"openwin(this.href); return false;\"";
	$siz=" Width=100 ";
	$p="png";
	$t="tiny.png";
	foreach $run (@sortruns){
	    
	    $donefile=0;
	    if(-e "$id/$run.done") {$donefile++;}
	    
	    $_=`grep $run $anadir/$id/run.txt`;
	    @db = split;
	    $date=@db[1];
	    $time=@db[2];
	    $begin=@db[3];
	    $end=@db[4];
	    $config=@db[5];
	    $type=@db[6];
	    $now=time;
	    $length = $end - $begin;
	    $ago = $now - $end;
	    if($end==0){$length=-1; $ago=-1;}
	    if($type eq "pedestal") {next;}
	    if($config eq "pedAsPhys_tcd_only" || $config eq " fgtPedestals") {$type="pedestal";}
	    
	    if($donefile==0 && $ago>60 && ($type eq "pedestal" || $length>180) ) {
		
		$evpcount=0;
		if(-d "$datadir/$run") {
		    $evpcount = `ls $datadir/$run/?* | wc -l`;
		    $evpcount =~ s/\n//g;
		}
		
		printf("Run$run has no $id/$run.done, $datadir/$run/ has $evpcount events, and run ends $ago (sec) ago, and submit=$submit\n");
		if($evpcount<50 && $submit>0) {		    
		    `touch $id/$run.done`;
		    if($submit==2){
			`makeplot $run $type`;
		    }elsif($submit==1){
			$condor="$id/condor/submit_$run.txt";
			if (-e "$condor") {} else {`\rm $condor`;}
			print("Creating $condor\n");
			open(OUTC, "> $condor\n");
			print(OUTC "Executable   = makeplot\n");
			print(OUTC "Universe     = vanilla\n");
			print(OUTC "notification = never\n");
			print(OUTC "getenv       = True\n");
			print(OUTC "Priority    = +1\n");
			print(OUTC "Arguments = $run $type\n");
			print(OUTC "Log    = $anadir/$id/log/$run.log\n");
			print(OUTC "Output = $anadir/$id/log/$run.log\n");
			print(OUTC "Error  = $anadir/$id/log/$run.log\n");
			print(OUTC "Queue\n\n");
			close(OUTC);
			print(OUTC "\n");
			$cmd = "condor_submit ${condor}\n";
			print $cmd;
			#system("condor_submit ${condor}\n");
		    }
		}
	    }
	    
	    print(OUT1 "<tr><td><a href=\"http://online.star.bnl.gov/RunLog/index.php?r=$run\">$run</a></td>");	   
	    print(OUT1 "<td>$time</td><td>$length</td><td>$config</td><td>$type</td>");
	    
	    $f="fgtScopeTrace_${run}";if(-e "$plotdir/$f.pdf"){print(OUT1 "<td><a href=\"$id\/$f.pdf\" $tgt>pdf                         </a></td>");} else {print(OUT1 "<td></td>");}
	    if($type eq "pedestal"){
	    $f="${run}_ped";          if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    }else{
	    $f="${run}_plot";         if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
     	    }
	    $f="${run}_ADCvsTB";      if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="fgtScopeTrace_${run}";if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="${run}_NHitStrip";    if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="${run}_PhiHit";       if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="${run}_RHit";         if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="${run}_NCluster";     if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="${run}_ClusterSize";  if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="${run}_ClusterCharge";if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="${run}_ChargeAsy";    if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    $f="${run}_XY";           if(-e "$plotdir/$f.$p") {print(OUT1 "<td><a href=\"$id\/$f.$p\" $tgt><img src=\"$id\/$f.$t\" $siz></a></td>");} else {print(OUT1 "<td></td>");}
	    print(OUT1 "<tr>\n");
	}
	print(OUT1 "</table>\n");
	print(OUT1 "</BODY></HTML>\n");
    }
}
print(OUT "</table>\n");
print(OUT "</BODY></HTML>\n");

`\mv html/*.php $anadir/www/`;
