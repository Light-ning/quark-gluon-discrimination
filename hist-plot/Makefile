# Default target
default: makingHist drawk draw makingHist_signal

# Rule for makingHist
makingHist_signal: makingHist_signal.C 
	g++ makingHist_signal.C `root-config --cflags --glibs` -o makingHist_signal


makingHist: makingHist.C 
	g++ makingHist.C `root-config --cflags --glibs` -o makingHist

# Rule for kinemarics draw
drawk: drawk.C
	g++ drawk.C `root-config --cflags --glibs` -o drawk

# Rule for main draw
draw: draw.C
	g++ draw.C `root-config --cflags --glibs` -o draw

clean:
	rm -rf makingHist drawk draw

cleanroot:
	rm *.root log.* timelog.*
