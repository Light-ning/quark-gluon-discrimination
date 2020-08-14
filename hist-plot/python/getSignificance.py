from ROOT import *
import numpy as np
import math
from optparse import OptionParser
parser=OptionParser()

parser.add_option("--m",dest="m",help="mass",default="7000")
parser.add_option("--model",dest="model",help="model name",default="String")

(options,args)=parser.parse_args()
m = options.m
model = options.model


def myText(x,y,text, color = 1):
     l = TLatex()
     l.SetTextSize(0.025)
     l.SetNDC()
     l.SetTextColor(color)
     l.DrawLatex(x,y,text)
     pass


f1 = TFile("../output/"+model+"_MC_"+m+"_mc16a_dummy.root")
f2 = TFile("../output/"+model+"_MC_"+m+"_mc16d_dummy.root")
f3 = TFile("../output/"+model+"_MC_"+m+"_mc16e_dummy.root")
fb1 = TFile("../output/QCD_a.root")
fb2 = TFile("../output/QCD_d.root")
fb3 = TFile("../output/QCD_e.root")

HistMjj_1 = f1.Get("HistMjj")
HistMjj_2 = f2.Get("HistMjj")
HistMjj_3 = f3.Get("HistMjj")
HistMjj_b1 = fb1.Get("HistMjj")
HistMjj_b2 = fb2.Get("HistMjj")
HistMjj_b3 = fb3.Get("HistMjj")

xs = 0.
if model == "String":
	if m == "7000":
		xs=7.09
	elif m == "7500":
		xs=1.86
	elif m == "8000":
		xs=4.66e-1
	elif m == "8500":
		xs=1e-1
	elif m == "9000":
		xs=1.99e-2

HistMjj_1.Scale(xs*36.20766/HistMjj_1.Integral())
HistMjj_2.Scale(xs*44.3074/HistMjj_2.Integral())
HistMjj_3.Scale(xs*58.4501/HistMjj_3.Integral())
HistMjj_b1.Scale(36.20766)
HistMjj_b2.Scale(44.3074)
HistMjj_b3.Scale(58.4501)

HistMjj_1.Add(HistMjj_2)
HistMjj_1.Add(HistMjj_3)

HistMjj_b1.Add(HistMjj_b2)
HistMjj_b1.Add(HistMjj_b3)

sigma = 0.
sigma_total = 0.
yields = 0.
for i in range(1,HistMjj_1.GetNbinsX()+1):
	si = HistMjj_1.GetBinContent(i)
	bi = HistMjj_b1.GetBinContent(i)
	print i, si, bi
	if bi != 0:
		sigma += 2*((si+bi)*np.log(1+(si/bi))-si)

sigma_total = math.sqrt(sigma)	
yields = HistMjj_1.Integral()

#c = TCanvas("c","c",500,500)
#gStyle.SetOptStat(0)
outfile = open("../output/Significance_"+model+".txt","w+")

line="mass : "+m+" & yield : "+str(yields)+" &   sigma : "+str(sigma_total)
print line
#c.Print("../output/Significance.txt")
outfile.write(line) 

outfile.close()
