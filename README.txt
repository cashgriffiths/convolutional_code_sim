Executable usage: ./a.out <snr-db> <message_len> 
Compile executable: gcc -O3 convo_sim.c -Wno-deprecated-non-prototype

Script usage: ./sim.sh <snr-db-start> <snr-db-end> <message_len>
Script will run <snr-db-end> - <snr-db-start> (both integers) iterations, one for each SNR value (integers)
and print output to console. If <snr-db-start> is less than <snr-db-end>, the script will execute in decrement mode.
