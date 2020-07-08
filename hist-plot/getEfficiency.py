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

gStyle.SetOptStat(0)
c.Divide(2,1)

top = c.cd(1)
top.SetPad(0.0,0.0,1.0,1.0) 
top.SetFillColor(0)
top.SetBorderMode(0)
top.SetBorderSize(2)
top.SetTickx(1)
top.SetTicky(1)
top.SetLeftMargin(0.14)
top.SetRightMargin(0.055)
top.SetBottomMargin(0.3)#0.25
top.SetFrameBorderMode(0)
#top.SetLogy(1)

bot = c.cd(2)
bot.SetPad(0.0,0.0,1.0,0.3) 
bot.SetFillColor(0)
bot.SetBorderMode(0)
bot.SetBorderSize(2)
bot.SetTickx(1)
bot.SetTicky(1)
bot.SetLeftMargin(0.14)
bot.SetRightMargin(0.055)
bot.SetTopMargin(0.1)
bot.SetBottomMargin(0.4)
bot.SetFrameBorderMode(0)
gPad.SetLeftMargin(0.15)

HistLeadingJetPt_TruthG_TaggedG_eff.SetMarkerColor(1)
HistLeadingJetPt_TruthG_TaggedG_eff.SetLineColor(1)
HistLeadingJetPt_TruthG_TaggedG_eff.SetMarkerSize(0.2) 
HistLeadingJetPt_TruthG_TaggedG_eff.SetMarkerStyle(20)

HistLeadingJetPt_TruthG_TaggedG.SetTitle("")
HistLeadingJetPt_TruthG_TaggedG.GetYaxis().SetTitle("Entries")
HistLeadingJetPt_TruthQ_TaggedG.SetMarkerColor(46)
HistLeadingJetPt_TruthQ_TaggedG.SetLineColor(46)
HistLeadingJetPt_TruthQ_TaggedG.SetMarkerSize(0.5)
HistLeadingJetPt_TruthQ_TaggedG.SetMarkerStyle(21)
HistLeadingJetPt_TruthQ_TaggedG.GetYaxis().SetRangeUser(-0.007,HistLeadingJetPt_TruthG.GetMaximum()*1.5)
HistLeadingJetPt_TruthQ_TaggedG.GetXaxis().SetRangeUser(0,8000)

HistLeadingJetPt_TruthG.SetMarkerColor(9)
HistLeadingJetPt_TruthG.SetLineColor(9)
HistLeadingJetPt_TruthG.SetMarkerSize(0.5)
HistLeadingJetPt_TruthG.SetMarkerStyle(22)

HistLeadingJetPt_TruthG_TaggedG_eff.GetXaxis().SetTitle("1st Jet pT [GeV]")
HistLeadingJetPt_TruthG_TaggedG_eff.GetYaxis().SetTitle("Gluon Tagging eff")
HistLeadingJetPt_TruthG_TaggedG_eff.GetYaxis().SetNdivisions(505)
HistLeadingJetPt_TruthG_TaggedG_eff.GetYaxis().SetRangeUser(0.5,1.5)

top.cd()
HistLeadingJetPt_TruthQ_TaggedG.Draw()
HistLeadingJetPt_TruthG.Draw("same")

leg = TLegend(0.6,0.5,0.9,0.7);
leg.SetTextFont(42);
leg.SetFillStyle(0);
leg.SetFillColor(0);
leg.SetBorderSize(0);
leg.SetFillStyle(0)
leg.SetNColumns(1)
leg.AddEntry(HistLeadingJetPt_TruthG,"Truth Gluon","l")
leg.AddEntry(HistLeadingJetPt_TruthQ_TaggedG,"Truth Gluon tagged Gluon","l")
leg.Draw();

myText(0.58,0.76,"#it{#bf{#scale[1.3]{#bf{ATLAS} Simulation Internal}}}")
myText(0.58,0.7,"#bf{#scale[1.]{#sqrt{s} = 13 TeV}}")
#myText(0.18,0.78,"#bf{#scale[1.5]{pT range: 750 - 1000 GeV}}")


line = TLine(0,1,8000,1)

bot.cd()
HistLeadingJetPt_TruthG_TaggedG_eff.Draw()
line.Draw("same")

c.Print("./output/Leading_eff.pdf")

x = input()
