using namespace std;

void add() {
    
    string root_lists = "outfile";
    string sLine="";
    ifstream infile;
    infile.open(root_lists.c_str());//Data());
    
    int i=0;
    TH1D* HistMjj;
    TH1D* HistLJetPt;
    TH1D* HistSJetPt;
    TH1D* HistQQMjj;
    TH1D* HistQGMjj;
    TH1D* HistGGMjj;
    while(getline(infile,sLine))  {
        TFile *f2 = new TFile(sLine.c_str());
	if (f2 == 0){
	  cout << "No output file named " << infile << endl;
	  continue;
	}
        if(i==0){
            HistMjj = (TH1D*) f2->Get("HistMjj");
            HistLJetPt = (TH1D*) f2->Get("HistLJetPt");
            HistSJetPt = (TH1D*) f2->Get("HistSJetPt");
            HistQQMjj = (TH1D*) f2->Get("HistQQMjj");
            HistQGMjj = (TH1D*) f2->Get("HistQGMjj");
            HistGGMjj = (TH1D*) f2->Get("HistGGMjj");
        }
        else{
            HistMjj->Add((TH1D*) f2->Get("HistMjj"),1);
            HistLJetPt->Add((TH1D*) f2->Get("HistLJetPt"),1);
            HistSJetPt->Add((TH1D*) f2->Get("HistSJetPt"),1);
            HistQQMjj->Add((TH1D*) f2->Get("HistQQMjj"),1);
            HistQGMjj->Add((TH1D*) f2->Get("HistQGMjj"),1);
            HistGGMjj->Add((TH1D*) f2->Get("HistGGMjj"),1);
        }
        i++;
    }

    TFile *fout = TFile::Open("plot.root", "recreate");
    HistMjj->Write();
    HistLJetPt->Write();
    HistSJetPt->Write();
    
    HistQQMjj->Write();
    HistQGMjj->Write();
    HistGGMjj->Write();

    fout->Close();
}
