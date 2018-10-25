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
    int time;
    int start;
    int max_time_to_end;
    struct node *next;
} adj_list;

typedef struct stack {
    int top;
    int * data;
} stack;

typedef adj_list entry;

adj_list * graph;

void init_graph(int, int);

void free_graph(int, int);

void push_vertex(adj_list *, int);

int dfs(adj_list *, int, int);

int longest_time(adj_list *, int);

void main() {
    int n,m,i,max;
    int is_init = 0;

    while(1) {
        free_graph(n, is_init);
        scanf("%d %d", &n, &m);
        if (!n && !m)
            return;
        init_graph(n, m);
        printf("%d\n", longest_time(graph, n));
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

int longest_time(adj_list *graph, int n) {
    int i, time;
    for (i = 0, time = 0; i < n; i++) {
        if ((graph + i)->start == 1) // only starts search on fountain vertices
            time = max (time, dfs(graph, n, i));
    }
    return time;
}

int dfs(adj_list *graph, int n, int r) {
    int i, neighbours, current_vertex_idx, tmp, cumulative_time;
    entry * current_vertex;

    neighbours = (graph + r)->v;

    current_vertex = (graph + r);
    tmp = current_vertex->time;
    cumulative_time = 0;

    for (i = 0; i < neighbours; i++) {
        current_vertex = current_vertex->next;
        current_vertex_idx = current_vertex->v;

        if ((graph + current_vertex_idx)->max_time_to_end != -1)
            tmp = (graph + current_vertex_idx)->max_time_to_end;
        else
            tmp = dfs(graph, n, current_vertex_idx);
        
        // We check all paths that can be explored by vertex r and save the biggest one (the one whose cumulative sum is the biggest)
        cumulative_time = max(cumulative_time, tmp);
    }

    // The reason we can memoize this is that all paths (u, v) on a DAG are such that v is a sink
    (graph + r)->max_time_to_end = max (cumulative_time + (graph + r)->time, (graph + r)->max_time_to_end);

    return cumulative_time + (graph + r)->time;
}

void init_graph(int n, int m) {
    int i,u,v,p;

    graph = malloc(sizeof(adj_list) * n);

    for (i = 0; i < n; i++) {
        (graph + i)->time = -1;
        (graph + i)->start = 1;
        (graph + i)->max_time_to_end = -1;
        (graph + i)->v = 0;
        (graph + i)->next = NULL;
    }

    for (i = 0; i < n; i++) {
        scanf("%d", &v);
        (graph + i)->time = v;
    }

    for(i = 0; i < m; i++) {
        scanf("%d %d", &v, &u);
        push_vertex(graph + v, u);
        (graph + u)->start = 0;
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
}