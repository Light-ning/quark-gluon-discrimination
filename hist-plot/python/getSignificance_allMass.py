from ROOT import *
import numpy as np
import math

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
		#print HistMjj_GJ_1.GetBinCenter(i), si, bi
		if bi != 0:
			sigma += 2*((si+bi)*np.log(1+(si/bi))-si)

	sigma_total = math.sqrt(sigma)	
	yields = HistMjj.Integral()
	yields_b = HistMjj_b.Integral()

	#outfile = open("../output/f_Sig_GJnificance_"+model+".txt","w+")

	line= str(m)+" & "+str(yields)+" & "+str(sigma_total)+" \\\ "#+str(yields_b)
	print line
	
	Sig.SetBinContent(Sig.FindBin(m),sigma_total)

	return Sig

model = "String"
Sig_JJ = TH1D("Sig_JJ","",4,6500,9000)
Sig_GJ = TH1D("Sig_GJ","",4,6500,9000)
Sig_GG = TH1D("Sig_GG","",4,6500,9000)
f_Sig_GJ = TH1D("f_Sig_GJ","",4,6500,9000)
f_Sig_GG = TH1D("f_Sig_GG","",4,6500,9000)
c = TCanvas("c","c",500,500)

def getYields(a,d,e,ba,bd,be,fmodel):
	f1 = TFile(a)
	f2 = TFile(d)
	f3 = TFile(e)
	fb1 = TFile(ba)
	fb2 = TFile(bd)
	fb3 = TFile(be)

	HistMjj_JJ_1 = f1.Get("HistMjj")
	HistMjj_JJ_2 = f2.Get("HistMjj")
	HistMjj_JJ_3 = f3.Get("HistMjj")
	HistMjj_JJ_b1 = fb1.Get("HistMjj")
	HistMjj_JJ_b2 = fb2.Get("HistMjj")
	HistMjj_JJ_b3 = fb3.Get("HistMjj")

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
	if fmodel == "String":
		if m == 7000:
			xs=7.09
		elif m == 7500:
			xs=1.86
		elif m == 8000:
			xs=4.66e-1
		elif m == 8500:
			xs=1e-1
		elif m == 9000:
			xs=1.99e-2
#for JJ
	HistMjj_JJ_1.Scale(xs*36.20766/cutflow_weight_1.GetBinContent(1))
	HistMjj_JJ_2.Scale(xs*44.3074/cutflow_weight_2.GetBinContent(1))
	HistMjj_JJ_3.Scale(xs*58.4501/cutflow_weight_3.GetBinContent(1))

	HistMjj_JJ_b1.Scale(36.20766)
	HistMjj_JJ_b2.Scale(44.3074)
	HistMjj_JJ_b3.Scale(58.4501)

	HistMjj_JJ_1.Add(HistMjj_JJ_2)
	HistMjj_JJ_1.Add(HistMjj_JJ_3)

	HistMjj_JJ_b1.Add(HistMjj_JJ_b2)
	HistMjj_JJ_b1.Add(HistMjj_JJ_b3)

#for GJ
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

#for GG
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

	return HistMjj_JJ_1, HistMjj_JJ_b1, HistMjj_GJ_1, HistMjj_GJ_b1 ,HistMjj_GG_1, HistMjj_GG_b1

for m in range(7000,9500,500):
	
	i_HistMjj_JJ_1, i_HistMjj_JJ_b1 ,i_HistMjj_GJ_1, i_HistMjj_GJ_b1 , i_HistMjj_GG_1, i_HistMjj_GG_b1 = getYields("../output/"+model+"_MC_"+str(m)+"_mc16a_dummy.root","../output/"+model+"_MC_"+str(m)+"_mc16d_dummy.root","../output/"+model+"_MC_"+str(m)+"_mc16e_dummy.root","../output/QCD_a.root","../output/QCD_d.root","../output/QCD_e.root",model)
	f_HistMjj_JJ_1,f_HistMjj_JJ_b1, f_HistMjj_GJ_1, f_HistMjj_GJ_b1 , f_HistMjj_GG_1, f_HistMjj_GG_b1 = getYields("../output/float/"+model+"_MC_"+str(m)+"_mc16a_dummy.root","../output/float/"+model+"_MC_"+str(m)+"_mc16d_dummy.root","../output/float/"+model+"_MC_"+str(m)+"_mc16e_dummy.root","../output/float/QCD_a.root","../output/float/QCD_d.root","../output/float/QCD_e.root",model)

# get the sigma of 	
	Sig_JJ = getSigma(i_HistMjj_JJ_1,i_HistMjj_JJ_b1, Sig_JJ) 
	Sig_GJ = getSigma(i_HistMjj_GJ_1,i_HistMjj_GJ_b1, Sig_GJ) 
	Sig_GG = getSigma(i_HistMjj_GG_1,i_HistMjj_GG_b1, Sig_GG) 
	f_Sig_GJ = getSigma(f_HistMjj_GJ_1,f_HistMjj_GJ_b1, f_Sig_GJ) 
	f_Sig_GG = getSigma(f_HistMjj_GG_1, f_HistMjj_GG_b1, f_Sig_GG) 


fout = TFile("../output/float/"+model+".root","recreate")


#plotting 
#gROOT.SetStyle("ATLAS")
gStyle.SetOptStat(0)
#gStyle.SetOptTitle(kFALSE)
Sig_GJ.SetTitle("")
Sig_GJ.SetLineColor(1)
Sig_GJ.GetXaxis().SetTitle("Mjj [GeV]")
Sig_GJ.GetYaxis().SetTitle("Significance")

Sig_GG.SetLineColor(2)
Sig_JJ.SetLineColor(21)
f_Sig_GJ.SetLineColor(8)
f_Sig_GG.SetLineColor(9)

leg = TLegend(0.63,0.6,0.84,0.75)
leg.SetTextFont(42)
leg.SetBorderSize(0)
leg.SetNColumns(1)
leg.AddEntry(Sig_JJ,"JJ","l")
leg.AddEntry(Sig_GJ,"GJ (int)","l")
leg.AddEntry(f_Sig_GJ,"GJ (float)","l")
leg.AddEntry(Sig_GG,"GG (int)","l")
leg.AddEntry(f_Sig_GG,"GG (float)","l")

myText(0.18,0.84,"#it{#bf{#scale[1.8]{#bf{ATLAS} Simulation Internal}}}")
myText(0.18,0.80,"#bf{#scale[1.5]{#sqrt{s} = 13 TeV}}")


Sig_GJ.Draw("PC")
Sig_GG.Draw("samePC")
f_Sig_GJ.Draw("samePC")
f_Sig_GG.Draw("samePC")
leg.Draw()

c.SaveAs("../output/float/"+model+".pdf")

c.Write()
#HistMjj_GJ_1.Write()
#f_Sig_GJ.Write()
fout.Write()
fout.Close()
#outfile.write(line) 

#outfile.close()
