#include<stdio.h>
#include<stdlib.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

double * dist;

void init_graph(int, int);

void free_graph(int, int);

int modified_floydwarshall(double *, int);

int money_scheme(double *, int);

void main() {
    int n,i,max;
    int is_init = 0;

    while(1) {
        free_graph(n, is_init);
        scanf("%d", &n);
        if (!n)
            return;
        init_graph(n, n);
        if (money_scheme(dist, n) == 1)
            printf("S\n");
        else
            printf("N\n");
        is_init = 1;
    }
}

int money_scheme(double *dist, int n) {
    int i;
    modified_floydwarshall(dist, n);
    for (i = 0; i < n; i++) {
        if (dist[i*n + i] > 1)
            return 1;
    }
    return 0;
}

int modified_floydwarshall(double *dist, int n) {
    int i, j, k;

    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (dist[i*n + j] < dist[i*n + k] * dist[k*n + j])
                    dist[i*n + j] = dist[i*n + k] * dist[k*n + j];
            }
        }
    }
}

void init_graph(int n, int m) {
    int i,j;
    double w;

    dist = malloc(sizeof(double) * n * n);

    for(i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%lf", &w);
            dist[i*n + j] = w;
        }
    }
}

void free_graph(int n, int is_init) {

    if (!is_init) 
        return;

    free(dist);
}