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
// TODO: rearrange cycles for OpenMP integration

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


// Calculation of the probability of the state by multiplying probabilities of every edge
double Graph::calculate_state()
{
    double graphprobabil = 0;
//    #pragma omp parallel for reduction(+:graphprobabil)
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



// recursive altered deep first search that sends bool in return found m
// TODO: redo function without recursive for open MP
bool Graph::DFS(int v, int m)
{
    // Mark the current node as visited and
    // print it
//#pragma omp critical
    visited[v] = true;
    //cout << v << " ";
    // Recur for all the vertices adjacent
    // to this vertex

    list<Edge*>::iterator i;
//#pragma omp parallel for
    for (auto i : adj[v]) {
//      #pragma omp critical
        if (!visited[i->getEnd()] && i->getState())
        {
//            #pragma omp task
            DFS(i->getEnd(), m);
        }
    }
    //list<Edge>::iterator iter = std::find(adj[v].begin(), adj[v].end(), iter->getEnd()==m);
//    #pragma omp critical
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
    // Plug graph here
    Graph g;
    {
        //example e
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    //g.addEdge(2, 5);
    //g.addEdge(3, 7);
    //g.addEdge(3, 4);
    //g.addEdge(4, 3);
    //g.addEdge(4, 5);
    //g.addEdge(5, 2);
    //g.addEdge(5, 6);
    //g.addEdge(5, 8);
    //g.addEdge(5, 4);
    //g.addEdge(6, 5);
    //g.addEdge(6, 7);
    //g.addEdge(7, 6);
    //g.addEdge(7, 3);
    //g.addEdge(7, 8);
    //g.addEdge(7, 9);
    //g.addEdge(8, 5);
    //g.addEdge(8, 7);
    //g.addEdge(8, 9);

}
    unsigned int var, sol;
    sol = 0;
    vector<Edge*> pointeredges = g.getEdges();

    for (var = 0; var < pointeredges.size(); var++) 
    {
        std::cout << pointeredges[var]->getState()<<" ";
    }
    std::cout << "\n";
    double sum = 0;
    var = 0;
    bool message = true;

    //TODO: OpenMP refactor
    //omp_set_num_threads(4);
    //cout << " max threads" << omp_get_max_threads();

    //Test
    int numberOfiter = powl(2, pointeredges.size());
    std::cout << "number of supposed iter" << numberOfiter <<"\n";
    for (int i = 0; i < numberOfiter; i++)
    {

        sol++;
        if (!pointeredges[var]->getState()) {
            
            pointeredges[var]->changeState(true);
            var = 0;
          // printf("%d", sol); printf("\n");
        }
        else { pointeredges[var]->changeState(false); var++; }
        //Debug slow


        //if (var > pointeredges.size() / 2) std::cout << "Half way there\n";
        
        for (int j = 0; j < pointeredges.size(); j++)
        {
            std::cout << pointeredges[j]->getState() << " ";
            //sol++;
        }
        std::cout << sum << "\n";

        g.clearVisited();

        if (g.DFS(1, 4))
        {
            sum += g.calculate_state();
        }
        //if (message) { cout << " number of threads" << omp_get_num_threads() << "\n"; message = false; }

        

    }
    std::cout << " final score is " << sum <<"Itteration number "<< sol <<" How much should be " <<pow(2,pointeredges.size()) << "\n";

    //

    //Идет перебор по состояниям,код выполняется если у нас не существует дуги
    // начинаем с пустого состояния всех дуг, потом потом по одной включаем 
    // и проверяем каждое состояние через DFS есть ли в данном состоянии указаный узел
    // если узел есть считаем состояние как произведение вероятностей для каждой дуги в схеме , где 1-вер для выключенной
    // 1+вер для включенной, потом мы суммируем все полученные вероятности состояний и получаем оценку для всей сети
    /*
    do {
        if (!pointeredges[var]->getState()) {
            
            pointeredges[var]->changeState(true);
            
            ////Debug slow
            //for (var = 0; var < pointeredges.size(); var++)
            //{
            //    std::cout << pointeredges[var]->getState() << " ";
            //    
            //    //sol++;
            //}
            ////if (var > pointeredges.size() / 2) std::cout << "Half way there\n";
            //std::cout << sum << "\n";

            g.clearVisited();
            
            if (g.DFS(1, 9))
            {
                sum += g.calculate_state();
            }
            if (message) { cout << " number of threads" << omp_get_num_threads() << "\n"; message = false; }

            var = 0;
            
           // printf("%d", sol); printf("\n");
        }
        else { pointeredges[var]->changeState(false);var++; }
    } while (var < pointeredges.size());
    std::cout <<" final score is " << sum << "\n";
   */ 
    return 0;
}

// improved by Vishnudev C
