//
// AED, November 2021
//
// Solution of the first practical assignement (subset sum problem)
//
// Place your student numbers and names here
// Ana Raquel Paradinha   102491 
// Paulo Pinto            103234
// Tiago Carvalho         104142

#if __STDC_VERSION__ < 199901L
# error "This code must must be compiled in c99 mode or later (-std=c99)" // to handle the unsigned long long data type
#endif
#ifndef STUDENT_H_FILE
# define STUDENT_H_FILE "000000.h"
#endif

//
// include files
//

#include <stdio.h>
#include <stdlib.h>
#include "elapsed_time.h"
#include <math.h>
#include STUDENT_H_FILE

//
// custom data types
//
// the STUDENT_H_FILE defines the following constants and data types
//
//   #define min_n       24                   --- the smallest n value we will handle
//   #define max_n       57                   --- the largest n value we will handle
//   #define n_sums      20                   --- the number of sums for each n value
//   #define n_problems  (max_n - min_n + 1)  --- the number of n values
//
//   typedef unsigned long long integer_t;    ---  64-bit unsigned integer
//   typedef struct
//   {
//     int n;                                 --- number of elements of the set (for a valid problem, min_n <= n <= max_n)
//     integer_t p[max_n];                    --- the elements of the set, already sorted in increasing order (only the first n elements are used)
//     integer_t sums[n_sums];                --- several sums (problem: for each sum find the corresponding subset)
//   }
//   subset_sum_problem_data_t;               --- weights p[] and sums for a given value of n
//
//   subset_sum_problem_data_t all_subset_sum_problems[n_problems]; --- // the problems
//
//
// place your code here
//
// possible function prototype for a recursive brute-force function:
//   int brute_force(int n,integer_t p[n],integer_t desired_sum,int current_index,integer_t partial_sum,int b[n]);
// it sould return 1 when the solution is found and 0 otherwise
// note, however, that you may get a faster function by reducing the number of function arguments (maybe a single pointer to a struct?)
//
int ctr = 0;
// Check all possible subsums of p until the sum is found and return the array b matching that sequence
int brute_force(int n, integer_t *p, integer_t sum, int next_index, integer_t partial_sum, int *b){
    // Check if the function has found the sum.
    if(sum == partial_sum){
        // Initiate every not-initiated elements of b.
        for(int i = next_index; i < n; i++){
            b[i] = 0;
        }
        // Return of that success.
        return 1;
    }
    // Return after every sequence.
    if(next_index == n){return 0;}
    // b's value on the current changes to 0.
    b[next_index] = 0;
    // Check if the function can found a solution using the next value of p.
    if(brute_force(n, p, sum, next_index + 1, partial_sum, b) == 1){return 1;}
    // b's value on the current index changes to 1.
    b[next_index] = 1; 
    // Check if the function can found a solution using the next and the current value of p.
    return brute_force(n, p, sum, next_index + 1, partial_sum + p[next_index], b);
}
int branch_and_bound(int n, integer_t *p, integer_t sum, int next_index, integer_t partial_sum, int *b){
    if(sum == partial_sum){
        for(int i = next_index; i < n; i++){
            b[i] = 0;
        }   
        return 1;
    }
    if(next_index == n) return 0;
    if(partial_sum > sum) return 0; 
    else {
        integer_t remaining_sum = 0;
        for(int j = next_index; j < n; j++){
            remaining_sum += p[j];
        }
        if((partial_sum + remaining_sum) < sum){
            return 0;
        } else {
            b[next_index] = 0;
            int result = branch_and_bound(n, p, sum, next_index+1, partial_sum, b);
            if(result == 1){return 1;}
            b[next_index] = 1;
            return branch_and_bound(n, p, sum,next_index+1, partial_sum+p[next_index], b);
        }
    }
}
int meet_in_the_middle(int na, int nb, integer_t *a, integer_t *b, integer_t sum, int *index) {
    int i = 0;
    int j = nb-1;
    integer_t sum_of_sums;
    do {
        sum_of_sums = a[i] + b[j];
        if (((i == na)) | ((j < 0))) {
            return EXIT_FAILURE;
        }
        if (sum_of_sums < sum) {
            i++;
        }
        else if (sum_of_sums > sum) {
            j--;
        }
    } while(sum_of_sums != sum);
    index[0] = i;
    index[1] = j;
    return EXIT_SUCCESS;
}
void subsetSums(integer_t *p, int index, int n_1, integer_t sum, integer_t *subsums) {
    if (index > n_1) {
        subsums[ctr] = sum;
        ctr++;
        return;
    }
    // Subset including p[index]
    subsetSums(p, index + 1, n_1, sum + p[index],subsums);
    // Subset excluding p[index]
    subsetSums(p, index + 1, n_1, sum,subsums);
}
int compare(const void *s1, const void *s2){
    if (*(integer_t*)s1 < *(integer_t*)s2 ) return -1;
    if (*(integer_t*)s1 > *(integer_t*)s2 ) return 1;
    return 0;
}

//
// main program
//
int main(void)
{
    int choice;
    // ----- THE USER MAY COMMENT OR UNCOMMENT THE FOLLOWING LINES DEPENDING ON THE INTENDED METHOD ----- //
    // choice = 1; // Use the "brute force" method
    // choice = 2; // Use the "branch and bound" method
     choice = 3; // Use the "meet in the middle" method
    // -------------------------------------------------------------------------------------------------- //
    
    // Initiate the files.
    FILE *stderr; // Responsible for listing the results.
    FILE *times_file; // Responsible for listing the time the program took to find every subsum.
    // Open the following files. Need to be manually changed to feature new files.
    stderr = fopen("Solution_000000.txt", "a"); 
    if (choice == 1){times_file = fopen("Time_000000.txt", "a");}
    if (choice == 2){times_file = fopen("Time2_000000.txt", "a");}
    if (choice == 3){times_file = fopen("Time3_000000.txt", "a");}

    fprintf(stderr,"Program configuration:\n");
    fprintf(stderr,"  min_n ....... %d\n",min_n);
    fprintf(stderr,"  max_n ....... %d\n",max_n);
    fprintf(stderr,"  n_sums ...... %d\n",n_sums);
    fprintf(stderr,"  n_problems .. %d\n",n_problems);
    fprintf(stderr,"  integer_t ... %d bits\n",8 * (int)sizeof(integer_t));
    
    //
    // for each n
    //
    for(int i = 0; i < n_problems; i++) {


        int n = all_subset_sum_problems[i].n; // the value of n
        //if(n > 20) continue; // skip large values of n
        integer_t *p = all_subset_sum_problems[i].p; // the weights
        fprintf(stderr, "n -> %d\n",n);
        double tempo = cpu_time();

        //
        // for each sum
        //

        // use the "brute force" method
        if (choice == 1){ 
            for(int j = 0 ; j < n_sums; j++){
                // allocate memory to array b
                int *b = (int *) malloc(n * sizeof(int));
                integer_t sum = all_subset_sum_problems[i].sums[j];
                // execution of brute_force function
                brute_force(n, p, sum, 0, 0, b);
                for(int index = 0; index < n; index++){
                    fprintf(stderr, "%d", b[index]);
                }
                fprintf(stderr, "\n");
                // deallocate memory on array b
                free(b);
            }
        }

        // use the "branch and bound" method
        else if (choice == 2){ 
            for(int j = 0; j < n_sums; j++){
                // allocate memory to array b
                int *b = (int *) malloc(n * sizeof(int));
                integer_t sum = all_subset_sum_problems[i].sums[j];
                // execution of brute_force function
                branch_and_bound(n, p, sum, 0, 0, b);
                for(int index = 0; index < n; index++){
                    fprintf(stderr, "%d", b[index]);
                }
                fprintf(stderr, "\n");
                // deallocate memory on array b
                free(b);
            }
        }

        // use the "Meet in the middle" method
        else if (choice == 3){ 
            int n1 = n/2;
            int n2 = n - n1;
            // Guardar memória para cada um dos arrays
            integer_t *p1 = (integer_t *) malloc(n1 * sizeof(integer_t));
            integer_t *p2 = (integer_t *) malloc(n2 * sizeof(integer_t));
            // Criação dos arrays
            for (int k = 0; k < n1; k++){
                p1[k] = p[k];
                p2[k] = p[n1 + k];
            }
            p2[n2-1]=p[n-1];
            int na = pow(2,n1);
            integer_t *a = (integer_t *) malloc(na * sizeof(integer_t));
            subsetSums(p1, 0, n1-1, 0, a);
            ctr=0;
            int nb = pow(2,n2);
            integer_t *b = (integer_t *) malloc(nb * sizeof(integer_t));
            subsetSums(p2, 0, n2-1, 0, b);
            ctr=0;
            qsort(a, na, sizeof(integer_t), compare);
            qsort(b, nb, sizeof(integer_t), compare);
            // FUNCIONA DAQUI PARA CIMA
            for(int j = 0; j < n_sums; j++){
                integer_t sum = all_subset_sum_problems[i].sums[j];
                int index[2] = {0,0};
                meet_in_the_middle(na, nb, a, b, sum, index);
                int *bA = (int *) malloc(n1 * sizeof(int));
                int *bB = (int *) malloc(n2 * sizeof(int));
                branch_and_bound(n1, p1, a[index[0]], 0, 0, bA);
                branch_and_bound(n2, p2, b[index[1]], 0, 0, bB);
                int *bin = (int *) malloc(n * sizeof(int));
                for (int k = 0; k < n1; k++){
                    bin[k] = bA[k];
                    bin[n1+k] = bB[k];
                }
                bin[n-1] = bB[n2-1];
                //fprintf(stderr, "sum -> %lld; result -> ", sum);
                for(int index = 0; index < n; index++){
                    fprintf(stderr, "%d", bin[index]);
                }
                fprintf(stderr, "\n");
                free(bA);
                free(bB);
                free(bin);
            }
            free(p1);
            free(p2);
            free(a);
            free(b);
        }

        tempo = cpu_time() - tempo;
        //fprintf(times_file,"%17.15f\n",tempo);
        printf("Method %d : %d done in -> %17.15f seconds \n", choice, n, tempo);
    }
    return EXIT_SUCCESS;
}
