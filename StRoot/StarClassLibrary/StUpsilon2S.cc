/***************************************************************************
 *
 * $Id: StUpsilon2S.cc,v 1.1 2010/01/28 19:33:20 jwebb Exp $
 *
 * Author: Thomas Ullrich, May 99 (based on Geant4 code, see below) 
 ***************************************************************************
 *
 * The design of the StParticleDefinition class and all concrete
 * classes derived from it is largely based on the design of the 
 * G4ParticleDefinition class from Geant4 (RD44).
 * Although the code is in large parts different (modified or rewritten)
 * and adapted to the STAR framework the basic idea stays the same.
 *
 ***************************************************************************
 *
 * $Log: StUpsilon2S.cc,v $
 * Revision 1.1  2010/01/28 19:33:20  jwebb
 * Added the upsilon resonances to the StarClassLibrary.  This makes the particles
 * available by name and by PDG id in StParticleTable.
 *
 * Revision 1.1  1999/05/14 18:47:50  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include "StUpsilon2S.hh" 
#include "PhysicalConstants.h"

StUpsilon2S::StUpsilon2S(const string  &  aName,  
	       double           mass,     
	       double           width,
	       double           charge,   
	       int              iSpin,
	       int              iParity,
	       int              iConjugation,
	       int              iIsospin,   
	       int              iIsospinZ, 
	       int              gParity,
	       const string  &  pType,
	       int              lepton,
	       int              baryon,
	       int              encoding,
	       bool             stable,
	       double           lifetime)
    : StMeson(aName, mass, width, charge, iSpin, iParity,
	      iConjugation, iIsospin, iIsospinZ, gParity,
	      pType, lepton, baryon, encoding, stable,
	      lifetime) {/* noop */}

// ......................................................................
// ...                 static member definitions                      ...
// ......................................................................
//     
//    Arguments for constructor are as follows
//               name             mass          width         charge
//             2*spin           parity  C-conjugation
//          2*Isospin       2*Isospin3       G-parity
//               type    lepton number  baryon number   PDG encoding
//             stable         lifetime   
//
StUpsilon2S StUpsilon2S::mUpsilon2S
(
 "Upsilon2S",     10.02326*GeV,        31.98*MeV,          0., 
 2,              -1,            -1,          
 0,               0,            -1,             
 "meson",               0,             0,      100553,
 false,          0.0*nanosecond
);
