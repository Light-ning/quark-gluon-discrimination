from ROOT import *

def myText(x,y,text, color = 1):
     l = TLatex()
     l.SetTextSize(0.025)
     l.SetNDC()
     l.SetTextColor(color)
     l.DrawLatex(x,y,text)
     pass


f1 = TFile("Hprime_4000.root")

HistMjj = f1.Get("HistMjj")
HistMjj_GG = f1.Get("HistMjj_GG")
HistMjj_QQ = f1.Get("HistMjj_QQ")
HistMjj_QG = f1.Get("HistMjj_QG")


c = TCanvas("c","c",500,500)


gStyle.SetOptStat(0)

HistMjj.SetTitle("Ntrk Fraction")
HistMjj.GetYaxis().SetTitle("Entries")
HistMjj.GetXaxis().SetTitle("m_{jj} GeV")
HistMjj.GetYaxis().SetRangeUser(-0.007,HistMjj_QG.GetMaximum()*1.5)
HistMjj.GetXaxis().SetRangeUser(0,6000)
HistMjj_GG.SetMarkerColor(2)
HistMjj_GG.SetLineColor(2)
HistMjj_GG.SetMarkerSize(0.5)
HistMjj_GG.SetMarkerStyle(21)

HistMjj_QG.SetMarkerColor(8)
HistMjj_QG.SetLineColor(8)
HistMjj_QG.SetMarkerSize(0.5)
HistMjj_QG.SetMarkerStyle(22)

HistMjj_QQ.SetMarkerColor(4)
HistMjj_QQ.SetLineColor(4)
HistMjj_QQ.SetMarkerSize(0.5)
HistMjj_QQ.SetMarkerStyle(22)

HistMjj.Draw()
HistMjj_GG.Draw("same")
HistMjj_QQ.Draw("same")
HistMjj_QG.Draw("same")

leg = TLegend(0.6,0.5,0.9,0.7);
leg.SetTextFont(42);
leg.SetFillStyle(0);
leg.SetFillColor(0);
leg.SetBorderSize(0);
leg.SetFillStyle(0)
leg.SetNColumns(1)
leg.AddEntry(HistMjj,"Total","l")
leg.AddEntry(HistMjj_QQ,"QQ Fraction","l")
leg.AddEntry(HistMjj_QG,"QG Fraction","l")
leg.AddEntry(HistMjj_GG,"GG Fraction","l")
leg.Draw();

myText(0.58,0.76,"#it{#bf{#scale[1.3]{#bf{ATLAS} Simulation Internal}}}")
myText(0.58,0.7,"#bf{#scale[1.]{#sqrt{s} = 13 TeV}}")
#myText(0.18,0.78,"#bf{#scale[1.5]{pT range: 750 - 1000 GeV}}")

c.Print("./output/fraction_H4000.pdf")

x = input()
