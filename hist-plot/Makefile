# Default target
default: makingHist drawk

# Rule for makingHist
makingHist: makingHist.C 
	g++ makingHist.C `root-config --cflags --glibs` -o makingHist

# Rule for kinemarics draw
drawk: drawk.C
	g++ drawk.C `root-config --cflags --glibs` -o drawk

clean:
	rm -rf makingHist drawk


