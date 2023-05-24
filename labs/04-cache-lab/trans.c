/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{   
    //deal with the block in the same row first
    //rb = 9, cb = 9 for M = 61, N = 67 case
    int i, j, k, l;
    if (M != N) {
        int rb_size; //block size for a step in row wise
        int cb_size; //block size for a step in col wise
        rb_size = 9;
        cb_size = 9;
        int iB = cb_size; // block size in col wise for the last block
        for (i = 0; i < N; i += cb_size) {
            if (i + cb_size >= N)
                iB = N - i;
            int jB = rb_size; // block size in row wise for the last block
            for (j = 0; j < M; j += rb_size){       
                if (j + rb_size >= M)
                    jB = M - j;
                for (k = j; k < j + jB; k ++) {
                    for (l = i; l < i + iB; l ++) {
                        B[k][l] = A[l][k];
                    }
                }
            }
        }
    }
    else if (M == 32){
        int a, b, c, d, e, f, g, h;
        for (i = 0; i < N; i += 8){
            for (j = 0; j < M; j += 8) {
                for (k = i; k < i + 8; k ++){
                    a = A[k][j];
                    b = A[k][j + 1];
                    c = A[k][j + 2];
                    d = A[k][j + 3];
                    e = A[k][j + 4];
                    f = A[k][j + 5];
                    g = A[k][j + 6];
                    h = A[k][j + 7];
                    B[j][k] = a;
                    B[j + 1][k] = b;
                    B[j + 2][k] = c;
                    B[j + 3][k] = d;
                    B[j + 4][k] = e;
                    B[j + 5][k] = f;
                    B[j + 6][k] = g;
                    B[j + 7][k] = h;
                }
            }
        }
    }
    else {
        int a, b, c, d;
        for (i = 0; i < N; i += 4){
            for (j = 0; j < M; j += 4) {
                for (k = i; k < i + 4; k ++){
                    a = A[k][j];
                    b = A[k][j + 1];
                    c = A[k][j + 2];
                    d = A[k][j + 3];
                    B[j][k] = a;
                    B[j + 1][k] = b;
                    B[j + 2][k] = c;
                    B[j + 3][k] = d;
                }
            }
        }
    }


        /*
    //col wise first then row wise copy
    int iB = rb_size;
    for (i = 0; i < M; i += rb_size) {
        if (i + rb_size >= M)
            iB = M - i;
        int jB = cb_size; // block size in row wise for the last block
        for (j = 0; j < N; j += cb_size){       
            if (j + cb_size >= N)
                jB = N - j;
            for (k = j; k < j + jB; k ++) {
                for (l = i; l < i + iB; l ++) {
                    B[k][l] = A[l][k];
                }
            }
        }
    }
    */
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

