for i in $(seq 12); do
    ./makingHist MC MC16a_JZ${i}W outTree &
done
for j in $(seq 12); do
    ./makingHist MC MC16d_JZ${j}W outTree &
done
for k in $(seq 12); do
    ./makingHist MC MC16e_JZ${k}W outTree &
done
