for i in $(ls /eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningData/$1); do
    ./makingHist $1 $i outTree &
done
