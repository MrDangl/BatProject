// CMakeProject1.cpp: определяет точку входа для приложения.
//


// C++ program to print DFS traversal from
// a given vertex in a  given graph
#include <iostream>
#include <list>
#include <map>
#include <vector>
#define MaxNvar  42
using namespace std;

// Graph class represents a directed graph
// using adjacency list representation
class Edge {
    unsigned int start, end;
    float probability = 0.8f;
    bool state;
    public:
    int getStart()
    {
        return start;
    }
    int getEnd()
    {
        return end;
    }
    bool getState()
    {
        return state;
    }
    void changeState(bool change)
    {
        state = change;
    }
    Edge(int nodeStart, int nodeEnd)
    {
        start = nodeStart; end = nodeEnd;
    }
};


class Graph {
public:
    map<int, bool> visited;
    map<int, list<int> > adj;
    vector<Edge*> edges;

    // Function to add an edge to graph
    void addEdge(int v, int w);

    // DFS traversal of the vertices
    // reachable from v
    void DFS(int v);
    vector<Edge*> getEdges();
};

void Graph::addEdge(int v, int w)
{
    // Add w to v’s list.
    Edge *edge = new Edge(v, w);
    adj[v].push_back(w);
    edges.push_back(edge);
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

vector<Edge*> Graph::getEdges()
{
    return edges;
}

// Driver code
int main()
{
    Graph g;
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    unsigned int var, sol, Xvar;
    sol = 0;
    vector<Edge*> pointeredges = g.getEdges();

    for (var = 0; var < pointeredges.size(); var++) printf("%d ", pointeredges[var]->getState()); printf("\n");
    var = 0;
    do {
        if (var) {
            //for (var = 0; var < MaxNvar; var++) printf("%d ", V[var]); printf("\n");
            var = 0;
            sol++;
            printf("%d", sol); printf("\n");
        }
        else { var++; }
    } while (var < MaxNvar);
    printf("%d ",sol);
    return 0;
}

// improved by Vishnudev C
