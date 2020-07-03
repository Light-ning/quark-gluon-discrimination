for i in $(ls /eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningData/$1); do
    root -b -q "makingHist.C(\"$1\", \"$i\", \"outTree\")" &
done
