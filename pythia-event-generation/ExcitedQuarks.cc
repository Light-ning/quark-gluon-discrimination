// main75.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program to study jets in Dark Matter production.

#include <iostream>

#include "Pythia8/Pythia.h"

// head file for jet clustering
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

// head file for ROOT histogram plotting
#include "TH1.h"
#include "TVirtualPad.h"
#include "TApplication.h"
#include "Tfile.h"

using namespace Pythia8;

int main(int argc, char* argv[]) {

  // Generator. Process selection. Initialization. Event shorthand.
  Pythia pythia;

  pythia.readFile("MC301299.cmnd");
  pythia.init();

  // Fastjet analysis - select algorithm and parameters.
  double Rparam = 0.4;
  fastjet::Strategy               strategy = fastjet::Best;
  fastjet::RecombinationScheme    recombScheme = fastjet::E_scheme;
  fastjet::JetDefinition         *jetDef = NULL;
  jetDef = new fastjet::JetDefinition( fastjet::antikt_algorithm, Rparam,
           recombScheme, strategy);

  // Fastjet input.
  std::vector <fastjet::PseudoJet> fjInputs;

  int nEvent = 1000;
  double MassMax = 4000.;
  double pTMax = 3000.;

  // settings for using ROOT histograms
  TApplication theApp("hist", &argc, argv);
  TFile *outFile = new TFile("MC301299.root", "RECREATE");

  // Histograms of parton level
  TH1F *HMassR = new TH1F("HMassR", "Mass of new resonances", 100, 0., MassMax);
  TH1F *HptR = new TH1F("HptR", "pT of new resonances", 100, 0., pTMax);
  TH1F *HptD1 = new TH1F("HptD1", "pT of daughter 1", 100, 0., pTMax);
  TH1F *HptD2 = new TH1F("HptD2", "pT of daughter 2", 100, 0., pTMax);
  TH1F *HetaR = new TH1F("HetaR", "eta of new resonances", 100, 0., 2.);
  TH1F *HetaD1 = new TH1F("HetaD1", "eta of daughter 1", 100, 0., 2.);
  TH1F *HetaD2 = new TH1F("HetaD2", "eta of daughter 2", 100, 0., 2.);

  // Histograms of particle level
  TH1F *HMassC = new TH1F("HMassC", "Mass of dijet candidates", 100, 0., MassMax);
  TH1F *HptC = new TH1F("HptC", "pT of dijet candidates", 100, 0., pTMax);
  TH1F *HptJ1 = new TH1F("HptJ1", "pT of leading jets", 100, 0, pTMax);
  TH1F *HptJ2 = new TH1F("HptJ2", "pT of sub leading jets", 100, 0, pTMax);
  TH1F *HetaC = new TH1F("HetaC", "eta of dijet candidates", 100, 0., 2.);
  TH1F *HetaJ1 = new TH1F("HetaJ1", "eta of leading jets", 100, 0., 2.);
  TH1F *HetaJ2 = new TH1F("HetaJ2", "eta of sub leading jets", 100, 0., 2.);
  TH1F *HYstar = new TH1F("HYstar", "Ystar", 100, 0., 0.6);

  int iErr = 0;

  // Begin event loop. Generate event. Skip if error.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if (!pythia.next()) {
      if (++iErr < 100) continue;
      else {
        cout << "Too many errors" << endl;
        break;
      }
    }

    // find the first new resonance at the parton level
    int iRes = 0;
    while (pythia.event[iRes].idAbs() != 4000001
	   && pythia.event[iRes].idAbs() != 4000002
	   && iRes < pythia.event.size()) iRes++;
    // find the last appearance of the new resonance found above
    while (pythia.event[iRes].daughter1() == pythia.event[iRes].daughter2()
	   && iRes < pythia.event.size())
      iRes = pythia.event[iRes].daughter1();

    if (iRes >= pythia.event.size()){
      cout << "No new resonance, or the new resonance has no daughters in event"
	   << iEvent << endl;
      continue;
    }

    // index of daughters
    int iD1 = pythia.event[iRes].daughter1();
    int iD2 = pythia.event[iRes].daughter2();

    HMassR -> Fill(pythia.event[iRes].m());
    HptR -> Fill(pythia.event[iRes].p().pT());
    HptD1 -> Fill(pythia.event[iD1].p().pT());
    HptD2 -> Fill(pythia.event[iD2].p().pT());
    HetaR -> Fill(pythia.event[iRes].p().eta());
    HetaD1 -> Fill(pythia.event[iD1].p().eta());
    HetaD2 -> Fill(pythia.event[iD2].p().eta());

    fjInputs.clear();

    // Loop over event record to decide what to pass to FastJet.
    for (int i = 0; i < pythia.event.size(); ++i) {
      // Final state only.
      if (!pythia.event[i].isFinal()) continue;

      // No neutrinos or DM.
      // 12 for electron neutrino, 14 for muon neutrino, 16 for tauon neutrino, 52 for dark matter with spin 1 / 2
      // Pdgid can be accessed in https://twiki.cern.ch/twiki/bin/view/Main/PdgId
      if ( pythia.event[i].idAbs() == 12 || pythia.event[i].idAbs() == 14
	|| pythia.event[i].idAbs() == 16)
	continue;

      // Only |eta| < 3.6.
      //      if (abs(pythia.event[i].eta()) > 3.6) continue;
      // still don't know how to cut.
      // put every particle into list?

      // Store as input to Fastjet.
      fjInputs.push_back( fastjet::PseudoJet( pythia.event[i].px(),
        pythia.event[i].py(), pythia.event[i].pz(), pythia.event[i].e() ) );

    }

    // Check that event contains analyzable particles.
    if (fjInputs.size() == 0) {
      cout << "Error: event with no final state particles" << endl;
      continue;
    }

    // Run Fastjet algorithm.
    vector <fastjet::PseudoJet> inclusiveJets, sortedJets;
    fastjet::ClusterSequence clustSeq(fjInputs, *jetDef);

    // Extract inclusive jets sorted by pT (note minimum pT of 20.0 GeV).
    inclusiveJets = clustSeq.inclusive_jets(20.0);
    sortedJets    = sorted_by_pt(inclusiveJets);

    // need at least 2 jets to finish leading jet and sub-leading jet analysis
    if(sortedJets.size() < 2) {
      cout << " No enough jets found in event " << iEvent << endl;
      continue;
    }

    // cut the events where jets have too little pT
    //    if (sortedJets[0].pt() <= 420 || sortedJets[1].pt() <= 150)
    //      continue;

    Vec4 pJ1(sortedJets[0].px(), sortedJets[0].py(), sortedJets[0].pz(), sortedJets[0].e());
    Vec4 pJ2(sortedJets[1].px(), sortedJets[1].py(), sortedJets[1].pz(), sortedJets[1].e());
    Vec4 pC = pJ1 + pJ2;

    double Ystar = (sortedJets[0].rap() - sortedJets[1].rap()) / 2;

    // cut the events where jet eta or ystar is too large
    //    if (abs(etaC) >= 2 || abs(Ystar) >= 0.6) continue;

    HMassC -> Fill(pC.mCalc());
    HptC -> Fill(pC.pT());
    HptJ1 -> Fill(sortedJets[0].pt());
    HptJ2 -> Fill(sortedJets[1].pt());
    HetaC -> Fill(pC.eta());
    HetaJ1 -> Fill(sortedJets[0].eta());
    HetaJ2 -> Fill(sortedJets[1].eta());
    HYstar -> Fill(Ystar);
    
  // End of event loop. Statistics. Histogram.
  }

  HMassR -> Write();
  HptR -> Write();
  HptD1 -> Write();
  HptD2 -> Write();
  HetaR -> Write();
  HetaD1 -> Write();
  HetaD2 -> Write();

  HMassC -> Write();
  HptC -> Write();
  HptJ1 -> Write();
  HptJ2 -> Write();
  HetaC -> Write();
  HetaJ1 -> Write();
  HetaJ2 -> Write();
  HYstar -> Write();
  
  delete outFile;
  pythia.stat();

  // Done.
  return 0;
}
