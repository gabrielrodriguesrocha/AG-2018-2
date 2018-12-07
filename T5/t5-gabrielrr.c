#include<stdio.h>
#include<stdlib.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

typedef struct node {
    // v functions both as the index of the neighbouring vertex and
    // the number of neighbours of the i-th vertex if accessed from the head of the list
    int v;
    int dist;
    int prev;
    int greenhouse;
    int greenhouse_dist;
    struct node *next;
} adj_list;

typedef struct priority_queue {
    int v;
    int dist;
    struct priority_queue * next;
} priority_queue;

typedef struct {
    int v;
    int dist;
} node_t;

typedef struct {
    node_t *nodes;
    int len;
} heap_t;

typedef adj_list entry;

adj_list * graph;
heap_t heap;

void init_graph(int, int);

void free_graph(int, int);

void clear_search(int);

void push_vertex(adj_list *, int, int);

void push_heap (heap_t *h, int dist, int v);

int pop_heap (heap_t *h);

void find_greenhouses(int, int);

void dijkstra(int, int);

int longest_time(adj_list *, int);

void main() {
    int n,m,k,i;
    int is_init = 0;
    scanf("%d %d %d", &n, &m, &k);
    init_graph(n, m);
    find_greenhouses(n, k);
    for (i = k; i < n; i++) {
        printf("%d", (graph + i)->greenhouse);
        if (i != n - 1)
            printf(" ");
    }
    printf("\n");
    free_graph(n, 1);
}

void find_greenhouses(int n, int k) {
    int i, j;
    for (i = 0; i < k; i++) {
        dijkstra(n, i);
        for (j = k; j < n; j++) {
            if ((graph + j)->dist < (graph + j)->greenhouse_dist || (graph + j)->greenhouse_dist == -1) {
                (graph + j)->greenhouse_dist = (graph + j)->dist;
                (graph + j)->greenhouse = i;
            }
        }
        clear_search(n);
    }
}

void dijkstra(int n, int root) {
    entry *current_vertex;
    int neighbours, q, i, tmp, not_visited;

    push_heap(&heap, 0, root);

    while(heap.len) {
        q = pop_heap(&heap);
        current_vertex = graph + q;
        neighbours = current_vertex->v;
        current_vertex = current_vertex->next;
        for (i = 0; i < neighbours; i++) {
            tmp = (graph + q)->dist + current_vertex->dist;
            not_visited = (graph + current_vertex->v)->dist == -1 ? 1 : 0;
            if((graph + current_vertex->v)->dist > tmp || (graph + current_vertex->v)->dist == -1) {
                (graph + current_vertex->v)->dist = tmp;
                (graph + current_vertex->v)->prev = q;
            }
            if (not_visited)
                push_heap(&heap, (graph + current_vertex->v)->dist, current_vertex->v);
            current_vertex = current_vertex->next;
        }
    }
}

void push_vertex(adj_list *head, int v, int dist) {
    entry *new_entry = malloc(sizeof(entry));
    new_entry->v = v;
    new_entry->dist = dist;
    new_entry->next = head->next;
    head->next = new_entry;
    head->v = head->v + 1;
}

void init_graph(int n, int m) {
    int i,u,v,dist,p;

    graph = malloc(sizeof(adj_list) * n);
    heap.nodes = malloc(n * sizeof(node_t));
    heap.len = 0;

    for (i = 0; i < n; i++) {
        (graph + i)->prev = -1;
        (graph + i)->dist = -1;
        (graph + i)->greenhouse = -1;
        (graph + i)->greenhouse_dist = -1;
        (graph + i)->v = 0;
        (graph + i)->next = NULL;
    }

    for(i = 0; i < m; i++) {
        scanf("%d %d %d", &v, &u, &dist);
        push_vertex(graph + v, u, dist);
        push_vertex(graph + u, v, dist);
    }
}

void free_graph(int n, int is_init) {
    int i, k, neighbours;
    entry *cur, *del;

    if (!is_init) 
        return;

    for (i = 0; i < n; i++) {
        cur = graph + i;
        neighbours = cur->v;
        cur = cur->next;
        for (k = 0; k < neighbours && cur != NULL; k++) {
            del = cur;
            cur = cur->next;
            free(del);
        }
    }

    free(graph);
    free(heap.nodes);
}

void clear_search(int n) {
    int i;
    for (i = 0; i < n; i++) {
        (graph + i)->prev = -1;
        (graph + i)->dist = -1;
    }
}

// Credit where credit is due, props to Rosetta Code:
// https://rosettacode.org/wiki/Priority_queue#C

void push_heap (heap_t *h, int dist, int v) {
    int i = h->len + 1;
    int j = i / 2;
    while (i > 1 && h->nodes[j].dist > dist) {
        h->nodes[i] = h->nodes[j];
        i = j;
        j = j / 2;
    }
    h->nodes[i].dist = dist;
    h->nodes[i].v = v;
    h->len++;
}

int pop_heap (heap_t *h) {
    int i, j, k;
    if (!h->len) {
        return -1;
    }
    int v = h->nodes[1].v;
 
    h->nodes[1] = h->nodes[h->len];
 
    h->len--;
 
    i = 1;
    while (i!=h->len+1) {
        k = h->len+1;
        j = 2 * i;
        if (j <= h->len && h->nodes[j].dist < h->nodes[k].dist) {
            k = j;
        }
        if (j + 1 <= h->len && h->nodes[j + 1].dist < h->nodes[k].dist) {
            k = j + 1;
        }
        h->nodes[i] = h->nodes[k];
        i = k;
    }
    return v;
}