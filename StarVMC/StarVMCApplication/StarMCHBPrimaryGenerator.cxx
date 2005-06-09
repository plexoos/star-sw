// $Id: StarMCHBPrimaryGenerator.cxx,v 1.1 2005/06/09 20:13:47 fisyak Exp $
#include "StarMCHBPrimaryGenerator.h"
#include "tables/St_particle_Table.h"
#include "TDatabasePDG.h"

ClassImp(StarMCHBPrimaryGenerator);
struct HepEvent_t {
  Int_t            itrac;
  Char_t           istat;
  particle_st          p;
};
//_____________________________________________________________________________
StarMCHBPrimaryGenerator::StarMCHBPrimaryGenerator(const Char_t *HbFile, TDataSet *Data) : 
  StarMCPrimaryGenerator(), fHBFileName(HbFile), fHBTree(0), fHBFile(0), 
  fEntry(-1), fnEntries(0), fHbtId(999), fData(Data) 
{
  fHBFile = new TFile(fHBFileName.Data());
  assert(fHBFile);
  cout << "StarMCHBPrimaryGenerator open " << fHBFileName << endl;
  if (Debug()) fHBFile->ls();
}
//_____________________________________________________________________________
void StarMCHBPrimaryGenerator::GeneratePrimary(const TVector3& origin) {     
#if 0
  // Add one primary particle to the user stack (derived from TVirtualMCStack).
 // Track ID (filled by stack)
 Int_t ntr;
 
 // Option: to be tracked
 Int_t toBeDone = 1; 
 
 // Particle type
 Int_t pdg  = fId;
 
 // Polarization
 Double_t polx = 0.; 
 Double_t poly = 0.; 
 Double_t polz = 0.; 

 // Position
 Double_t vx  = origin.X(); 
 Double_t vy  = origin.Y(); 
 Double_t vz =  origin.Z(); 
 Double_t tof = 0.;
 // Energy (in GeV)
 Double_t pT        = fpT_min + (fpT_max - fpT_min)*gRandom->Rndm();
 Double_t eta       = fEta_min + (fEta_max - fEta_min)*gRandom->Rndm();
 Double_t phi       = fPhi_min + (fPhi_max - fPhi_min)*gRandom->Rndm();
 // Particle momentum
 Double_t px, py, pz;
 px = pT*TMath::Cos(phi); 
 py = pT*TMath::Sin(phi);
 pz = pT*TMath::SinH(eta);
 // Double_t kinEnergy = 0.050;  
 Double_t mass = TDatabasePDG::Instance()->GetParticle(fId)->Mass();
 Double_t e  = TMath::Sqrt(mass*mass + pz*pz + pT*pT);
 // Add particle to stack 
 fStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
                  kPPrimary, ntr, 1., 0);
#endif
}
//_____________________________________________________________________________
void StarMCHBPrimaryGenerator::GeneratePrimaries() {// generate primaries from HBOOK NTuple
  static HepEvent_t event;
  static St_particle *particle = 0;
  static Double_t ct = TMath::Ccgs()/0.1; // mm s^-1
  Double_t polx = 0, poly = 0, polz = 0;
  // Add one primary particle to the user stack (derived from TVirtualMCStack).
  // Track ID (filled by stack)
  Int_t ntr;
  // Option: to be tracked
  Int_t toBeDone = 1; 
  if (! fHBTree) {
    fHBTree = (TTree*) fHBFile->Get(Form("h%i",fHbtId));
    assert(fHBTree);
    fHBTree->SetBranchAddress("itrac",&event.itrac);
    fHBTree->SetBranchAddress("istat",&event.istat);
    fHBTree->SetBranchAddress("ipdg",&event.p.idhep);
    fHBTree->SetBranchAddress("moth1",&event.p.jmohep[0]);
    fHBTree->SetBranchAddress("moth2",&event.p.jmohep[1]);
    fHBTree->SetBranchAddress("idau1",&event.p.jdahep[0]);
    fHBTree->SetBranchAddress("idau2",&event.p.jdahep[1]);
    fHBTree->SetBranchAddress("Pxyz",&event.p.phep[0]);
    fHBTree->SetBranchAddress("ener",&event.p.phep[3]);
    fHBTree->SetBranchAddress("mass",&event.p.phep[4]);
    fHBTree->SetBranchAddress("Vxyz",&event.p.vhep[0]);
    fHBTree->SetBranchAddress("Vtime",&event.p.vhep[3]);
    fnEntries = fHBTree->GetEntriesFast();
    fEntry = 0;
  }
  if (fData) {
    particle = (St_particle *) fData->Find("particle");
    if (particle) SafeDelete(particle);
    particle = new St_particle("particle",100);
    fData->Add(particle);
  }
    
  Int_t nbytes = 0, nb = 0;
  if (Debug()) 
    printf("entry  trk ist   ipdg   m1    m2  d1   d2    Pxyz[3]                    "
	   " ener         mass      Vxyz[3]                       Vtime\n");
  Int_t NpHEP = 0;
  Int_t NPrim = 0;
  Int_t IdGen = 0;
  for (; fEntry<fnEntries;fEntry++) {
    Long64_t ientry = fHBTree->LoadTree(fEntry);
    if (ientry < 0) break;
    nb = fHBTree->GetEntry(fEntry);   nbytes += nb;
    event.p.isthep = event.istat;
    if (event.istat > 10 && event.p.idhep > 999990) {
      NpHEP = event.itrac;
      if (event.p.idhep  <= 999996) event.p.idhep = 999997 - event.p.idhep;
    }
    if (Debug()) {
      printf("%5i%5i%4i%7i%5i%5i%5i%5i",
	     (int) ientry, event.itrac, event.p.isthep, event.p.idhep, 
	     event.p.jmohep[0],event.p.jmohep[1], 
	     event.p.jdahep[0],event.p.jdahep[1]);
      printf("%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f%10.3f\n",
	     event.p.phep[0], event.p.phep[1], event.p.phep[2], event.p.phep[3], event.p.phep[4], 
	     event.p.vhep[0], event.p.vhep[1], event.p.vhep[2], event.p.vhep[3]);	     
    }
    if (particle) {particle->AddAt(&event.p.isthep); IdGen = particle->GetNRows();}
    if (event.istat < 10) {
      if ( TDatabasePDG::Instance()->GetParticle(event.p.idhep)) {
	// Add particle to stack 
	Int_t parent = event.p.jmohep[0] - 1;
	toBeDone = 1;
	if (event.p.isthep >= 2 || event.p.jdahep[0] || event.p.jdahep[1]) toBeDone = 0;
	((StarMCStack *)fStack)->PushTrack(toBeDone, parent, event.p.idhep, 
					   event.p.phep[0], event.p.phep[1], event.p.phep[2], event.p.phep[3],// px, py, pz, e, 
					   fOrigin.x()+event.p.vhep[0]/10, fOrigin.y()+event.p.vhep[1]/10, fOrigin.z()+event.p.vhep[2]/10, 
					   event.p.vhep[3]/ct,//vx, vy, vz, tof, (mm->cm) 
					   polx, poly, polz, 
					   kPPrimary, ntr, 1., IdGen); // mech, &ntr, weight, status
      } else {
	if (Debug())
	  cout << "StarMCHBPrimaryGenerator::GeneratePrimaries\t" << event.p.idhep << " is not in TDatabasePDG. skip it." << endl;
      }
      NPrim++;
      if (event.itrac == -1 || event.itrac == 65535 || NPrim == NpHEP) {fEntry++; break;}
    }
  }
  
  Int_t NPrimary = fStack->GetNtrack();
  ((StarMCStack *)fStack)->SetNprimary(NPrimary);
}
