#include<fstream>
#include<iostream>
#define MAX_VERTICES 50 
#define TRUE 1
#define FALSE 0
using namespace std;
typedef struct node* nodePointer;
struct node {
    int vertex;
    nodePointer link;
};
nodePointer createNode(int);

typedef struct Graph* graphPointer;
struct Graph {
    int nVertices;
    nodePointer* listAdj;
};


nodePointer createNode(int v) {
    nodePointer Nodeptr = (nodePointer)malloc(sizeof(node));
    Nodeptr->vertex = v;
    Nodeptr->link = NULL;
    return Nodeptr;
}


graphPointer createGraph(int vertices) {
    graphPointer graph = (graphPointer)malloc(sizeof(Graph));
    graph->nVertices = vertices;

    graph->listAdj = (nodePointer*)malloc(vertices * sizeof(nodePointer));

    int i;
    for (i = 0; i < vertices; i++)
        graph->listAdj[i] = NULL;

    return graph;
}

void insertEdge(graphPointer graph, int s, int d) {
    
    nodePointer Nodeptr = createNode(d);
    Nodeptr->link = graph->listAdj[s];
    graph->listAdj[s] = Nodeptr;


}


short int visited[MAX_VERTICES];

void dfs(int v, graphPointer graph, ofstream& out) {
    /*depth first search of a graph beginning at V*/
    nodePointer w;
    visited[v] = TRUE;

   // printf("%5d", v);
    out << v;
    out << " ";
    for (w = graph->listAdj[v]; w; w = w->link)
        if (!visited[w->vertex])
            dfs(w->vertex, graph, out);


}
void connected(int numV, graphPointer graph, ofstream& out) {
    /*determine the connected components of a graph*/
    for (int i = 0; i < numV; i++)
        if (!visited[i]) {
            dfs(i, graph, out);
            //cout<<"\n";
            out << "\n";


        }
}




int main() {

    ifstream in;
    ofstream out;
    in.open("input.txt");
    out.open("output.txt");
    if (in.fail()) cout << "File is not open" << endl;

    int numV;  /*the number of vertex*/
    in >> numV;
    int x;  /*search*/
    graphPointer graph = createGraph(numV);
    for (int i = 0; i < numV; i++) {

        for (int j = 0; j < numV; j++) {
            in >> x;
            if (x == 1) {
                insertEdge(graph, i, j);
            }
        }

    }

    connected(numV, graph, out);
    in.close();
    return 0;
}