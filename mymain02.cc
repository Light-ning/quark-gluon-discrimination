// main75.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program to study jets in Dark Matter production.

#include <iostream>

#include "Pythia8/Pythia.h"

#include "TH1.h"

#include "TVirtualPad.h"
#include "TApplication.h"
#include "Tfile.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

using namespace Pythia8;

int main(int argc, char* argv[]) {

  // Generator. Process selection. Initialization. Event shorthand.
  Pythia pythia;

  pythia.readFile("MC301295.cmnd");
  pythia.init();

  double Rparam = 0.4;
  fastjet::Strategy strategy = fastjet::Best;
  fastjet::RecombinationScheme recombScheme = fastjet::E_scheme;
  fastjet::JetDefinition *jetDef = NULL;
  jetDef = new fastjet::JetDefinition(fastjet::antikt_algorithm, Rparam, recombScheme, strategy);

  std::vector <fastjet::PseudoJet> fjInputs;

  TApplication theApp("hist", &argc, argv);
  TFile* outFile = new TFile("myhist01.root", "RECREATE");
  TH1F *HMassC = new TH1F("HMassC", "Mass of dijet candidates", 100, 0., 1000.);

  int nEvent = 1000;

  for (int iEvent = 0; iEvent < nEvent; iEvent++){
    cout << iEvent << endl;
    if (!pythia.next()) continue;
    fjInputs.clear();
    for (int i = 0; i < pythia.event.size(); i ++){
      if (!pythia.event[i].isFinal()) continue;
      if (pythia.event[i].idAbs() == 12 || pythia.event[i].idAbs() == 14 || pythia.event[i].idAbs() == 16) continue;
      fjInputs.push_back(fastjet::PseudoJet(pythia.event[i].px(), pythia.event[i].py(), pythia.event[i].pz(), pythia.event[i].e()));
      if (fjInputs.size() == 0){
	cout << "Error: event with no final state particles" << endl;
	continue;
      }
    }

    vector <fastjet::PseudoJet> inclusiveJets, sortedJets;
    fastjet::ClusterSequence clustSeq(fjInputs, *jetDef);

    inclusiveJets = clustSeq.inclusive_jets(20.0);
    sortedJets = sorted_by_pt(inclusiveJets);

    if (sortedJets.size() < 2){
      cout << "No enough jets found in event " << iEvent << endl;
      continue;
    }

    Vec4 pJ1(sortedJets[0].px(), sortedJets[0].py(), sortedJets[0].pz(), sortedJets[0].e());
    Vec4 pJ2(sortedJets[1].px(), sortedJets[1].py(), sortedJets[1].pz(), sortedJets[1].e());
    Vec4 pC = pJ1 + pJ2;

    HMassC -> Fill(pC.mCalc());
  }

  HMassC->Draw();
  gPad -> WaitPrimitive();
  HMassC -> Write();
  delete outFile;
  
  pythia.stat();

  return 0;
}
