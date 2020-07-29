#include "TH1.h"
#include "TF1.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <math.h>

#include "TEnv.h"
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TPad.h"
#include "TVector.h"
#include "TStopwatch.h"
#include "TLine.h"
#include "TLegend.h"
#include "TSystem.h"

using namespace std;

double binLowMassGeV[] = {203.0, 216.0, 229.0, 243.0, 257.0, 272.0, 287.0, 303.0, 319.0, 335.0, 352.0, 369.0, 387.0, 405.0, 424.0, 443.0, 462.0, 482.0, 502.0, 523.0, 544.0, 566.0, 588.0, 611.0, 634.0, 657.0, 681.0, 705.0, 730.0, 755.0, 781.0, 807.0, 834.0, 861.0, 889.0, 917.0, 946.0, 976.0, 1006.0, 1037.0, 1068.0, 1100.0, 1133.0, 1166.0, 1200.0, 1234.0, 1269.0, 1305.0, 1341.0, 1378.0, 1416.0, 1454.0, 1493.0, 1533.0, 1573.0, 1614.0, 1656.0, 1698.0, 1741.0, 1785.0, 1830.0, 1875.0, 1921.0, 1968.0, 2016.0, 2065.0, 2114.0, 2164.0, 2215.0, 2267.0, 2320.0, 2374.0, 2429.0, 2485.0, 2542.0, 2600.0, 2659.0, 2719.0, 2780.0, 2842.0, 2905.0, 2969.0, 3034.0, 3100.0, 3167.0, 3235.0, 3305.0, 3376.0, 3448.0, 3521.0, 3596.0, 3672.0, 3749.0, 3827.0, 3907.0, 3988.0, 4070.0, 4154.0, 4239.0, 4326.0, 4414.0, 4504.0, 4595.0, 4688.0, 4782.0, 4878.0, 4975.0, 5074.0, 5175.0, 5277.0, 5381.0, 5487.0, 5595.0, 5705.0, 5817.0, 5931.0, 6047.0, 6165.0, 6285.0, 6407.0, 6531.0, 6658.0, 6787.0, 6918.0, 7052.0, 7188.0, 7326.0, 7467.0, 7610.0, 7756.0, 7904.0, 8055.0, 8208.0, 8364.0, 8523.0, 8685.0, 8850.0, 9019.0, 9191.0, 9366.0, 9544.0, 9726.0, 9911.0, 10100.0, 10292.0, 10488.0, 10688.0, 10892.0, 11100.0, 11312.0, 11528.0, 11748.0, 11972.0, 12200.0, 12432.0, 12669.0, 12910.0, 13156.0};
double binLowPtGeV[] = {15. ,20. ,25. ,35. ,45. ,55. ,70. ,85. ,100. ,116. ,134. ,152. ,172. ,194. ,216. ,240. ,264. ,290. ,318. ,346.,376.,408.,442.,478.,516.,556.,598.,642.,688.,736.,786.,838.,894.,952.,1012.,1076.,1162.,1250.,1310.,1420.,1530.,1750.,1992.,2250.,2500.,2850.,3200.,3600.,4000.,4600.};

float leadingJetPtMin = 420.;
float jetPtMin = 150.;
float etaMax = 2.1;
float yStarMax = 0.6;
float mjjMin = 1100;
string trigger = "HLT_j420";

double gluonTrackOffset = -7.26742;
double gluonTrackSlope = 4.16218;
//double quarkTrackOffset = -7.55743;
//double quarkTrackSlope = 3.5915;

//bool getQuarkSelection(float pt, float ntrack);
bool getGluonSelection(float pt, float ntrack);
int getPartonLabel(int partonlabelid);
TH1D *getHist(TString type, TString name, TString title);

int main(int argc,char **argv){
    TString dataset = argv[1];
    string period = argv[2];// a, d, or e
    TString mass = argv[3];
    string num = argv[4]; // data period or mc slice
 
    // Mass Hist
    TH1D *HistMjj = getHist("mass", "HistMjj", "mjj");
    TH1D *HistMjj_TruthGG = getHist("mass", "HistMjj_TruthGG", "mjj in truth labelled GG events");
    TH1D *HistMjj_TruthGJ = getHist("mass", "HistMjj_TruthGJ", "mjj in truth labelled GJ events");
    TH1D *HistMjj_TruthQG = getHist("mass", "HistMjj_TruthQG", "mjj in truth labelled QG events");
    TH1D *HistMjj_TruthQQ = getHist("mass", "HistMjj_TruthQQ", "mjj in truth labelled QQ events");
    TH1D *HistMjj_GG = getHist("mass", "HistMjj_GG", "mjj in tagged GG events");
    TH1D *HistMjj_GJ = getHist("mass", "HistMjj_GJ", "mjj in tagged GJ events");
    TH1D *HistMjj_QG = getHist("mass", "HistMjj_QG", "mjj in tagged QG events");
    TH1D *HistMjj_QQ = getHist("mass", "HistMjj_QQ", "mjj in tagged QQ events");

    // Leading jet pt Hist
    TH1D *HistLeadingJetPt = getHist("pt", "HistLeadingJetPt", "leading jet pt");
    TH1D *HistLeadingJetPt_TruthG = getHist("pt", "HistLeadingJetPt_TruthG", "leading jet pt in truth labelled G");
    TH1D *HistLeadingJetPt_TruthQ = getHist("pt", "HistLeadingJetPt_TruthQ", "leading jet pt in truth labelled Q");
    TH1D *HistLeadingJetPt_TruthG_TaggedG = getHist("pt", "HistLeadingJetPt_TruthG_TaggedG", "leading jet pt in truth labelled G tagged as G");
    TH1D *HistLeadingJetPt_TruthQ_TaggedG = getHist("pt", "HistLeadingJetPt_TruthQ_TaggedG", "leading jet pt in truth labelled Q tagged as G");
    TH1D *HistLeadingJetPt_TruthGG = getHist("pt", "HistLeadingJetPt_TruthGG", "leading jet pt in truth labelled GG events");
    TH1D *HistLeadingJetPt_TruthGJ = getHist("pt", "HistLeadingJetPt_TruthGJ", "leading jet pt in truth labelled GJ events");
    TH1D *HistLeadingJetPt_TruthQG = getHist("pt", "HistLeadingJetPt_TruthQG", "leading jet pt in truth labelled QG events");
    TH1D *HistLeadingJetPt_TruthQQ = getHist("pt", "HistLeadingJetPt_TruthQQ", "leading jet pt in truth labelled QQ events");
    TH1D *HistLeadingJetPt_G = getHist("pt", "HistLeadingJetPt_G", "leading jet pt in tagged G events");
    TH1D *HistLeadingJetPt_Q = getHist("pt", "HistLeadingJetPt_Q", "leading jet pt in tagged Q events");
    TH1D *HistLeadingJetPt_GG = getHist("pt", "HistLeadingJetPt_GG", "leading jet pt in tagged GG events");
    TH1D *HistLeadingJetPt_GJ = getHist("pt", "HistLeadingJetPt_GJ", "leading jet pt in tagged GJ events");
    TH1D *HistLeadingJetPt_QG = getHist("pt", "HistLeadingJetPt_QG", "leading jet pt in tagged QG events");
    TH1D *HistLeadingJetPt_QQ = getHist("pt", "HistLeadingJetPt_QQ", "leading jet pt in tagged QQ events");

    // Sub Leading jet pt Hist
    TH1D *HistSubJetPt = getHist("pt", "HistSubJetPt", "sub leading jet pt");
    TH1D *HistSubJetPt_TruthG = getHist("pt", "HistSubJetPt_TruthG", "sub leading jet pt in truth labelled G");
    TH1D *HistSubJetPt_TruthQ = getHist("pt", "HistSubJetPt_TruthQ", "sub leading jet pt in truth labelled Q");
    TH1D *HistSubJetPt_TruthG_TaggedG = getHist("pt", "HistSubJetPt_TruthG_TaggedG", "sub leading jet pt in truth labelled G tagged as G");
    TH1D *HistSubJetPt_TruthQ_TaggedG = getHist("pt", "HistSubJetPt_TruthQ_TaggedG", "sub leading jet pt in truth labelled Q tagged as G");
    TH1D *HistSubJetPt_TruthGG = getHist("pt", "HistSubJetPt_TruthGG", "sub leading jet pt in truth labelled GG events");
    TH1D *HistSubJetPt_TruthGJ = getHist("pt", "HistSubJetPt_TruthGJ", "sub leading jet pt in truth labelled GJ events");
    TH1D *HistSubJetPt_TruthQG = getHist("pt", "HistSubJetPt_TruthQG", "sub leading jet pt in truth labelled QG events");
    TH1D *HistSubJetPt_TruthQQ = getHist("pt", "HistSubJetPt_TruthQQ", "sub leading jet pt in truth labelled QQ events");
    TH1D *HistSubJetPt_G = getHist("pt", "HistSubJetPt_G", "sub leading jet pt in tagged G events");
    TH1D *HistSubJetPt_Q = getHist("pt", "HistSubJetPt_Q", "sub leading jet pt in tagged Q events");
    TH1D *HistSubJetPt_GG = getHist("pt", "HistSubJetPt_GG", "sub leading jet pt in tagged GG events");
    TH1D *HistSubJetPt_GJ = getHist("pt", "HistSubJetPt_GJ", "sub leading jet pt in tagged GJ events");
    TH1D *HistSubJetPt_QG = getHist("pt", "HistSubJetPt_QG", "sub leading jet pt in tagged QG events");
    TH1D *HistSubJetPt_QQ = getHist("pt", "HistSubJetPt_QQ", "sub leading jet pt in tagged QQ events");

    // number of track hist
    TH1D *HistNTrkl = getHist("ntrk", "HistNTrkl", "number of tracks");
    TH1D *HistNTrkl_Q = getHist("ntrk", "HistNTrkl_Q", "number of tracks in Q jets");
    TH1D *HistNTrkl_G = getHist("ntrk", "HistNTrkl_G", "number of tracks in G jets");
    TH1D *HistNTrks = getHist("ntrk", "HistNTrks", "number of tracks");
    TH1D *HistNTrks_Q = getHist("ntrk", "HistNTrks_Q", "number of tracks in Q jets");
    TH1D *HistNTrks_G = getHist("ntrk", "HistNTrks_G", "number of tracks in G jets");
    
    TH1D *HistyStar = getHist("ystar", "HistyStar", "yStar");
    
    TH1D *HistLjeteta = getHist("eta", "HistLjeteta", "eta");
    TH1D *HistSjeteta = getHist("eta", "HistSjeteta", "eta");
    TH1D *HistLjetphi = getHist("phi", "HistLjetphi", "phi");
    TH1D *HistSjetphi = getHist("phi", "HistSjetphi", "phi");
    
    TH1D *Histdeltaeta = getHist("deltaeta", "Histdeltaeta", "deltaeta");
    TH1D *Histdeltaphi = getHist("deltaphi", "Histdeltaphi", "deltaphi");
    TH1D *Cutflow = new TH1D("cutflow", "cutflow", 9, 0.5, 9.5);
    TH1D *Cutflow_weight = new TH1D("cutflow_weight", "cutflow_weight", 9, 0.5, 9.5);

    // variables used
    vector<float> *jet_pt = 0, *jet_NumTrkPt500PV = 0, *jet_eta = 0, *jet_phi = 0;
    vector<int> *jet_PartonTruthLabelID = 0, *jet_clean_passLooseBad = 0;
    vector<string> *passedTriggers = 0;
    int njet;
    float mjj, weight, yStar;
    double w = 1, sampleEvents = 0;

    // Get TotalEventWeight here

	string p = "mc16"+period;
	string p1 = "mc"+period;
	string slice = "JZ"+num+"W";
	string datap = "data"+num;
	ifstream infile;
	ifstream infile1;
	string root_list="";
	infile.open("../Input/"+dataset+".txt");
	infile1.open("../Input/"+dataset+".txt");
	while(getline(infile,root_list)){	
		if( (dataset.Contains("MC")) && (root_list.find(slice) != string::npos)) {
			cout<<"in file: "<<root_list.c_str()<<endl;;
			TFile *f = TFile::Open(root_list.c_str());
			TH1D *h = (TH1D*) f->Get("cutflow_weighted");
			if (h != 0){
				sampleEvents += h->GetBinContent(1);
			} else cout << "No cutflow_weighted in " << root_list.c_str() << endl;
		}
	}
	while(getline(infile1,root_list)){	
		if(((root_list.find(mass) != string::npos) && ((root_list.find(p) != string::npos) || (root_list.find(p1) != string::npos))) || (root_list.find(slice) != string::npos) || (root_list.find(datap) != string::npos)){
			TFile *f = TFile::Open(root_list.c_str());
			TTree *t = (TTree*) f->Get("outTree");

        if ((t != 0)){

            // set the needed branch status and branch address
            // to get variables from the ttree
            t->SetBranchStatus("*", 0);
            t->SetBranchStatus("mjj", 1);
            t->SetBranchStatus("weight", 1);
            t->SetBranchStatus("jet_pt", 1);
            t->SetBranchStatus("jet_NumTrkPt500PV", 1);
            t->SetBranchStatus("yStar", 1);
            t->SetBranchStatus("passedTriggers", 1);
            t->SetBranchStatus("njet", 1);
            t->SetBranchStatus("jet_eta", 1);
            t->SetBranchStatus("jet_phi", 1);
            t->SetBranchStatus("jet_clean_passLooseBad", 1);

            t->SetBranchAddress("mjj", &mjj);
            t->SetBranchAddress("weight", &weight);
            t->SetBranchAddress("jet_pt", &jet_pt);
            t->SetBranchAddress("jet_NumTrkPt500PV", &jet_NumTrkPt500PV);
            t->SetBranchAddress("yStar", &yStar);
            t->SetBranchAddress("passedTriggers", &passedTriggers);
            t->SetBranchAddress("njet", &njet);
            t->SetBranchAddress("jet_eta", &jet_eta);
            t->SetBranchAddress("jet_phi", &jet_phi);
            t->SetBranchAddress("jet_clean_passLooseBad", &jet_clean_passLooseBad);
            if (dataset.Contains("MC")){
                t->SetBranchStatus("jet_PartonTruthLabelID", 1);
                t->SetBranchAddress("jet_PartonTruthLabelID", &jet_PartonTruthLabelID);
            }

            // loop all the entries in the ttree
            int nEntries = t->GetEntries();

            for (int i = 0; i < nEntries; i++){
                t->GetEntry(i);

                // calculate weight
                // and fill mjj, leading jet pt, sub leading jet pt and ntrack hist
                if (dataset.Contains("MC")){
                    w = weight / sampleEvents;
                }
                
                vector<string>::iterator location = find(passedTriggers->begin(), passedTriggers->end(), trigger);
                Cutflow->Fill(1);
                Cutflow->Fill(2);
                if ((*jet_clean_passLooseBad)[0] == 0) continue;
                if ((*jet_clean_passLooseBad)[1] == 0) continue;
                Cutflow->Fill(3);
                if (location == passedTriggers->end()) continue;
                Cutflow->Fill(4);
                Cutflow->Fill(5);
                if ((*jet_pt)[0] < leadingJetPtMin) continue;
                Cutflow->Fill(6);
                if (mjj < mjjMin) continue;
                Cutflow->Fill(7);
                if (abs(yStar) >= yStarMax) continue;
                Cutflow->Fill(8);
                if (abs((*jet_eta)[0]) >= etaMax) continue;
                if (abs((*jet_eta)[1]) >= etaMax) continue;
                Cutflow->Fill(9);
				Cutflow_weight->Add(Cutflow,w);
                
                HistMjj->Fill(mjj, w);
                HistyStar->Fill(yStar, w);
                HistLeadingJetPt->Fill((*jet_pt)[0], w);
                HistSubJetPt->Fill((*jet_pt)[1], w);
                HistLjeteta->Fill((*jet_eta)[0], w);
                HistSjeteta->Fill((*jet_eta)[1], w);
                HistLjetphi->Fill((*jet_phi)[0], w);
                HistSjetphi->Fill((*jet_phi)[1], w);
                Histdeltaeta->Fill((*jet_eta)[0]-(*jet_eta)[1], w);
                Histdeltaphi->Fill((*jet_phi)[0]-(*jet_phi)[1], w);
                HistNTrkl->Fill((*jet_NumTrkPt500PV)[0], w);
                HistNTrks->Fill((*jet_NumTrkPt500PV)[1], w);

                // parton truth label
                if (dataset.Contains("MC")){
                    int truthLeading = getPartonLabel((*jet_PartonTruthLabelID)[0]);
                    int truthSub = getPartonLabel((*jet_PartonTruthLabelID)[1]);
                    int isTaggedG_Lead = getGluonSelection((*jet_pt)[0], (*jet_NumTrkPt500PV)[0]);
                    int isTaggedG_Sub =  getGluonSelection((*jet_pt)[1], (*jet_NumTrkPt500PV)[1]);
                    if (truthLeading == 1) {
                        HistNTrkl_G->Fill((*jet_NumTrkPt500PV)[0], w);
                        HistLeadingJetPt_TruthG->Fill((*jet_pt)[0], w);
                        if(isTaggedG_Lead) HistLeadingJetPt_TruthG_TaggedG->Fill((*jet_pt)[0], w);
                    }
                    else if (truthLeading == 0)  {
                        HistNTrkl_Q->Fill((*jet_NumTrkPt500PV)[0], w);
                        HistLeadingJetPt_TruthQ->Fill((*jet_pt)[0], w);
                        if(isTaggedG_Lead) HistLeadingJetPt_TruthQ_TaggedG->Fill((*jet_pt)[0], w);
                    }
                    if (truthSub == 1) {
                        HistNTrks_G->Fill((*jet_NumTrkPt500PV)[1], w);
                        HistSubJetPt_TruthG->Fill((*jet_pt)[1], w);
                        if (isTaggedG_Sub)HistSubJetPt_TruthG_TaggedG->Fill((*jet_pt)[1], w);
                    }
                    else if (truthSub == 0) {
                        HistNTrks_Q->Fill((*jet_NumTrkPt500PV)[1], w);
                        HistSubJetPt_TruthQ->Fill((*jet_pt)[1], w);
                        if (isTaggedG_Sub)HistSubJetPt_TruthQ_TaggedG->Fill((*jet_pt)[1], w);
                    }
                    if (truthLeading == 1 || truthSub == 1){  // GJ dijet
                        HistMjj_TruthGJ->Fill(mjj, w);
                        HistLeadingJetPt_TruthGJ->Fill((*jet_pt)[0], w);
                        HistSubJetPt_TruthGJ->Fill((*jet_pt)[1], w);
                    }
                    if ((truthLeading + truthSub) == 2){  // GG dijet
                        HistMjj_TruthGG->Fill(mjj, w);
                        HistLeadingJetPt_TruthGG->Fill((*jet_pt)[0], w);
                        HistSubJetPt_TruthGG->Fill((*jet_pt)[1], w);
                    } else if ((truthLeading + truthSub) == 1){  // QG dijet
                        HistMjj_TruthQG->Fill(mjj, w);
                        HistLeadingJetPt_TruthQG->Fill((*jet_pt)[0], w);
                        HistSubJetPt_TruthQG->Fill((*jet_pt)[1], w);
                    } else if ((truthLeading + truthSub) == 0){  // QQ dijet
                        HistMjj_TruthQQ->Fill(mjj, w);
                        HistLeadingJetPt_TruthQQ->Fill((*jet_pt)[0], w);
                        HistSubJetPt_TruthQQ->Fill((*jet_pt)[1], w);
                    }
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
        } else cout << "No outtree" << endl;

			f->Close();
			delete f, t;

			TFile *fout = TFile::Open("../output/"+dataset + "_" +mass+"_"+p+"_"+num+".root", "recreate");
			Cutflow->Write();
			Cutflow_weight->Write();
			HistMjj->Write();
			HistMjj_GG->Write();
			HistMjj_GJ->Write();
			HistMjj_QG->Write();
			HistMjj_QQ->Write();

			HistLeadingJetPt->Write();
			HistLeadingJetPt_GG->Write();
			HistLeadingJetPt_GJ->Write();
			HistLeadingJetPt_QG->Write();
			HistLeadingJetPt_QQ->Write();

			HistSubJetPt->Write();
			HistSubJetPt_GG->Write();
			HistSubJetPt_GJ->Write();
			HistSubJetPt_QG->Write();
			HistSubJetPt_QQ->Write();

			HistNTrkl->Write();
			HistNTrkl_G->Write();
			HistNTrkl_Q->Write();
			HistNTrks->Write();
			HistNTrks_G->Write();
			HistNTrks_Q->Write();
			
			HistyStar->Write();
			HistLjeteta->Write();
			HistSjeteta->Write();
			HistLjetphi->Write();
			HistSjetphi->Write();
			Histdeltaeta->Write();
			Histdeltaphi->Write();
			
			if (dataset.Contains("MC")){
				HistMjj_TruthGG->Write();
				HistMjj_TruthGJ->Write();
				HistMjj_TruthQG->Write();
				HistMjj_TruthQQ->Write();
				HistLeadingJetPt_TruthG->Write();
				HistLeadingJetPt_TruthQ->Write();
				HistLeadingJetPt_TruthG_TaggedG->Write();
				HistLeadingJetPt_TruthQ_TaggedG->Write();
				HistLeadingJetPt_TruthGG->Write();
				HistLeadingJetPt_TruthGJ->Write();
				HistLeadingJetPt_TruthQG->Write();
				HistLeadingJetPt_TruthQQ->Write();
				HistSubJetPt_TruthGG->Write();
				HistSubJetPt_TruthGJ->Write();
				HistSubJetPt_TruthQG->Write();
				HistSubJetPt_TruthQQ->Write();
				HistSubJetPt_TruthG->Write();
				HistSubJetPt_TruthQ->Write();
				HistSubJetPt_TruthG_TaggedG->Write();
				HistSubJetPt_TruthQ_TaggedG->Write();
				}

    		fout->Close();
		}
	}
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
    int SigmoidnTrack = (int)(gluonTrackSlope * value + gluonTrackOffset);
//    double SigmoidnTrack = gluonTrackSlope * value + gluonTrackOffset;
    if (ntrack >= SigmoidnTrack) return 1;
    else return 0;
}

int getPartonLabel(int partonlabelid){
    int type = -99 ;
    if (partonlabelid == 21){
        type = 1;
    }else if (0<partonlabelid && partonlabelid<7){
        type = 0;
    }
    return type;
}

TH1D *getHist(TString type, TString name, TString title){
    TH1D *Hist;
    if (type=="mass"){
        Hist = new TH1D(name, title, sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
        Hist->GetXaxis()->SetTitle("m_{jj} [GeV]");
    }
    if (type=="pt"){
        Hist = new TH1D(name, title, sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
        Hist->GetXaxis()->SetTitle("Pt [GeV]");
    }
    if (type=="ntrk"){
        Hist = new TH1D(name, title, 80, -0.5, 79.5);
        Hist->GetXaxis()->SetTitle("N_{track}");
    }
    if (type=="ystar"){
        Hist = new TH1D(name, title, 40, -1.2, 1.2);
        Hist->GetXaxis()->SetTitle("yStar");
    }
    if (type=="eta"){
        Hist = new TH1D(name, title, 50, -5, 5);
        Hist->GetXaxis()->SetTitle("eta");
    }
    if (type=="phi"){
        Hist = new TH1D(name, title, 100, -5, 5);
        Hist->GetXaxis()->SetTitle("phi");
    }
    if (type=="deltaeta"){
        Hist = new TH1D(name, title, 50, -5, 5);
        Hist->GetXaxis()->SetTitle("delta_eta");
    }
    if (type=="deltaphi"){
        Hist = new TH1D(name, title, 100, -5, 5);
        Hist->GetXaxis()->SetTitle("delta_phi");
    }
    Hist->Sumw2();
    return Hist;
}
