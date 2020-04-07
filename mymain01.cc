// main75.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program to study jets in Dark Matter production.

#include "Pythia8/Pythia.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

using namespace Pythia8;

int main() {

  // Generator. Process selection. Initialization. Event shorthand.
  Pythia pythia;

  pythia.readFile("MC301295.cmnd");
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
  double MassMax = 1000.;
  double pTMax = 1000.;

  // Histograms of parton level
  Hist HMassR("Mass of new resonances", 100, 0., MassMax);
  Hist HptR("pT of new resonances", 100, 0., pTMax);
  Hist HptD1("pT of daughter 1", 100, 0., pTMax);
  Hist HptD2("pT of daughter 2", 100, 0., pTMax);
  Hist HetaR("eta of new resonances", 100, 0., 2.);
  Hist HetaD1("eta of daughter 1", 100, 0., 2.);
  Hist HetaD2("eta of daughter 2", 100, 0., 2.);

  // Histograms of particle level
  Hist HMassC("Mass of dijet candidates", 100, 0., MassMax);
  Hist HptC("pT of dijet candidates", 100, 0., pTMax);
  Hist HptJ1("pT of leading jets", 100, 0, pTMax);
  Hist HptJ2("pT of sub leading jets", 100, 0, pTMax);
  Hist HetaC("eta of dijet candidates", 100, 0., 2.);
  Hist HetaJ1("eta of leading jets", 100, 0., 2.);
  Hist HetaJ2("eta of sub leading jets", 100, 0., 2.);
  Hist HYstar("Ystar", 100, 0., 0.6);

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

    HMassR.fill(pythia.event[iRes].m());
    HptR.fill(pythia.event[iRes].p().pT());
    HptD1.fill(pythia.event[iD1].p().pT());
    HptD2.fill(pythia.event[iD2].p().pT());
    HetaR.fill(pythia.event[iRes].p().eta());
    HetaD1.fill(pythia.event[iD1].p().eta());
    HetaD2.fill(pythia.event[iD2].p().eta());

    fjInputs.clear();

    // Loop over event record to decide what to pass to FastJet.
    for (int i = 0; i < pythia.event.size(); ++i) {
      // Final state only.
      if (!pythia.event[i].isFinal()) continue;

      // No neutrinos or DM.
      // 12 for electron neutrino, 14 for muon neutrino, 16 for tauon neutrino, 52 for dark matter with spin 1 / 2
      // Pdgid can be accessed in https://twiki.cern.ch/twiki/bin/view/Main/PdgId
      if ( pythia.event[i].idAbs() == 12 || pythia.event[i].idAbs() == 14
	|| pythia.event[i].idAbs() == 16 || pythia.event[i].idAbs() == 52)
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

    HMassC.fill(pC.mCalc());
    HptC.fill(pC.pT());
    HptJ1.fill(sortedJets[0].pt());
    HptJ2.fill(sortedJets[1].pt());
    HetaC.fill(pC.eta());
    HetaJ1.fill(sortedJets[0].eta());
    HetaJ2.fill(sortedJets[1].eta());
    HYstar.fill(Ystar);
    
  // End of event loop. Statistics. Histogram.
  }
  pythia.stat();

  // parton level histogram
  cout << HMassR << HptR << HptD1 << HptD2 << HetaR << HetaD1 << HetaD2 << endl;
  // particle level histogram
  cout << HMassC << HptC << HptJ1 << HptJ2 << HetaC << HetaJ1 << HetaJ2 << HYstar << endl;

  // Done.
  return 0;
}
