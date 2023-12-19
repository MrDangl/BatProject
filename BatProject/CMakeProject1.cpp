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
    double probability = 0.9;
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
        start = nodeStart; end = nodeEnd; state = false;
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
    void clearVisited();
    void addEdge(int v, int w);
    void addEdge(int v, int w,bool state);
    double calculate_state();

    // DFS traversal of the vertices
    // reachable from v
    bool DFS(int v, int m);
    vector<Edge*> getEdges();
};
void Graph::clearVisited()
{
    visited.clear();
}

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
    #pragma omp parallel for reduction(+:graphprobabil)
    for (int i = 0; i<edges.size();i++) {
        if (edges[i]->getState()) graphprobabil *= edges[i]->getProbability();
        else graphprobabil *= 1 - edges[i]->getProbability();
        if (graphprobabil == 0)
        {
            if (edges[i]->getState()) graphprobabil = edges[i]->getProbability();
            else graphprobabil = 1 - edges[i]->getProbability();
        }
    }
//std::cout << graphprobabil<<"\n";
    return graphprobabil;
}




bool Graph::DFS(int v, int m)
{
    // Mark the current node as visited and
    // print it
#pragma omp critical
    visited[v] = true;
    //cout << v << " ";
    // Recur for all the vertices adjacent
    // to this vertex

    list<Edge*>::iterator i;
#pragma omp parallel for
    for (auto i : adj[v]) {
        #pragma omp critical
        if (!visited[i->getEnd()] && i->getState())
        {
            #pragma omp task
            DFS(i->getEnd(), m);
        }
    }
    //list<Edge>::iterator iter = std::find(adj[v].begin(), adj[v].end(), iter->getEnd()==m);
    #pragma omp critical
    if (visited[m])
    {
        //cout << "found" << "\n";
        return true;
    }
    else return false;

}

vector<Edge*> Graph::getEdges()
{
    return edges;
}

// Driver code
int main()
{
    Graph g;
    {
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 3);
    g.addEdge(3, 7);
    g.addEdge(4, 2);
    g.addEdge(4, 5);
    g.addEdge(4, 8);
    g.addEdge(5, 2);
    g.addEdge(5, 9);
    g.addEdge(5, 8);
    g.addEdge(5, 4);
    g.addEdge(5, 6);
    g.addEdge(6, 5);
    g.addEdge(6, 10);
    g.addEdge(6, 13);
    g.addEdge(6, 7);
    g.addEdge(7, 6);
    g.addEdge(7, 3);
    g.addEdge(7, 10);
    g.addEdge(7, 11);
    g.addEdge(7, 12);
    g.addEdge(7, 15);
    g.addEdge(8, 4);
    g.addEdge(8, 5);
    g.addEdge(8, 16);
    g.addEdge(9, 5);
    g.addEdge(9, 13);
    g.addEdge(10, 13);
    g.addEdge(10, 11);
    g.addEdge(10, 6);
    g.addEdge(10, 7);
    g.addEdge(11, 7);
    g.addEdge(11, 10);
    g.addEdge(12, 7);
    g.addEdge(12, 15);
    g.addEdge(13, 6);
    g.addEdge(13, 9);
    g.addEdge(13, 10);
    g.addEdge(13, 14);
    g.addEdge(13, 15);
    g.addEdge(13, 16);
    g.addEdge(14, 13);
    g.addEdge(14, 15);
    g.addEdge(15, 7);
    g.addEdge(15, 12);
    g.addEdge(15, 14);
    g.addEdge(15, 13);
    g.addEdge(15, 16);
}
    unsigned int var, sol;
    sol = 0;
    vector<Edge*> pointeredges = g.getEdges();

    for (var = 0; var < pointeredges.size(); var++) 
    {
        std::cout << pointeredges[var]->getState()<<" ";
    }
    std::cout << "\n";
    //bool i = g.DFS(0,3);
    //std::cout << "found path"<<i;
    double sum = 0;
    var = 0;
    bool message = true;
    omp_set_num_threads(4);
    cout << " max threads" << omp_get_max_threads();
    do {
        if (!pointeredges[var]->getState()) {
            
            pointeredges[var]->changeState(true);
            //
            //for (var = 0; var < pointeredges.size(); var++)
            //{
            //    std::cout << pointeredges[var]->getState() << " ";
            //    
            //    //sol++;
            //}
            g.clearVisited();
            
            if (g.DFS(1, 16))
            {
                sum += g.calculate_state();
            }
            if (message) { cout << " number of threads" << omp_get_num_threads() << "\n"; message = false; }
            //std::cout << sum << "\n";
            var = 0;
            if(var>pointeredges.size()/4) std::cout << "Half way there\n";
           // printf("%d", sol); printf("\n");
        }
        else { pointeredges[var]->changeState(false);var++; }
    } while (var < pointeredges.size());
    std::cout <<" final score is " << sum << "\n";
    return 0;
}

// improved by Vishnudev C
