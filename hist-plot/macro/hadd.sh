for i in {1000..7000..500}; do
	rm ../output/Hprime_MC_${i}.root
	hadd ../output/Hprime_MC_${i}.root ../output/Hprime_MC_${i}_mc16a_dummy.root ../output/Hprime_MC_${i}_mc16d_dummy.root ../output/Hprime_MC_${i}_mc16e_dummy.root
done
