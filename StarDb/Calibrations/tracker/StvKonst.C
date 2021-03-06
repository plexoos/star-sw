TDataSet *CreateTable() { 
// -----------------------------------------------------------------
// StvTpcOuterHitErrs Allocated rows: 1  Used rows: 1  Row size: 48 bytes
//  Table: StvKonst_st[0]--> StvKonst_st[0]
// ====================================================================
// ------  Test whether this table share library was loaded ------
  if (!TClass::GetClass("St_StvKonst")) return 0;
StvKonst_st row;
St_StvKonst *tableSet = new St_StvKonst("StvKonst",1);
//
memset(&row,0,tableSet->GetRowSize());
//  double myProb = TMath::Prob(30**3,2);
  double myProb = TMath::Prob(30,2);
//double myFaktor = 3;
  double myFaktor = 1;
  row.mXi2Hit = ROOT::Math::chisquared_quantile_c(myProb,2)    *myFaktor;	//Xi2 to accept new hit
  row.mXi2Trk = ROOT::Math::chisquared_quantile_c(myProb,30)/30*myFaktor;//Xi2 to accept new track
  row.mXi2Vtx = ROOT::Math::chisquared_quantile_c(myProb,2);		//Xi2 to accept vertex
  row.mXi2Joi = ROOT::Math::chisquared_quantile_c(myProb,5)*myFaktor;	//Xi2 in Refit join left & right subtrack
  row.mXi2Hlx = row.mXi2Hit*9;  //Xi2 in Helix, .
  row.mDca2dZeroXY = 6.;	//max 2d dca to X=Y=0  for primary track
  row.mDca3dVertex = 3.;	//max 3d dca to vertex for primary track
  row.mMaxCurv     = 1./33;	//Maximal allowed curvature(33cm radius)
  row.mMinP2  = 0.003*0.003;	//Geant3 cut for too small momentum**2	
  row.mMaxPti   =  20;		/*Maximal allowed 1/pt or Pt = 50MeV*/
  row.mMaxRes	= 0.5;		/*Maximal allowed residual */
  row.mCoeWindow=  9.;		/*Maximal window to search hits in errors*/
  row.mMaxWindow= 10.;		/*Maximal window to search hits in centimeters*/

//		MidEta
  row.mMinSeedHits = 5;		/*Min number of seed hits allowed*/
  row.mMaxSeedHits =10;		/*Max number of seed hits allowed*/
  row.mMinHits 	= 5;		/*Min number of hits allowed*/
  row.mNorHits 	=10;		/*Normal number of hits allowed*/
  row.mGoodHits =15;		/*Good number of hits */
//	MidEta hitCount hitCount hitCount hitCount hitCount 
  row.mMinTotHits =3;       	/*Min number hits for track*/
  row.mMinGoodHits=3;       	/*Min number good hits for track*/
  row.mMinContHits=3;       	/*Min length of good hit sequence*/
//row.mMaxContNits=15;      	/*Max length of acceptable non hit sequence*/
  row.mMaxContNits=10;      	/*Max length of acceptable non hit sequence*/
  row.mMaxTotNits =20;      	/*Max number of acceptable non hits*/

  row.mRxyMax = 207;		//Max radius for tracking
  row.mZMax   = 220;		//Max Z      for tracking
//		ForwEta version
  row.mMinSeedHitsFw = 4;	/*Min number of seed hits allowed*/
  row.mMaxSeedHitsFw = 6;	/*Max number of seed hits allowed*/
  row.mMinHitsFw = 4;		/*Min number of hits allowed*/
  row.mNorHitsFw = 5;		/*Normal number of hits allowed*/
  row.mGoodHitsFw =5;		/*Good number of hits */

//		ForwEta hitCount hitCount hitCount hitCount hitCount 
  row.mMinTotHitsFw =3;       	/*Min number hits for track*/
  row.mMinGoodHitsFw=3;       	/*Min number good hits for track*/
  row.mMinContHitsFw=3;       	/*Min length of good hit sequence*/
  row.mMaxContNitsFw=1;      	/*Max length of acceptable non hit sequence*/
  row.mMaxTotNitsFw =2;      	/*Max number of acceptable non hits*/
  row.mRxyMaxFw = 207;		//Max radius for tracking
  row.mZMaxFw   = 700;		//Max Z      for tracking


tableSet->AddAt(&row);
// ----------------- end of code ---------------
 return (TDataSet *)tableSet;
}
