# Default target
default: makingHist drawk draw

# Rule for makingHist
makingHist: makingHist.cxx
	g++ makingHist.cxx `root-config --cflags --glibs` -o makingHist

# Rule for kinemarics draw
drawk: drawk.cxx
	g++ drawk.cxx `root-config --cflags --glibs` -o drawk

# Rule for main draw
draw: draw.cxx
	g++ draw.cxx `root-config --cflags --glibs` -o draw

clean:
	rm -rf makingHist drawk draw

cleanroot:
	rm *.root log.* timelog.*
