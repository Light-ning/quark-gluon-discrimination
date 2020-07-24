for i in $(seq 12); do
    ../run/makingHist QCD_MC16a a dummy ${i} &
done
for i in $(seq 12); do
    ../run/makingHist QCD_MC16d d dummy ${i} &
done
for i in $(seq 12); do
    ../run/makingHist QCD_MC16e e dummy ${i} &
done
