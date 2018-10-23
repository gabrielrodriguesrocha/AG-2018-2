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
int * erdos_numbers;
adj_list * graph;

void init_graph(int, int);

void free_graph(int, int);

void push_vertex(adj_list *, int);

int bfs(adj_list *, int, int);

int max_erdos_number(adj_list *, int);

void main() {
    int n,m,i,max;
    int is_init = 0;

    while(1) {
        free_graph(n, is_init);
        scanf("%d %d", &n, &m);
        if (!n && !m)
            return;
        init_graph(n, m);
        if ((max = max_erdos_number(graph, n)) == -1) { printf("infinito\n"); }
        else { printf("%d\n", max); }
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

int max_erdos_number(adj_list *graph, int n) {
    int i,max;
    bfs(graph, n, 0);
    for (i = 0, max = 0; i < n; i++) {
        if (states[i] == -1) // the graph isn't connected, hence there's an isolated vertex whose number is infinity
            return -1;
        max = erdos_numbers[i] > max ? erdos_numbers[i] : max;
    }
    return max;
}

int bfs(adj_list *graph, int n, int r) {
    int i;
    int queue[n], queue_start, queue_end, current_erdos_number, neighbours;
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
        current_erdos_number = erdos_numbers[queue[queue_start]];
        for (i = 0; i < neighbours; i++) {
            if (states[current_vertex->v] == 1) { current_vertex = current_vertex->next; continue; }
            if (states[current_vertex->v] == -1) {
                queue[++queue_end] = current_vertex->v;
                states[current_vertex->v] = 0;
                erdos_numbers[current_vertex->v] = current_erdos_number + 1;
            }
            current_vertex = current_vertex->next;
        }
        queue_start++;
    }

    return 0;
}

void init_graph(int n, int m) {
    int i,u,v;

    colors = malloc(sizeof(int) * n);
    states = malloc(sizeof(int) * n);
    erdos_numbers = malloc(sizeof(int) * n);
    graph = malloc(sizeof(adj_list) * n);

    for (i = 0; i < n; i++) {
        colors[i] = -1;
        states[i] = -1;
        erdos_numbers[i] = 0;
        (graph + i)->v = 0;
        (graph + i)->next = NULL;
    }

    for(i = 0; i < m; i++) {
        scanf("%d %d\n", &v, &u);
        //u--; v--;
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
    free(erdos_numbers);

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