Executable usage: ./convo_sim <snr-db> <message_len> <g1> <g2>
Run make to compile, make clean to remove. 
The simulator will determine memory size from provided generator polynomials <g1> and <g2>. The generator polynomials are interpreted in octal format. SNR is provided as a float in dB

Script usage: ./sim.sh <snr-db-start> <snr-db-end> <message_len> <g1> <g2>
Script will run <snr-db-end> - <snr-db-start> (both integers) iterations, one for each SNR value (integers)
and print output to console. If <snr-db-start> is less than <snr-db-end>, the script will execute in decrement mode.
