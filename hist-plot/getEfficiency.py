from ROOT import *

def myText(x,y,text, color = 1):
     l = TLatex()
     l.SetTextSize(0.025)
     l.SetNDC()
     l.SetTextColor(color)
     l.DrawLatex(x,y,text)
     pass


f1 = TFile("mjj.root")

HistLeadingJetPt_TruthG = f1.Get("HistLeadingJetPt_TruthG")
HistLeadingJetPt_TruthQ = f1.Get("HistLeadingJetPt_TruthQ")
HistLeadingJetPt_TruthG_TaggedG = f1.Get("HistLeadingJetPt_TruthG_TaggedG")
HistLeadingJetPt_TruthQ_TaggedG = f1.Get("HistLeadingJetPt_TruthQ_TaggedG")


c = TCanvas("c","c",500,500)

HistLeadingJetPt_TruthG_TaggedG_eff = HistLeadingJetPt_TruthG_TaggedG.Clone("")
HistLeadingJetPt_TruthG_TaggedG_eff.Divide(HistLeadingJetPt_TruthG)
HistLeadingJetPt_TruthG_TaggedG_eff.GetYaxis().SetTitle("Gluon Tag eff")

gStyle.SetOptStat(0)

HistLeadingJetPt_TruthG_TaggedG_eff.Draw()

c.Print("./output/Leading_eff.pdf")

x = input()
