echo "[WARNING] papi needs to get permission to read system events"
sudo sh -c 'echo 0 >/proc/sys/kernel/perf_event_paranoid'

for counter in {2048..128000..128}; 
do 
./test.bin $counter mergesort;
./test.bin $counter quicksort; 
done
