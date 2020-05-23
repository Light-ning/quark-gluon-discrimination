// main75.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program to study jets in Dark Matter production.

#include <iostream>
#include <cmath>
#include <fstream>

#include "Pythia8/Pythia.h"

// head file for jet clustering
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/contrib/DistanceMeasure.hh"
#include "fastjet/contrib/QCDAwarePlugin.hh"

// head file for ROOT histogram plotting
#include "TH1.h"
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

  if (argc > 1) pythia.readFile(argv[1]);
  else  pythia.readFile("QCDProcess.cmnd");
  pythia.init();

  // Fastjet analysis - select algorithm and parameters.
  double Rparam = 0.4;
  fastjet::Strategy               strategy = fastjet::Best;
  fastjet::RecombinationScheme    recombScheme = fastjet::E_scheme;
  fastjet::JetDefinition         *jetDef = NULL;
  jetDef = new fastjet::JetDefinition( fastjet::antikt_algorithm, Rparam,
           recombScheme, strategy);

  // QCD - aware Fastjet analysis
  double Ghostparam = 1e-20;
  fastjet::contrib::QCDAwarePlugin::AntiKtMeasure *akt = new fastjet::contrib::QCDAwarePlugin::AntiKtMeasure(Rparam);
  fastjet::contrib::QCDAwarePlugin::QCDAwarePlugin *qcdawareakt = new fastjet::contrib::QCDAwarePlugin::QCDAwarePlugin(akt);

  // Fastjet input.
  std::vector <fastjet::PseudoJet> fjInputs, QCDfjInputs;

  // set parameter for jet splitting
  double dR = 0.2;

  int nEvent = 100000;
  double MassMax = 10000.;
  double pTMax = 5000.;

  // output file with tried event number & cross section
  ofstream information;
  if (argc > 3) information.open(argv[3], ios::out);
  else information.open("Info.txt", ios::out);

  // settings for using ROOT histograms
  TFile *outFile;
  if (argc > 2) outFile = new TFile(argv[2], "RECREATE");
  else outFile = new TFile("QCDProcess.root", "RECREATE");

  TApplication theApp("hist", &argc, argv);

  // Histograms of parton level
  TH1F *HptD1 = new TH1F("HptD1", "pT of daughter 1", 100, 0., pTMax);
  TH1F *HptD2 = new TH1F("HptD2", "pT of daughter 2", 100, 0., pTMax);
  TH1F *HetaD1 = new TH1F("HetaD1", "eta of daughter 1", 100, 0., 2.);
  TH1F *HetaD2 = new TH1F("HetaD2", "eta of daughter 2", 100, 0., 2.);

  // Histograms of particle level
  TH1F *HMassC = new TH1F("HMassC", "Mass of dijet candidates", 100, 0., MassMax);
  TH1F *HMassCun= new TH1F("HMassCun", "Mass of unlabelled dijet candidates", 100., 0., MassMax);
  TH1F *HMassCqq = new TH1F("HMassCqq", "Mass of qq dijet candidates", 100., 0., MassMax);
  TH1F *HMassCgg = new TH1F("HMassCgg", "Mass of gg dijet candidates", 100., 0., MassMax);
  TH1F *HMassCqg = new TH1F("HMassCqg", "Mass of qg dijet candidates", 100., 0., MassMax);
  
  TH1F *HptC = new TH1F("HptC", "pT of dijet candidates", 100, 0., pTMax);
  TH1F *HptJ1 = new TH1F("HptJ1", "pT of leading jets", 100, 0, pTMax);
  TH1F *HptJ2 = new TH1F("HptJ2", "pT of sub leading jets", 100, 0, pTMax);
  TH1F *HetaC = new TH1F("HetaC", "eta of dijet candidates", 100, 0., 2.);
  TH1F *HetaJ1 = new TH1F("HetaJ1", "eta of leading jets", 100, 0., 2.);
  TH1F *HetaJ2 = new TH1F("HetaJ2", "eta of sub leading jets", 100, 0., 2.);
  TH1F *HYstar = new TH1F("HYstar", "Ystar", 100, 0., 0.6);

  // Histogram of dijet type
  TH1F *HDijetType = new TH1F("HDijetType", "0: qq jets\t1: gg jets\t2: qg jets", 10, -2., 3.);

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

    // hard coding to find 2 partons before showering
    int id11 = 5, id21 = 6;

    HptD1  -> Fill(pythia.process[id11].pT());
    HptD2  -> Fill(pythia.process[id21].pT());
    HetaD1 -> Fill(pythia.process[id11].eta());
    HetaD2 -> Fill(pythia.process[id21].eta());

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
    std::vector <fastjet::PseudoJet> inclusiveJets, sortedJets;
    fastjet::ClusterSequence clustSeq(fjInputs, *jetDef);

    // Extract inclusive jets sorted by pT (note minimum pT of 20.0 GeV).
    inclusiveJets = clustSeq.inclusive_jets(20.0);
    sortedJets    = sorted_by_pt(inclusiveJets);

    // need at least 2 jets to finish leading jet and sub-leading jet analysis
    if (sortedJets.size() < 2) {
      cout << "No enough jets found in event " << iEvent << endl;
      continue;
    }

    Vec4 pJ1(sortedJets[0].px(), sortedJets[0].py(), sortedJets[0].pz(), sortedJets[0].e());
    Vec4 pJ2(sortedJets[1].px(), sortedJets[1].py(), sortedJets[1].pz(), sortedJets[1].e());
    Vec4 pC = pJ1 + pJ2;

    double Ystar = (sortedJets[0].rap() - sortedJets[1].rap()) / 2;

    // cut events with too soft leading jets and sub leading jets
    if ((pJ1.pT() < 420) || (pJ2.pT() < 150)){
      cout << "Jets too soft in event " << iEvent << endl;
      continue;
    }

    // cut the events where jet eta is too large
    if ((abs(pJ1.eta()) >= 2) || (abs(pJ2.eta()) >= 2)){
      cout << "Jets with too large eta in event " << iEvent << endl;
      continue;
    }

    HMassC -> Fill(pC.mCalc());
    HptC -> Fill(pC.pT());
    HptJ1 -> Fill(sortedJets[0].pt());
    HptJ2 -> Fill(sortedJets[1].pt());
    HetaC -> Fill(pC.eta());
    HetaJ1 -> Fill(sortedJets[0].eta());
    HetaJ2 -> Fill(sortedJets[1].eta());
    HYstar -> Fill(Ystar);


    // Qcdaware jet clustering

    QCDfjInputs.clear();

    // Particle loop to pick final partons
    for (int i = 0; i < pythia.event.size(); i++){
      // partons only
      if (!pythia.event[i].isParton()) continue;
      // no parton child
      if (pythia.event[pythia.event[i].daughter1()].isParton() || pythia.event[pythia.event[i].daughter2()].isParton()) continue;
      // reject if the parton is from hadron or tau decay
      bool fromHorT = 0;
      for (int mother: pythia.event[i].motherList()){
	fromHorT = fromHorT || (pythia.event[mother].isHadron() && pythia.event[mother].statusHepMC() == 2);
	fromHorT = fromHorT || ((pythia.event[mother].idAbs() == 15) && (pythia.event[mother].statusHepMC() == 2));
      }
      if (fromHorT) continue;

      // put the selected parton in fastjet input vector
      fastjet::PseudoJet FinalParton(pythia.event[i].px(), pythia.event[i].py(), pythia.event[i].pz(), pythia.event[i].e());
      FinalParton.set_user_index(pythia.event[i].id());

      QCDfjInputs.push_back(FinalParton);
    }

    // if no final partons
    if (fjInputs.size() == 0){
      cout << "Error: no final partons in event " << iEvent << endl;
      HDijetType->Fill(-1);
      HMassCun->Fill(pC.mCalc());
      continue;
    }

    // QCD aware jet clustering
    std::vector <fastjet::PseudoJet> QCDSortedJets;

    fastjet::ClusterSequence QCDclustSeq(QCDfjInputs, qcdawareakt);
    QCDSortedJets = sorted_by_pt(QCDclustSeq.inclusive_jets(20.0));

    // no enough parton jets
    if (QCDSortedJets.size() < 2){
      cout << "No enough parton jets found in event" << iEvent << endl;
      HDijetType->Fill(-1);
      HMassCun->Fill(pC.mCalc());
      continue;
    }

    // add "ghost" parton jet to the pseudojet list
    // ghostify the parton jet
    std::vector <fastjet::PseudoJet> refjInput;
    refjInput.clear();
    // default user index of pseudo jet is -1, the same as anti-down quark
    // so set the user index of particle jet as 0
    for (fastjet::PseudoJet particlejet: sortedJets){
      particlejet.set_user_index(0);
      refjInput.push_back(particlejet);
    }
    for (fastjet::PseudoJet partonjet: QCDSortedJets){
      // ghostify
      //      cout << "Partonjet:"
      //	   << partonjet.eta() << " "
      //	   << partonjet.phi() << " ";
      partonjet.reset_momentum(partonjet.px() * Ghostparam, partonjet.py() * Ghostparam, partonjet.pz() * Ghostparam, partonjet.e() * Ghostparam);
      //      cout << partonjet.pt() / Ghostparam << " "
      //	   << partonjet.e() / Ghostparam << " "
      //	   << partonjet.user_index() << " "
      //	   << endl;
      refjInput.push_back(partonjet);
    }

    // reclustering
    fastjet::ClusterSequence reClustSeq(refjInput, *jetDef);
    std::vector <fastjet::PseudoJet> reSortedJets;
    reSortedJets = sorted_by_pt(reClustSeq.inclusive_jets(20.0));

    // labelling
    // pick the closest parton jets to label
    // if the distance of all the parton constituents are larger than dR, the particle jet remain unlabelled
    int EventType = -1;
    double Rmin0 = dR, Rmin1 = dR;
    int label0 = 0, label1 = 0;
    for (fastjet::PseudoJet con: reSortedJets[0].constituents())
      if (con.user_index() != 0)
	if (DeltaR(reSortedJets[0].phi(), con.phi(), reSortedJets[0].eta(), con.eta()) < Rmin0){
	  Rmin0 = DeltaR(reSortedJets[0].phi(), con.phi(), reSortedJets[0].eta(), con.eta());
	  label0 = con.user_index();
	}
    for (fastjet::PseudoJet con: reSortedJets[1].constituents())
      if (con.user_index() != 0)
	if (DeltaR(reSortedJets[1].phi(), con.phi(), reSortedJets[1].eta(), con.eta()) < Rmin1){
	  Rmin1 = DeltaR(reSortedJets[1].phi(), con.phi(), reSortedJets[1].eta(), con.eta());
	  label1 = con.user_index();
	}

    // Delta R are all larger than dR
    if ((label0 == 0) || (label1 == 0)){
      HMassCun->Fill(pC.mCalc());
      HDijetType->Fill(-1);
      continue;
    }
    label0 = abs(label0);
    label1 = abs(label1);

    //    cout << "label0: " << label0 << " label1: " << label1 << endl;
    //    cout << "daughter0: " << pythia.process[id11].id() << " daughter1: " << pythia.process[id21].id() << endl << endl;
    
    if ((label0 <= 8) && (label1 <= 8)){
      HMassCqq->Fill(pC.mCalc());
      HDijetType->Fill(0);
    } else if ((label0 == 21) && (label1 == 21)){
      HMassCgg->Fill(pC.mCalc());
      HDijetType->Fill(1);
    } else if (((label0 == 21) && (label1 <= 8)) ||
	       ((label0 <= 8) && (label1 == 21))){
      HMassCqg->Fill(pC.mCalc());
      HDijetType->Fill(2);
    } else {
      HMassCun->Fill(pC.mCalc());
      HDijetType->Fill(-1);
    }

    /*    
    // old method
    // truth matching below:
    double phiJet1 = sortedJets[0].phi();
    double phiJet2 = sortedJets[1].phi();
    double etaJet1 = sortedJets[0].eta();
    double etaJet2 = sortedJets[1].eta();

    int ij1 = -1, ij2 = -1;
    for (int iParticle = 0; iParticle < pythia.event.size(); iParticle++){
      if ((pythia.event[iParticle].idAbs() != 21) && (pythia.event[iParticle].idAbs() > 8)) continue;
      if (DeltaR(pythia.event[iParticle].phi(), phiJet1, pythia.event[iParticle].eta(), etaJet1) <= dR){
	if (ij1 == -1) ij1 = iParticle;
	else if (pythia.event[iParticle].e() > pythia.event[ij1].e()) ij1 = iParticle;
      }
      if (DeltaR(pythia.event[iParticle].phi(), phiJet2, pythia.event[iParticle].eta(), etaJet2) <= dR){
	if (ij2 == -1) ij2 = iParticle;
	else if (pythia.event[iParticle].e() > pythia.event[ij2].e()) ij2 = iParticle;
      }
    }

    // the event type: unlabelled -> -1; qq -> 0; gg -> 1; qg -> 2
    int EventType = -1;
    if ((ij1 == ij2) || (ij1 == -1) || (ij2 == -1)) {
      HMassCun -> Fill(pC.mCalc());
      continue;
    }
    if ((pythia.event[ij1].idAbs() <=  8) && (pythia.event[ij2].idAbs() <=  8)){
      EventType = 0;
      HMassCqq -> Fill(pC.mCalc());
    }
    else if ((pythia.event[ij1].idAbs() == 21) && (pythia.event[ij2].idAbs() == 21)){
      EventType = 1;
      HMassCgg -> Fill(pC.mCalc());
    }
    else if (((pythia.event[ij1].idAbs() <=  8) && (pythia.event[ij2].idAbs() == 21)) ||
	     ((pythia.event[ij1].idAbs() == 21) && (pythia.event[ij2].idAbs() <=  8))){
      EventType = 2;
      HMassCqg -> Fill(pC.mCalc());
    }
    else HMassCun -> Fill(pC.mCalc());

    HDijetType -> Fill(EventType);
    */

    // End of event loop. Statistics. Histogram.
  }

  HptD1 -> Write();
  HptD2 -> Write();
  HetaD1 -> Write();
  HetaD2 -> Write();

  HMassC -> Write();
  HMassCun -> Write();
  HMassCqq -> Write();
  HMassCgg -> Write();
  HMassCqg -> Write();
  
  HptC -> Write();
  HptJ1 -> Write();
  HptJ2 -> Write();
  HetaC -> Write();
  HetaJ1 -> Write();
  HetaJ2 -> Write();
  HYstar -> Write();

  HDijetType -> Write();

  information << pythia.info.nTried() << endl
	      << pythia.info.sigmaGen() << endl;
  information.close();
  
  delete outFile;
  pythia.stat();

  // Done.
  return 0;
}
