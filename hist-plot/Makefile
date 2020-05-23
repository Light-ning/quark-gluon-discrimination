SHELL=/usr/bin/bash
target=$(shell python3 find.py)
path=/eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningMC/QCD_MC16a/
outDic=outDirectory/

.PHONY: all makingDic clean

all: $(target)

makingDic:
	@mkdir -p $(outDic)
	@mkdir -p $(dir $(target))

$(outDic)%.root: $(path)%.root
	@root -b -q "makingHist.C(\"$<\", \"outTree\", \"$@\")"

clean:
	@rm -rf $(outDic)
