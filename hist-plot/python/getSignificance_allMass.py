from ROOT import *
import numpy as np
import math
from optparse import OptionParser
parser=OptionParser()

parser.add_option("--model",dest="model",help="",default="String")
(options,args)=parser.parse_args()
model=options.model


def myText(x,y,text, color = 1):
     l = TLatex()
     l.SetTextSize(0.025)
     l.SetNDC()
     l.SetTextColor(color)
     l.DrawLatex(x,y,text)
     pass

def getSigma(fHistMjj, fHistMjj_b, Sig):
	sigma = 0.
	sigma_total = 0.
	yields = 0.
	yields_b = 0.
	for j in range(1,fHistMjj.GetNbinsX()+1):
		si = fHistMjj.GetBinContent(j)
		bi = fHistMjj_b.GetBinContent(j)
#		print fHistMjj.GetBinCenter(j), si, bi
		if bi != 0.:
		#if (si+bi) >= 1. and bi != 0.:
			sigma = 2*((si+bi)*np.log(1+(si/bi))-si)
			#sigma += 2*((si+bi)*np.log(1+(si/bi))-si)
			sigma_total = math.sqrt(abs(sigma))
			Sig.SetBinContent(j,sigma_total)
			print fHistMjj.GetBinCenter(j), " ", si," ", bi, " ", sigma_total

#	sigma_total = math.sqrt(sigma)	
	yields = fHistMjj.Integral()
	yields_b = fHistMjj_b.Integral()

	#outfile = open("../output/f_Sig_GJnificance_"+model+".txt","w+")

	line= str(m)+" & "+str(yields)+" & "+str(sigma_total)+" \\\ "#+str(yields_b)
	#print line
	#Sig.SetBinContent(Sig.FindBin(m),sigma_total)

	return Sig

if model == "String":
	mRange = range(7000,9500,500)
else:
	mRange = [1000]+range(2000,7500,500)
	
nBin = len(mRange)
Bmin = mRange[0]
Bmax = mRange[-1]

#Sig_JJ = TH1D("Sig_JJ","",nBin,Bmin,9000)
#Sig_GJ = TH1D("Sig_GJ","",nBin,Bmin,9000)
#Sig_GG = TH1D("Sig_GG","",nBin,Bmin,9000)
c = TCanvas("c","c",500,500)
fout = TFile("../output/"+model+".root","recreate")


for m in mRange:
	
	print m
	f1 = TFile("../output/"+model+"_MC_"+str(m)+"_mc16a_dummy.root")
	f2 = TFile("../output/"+model+"_MC_"+str(m)+"_mc16d_dummy.root")
	f3 = TFile("../output/"+model+"_MC_"+str(m)+"_mc16e_dummy.root")
	fb1 = TFile("../output/QCD_a.root")
	fb2 = TFile("../output/QCD_d.root")
	fb3 = TFile("../output/QCD_e.root")

	HistMjj_1 = f1.Get("HistMjj")
	HistMjj_2 = f2.Get("HistMjj")
	HistMjj_3 = f3.Get("HistMjj")
	HistMjj_b1 = fb1.Get("HistMjj")
	HistMjj_b2 = fb2.Get("HistMjj")
	HistMjj_b3 = fb3.Get("HistMjj")

	HistMjj_GJ_1 = f1.Get("HistMjj_GJ")
	HistMjj_GJ_2 = f2.Get("HistMjj_GJ")
	HistMjj_GJ_3 = f3.Get("HistMjj_GJ")
	HistMjj_GJ_b1 = fb1.Get("HistMjj_GJ")
	HistMjj_GJ_b2 = fb2.Get("HistMjj_GJ")
	HistMjj_GJ_b3 = fb3.Get("HistMjj_GJ")

	HistMjj_GG_1 = f1.Get("HistMjj_GG")
	HistMjj_GG_2 = f2.Get("HistMjj_GG")
	HistMjj_GG_3 = f3.Get("HistMjj_GG")
	HistMjj_GG_b1 = fb1.Get("HistMjj_GG")
	HistMjj_GG_b2 = fb2.Get("HistMjj_GG")
	HistMjj_GG_b3 = fb3.Get("HistMjj_GG")

	cutflow_weight_1 = f1.Get("cutflow_weight")
	cutflow_weight_2 = f2.Get("cutflow_weight")
	cutflow_weight_3 = f3.Get("cutflow_weight")

	xs = 0.
	if model == "String":
		if m == 7000:
			xs=7.09   # fb-1
		elif m == 7500:
			xs=1.86
		elif m == 8000:
			xs=4.66e-1
		elif m == 8500:
			xs=1e-1
		elif m == 9000:
			xs=1.99e-2
	elif model == "Hprime":
		if m == 1000:
			xs=2.6812e-1
		elif m == 2000:
			xs=2.8351e-5
		elif m == 2500:
			xs=1.0958e-6
		elif m == 3000:
			xs=6.53e-8
		elif m == 3500:
			xs=5.16e-9
		elif m == 4000:
			xs=4.9157e-10
		elif m == 4500:
			xs=5.3629e-11
		elif m == 5000:
			xs=6.3479e-12
		elif m == 5500:
			xs=7.999e-13
		elif m == 6000:
			xs=1.0483e-13
		elif m == 6500:
			xs=1.39e-14
		elif m == 7000:
			xs=1.855e-15

	HistMjj_1.Scale(xs*36.20766/cutflow_weight_1.GetBinContent(1))
	HistMjj_2.Scale(xs*44.3074/cutflow_weight_2.GetBinContent(1))
	HistMjj_3.Scale(xs*58.4501/cutflow_weight_3.GetBinContent(1))

	HistMjj_b1.Scale(36.20766)
	HistMjj_b2.Scale(44.3074)
	HistMjj_b3.Scale(58.4501)

	HistMjj_1.Add(HistMjj_2)
	HistMjj_1.Add(HistMjj_3)

	HistMjj_b1.Add(HistMjj_b2)
	HistMjj_b1.Add(HistMjj_b3)

	HistMjj_GJ_1.Scale(xs*36.20766/cutflow_weight_1.GetBinContent(1))
	HistMjj_GJ_2.Scale(xs*44.3074/cutflow_weight_2.GetBinContent(1))
	HistMjj_GJ_3.Scale(xs*58.4501/cutflow_weight_3.GetBinContent(1))

	HistMjj_GJ_b1.Scale(36.20766)
	HistMjj_GJ_b2.Scale(44.3074)
	HistMjj_GJ_b3.Scale(58.4501)

	HistMjj_GJ_1.Add(HistMjj_GJ_2)
	HistMjj_GJ_1.Add(HistMjj_GJ_3)

	HistMjj_GJ_b1.Add(HistMjj_GJ_b2)
	HistMjj_GJ_b1.Add(HistMjj_GJ_b3)

	HistMjj_GG_1.Scale(xs*36.20766/cutflow_weight_1.GetBinContent(1))
	HistMjj_GG_2.Scale(xs*44.3074/cutflow_weight_2.GetBinContent(1))
	HistMjj_GG_3.Scale(xs*58.4501/cutflow_weight_3.GetBinContent(1))

	HistMjj_GG_b1.Scale(36.20766)
	HistMjj_GG_b2.Scale(44.3074)
	HistMjj_GG_b3.Scale(58.4501)

	HistMjj_GG_1.Add(HistMjj_GG_2)
	HistMjj_GG_1.Add(HistMjj_GG_3)

	HistMjj_GG_b1.Add(HistMjj_GG_b2)
	HistMjj_GG_b1.Add(HistMjj_GG_b3)

# get the sigma of 	
	Sig_JJ = HistMjj_1.Clone() 
	Sig_JJ = getSigma(HistMjj_1,HistMjj_b1, Sig_JJ) 
	#Sig_GJ = getSigma(HistMjj_GJ_1,HistMjj_GJ_b1, Sig_GJ) 
	#Sig_GG = getSigma(HistMjj_GG_1,HistMjj_GG_b1, Sig_GG) 

	#plotting 
	#gROOT.SetStyle("ATLAS")
	gStyle.SetOptStat(0)
	gPad.DrawFrame(Bmin,10e-3,Bmax,25)
	#gStyle.SetOptTitle(kFALSE)
	Sig_JJ.SetTitle("")
	Sig_JJ.SetLineColor(1)
	Sig_JJ.GetXaxis().SetTitle("Mjj [GeV]")
	Sig_JJ.GetYaxis().SetTitle("Significance")

	Sig_GG.SetLineColor(2)
	Sig_GJ.SetLineColor(21)

	leg = TLegend(0.63,0.6,0.83,0.75)
	leg.SetTextFont(42)
	leg.SetBorderSize(0)
	leg.SetNColumns(1)
	leg.AddEntry(Sig_JJ,"JJ","l")
	#leg.AddEntry(Sig_GJ,"GJ","l")
	#leg.AddEntry(Sig_GG,"GG","l")

	Sig_JJ.Draw("PC")
	#Sig_GJ.Draw("samePC")
	#Sig_GG.Draw("samePC")
	leg.Draw()

	myText(0.18,0.85,"#it{#bf{#scale[1.8]{#bf{ATLAS} Simulation Internal}}}")
	myText(0.18,0.80,"#bf{#scale[1.5]{#sqrt{s} = 13 TeV}}")
	c.SaveAs("../output/"+model+"_"+str(m)+".pdf")

	#c.Write()
	#HistMjj_GJ_1.Write()
	#f_Sig_GJ.Write()
	fout.Write()
fout.Close()
#outfile.write(line) 

#outfile.close()
