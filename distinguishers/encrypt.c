#include "api.h"
#include "permutations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RATE (64 / 8)
#define PA_ROUNDS 12
#define PB_ROUNDS 6
#define IV                                                        \
  ((u64)(8 * (CRYPTO_KEYBYTES)) << 56 | (u64)(8 * (RATE)) << 48 | \
   (u64)(PA_ROUNDS) << 40 | (u64)(PB_ROUNDS) << 32)

int crypto_aead_encrypt(unsigned char* c
                        , unsigned long long* clen,
                        const unsigned char* m, unsigned long long mlen,
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* nsec, const unsigned char* npub,
                        const unsigned char* k) {
  const u64 K0 = BYTES_TO_U64(k, 8);
  const u64 K1 = BYTES_TO_U64(k + 8, 8);
  const u64 N0 = BYTES_TO_U64(npub, 8);
  const u64 N1 = BYTES_TO_U64(npub + 8, 8);
  state s;
  (void)nsec;

  // set ciphertext size
  *clen = mlen + CRYPTO_ABYTES;

  // initialization
  s.x0 = IV;
  s.x1 = K0;
  s.x2 = K1;
  s.x3 = N0;
  s.x4 = N1;
  //printstate("initial value:", s);
  P12(&s);
  //s.x3 ^= K0;
  //s.x4 ^= K1;
  //printstate("initialization:", s);
  U64_TO_BYTES(c, s.x0, 8);						// Only first 64 output bits
  U64_TO_BYTES(c + 8, s.x1, 8);
  U64_TO_BYTES(c + 16, s.x2, 8);
  U64_TO_BYTES(c + 24, s.x3, 8);
  U64_TO_BYTES(c + 32, s.x4, 8);

  /*
  // process associated data
  if (adlen) {
    while (adlen >= RATE) {
      s.x0 ^= BYTES_TO_U64(ad, 8);
      P6(&s);
      adlen -= RATE;
      ad += RATE;
    }
    s.x0 ^= BYTES_TO_U64(ad, adlen);
    s.x0 ^= 0x80ull << (56 - 8 * adlen);
    P6(&s);
  }
  s.x4 ^= 1;
  printstate("process associated data:", s);

  // process plaintext
  while (mlen >= RATE) {
    s.x0 ^= BYTES_TO_U64(m, 8);
    U64_TO_BYTES(c, s.x0, 8);
    P6(&s);
    mlen -= RATE;
    m += RATE;
    c += RATE;
  }
  s.x0 ^= BYTES_TO_U64(m, mlen);
  s.x0 ^= 0x80ull << (56 - 8 * mlen);
  U64_TO_BYTES(c, s.x0, mlen);
  c += mlen;
  printstate("process plaintext:", s);

  // finalization
  s.x1 ^= K0;
  s.x2 ^= K1;
  P12(&s);
  s.x3 ^= K0;
  s.x4 ^= K1;
  printstate("finalization:", s);

  // set tag
  U64_TO_BYTES(c, s.x3, 8);
  U64_TO_BYTES(c + 8, s.x4, 8);
  */

  return 0;
}

void set_nonce(unsigned char* nonce, unsigned long long val, int *CUBE, int CS){
    unsigned long long N = 0;
	for(int i = 0 ; i<CS; i++){
	    int t = (val>>i) & 1 ;
		if(t == 1) N |= (1ULL << (63 - CUBE[i])) ;
		//printf("%d\t %016llx\n", t, N);
	}
	//printf("%d\t %016llx\n", val, N);
	
	U64_TO_BYTES(nonce, N, 8);
	//for(int i = 0 ; i<8; i++)printf("%02X ",nonce[i] );
	//printf("\n");	

    // Set N_0 = N_1
	for(int i = 0 ; i<8; i++)nonce[8+i] = nonce[i] ; 	
}

void set_key(unsigned char* key, int *CUBE, int CS){
    unsigned long long K0 = 0, K1 = 0;

    // Set a random K0
    for(int i = 0 ; i<64; i++){
        int te = rand() % 2 ;
        if(te == 1) {
            K0 |= (1ULL << (63 - i)) ;
        }
        else{
            K0 &= (~(1ULL << (63 -i)));
        }
    }

    // Set a random K1
    for(int i = 0 ; i<64; i++){
        int te1 = rand() % 2 ;
        if(te1 == 1) {
            K1 |= (1ULL << (63 - i)) ;
        }
        else{
            K1 &= (~(1ULL << (63 -i)));
        }
    }

    /*
     * Set K0[i] = K1[i] for all i in CUBE \ {56, 57, 58, 59}
     * Set K0[i] = 1 + K1[i] for all i in CUBE  and i in {56, 57, 58, 59}
     */

    for(int i = 0 ; i<CS ; i++){
        int te2 = (K0 >> (63 - CUBE[i])) & 1 ;

        if(CUBE[i] == 56 || CUBE[i] == 57 || CUBE[i] == 58 || CUBE[i] == 59){
            if(te2 == 0) {
                K1 |= (1ULL << (63 - CUBE[i])) ;
            }
            else{
                K1 &= (~(1ULL << (63 - CUBE[i])));
            }
        }
        else{
            if(te2 == 0) {
                K1 &= (~(1ULL << (63 - CUBE[i])));
            }
            else{
                K1 |= (1ULL << (63 - CUBE[i])) ;
            }
        }
    }

    U64_TO_BYTES(key, K0, 8);
    U64_TO_BYTES(key+8, K1, 8);
}

void set_key_d21(unsigned char* key, int *CUBE, int CS){
    unsigned long long K0 = 0, K1 = 0;

    // Set a random K0
    for(int i = 0 ; i<64; i++){
        int te = rand() % 2 ;
        if(te == 1) {
            K0 |= (1ULL << (63 - i)) ;
        }
        else{
            K0 &= (~(1ULL << (63 -i)));
        }
    }

    // Set a random K1
    for(int i = 0 ; i<64; i++){
        int te1 = rand() % 2 ;
        if(te1 == 1) {
            K1 |= (1ULL << (63 - i)) ;
        }
        else{
            K1 &= (~(1ULL << (63 -i)));
        }
    }

    /*
     * Set K0[i] = K1[i]  = 0 for all i in CUBE \ {56, 57, 58, 59} or
     * Set K0[i] = 0, K1[i] = 1 for all i in CUBE  and i in {56, 57, 58, 59}
     */

    for(int i = 0 ; i<CS ; i++){

        if(CUBE[i] == 56 || CUBE[i] == 57 || CUBE[i] == 58 || CUBE[i] == 59){
            K0 &= (~(1ULL << (63 - CUBE[i])));
            K1 |= (1ULL << (63 - CUBE[i])) ;
        }
        else{
            // All zero case
            K0 &= (~(1ULL << (63 - CUBE[i])));
            K1 &= (~(1ULL << (63 - CUBE[i])));
        }
    }

    U64_TO_BYTES(key, K0, 8);
    U64_TO_BYTES(key+8, K1, 8);
}

void set_key_d22(unsigned char* key, int *CUBE, int CS){
    unsigned long long K0 = 0, K1 = 0;

    // Set a random K0
    for(int i = 0 ; i<64; i++){
        int te = rand() % 2 ;
        if(te == 1) {
            K0 |= (1ULL << (63 - i)) ;
        }
        else{
            K0 &= (~(1ULL << (63 -i)));
        }
    }

    // Set a random K1
    for(int i = 0 ; i<64; i++){
        int te1 = rand() % 2 ;
        if(te1 == 1) {
            K1 |= (1ULL << (63 - i)) ;
        }
        else{
            K1 &= (~(1ULL << (63 -i)));
        }
    }

    /*
     * Set K0[i] = K1[i]  = 1 for all i in CUBE \ {56, 57, 58, 59} or
     * Set K0[i] = 1, K1[i] = 0 for all i in CUBE  and i in {56, 57, 58, 59}
     */

    for(int i = 0 ; i<CS ; i++){

        if(CUBE[i] == 56 || CUBE[i] == 57 || CUBE[i] == 58 || CUBE[i] == 59){
            K0 |= (1ULL << (63 - CUBE[i])) ;
            K1 &= (~(1ULL << (63 - CUBE[i])));
        }
        else{
            // All one case
            K0 |= (1ULL << (63 - CUBE[i])) ;
            K1 |= (1ULL << (63 - CUBE[i])) ;
        }
    }

    U64_TO_BYTES(key, K0, 8);
    U64_TO_BYTES(key+8, K1, 8);
}

void distinguisher_1(int N){

    unsigned long long clen, exp, i, j;
    u8 key[16], nonce[16], ks[40], sum[8];

    // 5 rounds
    /*
    int CS = 13 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12} ;
    */

    // 6 rounds
    int CS = 24 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, \
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
    } ;

    // 7 rounds
    /*
    int CS = 46 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, \
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, \
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, \
    35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45} ;
    */

    for(exp = 0 ; exp< N ; exp++){
        for(i = 0;  i<8 ; i++) sum[i] = 0 ;
        set_key(key, CUBE, CS);

        //for(i = 0; i<8; i++)printf("%02X ", key[i]); printf("\n");
        //for(i = 0; i<8; i++)printf("%02X ", key[8 + i]); printf("\n");

        for(i = 0 ; i<16; i++) nonce[i] = 0 ;
        for(i = 0 ; i<((unsigned long long) 1<<(CS)); i++){
            set_nonce(nonce, i, CUBE, CS) ;
            crypto_aead_encrypt(ks, &clen, NULL, 0, NULL, 0, NULL, nonce, key);
            for(j = 0; j<8; j++)sum[j] ^= ks[j] ;
        }
        printf("Experiment: %llu |  Key: \t\t", exp);
        for(i = 0 ; i<8; i++)printf("%02X ", key[i]);
        printf(" | ");
        for(i = 0 ; i<8; i++)printf("%02X ", key[8 + i]);
        printf("\n\t\t Cube sum:\t");
        for(i = 0 ; i<8; i++)printf("%02X ", sum[i]) ;
        printf("\n\n");
    }
}

void distinguisher_21(int N){
    unsigned long long clen, exp, i, j;
    u8 key[16], nonce[16], ks[40], sum[8];

    // 5 rounds
    /*
    int CS = 9 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8} ;
    */

    // 6 rounds
    int CS = 17 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                  13, 14, 15, 16
    } ;

    // 7 rounds
    /*
    int CS = 33 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, \
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, \
    24, 25, 26, 27, 28, 29, 30, 31, 32} ;
    */

    for(exp = 0 ; exp< N ; exp++){
        for(i = 0;  i<8 ; i++) sum[i] = 0 ;
        set_key_d21(key, CUBE, CS);

        //for(i = 0; i<8; i++)printf("%02X ", key[i]); printf("\n");
        //for(i = 0; i<8; i++)printf("%02X ", key[8 + i]); printf("\n");

        for(i = 0 ; i<16; i++) nonce[i] = 0 ;
        for(i = 0 ; i<((unsigned long long) 1<<(CS)); i++){
            set_nonce(nonce, i, CUBE, CS) ;
            crypto_aead_encrypt(ks, &clen, NULL, 0, NULL, 0, NULL, nonce, key);
            for(j = 0; j<8; j++)sum[j] ^= ks[j] ;
        }
        printf("Experiment: %llu |  Key: \t\t", exp);
        for(i = 0 ; i<8; i++)printf("%02X ", key[i]);
        printf(" | ");
        for(i = 0 ; i<8; i++)printf("%02X ", key[8 + i]);
        printf("\n\t\t Cube sum:\t");
        for(i = 0 ; i<8; i++)printf("%02X ", sum[i]) ;
        printf("\n\n");
    }
}

void distinguisher_22(int N){
    unsigned long long clen, exp, i, j;
    u8 key[16], nonce[16], ks[40], sum[8];

    // 5 rounds
    /*
    int CS = 9 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8} ;
    */

    // 6 rounds
    int CS = 17 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
                  13, 14, 15, 16
    } ;

    // 7 rounds
    /*
    int CS = 33 ;
    int CUBE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, \
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, \
    24, 25, 26, 27, 28, 29, 30, 31, 32} ;
    */

    for(exp = 0 ; exp< N ; exp++){
        for(i = 0;  i<8 ; i++) sum[i] = 0 ;
        set_key_d22(key, CUBE, CS);

        //for(i = 0; i<8; i++)printf("%02X ", key[i]); printf("\n");
        //for(i = 0; i<8; i++)printf("%02X ", key[8 + i]); printf("\n");

        for(i = 0 ; i<16; i++) nonce[i] = 0 ;
        for(i = 0 ; i<((unsigned long long) 1<<(CS)); i++){
            set_nonce(nonce, i, CUBE, CS) ;
            crypto_aead_encrypt(ks, &clen, NULL, 0, NULL, 0, NULL, nonce, key);
            for(j = 0; j<8; j++)sum[j] ^= ks[j] ;
        }
        printf("Experiment: %llu |  Key: \t\t", exp);
        for(i = 0 ; i<8; i++)printf("%02X ", key[i]);
        printf(" | ");
        for(i = 0 ; i<8; i++)printf("%02X ", key[8 + i]);
        printf("\n\t\t Cube sum:\t");
        for(i = 0 ; i<8; i++)printf("%02X ", sum[i]) ;
        printf("\n\n");
    }
}


void distinguisher_1_small_cube(int N){
    unsigned long long clen, exp, i, j, a, l;
    u8 key[16], nonce[16], ks[40], sum[8];

    // 6 rounds
    int CS = 23 ;
    int CUBE[CS];

    // Change CS to 22 and 21 to observe p = 0.42 and p = 0.46, respectively as mentioned
    // in Section 4.3 of our paper.

    float p = 0 ;

    for(exp = 1 ; exp<= N ; exp++){

        for(i = 0 ; i< CS ; i++){
            while(1){
                // a = 64*drand48();
                a = 64*((double)rand()/(RAND_MAX+1.0));
                l = 1;
                for(j = 0; j<i; j++)
                    if(CUBE[j]==a)
                        l = 0;
                    if(l==1){
                        CUBE[i] = a;
                        break;
                    }
            }
        }

        for(i = 0;  i<CS ; i++){
            for(j = i+1;  j<CS ; j++){
                if(CUBE[i]>CUBE[j]){
                    CUBE[i] ^= CUBE[j];
                    CUBE[j] ^= CUBE[i];
                    CUBE[i] ^= CUBE[j];
                }
            }
        }

        for(i = 0;  i<8 ; i++) sum[i] = 0 ;
        set_key(key, CUBE, CS);

        //for(i = 0; i<8; i++)printf("%02X ", key[i]); printf("\n");
        //for(i = 0; i<8; i++)printf("%02X ", key[8 + i]); printf("\n");

        for(i = 0 ; i<16; i++) nonce[i] = 0 ;
        for(i = 0 ; i<((unsigned long long) 1<<(CS)); i++){
            set_nonce(nonce, i, CUBE, CS) ;
            crypto_aead_encrypt(ks, &clen, NULL, 0, NULL, 0, NULL, nonce, key);
            for(j = 0; j<8; j++)sum[j] ^= ks[j] ;
        }
        printf("Experiment: %llu  \t\t", exp);
        printf("\nCube: ");
        for(i = 0 ; i< CS-1 ; i++){
            printf("%d, ", CUBE[i]);
        }
        printf("%d", CUBE[CS-1]);

        printf("\n");
        printf("Key: \t\t");
        for(i = 0 ; i<8; i++)printf("%02X ", key[i]);
        printf(" | ");
        for(i = 0 ; i<8; i++)printf("%02X ", key[8 + i]);
        printf("\nCube sum:\t");
        for(i = 0 ; i<8; i++)printf("%02X ", sum[i]) ;

        for(i = 0;  i<8 ; i++){
            for(j = 0;  j<8 ; j++){
                if(sum[i]%2==1)
                    p = p + 1.0;
                sum[i]=sum[i]/2;
            }
        }
        printf("\nProbability:    %0.3f\n\n", p/(64*exp));
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    srand(time(NULL) + clock());

    int N1 = 1<<4 ;                            // Number of experiments for distinguisher 1
    int N2 = 1<<4 ;                            // Number of experiments for distinguisher 2


    printf("Distinguisher 1\n\n") ;
    distinguisher_1(N1);                      // Section 4.2, Example 1 for 6 rounds

    printf("\nDistinguisher 21\n\n") ;
    distinguisher_21(N2);                      // Section 4.2, Example 2 for 6 rounds; Section 5.2, Equation 17

    printf("\nDistinguisher 22\n\n") ;
    distinguisher_22(N2);                      // Section 4.2, Example 2 for 6 rounds; Section 5.2, Equation 18


    printf("Distinguisher 1 for small size random cubes\n\n") ;
    N1 = 1<<12 ;
    distinguisher_1_small_cube(N1);         // Section 4.3 experimental results for |I| = 23

    return 0;

}



