for i in {7000..9000..500}; do
   ../run/makingHist String_MC a ${i} dummy &
done
for i in {7000..9000..500}; do
   ../run/makingHist String_MC d ${i} dummy &
done

#for i in {1000..7000..500}; do
    #../run/makingHist Hprime_MC a ${i} dummy &
#done
#for i in {1000..7000..500}; do
#    ../run/makingHist Hprime_MC d ${i} dummy &
#done
#for i in {1000..7000..500}; do
#    ../run/makingHist Hprime_MC e ${i} dummy &
#done
