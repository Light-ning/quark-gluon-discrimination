for i in {1000..7000..500}; do
	nohup ./makingHist_signal MC $i a &
done
for i in {1000..7000..500}; do
	nohup ./makingHist_signal MC $i d &
done
for i in {1000..7000..500}; do
	nohup ./makingHist_signal MC $i e &
done
