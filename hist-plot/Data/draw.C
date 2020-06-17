using namespace std;

void draw() {
    TCanvas *c = new TCanvas("c","c",1100,750);
    gPad->SetLogy();
    TFile *f2 = new TFile("mjj.root");
    TH1D* HistMjj = (TH1D*) f2->Get("HistMjj");
    TH1D* HistMjj_GG = (TH1D*) f2->Get("HistMjj_GG");
    TH1D* HistMjj_GJ = (TH1D*) f2->Get("HistMjj_GJ");
    TH1D* HistMjj_QG = (TH1D*) f2->Get("HistMjj_QG");
    TH1D* HistMjj_QQ = (TH1D*) f2->Get("HistMjj_QQ");
    TH1D* HistLeadingJetPt = (TH1D*) f2->Get("HistLeadingJetPt");
    TH1D* HistLeadingJetPt_GG = (TH1D*) f2->Get("HistLeadingJetPt_GG");
    TH1D* HistLeadingJetPt_GJ = (TH1D*) f2->Get("HistLeadingJetPt_GJ");
    TH1D* HistLeadingJetPt_QG = (TH1D*) f2->Get("HistLeadingJetPt_QG");
    TH1D* HistLeadingJetPt_QQ = (TH1D*) f2->Get("HistLeadingJetPt_QQ");
    TH1D* HistSubJetPt = (TH1D*) f2->Get("HistSubJetPt");
    TH1D* HistSubJetPt_GG = (TH1D*) f2->Get("HistSubJetPt_GG");
    TH1D* HistSubJetPt_GJ = (TH1D*) f2->Get("HistSubJetPt_GJ");
    TH1D* HistSubJetPt_QG = (TH1D*) f2->Get("HistSubJetPt_QG");
    TH1D* HistSubJetPt_QQ = (TH1D*) f2->Get("HistSubJetPt_QQ");
    TH1D* HistNTrk = (TH1D*) f2->Get("HistNTrk");
    TH1D* HistNTrk_G = (TH1D*) f2->Get("HistNTrk_G");
    TH1D* HistNTrk_Q = (TH1D*) f2->Get("HistNTrk_Q");
    
    
    auto mlegend = new TLegend(0.8, 0.28, 0.89, 0.75);
    HistMjj->SetAxisRange(400., 8200.,"X");
    HistMjj->SetLineColor(0);
    HistMjj->Draw();
    HistMjj_GG->SetLineColor(2);
    HistMjj_GG->Draw("same");
    mlegend->AddEntry(HistMjj_GG, "GG", "l");
    HistMjj_QG->SetLineColor(3);
    HistMjj_QG->Draw("same");
    mlegend->AddEntry(HistMjj_QG, "QG", "l");
    HistMjj_QQ->SetLineColor(7);
    HistMjj_QQ->Draw("same");
    mlegend->AddEntry(HistMjj_QQ, "QQ", "l");
    mlegend->Draw();
    c->SaveAs(".mjj.png");
    
    gPad->SetLogy(0);
    gStyle->SetOptStat(0);
    HistMjj_QG->Divide(HistMjj);
    HistMjj_QG->SetLineColor(3);
    HistMjj_QG->SetTitle("fraction");
    HistMjj_QG->SetAxisRange(400., 8200.,"X");
    HistMjj_QG->SetAxisRange(0, 1,"Y");
    HistMjj_QG->Draw();
    HistMjj_GG->Divide(HistMjj);
    HistMjj_GG->SetLineColor(2);
    HistMjj_GG->Draw("same");
    HistMjj_QQ->Divide(HistMjj);
    HistMjj_QQ->SetLineColor(7);
    HistMjj_QQ->Draw("same");
    mlegend->Draw();
    c->SaveAs(".fra.png");
    gPad->SetLogy();
    gStyle->SetOptStat();
    
    auto lptlegend = new TLegend(0.8, 0.28, 0.89, 0.75);
    HistLeadingJetPt->SetAxisRange(400., 4200.,"X");
    HistLeadingJetPt->SetLineColor(0);
    HistLeadingJetPt->Draw();
    HistLeadingJetPt_GG->SetLineColor(2);
    HistLeadingJetPt_GG->Draw("same");
    lptlegend->AddEntry(HistLeadingJetPt_GG, "GG", "l");
    HistLeadingJetPt_QG->SetLineColor(3);
    HistLeadingJetPt_QG->Draw("same");
    lptlegend->AddEntry(HistLeadingJetPt_QG, "QG", "l");
    HistLeadingJetPt_QQ->SetLineColor(7);
    HistLeadingJetPt_QQ->Draw("same");
    lptlegend->AddEntry(HistLeadingJetPt_QQ, "QQ", "l");
    lptlegend->Draw();
    c->SaveAs(".LPt.png");
    
    auto sptlegend = new TLegend(0.8, 0.28, 0.89, 0.75);
    HistSubJetPt->SetAxisRange(400., 4200.,"X");
    HistSubJetPt->SetLineColor(0);
    HistSubJetPt->Draw();
    HistSubJetPt_GG->SetLineColor(2);
    HistSubJetPt_GG->Draw("same");
    sptlegend->AddEntry(HistLeadingJetPt_GG, "GG", "l");
    HistSubJetPt_QG->SetLineColor(3);
    HistSubJetPt_QG->Draw("same");
    sptlegend->AddEntry(HistSubJetPt_QG, "QG", "l");
    HistSubJetPt_QQ->SetLineColor(7);
    HistSubJetPt_QQ->Draw("same");
    sptlegend->AddEntry(HistSubJetPt_QQ, "QQ", "l");
    sptlegend->Draw();
    c->SaveAs(".SPt.png");

}
