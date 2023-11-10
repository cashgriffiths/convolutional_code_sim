#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

#define map(x)  ( (x) > 0 ? 1 : -1 )
#define demap(x) ( (x) > 0 ? 1 : 0 )
int i;

const int mu = 2; // memory of encoder
const float R = (float) 1/mu; // code rate
// generator polynomials for rate 1/2, mu = 2
const uint8_t g1 = 0b111;
const uint8_t g2 = 0b101;

void awgn();
void encoder();
void decoder();

// popcount utility for calculating xor
uint8_t popcount8( uint8_t b )
{
     b = b - ((b >> 1) & 0x55);
     b = (b & 0x33) + ((b >> 2) & 0x33);
     return (((b + (b >> 4)) & 0x0F) * 0x01);
}

uint8_t transition_codeword_pairs[1 << (mu + 2)];

// looks like magic but i swear its legit
// table stores codeword corresponding to state transitions
// array indexes look like (current state)+(next state), + respresenting append
// array entries are 3 bits; lower two are the codeword, 3rd bit is message bit
void tr_cw_pairs() {
    memset(transition_codeword_pairs, -1, 1 << (mu + 2));
    for (i = 0; i < 1 << mu; i++) {
        uint8_t state1 = i << 1, state2 = (i << 1) | 0b1;
        transition_codeword_pairs[(i << 2) | (state1 & 0b11)] = ((popcount8(state1 & g1) & 0b1) << 1) | (popcount8(state1 & g2) & 0b1);
        transition_codeword_pairs[(i << 2) | (state2 & 0b11)] = ((popcount8(state2 & g1) & 0b1) << 1) | (popcount8(state2 & g2) & 0b1) | 0b100;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: \n");
        exit(1);
    }
    float SNRdb = atof(argv[1]);
    int k = atoi(argv[2]);
    
    float EcNodB = SNRdb + 10*log10(R);  // general formula *if* there was coding
    float EcNo = pow(10.0, EcNodB/10.0);
    float sigsqrd = 0.5/EcNo; // var = No/2, Ec=1
    float sigma = sqrt(sigsqrd);

    long blk_errs = 0, blk_errs_max = 100, blk_cnt = 0, bit_errs;
    float r[(k+mu)*2];
    // float r[8] = {-.7, -.5, -.8, -.6, -1.1, .4, .9, .8};
    uint8_t m[k+mu], c[(k+mu)*2], y[k+mu+1];

    srand(time(NULL));
    tr_cw_pairs();

    while (blk_errs < blk_errs_max) {
        blk_cnt++;

        memset(y, 0, sizeof y);

        // get a block of data
        for(i=0; i<k; i++) m[i] = (rand()>>5) & 0b1;
        for(i=k; i<k+mu; i++) m[i] = 0;
        

        encoder(m, c, k+mu);

        awgn(sigma, c, r, sizeof c);
        
        
        

        // decode
        decoder(r, y, k+mu);
        
        /* debug nonsense */
        // for (i = 0; i < k; i++) printf("%d", m[i]);
        // printf("\n");
        // for (i = 0; i < 2* (k+mu); i++) printf("%d", c[i]);
        // printf("\n");
        // for (i = 0; i < 2* (k+mu); i++) printf("%f ", r[i]);
        // printf("\n");
        // for (i = 0; i < k; i++) printf("%d", y[i]);
        // printf("\n\n");

        // exit(0);

        // count bit errors and block errors
        int flag = 0;
        for(i=0; i<k+mu; i++)
            if(y[i] != m[i]) { bit_errs++; flag = 1; }
        if(flag) blk_errs++; 

    }
    printf("bit-error rate = %10.2e\nblock-error rate = %10.2e \n",
             (float)bit_errs/((k+mu)*blk_cnt), (float)blk_errs/blk_cnt);
}

// implements shift register encoder method
void encoder(m, c, k) 
uint8_t m[], c[];
int k;
{
    uint8_t state = 0;
    for (i = 0; i < k; i++) {
        uint8_t cw = transition_codeword_pairs[((state & 0b11) << 2) | (((state << 1) | m[i]) & 0b11)] & 3;
        c[2*i] = cw >> 1;
        c[2*i + 1] = cw & 0b1;
        state <<= 1;
        state |= m[i];
    }
}

/* this ones a doozy
 * viterbi traceback decoder for ZTCC
*/
void decoder(r, y, k) 
float r[];
uint8_t y[];
int k;
{
    float gamma[1 << mu][k+1]; // cunulative metric
    uint8_t h[1 << mu][k+1]; // state history

    // prepare arrays
    memset(h, 0, sizeof h);
    for (i = 0; i < 1 << mu; i++) gamma[i][0] = -1 * DBL_MAX;
    gamma[0][0] = 0;

    // prepare some variables
    float r1, r2;
    int j;
    uint8_t s1, s2, c1, c2;
    float test1, test2;

    // for each trellis stage
    for (i = 1; i < k+1; i++){
        // grab received codeword
        r1 = r[2*(i - 1)];
        r2 = r[2*(i - 1) +1];
        // for each possible encoder state
        for (j = 0; j < 1 << mu; j++) {
            // get S(s), i.e. possible previous states
            if (j & 0b10) s1 = 1, s2 = 3;
            else s1 = 0, s2 = 2;
            // codeword is lower 2 bits of table entry
            c1 = transition_codeword_pairs[(s1 << 2) | j] & 3;
            c2 = transition_codeword_pairs[(s2 << 2) | j] & 3;
            // calculate cumulative metric for each possible state
            test1 = gamma[s1][i-1] + r1 * map(c1 >> 1) + r2 * map(c1 & 0b1);
            test2 = gamma[s2][i-1] + r1 * map(c2 >> 1) + r2 * map(c2 & 0b1);
            // compare metrics and pick better one
            if (test1 > test2) {
                gamma[j][i] = test1;
                h[j][i] = s1;
            } else {
                gamma[j][i] = test2;
                h[j][i] = s2;
            }
        }
    }  

    /* debug nonsense */
    // for (int j = 0; j < k+1; j++) {
    //         printf("%10f ", gamma[0][j]);
    // } printf("\n");
    // for (int j = 0; j < k+1; j++) {
    //         printf("%10f ", gamma[1][j]);
    // } printf("\n");
    // for (int j = 0; j < k+1; j++) {
    //         printf("%10f ", gamma[2][j]);
    // } printf("\n");
    // for (int j = 0; j < k+1; j++) {
    //         printf("%10f ", gamma[3][j]);
    // } printf("\n\n");

    // for (int j = 0; j < k+1; j++) {
    //         printf("%10d ", h[0][j]);
    // } printf("\n");
    // for (int j = 0; j < k+1; j++) {
    //         printf("%10d ", h[1][j]);
    // } printf("\n");
    // for (int j = 0; j < k+1; j++) {
    //         printf("%10d ", h[2][j]);
    // } printf("\n");
    // for (int j = 0; j < k+1; j++) {
    //         printf("%10d ", h[3][j]);
    // } printf("\n");

    // slightly messy, fills result array with state transitions
    uint8_t c;
    for (int l = k; l > 0; l--) {
        c = h[y[l]][l];
        y[l-1] = c;
    }
    // converts state transitions to original message bit
    // separate loops for clarity, can be consolidated for performance
    for (int l = 0; l < k; l++) {
        y[l] = transition_codeword_pairs[y[l] << 2 | y[l+1]] >> 2;
    }
}

// marsaglia polar method
void awgn (sigma, x, y, len)
float sigma, y[];
uint8_t x[];
int len;
{
	int k;
	double u, v, s, a, b;
	
	for(k = 0; k < len; k+=2)
	{
            do
            {
		        u = (rand()/((double)RAND_MAX)) * 2.0 - 1.0;
		        v = (rand()/((double)RAND_MAX)) * 2.0 - 1.0;
                s = u*u + v*v;
            } while(s >= 1.0 || s == 0.0);
            s = sqrt(-2.0*log(s)/s);
	    y[k] = map(x[k]) + sigma*u*s;
	    y[k+1] = map(x[k+1]) + sigma*v*s;
	}
}

