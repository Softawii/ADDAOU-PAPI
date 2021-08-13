for counter in {2048..128000..128}; 
do 
./test.bin $counter mergesort;
./test.bin $counter quicksort; 
done