for i in {7000..9000..500}; do
   python ../python/getSignificance.py --model String --m ${i}
done

#for i in {1000..7000..500}; do
#    python ../python/getSignificance.py --model Hprime --m ${i}  &
#done
