// CMakeProject1.cpp: определяет точку входа для приложения.
//


// C++ program to print DFS traversal from
// a given vertex in a  given graph
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <omp.h>
#define MaxNvar  42
using namespace std;

// Graph class represents a directed graph
// using adjacency list representation
class Edge {
    unsigned int start, end;
    double probability = 0.8;
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
    double getProbability()
    {
        return probability;
    }
    void changeState(bool change)
    {
        state = change;
    }
    Edge(int nodeStart, int nodeEnd)
    {
        start = nodeStart; end = nodeEnd; state = true;
    }
    Edge(int nodeStart, int nodeEnd,bool statebool)
    {
        start = nodeStart; end = nodeEnd; state = statebool;
    }
};


class Graph {
public:
    map<int, bool> visited;
    map<int, list<Edge*> > adj;
    vector<int> nodes;
    vector<Edge*> edges;

    // Function to add an edge to graph
    void addEdge(int v, int w);
    void addEdge(int v, int w,bool state);
    double calculate_state();

    // DFS traversal of the vertices
    // reachable from v
    bool DFS(int v, int m);
    vector<Edge*> getEdges();
};

void Graph::addEdge(int v, int w)
{
    // Add w to v’s list.
    Edge *edge = new Edge(v, w);
    adj[v].push_back(edge);
    edges.push_back(edge);
}
void Graph::addEdge(int v, int w,bool state)
{
    // Add w to v’s list.
    Edge* edge = new Edge(v, w,state);
    adj[v].push_back(edge);
    edges.push_back(edge);
}

double Graph::calculate_state()
{
    double graphprobabil = 0;
    
    for (auto edge : edges)
    {
        if (edge->getState()) graphprobabil *= edge->getProbability();
        else graphprobabil *= 1 - edge->getProbability();
        if (graphprobabil == 0)
        {
            if (edge->getState()) graphprobabil = edge->getProbability();
            else graphprobabil = 1 - edge->getProbability();
        }
    }
    //std::cout << graphprobabil<<"\n";
    return graphprobabil;
}




bool Graph::DFS(int v, int m)
{
    // Mark the current node as visited and
    // print it
    visited[v] = true;
    cout << v << " ";
    // Recur for all the vertices adjacent
    // to this vertex
    list<Edge*>::iterator i;
    for (auto i : adj[v]) {
        if (!visited[i->getEnd()] && i->getState())
        {
            DFS(i->getEnd(), m);
        }
    }
    //list<Edge>::iterator iter = std::find(adj[v].begin(), adj[v].end(), iter->getEnd()==m);
    if (visited[m])
    {
        //cout << "found" << "\n";
        return true;
    }
    else false;

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
    g.addEdge(3, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 6);
    g.addEdge(4, 5);
    g.addEdge(5, 7,false);
    g.addEdge(6, 2);
    g.addEdge(6, 0,false);
    g.addEdge(6, 8);
    g.addEdge(8, 9);
    g.addEdge(8, 10);
    g.addEdge(9, 10);
    g.addEdge(9, 11);
    g.addEdge(9, 12);
    unsigned int var, sol, Xvar;
    sol = 0;
    vector<Edge*> pointeredges = g.getEdges();

    for (var = 0; var < pointeredges.size(); var++) 
    {
        std::cout << pointeredges[var]->getState()<<" ";
    }
    std::cout << "\n";
    bool i = g.DFS(0,11);
    std::cout << i;
    double sum = 0;
    var = 0;
    do {
        if (pointeredges[var]->getState()) {
            pointeredges[var]->changeState(false);
            #pragma omp parallel for reduction(+ : sum) 
            //for (var = 0; var < pointeredges.size(); var++)
            //{
            //    std::cout << pointeredges[var]->getState() << " ";
            //    
            //    //sol++;
            //}
            if (g.DFS(0, 12)) sum += g.calculate_state();
            std::cout << sum << "\n";
            var = 0;
            std::cout << "\n";
           // printf("%d", sol); printf("\n");
        }
        else { pointeredges[var]->changeState(true); var++; }
    } while (var < pointeredges.size());
    printf("%d ",sol);
    return 0;
}

// improved by Vishnudev C
