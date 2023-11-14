Executable usage: ./a.out <snr-db> <message_len> <g1> <g2>
Compile executable: gcc -O3 convo_sim.c -Wno-deprecated-non-prototype
The simulator will determine memory size from provided generator polynomials <g1> and <g2>. SNR is provided as a float in dB

Script usage: ./sim.sh <snr-db-start> <snr-db-end> <message_len> <g1> <g2>
Script will run <snr-db-end> - <snr-db-start> (both integers) iterations, one for each SNR value (integers)
and print output to console. If <snr-db-start> is less than <snr-db-end>, the script will execute in decrement mode.
