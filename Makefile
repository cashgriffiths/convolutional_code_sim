
convo_sim: convo_sim.c
	gcc -O3 convo_sim.c -Wno-deprecated-non-prototype -o convo_sim

clean:
	rm -f convo_sim