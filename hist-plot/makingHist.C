bool EventLevelCuts(float leadingJetPt, float subLeadingJetPt, float yStar, float mjj);
bool getQuarkSelection(float pt, float ntrack);
bool getGluonSelection(float pt, float ntrack);

double gluonTrackOffset = -7.55743;
double gluonTrackSlope = 3.5915;
double quarkTrackOffset = -7.55743;
double quarkTrackSlope = 3.5915;

//char infile[] = "user.bdong.17987685._000001.tree.root";
//char intree[] = "outTree";

float LJetPt = 420.;
float SJetPt = 150.;
float yStarMax = 0.6;
float mjjMin = 1100;

double binLowMassGeV[] = {203.0, 216.0, 229.0, 243.0, 257.0, 272.0, 287.0, 303.0, 319.0, 335.0, 352.0, 369.0, 387.0, 405.0, 424.0, 443.0, 462.0, 482.0, 502.0, 523.0, 544.0, 566.0, 588.0, 611.0, 634.0, 657.0, 681.0, 705.0, 730.0, 755.0, 781.0, 807.0, 834.0, 861.0, 889.0, 917.0, 946.0, 976.0, 1006.0, 1037.0, 1068.0, 1100.0, 1133.0, 1166.0, 1200.0, 1234.0, 1269.0, 1305.0, 1341.0, 1378.0, 1416.0, 1454.0, 1493.0, 1533.0, 1573.0, 1614.0, 1656.0, 1698.0, 1741.0, 1785.0, 1830.0, 1875.0, 1921.0, 1968.0, 2016.0, 2065.0, 2114.0, 2164.0, 2215.0, 2267.0, 2320.0, 2374.0, 2429.0, 2485.0, 2542.0, 2600.0, 2659.0, 2719.0, 2780.0, 2842.0, 2905.0, 2969.0, 3034.0, 3100.0, 3167.0, 3235.0, 3305.0, 3376.0, 3448.0, 3521.0, 3596.0, 3672.0, 3749.0, 3827.0, 3907.0, 3988.0, 4070.0, 4154.0, 4239.0, 4326.0, 4414.0, 4504.0, 4595.0, 4688.0, 4782.0, 4878.0, 4975.0, 5074.0, 5175.0, 5277.0, 5381.0, 5487.0, 5595.0, 5705.0, 5817.0, 5931.0, 6047.0, 6165.0, 6285.0, 6407.0, 6531.0, 6658.0, 6787.0, 6918.0, 7052.0, 7188.0, 7326.0, 7467.0, 7610.0, 7756.0, 7904.0, 8055.0, 8208.0, 8364.0, 8523.0, 8685.0, 8850.0, 9019.0, 9191.0, 9366.0, 9544.0, 9726.0, 9911.0, 10100.0, 10292.0, 10488.0, 10688.0, 10892.0, 11100.0, 11312.0, 11528.0, 11748.0, 11972.0, 12200.0, 12432.0, 12669.0, 12910.0, 13156.0};
double binLowPtGeV[] = {15. ,20. ,25. ,35. ,45. ,55. ,70. ,85. ,100. ,116. ,134. ,152. ,172. ,194. ,216. ,240. ,264. ,290. ,318. ,346.,376.,408.,442.,478.,516.,556.,598.,642.,688.,736.,786.,838.,894.,952.,1012.,1076.,1162.,1250.,1310.,1420.,1530.,1750.,1992.,2250.,2500.,2850.,3200.,3600.,4000.,4600.};

void makingHist(string infile, string intree, string outfile){

  TH1D *HistMjj = new TH1D("HistMjj", "HistMjj", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistMjj->GetXaxis()->SetTitle("m_{jj} [GeV]");
  HistMjj->Sumw2();
  TH1D *HistLJetPt = new TH1D("HistLJetPt", "HistLJetPt", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistMjj->GetXaxis()->SetTitle("Leading Jet Pt [GeV]");
  HistLJetPt->Sumw2();
  TH1D *HistSJetPt = new TH1D("HistSJetPt", "HistSJetPt", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistMjj->GetXaxis()->SetTitle("Sub-leading Jet Pt [GeV]");
  HistSJetPt->Sumw2();

  TH1D *HistQQMjj = new TH1D("HistQQMjj", "HistQQMjj", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistQQMjj->GetXaxis()->SetTitle("m_{jj} [GeV]");
  HistQQMjj->Sumw2();
  TH1D *HistQGMjj = new TH1D("HistQGMjj", "HistQGMjj", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistQGMjj->GetXaxis()->SetTitle("m_{jj} [GeV]");
  HistQGMjj->Sumw2();
  TH1D *HistGGMjj = new TH1D("HistGGMjj", "HistGGMjj", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistGGMjj->GetXaxis()->SetTitle("m_{jj} [GeV]");
  HistGGMjj->Sumw2();

  TH1D *HistQQFraction = new TH1D("HistQQFraction", "HistQQFraction", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistQQFraction->GetXaxis()->SetTitle("m_{jj} [GeV]");
  HistQQFraction->Sumw2();
  TH1D *HistQGFraction = new TH1D("HistQGFraction", "HistQGFraction", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistQGFraction->GetXaxis()->SetTitle("m_{jj} [GeV]");
  HistQGFraction->Sumw2();
  TH1D *HistGGFraction = new TH1D("HistGGFraction", "HistGGFraction", sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
  HistGGFraction->GetXaxis()->SetTitle("m_{jj} [GeV]");
  HistGGFraction->Sumw2();

  
  TFile *f = TFile::Open(infile.c_str());
  TTree *t = (TTree*) f->Get(intree.c_str());

  vector<float> *jet_pt = 0, *jet_NumTrkPt500PV = 0;
  float mjj, weight, yStar;
  double w = 1, sampleEvents = 0;

  sampleEvents = ((TH1D*)f->Get("cutflow_weighted"))->GetBinContent(1);

  t->SetBranchStatus("*", 0);
  t->SetBranchStatus("mjj", 1);
  t->SetBranchStatus("weight", 1);
  t->SetBranchStatus("jet_pt", 1);
  t->SetBranchStatus("jet_NumTrkPt500PV", 1);
  t->SetBranchStatus("yStar", 1);

  t->SetBranchAddress("mjj", &mjj);
  t->SetBranchAddress("weight", &weight);
  t->SetBranchAddress("jet_pt", &jet_pt);
  t->SetBranchAddress("jet_NumTrkPt500PV", &jet_NumTrkPt500PV);
  t->SetBranchAddress("yStar", &yStar);

  int nEntries = t->GetEntries();
  for (int i = 0; i < nEntries; i++){
    t->GetEntry(i);

    if (not EventLevelCuts((*jet_pt)[0], (*jet_pt)[1], yStar, mjj)) continue;
    
    w = weight / sampleEvents;
    HistMjj->Fill(mjj, w);
    HistLJetPt->Fill((*jet_pt)[0], w);
    HistSJetPt->Fill((*jet_pt)[1], w);

    bool quarkQuarkEvent = 0, quarkGluonEvent = 0, gluonGluonEvent = 0;
    quarkQuarkEvent = getQuarkSelection((*jet_pt)[0], (*jet_NumTrkPt500PV)[0]) and getQuarkSelection((*jet_pt)[1], (*jet_NumTrkPt500PV)[1]);
    gluonGluonEvent = getGluonSelection((*jet_pt)[0], (*jet_NumTrkPt500PV)[0]) and getGluonSelection((*jet_pt)[1], (*jet_NumTrkPt500PV)[1]);
    quarkGluonEvent = (getQuarkSelection((*jet_pt)[0], (*jet_NumTrkPt500PV)[0]) and getGluonSelection((*jet_pt)[1], (*jet_NumTrkPt500PV)[1])) || (getQuarkSelection((*jet_pt)[1], (*jet_NumTrkPt500PV)[1]) && getGluonSelection((*jet_pt)[0], (*jet_NumTrkPt500PV)[0]));

    if (quarkQuarkEvent) HistQQMjj->Fill(mjj, w);
    if (quarkGluonEvent) HistQGMjj->Fill(mjj, w);
    if (gluonGluonEvent) HistGGMjj->Fill(mjj, w);

  }

  f->Close();

  HistQQMjj->SetLineColor(kBlue);
  HistGGMjj->SetLineColor(kRed);
  HistQGMjj->SetLineColor(kGreen);

  *HistQQFraction = *HistQQMjj;
  *HistQGFraction = *HistQGMjj;
  *HistGGFraction = *HistGGMjj;

  HistQQFraction->Divide(HistMjj);
  HistQGFraction->Divide(HistMjj);
  HistGGFraction->Divide(HistMjj);

  TFile *fout = TFile::Open(outfile.c_str(), "recreate");
  HistMjj->Write();
  HistLJetPt->Write();
  HistSJetPt->Write();
  
  HistQQMjj->Write();
  HistQGMjj->Write();
  HistGGMjj->Write();

  fout->Close();

}

bool getQuarkSelection(float pt, float ntrack){
  // Q = 1, G = 0
  double value = log(pt);
  double SigmoidnTrack = quarkTrackSlope * value + quarkTrackOffset;
  if (ntrack < SigmoidnTrack) return 1;
  else return 0;
}

bool getGluonSelection(float pt, float ntrack){
  // G = 1, Q = 0
  double value = log(pt);
  double SigmoidnTrack = gluonTrackSlope * value + gluonTrackOffset;
  if (ntrack > SigmoidnTrack) return 1;
  else return 0;
}

bool EventLevelCuts(float leadingJetPt, float subLeadingJetPt, float yStar, float mjj){
  if (leadingJetPt < LJetPt) return 0;
  if (subLeadingJetPt < SJetPt) return 0;
  if (abs(yStar) > yStarMax) return 0;
  if (mjj < mjjMin) return 0;
  return 1;
}
