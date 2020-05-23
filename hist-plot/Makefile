SHELL=/usr/bin/bash
target=$(shell python3 find.py)
path=/eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningMC/QCD_MC16a/

.PHONY: all clean

all: $(target)

outDirectory/%.root: $(path)%.root
	@mkdir outDirectory
	@mkdir $(shell echo ${$@%/*.root})
	@root -b -q "makingPlot(\"$<\", \"outTree\", \"$@\")"

