for i in {1000..7000..500}; do
	rm Hprime_${i}.root
	hadd Hprime_${i}.root Hprime_${i}_a.root Hprime_${i}_d.root Hprime_${i}_e.root
done
