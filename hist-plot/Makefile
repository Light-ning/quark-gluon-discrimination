# Default target
default: makingHist 
#default: makingHist drawk draw 

# Rule for makingHist
makingHist: src/makingHist.C 
	g++ src/makingHist.C `root-config --cflags --glibs` -o run/makingHist

# Rule for kinemarics draw
#drawk: src/drawk.C
#	g++ src/drawk.C `root-config --cflags --glibs` -o run/drawk
#
## Rule for main draw
#draw: src/draw.C
#	g++ src/draw.C `root-config --cflags --glibs` -o run/draw

clean:
	rm -rf makingHist drawk draw

cleanroot:
	rm *.root log.* timelog.*
