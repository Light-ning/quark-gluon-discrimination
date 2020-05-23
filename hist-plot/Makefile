SHELL=/usr/bin/bash
target=$(shell python3 find.py)
path=/eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningMC/QCD_MC16a/
outDic=outDirectory/

.PHONY: all makingDic clean

all: $(target)

makingDic:
	@mkdir $(outDic)
	@mkdir $(dir $(target))

$(outDic)%.root: $(path)%.root
	@root -b -q "makingPlot(\"$<\", \"outTree\", \"$@\")"

clean:
	@rm -rf $(outDic)
