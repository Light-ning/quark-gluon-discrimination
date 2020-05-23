SHELL=/usr/bin/bash
target=$(shell python3 find.py)
path=/eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningMC/QCD_MC16a/

.PHONY: all makingDic clean

all: $(target)

makingDic:
	@mkdir outDirectory
	@echo $(dir $(target))
	@mkdir $(dir $(target))

outDirectory/%.root: $(path)%.root
	@root -b -q "makingPlot(\"$<\", \"outTree\", \"$@\")"
