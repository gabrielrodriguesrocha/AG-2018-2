#include<stdio.h>
#include<stdlib.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct node {
    // v functions both as the index of the neighbouring vertex and
    // the number of neighbours of the i-th vertex if accessed from the head of the list
    int v;
    int lowlink;
    int index;
    int scc;
    int on_stack;
    struct node *next;
} adj_list;

typedef struct stack {
    int top;
    int * data;
} stack;

typedef adj_list entry;

int v_index;
int scc_index;
stack tarjan_stack;
adj_list * graph;

void init_graph(int, int);

void free_graph(int, int);

void push_vertex(adj_list *, int);

void dfs(adj_list *, int, int);

int is_scc(adj_list *, int);

void main() {
    int n,m,i,max;
    int is_init = 0;

    while(1) {
        free_graph(n, is_init);
        scanf("%d %d", &n, &m);
        if (!n && !m)
            return;
        init_graph(n, m);
        printf("%d\n", is_scc(graph, n));
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

int is_scc(adj_list *graph, int n) {
    int i, scc;
    dfs(graph, n, 0);
    scc = (graph)->scc;
    for (i = 0; i < n; i++) {
        if ((graph + i)->scc != scc) // every vertex should be in the same SCC
            return 0;
    }
    return 1;
}

void dfs(adj_list *graph, int n, int r) {
    int i, neighbours, current_vertex_idx;
    entry * current_vertex;

    neighbours = (graph + r)->v;
    (graph + r)->index = v_index;
    (graph + r)->lowlink = v_index;
    v_index++;
    (graph + r)->on_stack = 1;
    tarjan_stack.data[tarjan_stack.top] = r;
    tarjan_stack.top++;

    current_vertex = (graph + r);

    for (i = 0; i < neighbours; i++) {
        current_vertex = current_vertex->next;
        current_vertex_idx = current_vertex->v;
        if ((graph + current_vertex_idx)->index == -1) {
            dfs(graph, n, current_vertex_idx);
            (graph + r)->lowlink = min((graph + current_vertex_idx)->lowlink, (graph + r)->lowlink);
        }
        else if((graph + current_vertex_idx)->on_stack) {
            (graph + r)->lowlink = min((graph + current_vertex_idx)->index, (graph + r)->lowlink);
        }
    }

    if ((graph + r)->lowlink == (graph + r)->index && (graph + r)->index != -1) {
        do {
            i = tarjan_stack.data[tarjan_stack.top - 1];
            (graph + i)->on_stack = 0;
            (graph + i)->scc = scc_index;
            tarjan_stack.top--;
        } while (i != r);
        scc_index++;
    }
}

void init_graph(int n, int m) {
    int i,u,v,p;

    tarjan_stack.top = 0;
    tarjan_stack.data = malloc(sizeof(int) * n);
    graph = malloc(sizeof(adj_list) * n);

    v_index = 0;
    scc_index = 0;

    for (i = 0; i < n; i++) {
        (graph + i)->index = -1;
        (graph + i)->lowlink = -1;
        (graph + i)->scc = -1;
        (graph + i)->on_stack = 0;
        (graph + i)->v = 0;
        (graph + i)->next = NULL;
    }

    for(i = 0; i < m; i++) {
        scanf("%d %d %d", &v, &u, &p);
        u--; v--;
        push_vertex(graph + v, u);
        if (p == 2)
            push_vertex(graph + u, v);
    }
}

void free_graph(int n, int is_init) {
    int i, k, neighbours;
    entry *cur, *del;

    if (!is_init) 
        return;

    free(tarjan_stack.data);

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