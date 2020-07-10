#include "TH1.h"
#include "TF1.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <stdio.h>

#include "TEnv.h"
#include "TFile.h"
#include "TH1F.h"
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
TString getInputPath(TString dataset, TString dataset0);
void getHist(TString histname, float type, float w);
std::map<TString, TH1F > Hist;

int main(int argc,char **argv){
    TString dataset = argv[1];
    TString dataset0 = argv[2];
    TString intree = argv[3];

    TH1F *Cutflow = new TH1F("cutflow", "cutflow", 9, 0.5, 9.5);
    TH1F *Cutflow_w = new TH1F("cutflow_weight", "cutflow_weight", 9, 0.5, 9.5);

    // variables used
    vector<float> *jet_pt = 0, *jet_NumTrkPt500PV = 0, *jet_eta = 0, *jet_phi = 0;
    vector<int> *jet_PartonTruthLabelID = 0, *jet_clean_passLooseBad = 0;
    vector<string> *passedTriggers = 0;
    int njet;
    float mjj, weight, yStar;
    float w = 1., sampleEvents = 0.;

    TString inputPath = getInputPath(dataset, dataset0);

    // Get TotalEventWeight here
    if (dataset=="MC"){
        void *dir0 = gSystem->OpenDirectory(inputPath);
        TString filename0 = gSystem->GetDirEntry(dir0);
        while (filename0 != ""){
            if (!filename0.Contains(".root")){
                filename0 = gSystem->GetDirEntry(dir0);
                continue;
            }
            TFile *f = TFile::Open(inputPath + filename0);
            TH1F *h = (TH1F*) f->Get("cutflow_weighted");
        
            if (h != 0){
                sampleEvents += h->GetBinContent(1);
            } else cout << "No cutflow_weighted in " << filename0 << endl;

            filename0 = gSystem->GetDirEntry(dir0);
        }
    }
  
    void *dir = gSystem->OpenDirectory(inputPath);
    TString filename = gSystem->GetDirEntry(dir);
    while (filename != ""){

        if (!filename.Contains(".root")){
            filename = gSystem->GetDirEntry(dir);
            continue;
        }
        TFile *f = TFile::Open(inputPath + filename);
        TTree *t = (TTree*) f->Get(intree);

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
            if (dataset=="MC"){
                t->SetBranchStatus("jet_PartonTruthLabelID", 1);
                t->SetBranchAddress("jet_PartonTruthLabelID", &jet_PartonTruthLabelID);
            }

            // loop all the entries in the ttree
            int nEntries = t->GetEntries();

            for (int i = 0; i < nEntries; i++){
                t->GetEntry(i);

                // calculate weight
                // and fill mjj, leading jet pt, sub leading jet pt and ntrack hist
                if (dataset=="MC"){
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
                if (abs(yStar) > yStarMax) continue;
                Cutflow->Fill(8);
                if (abs((*jet_eta)[0]) >= etaMax) continue;
                if (abs((*jet_eta)[1]) >= etaMax) continue;
                Cutflow->Fill(9);
				Cutflow_w = (TH1F*)Cutflow->Clone("Cutflow_weight");
				Cutflow_w->Scale(w);
               
                getHist("HistMjj", mjj ,w);
                getHist("HistyStar", yStar ,w);
                getHist("HistLeadingJetPt",(*jet_pt)[0], w);
                getHist("HistSubJetPt",(*jet_pt)[1], w);
                getHist("HistLjeteta",(*jet_eta)[0], w);
                getHist("HistSjeteta",(*jet_eta)[1], w);
                getHist("HistLjetphi",(*jet_phi)[0], w);
                getHist("HistSjetphi",(*jet_phi)[1], w);
                //getHist("Histdeltaeta",((*jet_eta)[0]-(*jet_eta)[1]), w);
                //getHist("Histdeltaphi",(*jet_phi)[0]-(*jet_phi)[1], w);
                getHist("HistNTrkl",(*jet_NumTrkPt500PV)[0], w);
                getHist("HistNTrks",(*jet_NumTrkPt500PV)[1], w);

                // parton truth label
                if (dataset=="MC"){
                    int truthLeading = getPartonLabel((*jet_PartonTruthLabelID)[0]);
                    int truthSub = getPartonLabel((*jet_PartonTruthLabelID)[1]);
                    int isTaggedG_Lead = getGluonSelection((*jet_pt)[0], (*jet_NumTrkPt500PV)[0]);
                    int isTaggedG_Sub =  getGluonSelection((*jet_pt)[1], (*jet_NumTrkPt500PV)[1]);
                    if (truthLeading == 1) {
                        getHist("HistNTrkl_G", (*jet_NumTrkPt500PV)[0], w);
                        getHist("HistLeadingJetPt_TruthG",(*jet_pt)[0], w);
                        if(isTaggedG_Lead) getHist("HistLeadingJetPt_TruthG_TaggedG",(*jet_pt)[0], w);
                    }
                    else if (truthLeading == 0)  {
                        getHist("HistNTrkl_Q",(*jet_NumTrkPt500PV)[0], w);
                        getHist("HistLeadingJetPt_TruthQ",(*jet_pt)[0], w);
                        if(isTaggedG_Lead) getHist("HistLeadingJetPt_TruthQ_TaggedG",(*jet_pt)[0], w);
                    }
                    if (truthSub == 1) {
                        getHist("HistNTrks_G",(*jet_NumTrkPt500PV)[1], w);
                        getHist("HistSubJetPt_TruthG",(*jet_pt)[1], w);
                        if (isTaggedG_Sub) getHist("HistSubJetPt_TruthG_TaggedG",(*jet_pt)[1], w);
                    }
                    else if (truthSub == 0) {
                        getHist("HistNTrks_Q",(*jet_NumTrkPt500PV)[1], w);
                        getHist("HistSubJetPt_TruthQ",(*jet_pt)[1], w);
                        if (isTaggedG_Sub) getHist("HistSubJetPt_TruthQ_TaggedG",(*jet_pt)[1], w);
                    }
                    if (truthLeading == 1 || truthSub == 1){  // GJ dijet
                        getHist("HistMjj_TruthGJ",mjj, w);
                        getHist("HistLeadingJetPt_TruthGJ",(*jet_pt)[0], w);
                        getHist("HistSubJetPt_TruthGJ",(*jet_pt)[1], w);
                    }
                    if ((truthLeading + truthSub) == 2){  // GG dijet
                        getHist("HistMjj_TruthGG",mjj, w);
                        getHist("HistLeadingJetPt_TruthGG",(*jet_pt)[0], w);
                        getHist("HistSubJetPt_TruthGG",(*jet_pt)[1], w);
                    } else if ((truthLeading + truthSub) == 1){  // QG dijet
                        getHist("HistMjj_TruthQG",mjj, w);
                        getHist("HistLeadingJetPt_TruthQG",(*jet_pt)[0], w);
                        getHist("HistSubJetPt_TruthQG",(*jet_pt)[1], w);
                    } else if ((truthLeading + truthSub) == 0){  // QQ dijet
                        getHist("HistMjj_TruthQQ",mjj, w);
                        getHist("HistLeadingJetPt_TruthQQ",(*jet_pt)[0], w);
                        getHist("HistSubJetPt_TruthQQ",(*jet_pt)[1], w);
                    }
                }
    
                // dijet gg/gj/jj selection by NumTrk cut
                int numberGJet = 0;
                numberGJet = getGluonSelection((*jet_pt)[0], (*jet_NumTrkPt500PV)[0]) + getGluonSelection((*jet_pt)[1], (*jet_NumTrkPt500PV)[1]);

                

                if (numberGJet >= 1){
                    getHist("HistMjj_GJ",mjj, w);
                    getHist("HistLeadingJetPt_GJ",(*jet_pt)[0], w);
                    getHist("HistSubJetPt_GJ",(*jet_pt)[1], w);
                }
                if (numberGJet == 2){
                    getHist("HistMjj_GG",mjj, w);
                    getHist("HistLeadingJetPt_GG",(*jet_pt)[0], w);
                    getHist("HistSubJetPt_GG",(*jet_pt)[1], w);
                } else if (numberGJet == 1){
                    getHist("HistMjj_QG",mjj, w);
                    getHist("HistLeadingJetPt_QG",(*jet_pt)[0], w);
                    getHist("HistSubJetPt_QG",(*jet_pt)[1], w);
                } else if (numberGJet == 0){
                    getHist("HistMjj_QQ",mjj, w);
                    getHist("HistLeadingJetPt_QQ",(*jet_pt)[0], w);
                    getHist("HistSubJetPt_QQ",(*jet_pt)[1], w);
                }
            }
        } else cout << "No " << intree << endl;

        f->Close();
        delete f, t;
        filename = gSystem->GetDirEntry(dir);
    }

    TFile *fout = TFile::Open(dataset + "_" + dataset0 + ".root", "recreate");
    Cutflow->Write();
	//Cutflow_w->Write();
    for(std::pair<TString, TH1F> hist : Hist) {
        TH1F hist1 = hist.second;
        hist1.Write();
    }
	fout->Write();
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

int getPartonLabel(int partonlabelid){
    int type = -99 ;
    if (partonlabelid == 21){
        type = 1;
    }else if (0<partonlabelid && partonlabelid<7){
        type = 0;
    }
    return type;
}

TString getInputPath(TString dataset, TString dataset0){
    TString inputPath;
    if (dataset=="MC"){
        inputPath = "/eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningMC/";
        if (dataset0 == "MC16a_JZ4W"){
            inputPath += "QCD_MC16a/user.bdong.mc16_13TeV.364704.Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ4WithSW.m16a_newJetCleaning_May9_tree.root/";
            return inputPath;
        }
        if (dataset0.Contains("MC16")){
            TString period = dataset0(dataset0.Index("MC16") + 4);
            if (!(dataset0.Contains("JZ") && dataset0.Contains("W"))){
                cout << "Wrong dataset tag: " << dataset0 << endl;
                return "";
            }
            TString jzxw = dataset0(dataset0.Index("JZ") + 2, dataset0.Index("W") - dataset0.Index("JZ") - 2);
            inputPath += "QCD_MC16" + period;
            inputPath += "/user.bdong.mc16_13TeV.3647" + ((jzxw.Length() == 1) ? ("0" + jzxw) : (jzxw)) + ".Pythia8EvtGen_A14NNPDF23LO_jetjet_JZ" + jzxw + "WithSW.mc16" + period + "_newJetCleaning_May9_tree.root/";
        }
    }
    else if (dataset.Contains("Data")){
        inputPath = "/eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningData/";
        inputPath += dataset + "/" + dataset0 + "/";
    }
    return inputPath;
}

void getHist(TString histname, float type, float w){
    auto itr = Hist.find(histname); 
    if (itr != Hist.end()) {
		Hist[histname].Sumw2();
		Hist[histname].Fill(type,w);
	}
    else{
        if (histname.Contains("Mjj")){
         TH1F hist(histname, histname, sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
			Hist[histname] = hist;
            Hist[histname].GetXaxis()->SetTitle("m_{jj} [GeV]");
        }
        if (histname.Contains("Pt")){
			TH1F hist(histname, histname, sizeof(binLowMassGeV) / sizeof(binLowMassGeV[0]) - 1, binLowMassGeV);
			Hist[histname] = hist;
            Hist[histname].GetXaxis()->SetTitle("pT [GeV]");
        }
        if (histname.Contains("NTrk")){
            TH1F hist(histname, histname, 80, -0.5, 79.5);
			Hist[histname] = hist;
            Hist[histname].GetXaxis()->SetTitle("N_{track}");
        }
        if (histname.Contains("yStar")){
            TH1F hist(histname, histname, 40, -1.2, 1.2);
			Hist[histname] = hist;
            Hist[histname].GetXaxis()->SetTitle("yStar");
        }
        if (histname.Contains("eta")){
            TH1F hist(histname, histname, 50, -5, 5);
			Hist[histname] = hist;
            Hist[histname].GetXaxis()->SetTitle("eta");
        }
        if (histname.Contains("phi")){
            TH1F hist(histname, histname, 100, -5, 5);
			Hist[histname] = hist;
            Hist[histname].GetXaxis()->SetTitle("phi");
        }
        if (histname.Contains("deltaeta")){
            TH1F hist(histname, histname, 50, -5, 5);
			Hist[histname] = hist;
            Hist[histname].GetXaxis()->SetTitle("delta_eta");
        }
        if (histname.Contains("deltaphi")){
            TH1F hist(histname, histname, 100, -5, 5);
			Hist[histname] = hist;
            Hist[histname].GetXaxis()->SetTitle("delta_phi");
        }
		Hist[histname].Sumw2();
        Hist[histname].Fill(type,w);
    }
    //return Hist;
}
