// main75.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program to study jets in Dark Matter production.

#include <iostream>
#include <cmath>

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

double DeltaPhi(double phi1, double phi2){
  if (abs(phi1 - phi2) > M_PI) return 2 * M_PI - abs(phi1 - phi2);
  else return abs(phi1 - phi2);
}

double DeltaR(double phi1, double phi2, double eta1, double eta2){
  return sqrt(pow(DeltaPhi(phi1, phi2), 2) + pow(eta1 - eta2, 2));
}

int main(int argc, char* argv[]) {

  // Generator. Process selection. Initialization. Event shorthand.
  Pythia pythia;

  pythia.readFile("QCDProcess.cmnd");
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

  // set parameter for jet splitting
  double dR = 0.2;

  int nEvent = 1000;
  double MassMax = 1000.;
  double pTMax = 1000.;

  // settings for using ROOT histograms
  TApplication theApp("hist", &argc, argv);
  TFile *outFile = new TFile("QCDProcess.root", "RECREATE");

  // Histograms of parton level
  TH1F *HptD1 = new TH1F("HptD1", "pT of daughter 1", 100, 0., 5.);
  TH1F *HptD2 = new TH1F("HptD2", "pT of daughter 2", 100, 0., 5.);
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

  // Histogram of dijet type
  TH1F *HDijetType = new TH1F("HDijetType", "0: qq jets\t1: gg jets\t2: qg jets", 6, 0., 3.);

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

    // find 2 protons
    int ip1 = 1, ip2 = 2;
    while (pythia.event[ip1].idAbs() != 2212 && ip1 < pythia.event.size()) ip2 = ++ip1;
    while (pythia.event[ip2].idAbs() != 2212 && ip2 < pythia.event.size()) ip2++;
    if (ip1 >= pythia.event.size() || ip2 >= pythia.event.size()){
      cout << "No enough protons in Event: " << iEvent << endl;
      continue;
    }

    // find 2 daughters and confirm the event type
    int id11 = pythia.event[ip1].daughter1();
    int id12 = pythia.event[ip1].daughter2();
    int id21 = pythia.event[ip2].daughter1();
    int id22 = pythia.event[ip2].daughter2();

    // EventType: 0: qq, 1: gg, 2: qg, -1: others
    int EventType = -1;

    if ((id11 == 0 && id12 == 0) || (id11 != 0 && id12 != 0)
      ||(id21 == 0 && id22 == 0) || (id21 != 0 && id22 != 0)){
      cout << "No QCD dijet process is generated in Event: " << iEvent << endl;
      continue;
    }

    id11 = id11 + id12;
    id21 = id21 + id22;

    HptD1  -> Fill(pythia.event[id11].pT());
    HptD2  -> Fill(pythia.event[id21].pT());
    HetaD1 -> Fill(pythia.event[id11].eta());
    HetaD2 -> Fill(pythia.event[id21].eta());

    // different types of processes
    if  (pythia.event[id11].idAbs() <=  8 && pythia.event[id21].idAbs() <=  8)  EventType = 0;
    if  (pythia.event[id11].idAbs() == 21 && pythia.event[id21].idAbs() == 21)  EventType = 1;
    if ((pythia.event[id11].idAbs() <=  8 && pythia.event[id21].idAbs() == 21)
      ||(pythia.event[id11].idAbs() == 21 && pythia.event[id21].idAbs() <=  8)) EventType = 2;
	
    // find the last appearance of daughter 1
    while (pythia.event[id11].daughter1() == pythia.event[id11].daughter2()
	   && pythia.event[id11].daughter1() != 0)
      id11 = pythia.event[id11].daughter1();
    // find the last appearance fo daughter 2
    while (pythia.event[id21].daughter1() == pythia.event[id21].daughter2()
	   && pythia.event[id21].daughter1() != 0)
      id21 = pythia.event[id21].daughter1();

    // record the phi and eta of 2 daughters
    // for the use of dijet splitting
    double phi1 = pythia.event[id11].phi();
    double phi2 = pythia.event[id21].phi();
    double eta1 = pythia.event[id11].eta();
    double eta2 = pythia.event[id21].eta();

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

    // calculate distance between parton and jet
    double d11 = DeltaR(phi1, sortedJets[0].phi(), eta1, sortedJets[0].eta());
    double d22 = DeltaR(phi2, sortedJets[1].phi(), eta2, sortedJets[1].eta());
    double d21 = DeltaR(phi2, sortedJets[0].phi(), eta2, sortedJets[0].eta());
    double d12 = DeltaR(phi1, sortedJets[1].phi(), eta1, sortedJets[1].eta());

    /*
    cout << "index: " << id11 << "\tpT: " << pythia.event[id11].pT() << "\teta: " << pythia.event[id11].eta() << "\tphi: " << pythia.event[id11].phi() << endl;
    cout << "index: " << id21 << "\tpT: " << pythia.event[id21].pT() << "\teta: " << pythia.event[id21].eta() << "\tphi: " << pythia.event[id11].phi() << endl;
    cout << "leading jet:\tpT: " << sortedJets[0].pt() << "\teta: " << sortedJets[0].eta() << "\tphi: " << sortedJets[0].phi() << endl;
    cout << "2-leading jet:\tpT: " << sortedJets[1].pt() << "\teta: " << sortedJets[1].eta() << "\tphi: " << sortedJets[1].phi() << endl;
    */

    if ((d11 < dR && d22 < dR) || (d21 < dR && d12 < dR))
      HDijetType -> Fill(EventType);

    // End of event loop. Statistics. Histogram.
  }

  HptD1 -> Write();
  HptD2 -> Write();
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

  HDijetType -> Write();
  
  delete outFile;
  pythia.stat();

  // Done.
  return 0;
}
