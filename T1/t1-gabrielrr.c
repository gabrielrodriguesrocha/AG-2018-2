#include<stdio.h>
#include<stdlib.h>

typedef struct node {
    // v functions both as the index of the neighbouring vertex and
    // the number of neighbours of the i-th vertex if accessed from the head of the list
    int v;
    struct node *next;
} adj_list;

typedef adj_list entry;

int * colors;
int * states; // -1 -> not visited, 0 -> visited, 1 -> already explored
adj_list * graph;

void init_graph(int, int);

void free_graph(int, int);

void push_vertex(adj_list *, int);

int bfs(adj_list *, int, int);

int is_bipartite(adj_list *, int);

void main() {
    int n,m,i;
    int is_init = 0;

    while(1) {
        free_graph(n, is_init);
        scanf("%d %d", &n, &m);
        if (!n && !m)
            return;
        init_graph(n, m);
        if (is_bipartite(graph, n)) { printf("SIM\n"); }
        else { printf("NAO\n"); }
        is_init = 1;
    }
}

void push_vertex(adj_list *head, int v) {
    entry *new_entry = malloc(sizeof(entry));
    new_entry->v = v;
    new_entry->next = head->next;
    head->next = new_entry;
    head->v = head->v + 1;
}

int is_bipartite(adj_list *graph, int n) {
    int i;
    for (i = 0; i < n; i++) { // loop through all connected components
        if (states[i] == -1 && !bfs(graph, n, i))
            return 0;
    }
    return 1;
}

int bfs(adj_list *graph, int n, int r) {
    int i;
    int queue[n], queue_start, queue_end, current_color, neighbours;
    entry *current_vertex;

    queue_start = 0;
    queue_end = 0;

    queue[queue_start] = r;

    colors[r] = 0;

    while (queue_end >= queue_start) {
        current_vertex = graph + queue[queue_start];
        neighbours = current_vertex->v;
        current_vertex = current_vertex->next;
        states[queue[queue_start]] = 1;
        current_color = !colors[queue[queue_start]];
        for (i = 0; i < neighbours; i++) {
            if (states[current_vertex->v] == 1) { current_vertex = current_vertex->next; continue; }
            if (states[current_vertex->v] == -1) {
                queue[++queue_end] = current_vertex->v;
                states[current_vertex->v] = 0;
                colors[current_vertex->v] = current_color;
            }
            else if (states[current_vertex->v] == 0) {
                if (colors[current_vertex->v] != current_color) { // connected component isn't bipartite
                    return 0;
                }
            }
            current_vertex = current_vertex->next;
        }
        queue_start++;
    }

    return 1; //connected component is bipartite
}

void init_graph(int n, int m) {
    int i,u,v;

    colors = malloc(sizeof(int) * n);
    states = malloc(sizeof(int) * n);
    graph = malloc(sizeof(adj_list) * n);

    for (i = 0; i < n; i++) {
        colors[i] = -1;
        states[i] = -1;
        (graph + i)->v = 0;
        (graph + i)->next = NULL;
    }

    for(i = 0; i < m; i++) {
        scanf("%d %d\n", &v, &u);
        u--; v--;
        push_vertex(graph + v, u);
        push_vertex(graph + u, v);
    }
}

void free_graph(int n, int is_init) {
    int i, k, neighbours;
    entry *cur, *del;

    if (!is_init) 
        return;

    free(colors);
    free(states);

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
}