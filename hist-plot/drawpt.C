using namespace std;

void drawpt(TString root_lists, TString root_file) {
    
    string sLine="";
    ifstream infile;
    infile.open(root_lists);//Data());
    
    auto legend = new TLegend(0.8, 0.28, 0.89, 0.75);
    TH1D* HistLJetPt;
    TCanvas *c = new TCanvas("c","c",1100,550);
    gPad->SetLogy();
    TFile *f2 = new TFile(root_file);
    HistLJetPt = (TH1D*) f2->Get("HistLeadingJetPt");
    HistLJetPt->SetAxisRange(400., 8100.,"X");
    HistLJetPt->SetLineColor(1);
    HistLJetPt->Draw();
    legend->AddEntry(HistLJetPt, "total", "l");
    int i=0;
    while(getline(infile,sLine))  {
        TFile *f2 = new TFile(sLine.c_str());
        HistLJetPt = (TH1D*) f2->Get("HistLeadingJetPt");
        HistLJetPt->SetAxisRange(400., 8100.,"X");
        if(i==0){
        HistLJetPt->SetLineColor(38);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ10W", "l");
        }
        if(i==1){
        HistLJetPt->SetLineColor(2);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ11W", "l");
        }
        if(i==2){
        HistLJetPt->SetLineColor(3);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ12W", "l");
        }
        if(i==5){
        HistLJetPt->SetLineColor(4);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ3W", "l");
        }
        if(i==6){
        HistLJetPt->SetLineColor(6);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ4W", "l");
        }
        if(i==7){
        HistLJetPt->SetLineColor(7);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ5W", "l");
        }
        if(i==8){
        HistLJetPt->SetLineColor(8);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ6W", "l");
        }
        if(i==9){
        HistLJetPt->SetLineColor(9);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ7W", "l");
        }
        if(i==10){
        HistLJetPt->SetLineColor(31);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ8W", "l");
        }
        if(i==11){
        HistLJetPt->SetLineColor(46);
        HistLJetPt->Draw("same");
        legend->AddEntry(HistLJetPt, "JZ9W", "l");
        }
        i++;
    }
    HistLJetPt = (TH1D*) f2->Get("HistLeadingJetPt");
    HistLJetPt->SetAxisRange(400., 8100.,"X");
    HistLJetPt->SetLineColor(1);
    HistLJetPt->Draw("same");
    legend->Draw();
    c->SaveAs("pt.pdf");
}
