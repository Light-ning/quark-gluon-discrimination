dataset=$1

hadd -f mjj.root *${dataset}*.root
rm outfile
if [[ ${dataset} == *Data* ]]; then
    ./draw data
elif [[ ${dataset} == MC ]]; then
    ./draw MC
elif [[ ${dataset} == *MC* ]]; then
    for i in $(find *${dataset}*.root); do
        echo $i >> outfile
    done
    ./draw MC
    ./drawpt
fi
