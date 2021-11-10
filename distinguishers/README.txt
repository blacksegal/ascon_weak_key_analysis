/*
 * For verification of distinguishers as given in Section 4 and Section 5 of our paper.
 * The provided C code is taken from https://ascon.iaik.tugraz.at/resources.html and adapted
    for our distinguishers.
 */

// Notes on the code
- To change the number of rounds, please go to function "P12(state* s)" in permutation.h file

- The "main" function in encrypt.c file consists of four functions:
    1. distinguisher_1()                        // Section 4.2, Example 1 for 6 rounds
    2. distinguisher_21()                       // Section 4.2, Example 2 for 6 rounds; Section 5.2, Equation 17
    3. distinguisher_22()                       // Section 4.2, Example 2 for 6 rounds; Section 5.2, Equation 18
    4. distinguisher_1_small_cube()             // Section 4.3 results for 6 rounds

- Some cubes are given in the individual functions for verification only. To have random cubes,
  please change the respective cube indices.

- Current settings in the code
    - Distinguisher 1:
        - 6 rounds and 16 experiments
        - 24 dimensional cube given by:
        const int CUBE[CS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};

    - Distinguisher 21 and Distinguihser 22:
        - 6 rounds and 16 experiments
        - 17 dimensional cube given by:
        const int CUBE[CS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} ;

    - Distinguisher_1_small_cube:
        - 6 rounds and 2^{12} experiments
        - Random 23-dimensional cubes


// Running the code
1. gcc encrypt.c -O3
2. ./a.out