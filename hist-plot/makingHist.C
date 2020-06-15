double binLowMassGeV[] = {203.0, 216.0, 229.0, 243.0, 257.0, 272.0, 287.0, 303.0, 319.0, 335.0, 352.0, 369.0, 387.0, 405.0, 424.0, 443.0, 462.0, 482.0, 502.0, 523.0, 544.0, 566.0, 588.0, 611.0, 634.0, 657.0, 681.0, 705.0, 730.0, 755.0, 781.0, 807.0, 834.0, 861.0, 889.0, 917.0, 946.0, 976.0, 1006.0, 1037.0, 1068.0, 1100.0, 1133.0, 1166.0, 1200.0, 1234.0, 1269.0, 1305.0, 1341.0, 1378.0, 1416.0, 1454.0, 1493.0, 1533.0, 1573.0, 1614.0, 1656.0, 1698.0, 1741.0, 1785.0, 1830.0, 1875.0, 1921.0, 1968.0, 2016.0, 2065.0, 2114.0, 2164.0, 2215.0, 2267.0, 2320.0, 2374.0, 2429.0, 2485.0, 2542.0, 2600.0, 2659.0, 2719.0, 2780.0, 2842.0, 2905.0, 2969.0, 3034.0, 3100.0, 3167.0, 3235.0, 3305.0, 3376.0, 3448.0, 3521.0, 3596.0, 3672.0, 3749.0, 3827.0, 3907.0, 3988.0, 4070.0, 4154.0, 4239.0, 4326.0, 4414.0, 4504.0, 4595.0, 4688.0, 4782.0, 4878.0, 4975.0, 5074.0, 5175.0, 5277.0, 5381.0, 5487.0, 5595.0, 5705.0, 5817.0, 5931.0, 6047.0, 6165.0, 6285.0, 6407.0, 6531.0, 6658.0, 6787.0, 6918.0, 7052.0, 7188.0, 7326.0, 7467.0, 7610.0, 7756.0, 7904.0, 8055.0, 8208.0, 8364.0, 8523.0, 8685.0, 8850.0, 9019.0, 9191.0, 9366.0, 9544.0, 9726.0, 9911.0, 10100.0, 10292.0, 10488.0, 10688.0, 10892.0, 11100.0, 11312.0, 11528.0, 11748.0, 11972.0, 12200.0, 12432.0, 12669.0, 12910.0, 13156.0};
double binLowPtGeV[] = {15. ,20. ,25. ,35. ,45. ,55. ,70. ,85. ,100. ,116. ,134. ,152. ,172. ,194. ,216. ,240. ,264. ,290. ,318. ,346.,376.,408.,442.,478.,516.,556.,598.,642.,688.,736.,786.,838.,894.,952.,1012.,1076.,1162.,1250.,1310.,1420.,1530.,1750.,1992.,2250.,2500.,2850.,3200.,3600.,4000.,4600.};

TString path = "/eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningMC/";
//char infile[] = "user.bdong.17987685._000001.tree.root";
//char intree[] = "outTree";

float leadingJetPtMin = 420.;
float jetPtMin = 150.;
float etaMax = 2.1;
float yStarMax = 0.6;
float mjjMin = 1100;
string trigger = "HLT_j420";

double gluonTrackOffset = -7.55743;
double gluonTrackSlope = 3.5915;
//double quarkTrackOffset = -7.55743;
//double quarkTrackSlope = 3.5915;

bool eventLevelCuts(int njet, float leadingJetPt, vector<string> *passedTriggers, float yStar, float mjj);
bool jetLevelCuts(float pt, int LooseBad, float eta);

//bool getQuarkSelection(float pt, float ntrack);
bool getGluonSelection(float pt, float ntrack);
TString getInputPath(TString dateset);
TH1D *getMassHist(TString name, TString title);
TH1D *getPtHist(TString name, TString title);
TH1D *getNTrkHist(TString name, TString title);

void makingHist(TString dataset, TString intree){

  // Mass Hist
  TH1D *HistMjj = getMassHist("HistMjj", "mjj");
  TH1D *HistMjj_TruthGG = getMassHist("HistMjj_TruthGG", "mjj in truth labelled GG events");
  TH1D *HistMjj_TruthGJ = getMassHist("HistMjj_TruthGJ", "mjj in truth labelled GJ events");
  TH1D *HistMjj_TruthQG = getMassHist("HistMjj_TruthQG", "mjj in truth labelled QG events");
  TH1D *HistMjj_TruthQQ = getMassHist("HistMjj_TruthQQ", "mjj in truth labelled QQ events");
  TH1D *HistMjj_GG = getMassHist("HistMjj_GG", "mjj in tagged GG events");
  TH1D *HistMjj_GJ = getMassHist("HistMjj_GJ", "mjj in tagged GJ events");
  TH1D *HistMjj_QG = getMassHist("HistMjj_QG", "mjj in tagged QG events");
  TH1D *HistMjj_QQ = getMassHist("HistMjj_QQ", "mjj in tagged QQ events");

  // Leading jet pt Hist
  TH1D *HistLeadingJetPt = getPtHist("HistLeadingJetPt", "leading jet pt");
  TH1D *HistLeadingJetPt_TruthGG = getPtHist("HistLeadingJetPt_TruthGG", "leading jet pt in truth labelled GG events");
  TH1D *HistLeadingJetPt_TruthGJ = getPtHist("HistLeadingJetPt_TruthGJ", "leading jet pt in truth labelled GJ events");
  TH1D *HistLeadingJetPt_TruthQG = getPtHist("HistLeadingJetPt_TruthQG", "leading jet pt in truth labelled QG events");
  TH1D *HistLeadingJetPt_TruthQQ = getPtHist("HistLeadingJetPt_TruthQQ", "leading jet pt in truth labelled QQ events");
  TH1D *HistLeadingJetPt_GG = getPtHist("HistLeadingJetPt_GG", "leading jet pt in tagged GG events");
  TH1D *HistLeadingJetPt_GJ = getPtHist("HistLeadingJetPt_GJ", "leading jet pt in tagged GJ events");
  TH1D *HistLeadingJetPt_QG = getPtHist("HistLeadingJetPt_QG", "leading jet pt in tagged QG events");
  TH1D *HistLeadingJetPt_QQ = getPtHist("HistLeadingJetPt_QQ", "leading jet pt in tagged QQ events");

  // Sub Leading jet pt Hist
  TH1D *HistSubJetPt = getPtHist("HistSubJetPt", "sub leading jet pt");
  TH1D *HistSubJetPt_TruthGG = getPtHist("HistSubJetPt_TruthGG", "sub leading jet pt in truth labelled GG events");
  TH1D *HistSubJetPt_TruthGJ = getPtHist("HistSubJetPt_TruthGJ", "sub leading jet pt in truth labelled GJ events");
  TH1D *HistSubJetPt_TruthQG = getPtHist("HistSubJetPt_TruthQG", "sub leading jet pt in truth labelled QG events");
  TH1D *HistSubJetPt_TruthQQ = getPtHist("HistSubJetPt_TruthQQ", "sub leading jet pt in truth labelled QQ events");
  TH1D *HistSubJetPt_GG = getPtHist("HistSubJetPt_GG", "sub leading jet pt in tagged GG events");
  TH1D *HistSubJetPt_GJ = getPtHist("HistSubJetPt_GJ", "sub leading jet pt in tagged GJ events");
  TH1D *HistSubJetPt_QG = getPtHist("HistSubJetPt_QG", "sub leading jet pt in tagged QG events");
  TH1D *HistSubJetPt_QQ = getPtHist("HistSubJetPt_QQ", "sub leading jet pt in tagged QQ events");

  // number of track hist
  TH1D *HistNTrk = getNTrkHist("HistNTrk", "number of tracks");
  TH1D *HistNTrk_Q = getNTrkHist("HistNTrk_Q", "number of tracks in Q jets");
  TH1D *HistNTrk_G = getNTrkHist("HistNTrk_G", "number of tracks in G jets");


  // variables used
  vector<float> *jet_pt = 0, *jet_NumTrkPt500PV = 0, *jet_eta = 0;
  vector<int> *jet_PartonTruthLabelID = 0, *jet_clean_passLooseBad;
  vector<string> *passedTriggers = 0;
  int njet;
  float mjj, weight, yStar;
  double w = 1, sampleEvents = 0;

  TString inputPath = getInputPath(dataset);


  void *dir = gSystem->OpenDirectory(inputPath);
  TString filename = gSystem->GetDirEntry(dir);
  while (filename != ""){

    if (!filename.Contains(".root")){
      filename = gSystem->GetDirEntry(dir);
      continue;
    }

    TFile *f = TFile::Open(inputPath + filename);
    TTree *t = (TTree*) f->Get(intree);
    TH1D *h = (TH1D*) f->Get("cutflow_weighted");

    if ((t != 0) && (h != 0)){

      // sample Events, used to calculate weight
      sampleEvents = h->GetBinContent(1);

      // set the needed branch status and branch address
      // to get variables from the ttree
      t->SetBranchStatus("*", 0);
      t->SetBranchStatus("mjj", 1);
      t->SetBranchStatus("weight", 1);
      t->SetBranchStatus("jet_pt", 1);
      t->SetBranchStatus("jet_NumTrkPt500PV", 1);
      t->SetBranchStatus("yStar", 1);
      t->SetBranchStatus("jet_PartonTruthLabelID", 1);
      t->SetBranchStatus("passedTriggers", 1);
      t->SetBranchStatus("njet", 1);
      t->SetBranchStatus("jet_eta", 1);
      t->SetBranchStatus("jet_clean_passLooseBad", 1);

      t->SetBranchAddress("mjj", &mjj);
      t->SetBranchAddress("weight", &weight);
      t->SetBranchAddress("jet_pt", &jet_pt);
      t->SetBranchAddress("jet_NumTrkPt500PV", &jet_NumTrkPt500PV);
      t->SetBranchAddress("yStar", &yStar);
      t->SetBranchAddress("jet_PartonTruthLabelID", &jet_PartonTruthLabelID);
      t->SetBranchAddress("passedTriggers", &passedTriggers);
      t->SetBranchAddress("njet", &njet);
      t->SetBranchAddress("jet_eta", &jet_eta);
      t->SetBranchAddress("jet_clean_passLooseBad", &jet_clean_passLooseBad);

      // loop all the entries in the ttree
      int nEntries = t->GetEntries();

      for (int i = 0; i < nEntries; i++){
	t->GetEntry(i);

	// event level cuts
	if (not eventLevelCuts(njet, (*jet_pt)[0], passedTriggers, yStar, mjj)) continue;
	// jet level cuts
	if (not jetLevelCuts((*jet_pt)[0], (*jet_clean_passLooseBad)[0], (*jet_eta)[0])) continue;
	if (not jetLevelCuts((*jet_pt)[1], (*jet_clean_passLooseBad)[1], (*jet_eta)[1])) continue;

	// calculate weight
	// and fill mjj, leading jet pt, sub leading jet pt and ntrack hist
	w = weight / sampleEvents;
	HistMjj->Fill(mjj, w);
	HistLeadingJetPt->Fill((*jet_pt)[0], w);
	HistSubJetPt->Fill((*jet_pt)[1], w);
	HistNTrk->Fill((*jet_NumTrkPt500PV)[0], w);

	// parton truth label
	int truthLeadingG = ((*jet_PartonTruthLabelID)[0] == 21);
	int truthSubG = ((*jet_PartonTruthLabelID)[1] == 21);
	if (truthLeadingG) HistNTrk_G->Fill((*jet_NumTrkPt500PV)[0], w);
	else HistNTrk_Q->Fill((*jet_NumTrkPt500PV)[0], w);
	if (truthSubG) HistNTrk_G->Fill((*jet_NumTrkPt500PV)[1], w);
	else HistNTrk_Q->Fill((*jet_NumTrkPt500PV)[1], w);
	if ((truthLeadingG + truthSubG) >= 1){  // GJ dijet
	  HistMjj_TruthGJ->Fill(mjj, w);
	  HistLeadingJetPt_TruthGJ->Fill((*jet_pt)[0], w);
	  HistSubJetPt_TruthGJ->Fill((*jet_pt)[1], w);
	}
	if ((truthLeadingG + truthSubG) == 2){  // GG dijet
	  HistMjj_TruthGG->Fill(mjj, w);
	  HistLeadingJetPt_TruthGG->Fill((*jet_pt)[0], w);
	  HistSubJetPt_TruthGG->Fill((*jet_pt)[1], w);
	} else if ((truthLeadingG + truthSubG) == 1){  // QG dijet
	  HistMjj_TruthQG->Fill(mjj, w);
	  HistLeadingJetPt_TruthQG->Fill((*jet_pt)[0], w);
	  HistSubJetPt_TruthQG->Fill((*jet_pt)[1], w);
	} else if ((truthLeadingG + truthSubG) == 0){  // QQ dijet
	  HistMjj_TruthQQ->Fill(mjj, w);
	  HistLeadingJetPt_TruthQQ->Fill((*jet_pt)[0], w);
	  HistSubJetPt_TruthQQ->Fill((*jet_pt)[1], w);
	}
	
	// dijet gg/gj/jj selection by NumTrk cut
	int numberGJet = 0;
	numberGJet = getGluonSelection((*jet_pt)[0], (*jet_NumTrkPt500PV)[0]) + getGluonSelection((*jet_pt)[1], (*jet_NumTrkPt500PV)[1]);

	if (numberGJet >= 1){
	  HistMjj_GJ->Fill(mjj, w);
	  HistLeadingJetPt_GJ->Fill((*jet_pt)[0], w);
	  HistSubJetPt_GJ->Fill((*jet_pt)[1], w);
	}
	if (numberGJet == 2){
	  HistMjj_GG->Fill(mjj, w);
	  HistLeadingJetPt_GG->Fill((*jet_pt)[0], w);
	  HistSubJetPt_GG->Fill((*jet_pt)[1], w);
	} else if (numberGJet == 1){
	  HistMjj_QG->Fill(mjj, w);
	  HistLeadingJetPt_QG->Fill((*jet_pt)[0], w);
	  HistSubJetPt_QG->Fill((*jet_pt)[1], w);
	} else if (numberGJet == 0){
	  HistMjj_QQ->Fill(mjj, w);
	  HistLeadingJetPt_QQ->Fill((*jet_pt)[0], w);
	  HistSubJetPt_QQ->Fill((*jet_pt)[1], w);
	}
      }
    } else cout << "No " << intree << " or no cutflow_weighted" << endl;

    f->Close();
    delete f, t, h;
    filename = gSystem->GetDirEntry(dir);
  }

  TFile *fout = TFile::Open(dataset + ".root", "recreate");
  HistMjj->Write();
  HistMjj_TruthGG->Write();
  HistMjj_TruthGJ->Write();
  HistMjj_TruthQG->Write();
  HistMjj_TruthQQ->Write();
  HistMjj_GG->Write();
  HistMjj_GJ->Write();
  HistMjj_QG->Write();
  HistMjj_QQ->Write();

  HistLeadingJetPt->Write();
  HistLeadingJetPt_TruthGG->Write();
  HistLeadingJetPt_TruthGJ->Write();
  HistLeadingJetPt_TruthQG->Write();
  HistLeadingJetPt_TruthQQ->Write();
  HistLeadingJetPt_GG->Write();
  HistLeadingJetPt_GJ->Write();
  HistLeadingJetPt_QG->Write();
  HistLeadingJetPt_QQ->Write();

  HistSubJetPt->Write();
  HistSubJetPt_TruthGG->Write();
  HistSubJetPt_TruthGJ->Write();
  HistSubJetPt_TruthQG->Write();
  HistSubJetPt_TruthQQ->Write();
  HistSubJetPt_GG->Write();
  HistSubJetPt_GJ->Write();
  HistSubJetPt_QG->Write();
  HistSubJetPt_QQ->Write();

  HistNTrk->Write();
  HistNTrk_G->Write();
  HistNTrk_Q->Write();

  fout->Close();
}

// we just apply gluon seleciton here
//bool getQuarkSelection(float pt, float ntrack){
  // Q = 1, G = 0
//  double value = log(pt);
//  double SigmoidnTrack = quarkTrackSlope * value + quarkTrackOffset;
//  if (ntrack < SigmoidnTrack) return 1;
//  else return 0;
//}

bool getGluonSelection(float pt, float ntrack){
  // G = 1, Q = 0
  double value = log(pt);
  double SigmoidnTrack = gluonTrackSlope * value + gluonTrackOffset;
  if (ntrack > SigmoidnTrack) return 1;
  else return 0;
}

bool eventLevelCuts(int njet, float leadingJetPt, vector<string> *passedTriggers, float yStar, float mjj){
  if (njet < 2) return 0;  
  if (leadingJetPt < leadingJetPtMin) return 0;
  vector<string>::iterator location = find(passedTriggers->begin(), passedTriggers->end(), trigger);
  if ((location - passedTriggers->begin()) >= passedTriggers->size()) return 0;
  //  if (abs(yStar) > yStarMax) return 0;
  //  if (mjj < mjjMin) return 0;
  return 1;
}

bool jetLevelCuts(float pt, int LooseBad, float eta){
  if (pt <= jetPtMin) return 0;
  if (LooseBad == 0) return 0;
  if (abs(eta) > etaMax) return 0;
  return 1;
}

TString getInputPath(TString dataset){
  TString inputPath = path;
  if (dataset == "MC16a_JZ4W"){
    inputPath += "QCD_MC16a/user.bdong.mc16_13TeV.364704.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ4WithSW.m16a_newJetCleaning_May9_tree.root/";
    return inputPath;
  }
  if (dataset.Contains("MC16")){
    TString period = dataset(dataset.Index("MC16") + 4);
    if (!(dataset.Contains("JZ") && dataset.Contains("W"))){
      cout << "Wrong dataset tag: " << dataset << endl;
      return "";
    }
    TString jzxw = dataset(dataset.Index("JZ") + 2, dataset.Index("W") - dataset.Index("JZ") - 2);
    inputPath += "QCD_MC16" + period;
    inputPath += "/user.bdong.mc16_13TeV.3647" + ((jzxw.Length() == 1) ? ("0" + jzxw) : (jzxw)) + ".Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ" + jzxw + "WithSW.mc16" + period + "_newJetCleaning_May9_tree.root/";
  }
  return inputPath;
}

TH1D *getMassHist(TString name, TString title){
  TH1D *massHist = new TH1D(name, title, sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  massHist->GetXaxis()->SetTitle("m_{jj} [GeV]");
  massHist->Sumw2();
  return massHist;
}

TH1D *getPtHist(TString name, TString title){
  TH1D *ptHist = new TH1D(name, title, sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  ptHist->GetXaxis()->SetTitle("Pt [GeV]");
  ptHist->Sumw2();
  return ptHist;
}

TH1D *getNTrkHist(TString name, TString title){
  TH1D *nTrkHist = new TH1D(name, title, 80, -0.5, 79.5);
  nTrkHist->GetXaxis()->SetTitle("N_{track}");
  nTrkHist->Sumw2();
  return nTrkHist;
}
