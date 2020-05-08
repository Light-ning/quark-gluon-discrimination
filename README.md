# quark-gluon-discrimination

- ExcitedQuarks.cc generates signal events (excited quarks), and uses FastJet (Anti-Kt algorithm) to cluster jets.

- QCDProcess.cc generates background events (QCD dijet process), and uses FastJet (Anti-Kt algorithm) to cluster jets, and uses QCD-aware clustering method (or so called ghost association method) to truth tag jets. Try using `mymain02 QCDProcessX.cmnd QCDProcessX.root` to run the executable and get result histograms.

Copy the codes to your `pythia8244/examples` to make the executable.

**Note: The Makefile should be modified properly before running mymain0X.cc file**