// sample/main.c
// Complex test program for AutoBuildTool
// - sieve of Eratosthenes (count primes up to LIMIT)
// - matrix multiplication (size MxM, measure time)
// - generate Mandelbrot PPM (width x height)
// Build: standard C11

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define SIEVE_LIMIT 100000      // number range for prime sieve
#define MAT_SIZE 140           // matrix multiply size (MAT_SIZE x MAT_SIZE)
#define MAN_WIDTH 400
#define MAN_HEIGHT 300
#define MAN_MAXITER 256

static double now_seconds(void){
    struct timespec ts;
#if defined(_WIN32)
    // On Windows, timespec from time.h might not be available in all compilers.
    // Use clock() fallback with CLOCKS_PER_SEC
    return (double)clock() / (double)CLOCKS_PER_SEC;
#else
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
#endif
}

static void sieve_count_primes(int limit, int *out_count){
    int n = limit + 1;
    unsigned char *isprime = (unsigned char*)malloc(n);
    if(!isprime){ fprintf(stderr,"malloc failed sieve\n"); *out_count = -1; return; }
    memset(isprime, 1, n);
    isprime[0] = isprime[1] = 0;
    int cnt = 0;
    for(int p=2; p*p<=limit; ++p){
        if(isprime[p]){
            for(int q=p*p; q<=limit; q+=p) isprime[q]=0;
        }
    }
    for(int i=2;i<=limit;++i) if(isprime[i]) ++cnt;
    free(isprime);
    *out_count = cnt;
}

static void matrix_mul_test(int M, double *out_time){
    // allocate A, B, C as contiguous arrays
    size_t n = (size_t)M*M;
    double *A = (double*)malloc(sizeof(double)*n);
    double *B = (double*)malloc(sizeof(double)*n);
    double *C = (double*)malloc(sizeof(double)*n);
    if(!A || !B || !C){
        fprintf(stderr,"malloc failed matrix\n"); *out_time = -1; return;
    }
    // initialize with deterministic pseudo-random values
    unsigned int seed = 123456789;
    for(size_t i=0;i<n;i++){
        seed = seed * 1103515245 + 12345;
        A[i] = (double)(seed % 1000) / 100.0;
        seed = seed * 1103515245 + 12345;
        B[i] = (double)(seed % 1000) / 100.0;
        C[i] = 0.0;
    }

    double t0 = now_seconds();
    // naive i-k-j triple loop (cache-friendly moderate variant)
    for(int i=0;i<M;i++){
        for(int k=0;k<M;k++){
            double aik = A[(size_t)i*M + k];
            for(int j=0;j<M;j++){
                C[(size_t)i*M + j] += aik * B[(size_t)k*M + j];
            }
        }
    }
    double t1 = now_seconds();
    *out_time = t1 - t0;

    // compute a small checksum for result to print
    double checksum = 0.0;
    for(int i=0;i<M;i+= (M>8? M/8 : 1)){
        checksum += C[(size_t)i*M + (i%M)];
    }

    // print checksum
    printf("Matrix multiply checksum (sample): %.6f\n", checksum);

    free(A); free(B); free(C);
}

static void mandelbrot_ppm(const char *path, int width, int height, int maxiter){
    FILE *f = fopen(path, "wb");
    if(!f){ fprintf(stderr,"Failed to open %s for writing\n", path); return; }
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    // compute
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            double cx = (x - 0.5*width) * (3.5/width) - 0.5;
            double cy = (y - 0.5*height) * (2.0/height);
            double zx = 0.0, zy = 0.0;
            int iter = 0;
            while(zx*zx + zy*zy <= 4.0 && iter < maxiter){
                double nx = zx*zx - zy*zy + cx;
                double ny = 2.0*zx*zy + cy;
                zx = nx; zy = ny;
                iter++;
            }
            unsigned char col[3];
            if(iter == maxiter){
                col[0] = col[1] = col[2] = 0;
            } else {
                int c = (int)(255.0 * iter / (double)maxiter);
                col[0] = (unsigned char)(c % 256);
                col[1] = (unsigned char)((c*3) % 256);
                col[2] = (unsigned char)((c*7) % 256);
            }
            fwrite(col, 1, 3, f);
        }
    }
    fclose(f);
}

int main(int argc, char **argv){
    printf("Complex test program start\n");
    printf("SIEVE_LIMIT=%d MAT_SIZE=%d MAN=%dx%d\n", SIEVE_LIMIT, MAT_SIZE, MAN_WIDTH, MAN_HEIGHT);

    // 1) Prime sieve
    printf("[1/3] Running sieve up to %d ...\n", SIEVE_LIMIT);
    double t0 = now_seconds();
    int prime_count = 0;
    sieve_count_primes(SIEVE_LIMIT, &prime_count);
    double t1 = now_seconds();
    printf("Found %d primes up to %d (%.3f s)\n", prime_count, SIEVE_LIMIT, t1 - t0);

    // 2) Matrix multiply
    printf("[2/3] Matrix multiply %dx%d ...\n", MAT_SIZE, MAT_SIZE);
    double mat_time = 0.0;
    double t2 = now_seconds();
    matrix_mul_test(MAT_SIZE, &mat_time);
    double t3 = now_seconds();
    printf("Matrix multiply took %.3f s (measured %.3f s)\n", mat_time, t3 - t2);

    // 3) Mandelbrot PPM
    printf("[3/3] Generating Mandelbrot PPM (%dx%d) ...\n", MAN_WIDTH, MAN_HEIGHT);
    double t4 = now_seconds();
    const char *outppm = "mandelbrot_test.ppm";
    mandelbrot_ppm(outppm, MAN_WIDTH, MAN_HEIGHT, MAN_MAXITER);
    double t5 = now_seconds();
    printf("Mandelbrot wrote %s (%.3f s)\n", outppm, t5 - t4);

    printf("All tasks done. Summary:\n");
    printf(" primes=%d, mat_time=%.3f s, mandelbrot_time=%.3f s\n", prime_count, mat_time, t5 - t4);
    return 0;
}
