// CMakeProject1.cpp: определяет точку входа для приложения.
//


// C++ program to print DFS traversal from
// a given vertex in a  given graph
#include <iostream>
#include <list>
#include <map>
#define MaxNvar   10
using namespace std;

// Graph class represents a directed graph
// using adjacency list representation
class Graph {
public:
    map<int, bool> visited;
    map<int, list<int> > adj;

    // Function to add an edge to graph
    void addEdge(int v, int w);

    // DFS traversal of the vertices
    // reachable from v
    void DFS(int v);
};

void Graph::addEdge(int v, int w)
{
    // Add w to v’s list.
    adj[v].push_back(w);
}

void Graph::DFS(int v)
{
    // Mark the current node as visited and
    // print it
    visited[v] = true;
    cout << v << " ";

    // Recur for all the vertices adjacent
    // to this vertex
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            DFS(*i);
}

// Driver code
int main()
{
    unsigned int var, sol, Xvar;
    unsigned int V[MaxNvar];

    for (var = 0; var < MaxNvar; var++) printf("%d ", V[var] = 0); printf("\n");
    var = 0;
    do {
        if (V[var] == 0) {
            V[var] = 1;

            for (var = 0; var < MaxNvar; var++) printf("%d ", V[var]); printf("\n");
            var = 0;
        }
        else { V[var] = 0; var++; }
    } while (var < MaxNvar);
    return 0;
}

// improved by Vishnudev C
