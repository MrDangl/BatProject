// CMakeProject1.cpp: определяет точку входа для приложения.
//


// C++ program to print DFS traversal from
// a given vertex in a  given graph

// omp_critical.cpp
// compile with: /openmp
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <bitset>
#include <omp.h>
#define MaxNvar  42
using namespace std;

// Graph class represents a directed graph
// using adjacency list representation
class Edge {
    unsigned int start, end;
    public:
    double probability = 0.9;
    bool state;
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

    Graph(const Graph& other);
    Graph();
    map<int, bool> visited;
    map<int, list<Edge> > adj;
    vector<int> nodes;
    vector<Edge> edges;

    // Function to add an edge to graph
    void clearVisited();
    void addEdge(int v, int w);
    void addEdge(int v, int w,bool state);
    double calculate_state();

    // DFS traversal of the vertices
    // reachable from v
    bool DFS(int v, int m);
    vector<Edge> getEdges();
};

class GState
{
    public :
        double probabil;
        Graph graph;
        std::string charState;
};

Graph::Graph(const Graph& other)
{
    this->visited = other.visited;
    this->adj = other.adj;
    this->nodes = other.nodes;
    this->edges = other.edges;
}
Graph::Graph()
{
    this->visited = map<int, bool>();
    this->adj = map<int, list<Edge>>();
    this->nodes = vector<int>();
    this->edges = vector<Edge>();

}

void Graph::clearVisited()
{
    visited.clear();
}

void Graph::addEdge(int v, int w)
{
    // Add w to v’s list.
    Edge edge = Edge(v, w);
    adj[v].push_back(edge);
    edges.push_back(edge);
}
void Graph::addEdge(int v, int w,bool state)
{
    // Add w to v’s list.
    Edge edge = Edge(v, w,state);
    adj[v].push_back(edge);
    edges.push_back(edge);
}


// Calculation of the probability of the state by multiplying probabilities of every edge
double Graph::calculate_state()
{
    double graphprobabil = 0;
//    #pragma omp parallel for reduction(+:graphprobabil)
    for (int i = 0; i<edges.size();i++) 
    {
        if (graphprobabil == 0)
        {
            if (edges[i].getState()) graphprobabil = edges[i].getProbability();
            else graphprobabil = 1 - edges[i].getProbability();
        }
        else 
        {
            if (edges[i].getState()) graphprobabil *= edges[i].getProbability();
            else graphprobabil *= 1 - edges[i].getProbability();
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

    list<Edge>::iterator i;
//#pragma omp parallel for
    for (auto i : adj[v]) {
//      #pragma omp critical
        if (!visited[i.getEnd()] && i.getState())
        {
//            #pragma omp task
            DFS(i.getEnd(), m);
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

vector<Edge> Graph::getEdges()
{
    return edges;
}



// Do while метод, работает на 2^n+1 дольше делает абсолютно одно и то же что и фор, работает одновременно только один
    // так как необходимо менять состояния на выключенные для каждого набора
    //Идет перебор по состояниям,код выполняется если у нас не существует дуги
    // начинаем с пустого состояния всех дуг, потом потом по одной включаем 
    // и проверяем каждое состояние через DFS есть ли в данном состоянии указаный узел
    // если узел есть считаем состояние как произведение вероятностей для каждой дуги в схеме , где 1-вер для выключенной
    // 1+вер для включенной, потом мы суммируем все полученные вероятности состояний и получаем оценку для всей сети
double calculateGraphReliabilityWhile(Graph n,int from, int to){
    
    double sum = 0;
    int var = 0;
    int numberOfIter = std::pow(2, n.getEdges().size());
    int sol = 0;
    auto pointeredges = n.getEdges();

    do {
        sol++;
        if (!pointeredges[var].getState()) {
            
            pointeredges[var].changeState(true);
            
            //Debug slow
            //for (var = 0; var < pointeredges.size(); var++)
            //{
            //    std::cout << pointeredges[var]->getState() << " ";
            //    
            //    //sol++;
            //}
            //if (var > pointeredges.size() / 2) std::cout << "Half way there\n";
            //std::cout << sum << "\n";

            Graph* temp = new Graph(n);
            
            temp->clearVisited();
            
            if (temp->DFS(from, to))
            {
                sum += temp->calculate_state();
            }
            var = 0;
            delete temp;
           // printf("%d", sol); printf("\n");
        }
        else { pointeredges[var].changeState(false);var++; }
    } while (var < pointeredges.size());
    return sum;
}


//TODO::OpenMP adapter
// var -иттератор указывающий на дугу,numbefOfIter-количество иттераций необходимых для алгоритма 
//Идет перебор по состояниям,код выполняется если у нас не существует дуги
// начинаем с выключенных состояний всех дуг, потом потом по одной включаем 
// и проверяем каждое состояние через DFS есть ли в данном состоянии указаный узел
// если узел есть считаем состояние как произведение вероятностей для каждой дуги в схеме , где 1-вер для выключенной
// 1+вер для включенной, потом мы суммируем все полученные вероятности состояний и получаем оценку для всей сети
double calculateGraphReliabilityFor(Graph n,int from, int to)
{
    double sum = 0;
    int var = 0;
    long numberOfIter = std::pow(2, n.getEdges().size());
    long sol = 0;
    auto pointeredges = n.getEdges();
    #pragma omp parallel
    {
            #pragma omp for reduction(+:sum)
            for (sol = 0; sol < numberOfIter; sol++)
            {
                
                if (var < pointeredges.size())
                {
                    if (pointeredges[var].getState()) {

                        while (pointeredges[var].getState() != false)
                        {
                            pointeredges[var].changeState(false);
                            if (var < pointeredges.size() - 1)
                            {
                               
                                var++;
                            }
                        }
                        
                    }
                    if (!pointeredges[var].getState())
                    {
                        pointeredges[var].changeState(true);
                        var = 0;
                    }

                }

                //Debug slow
                //if (var > pointeredges.size() / 2) std::cout << "Half way there\n";

                //for (int j = 0; j < pointeredges.size(); j++)
                //{
                //    std::cout << pointeredges[j]->getState() << " ";
                //    //sol++;
                //}
                //std::cout << sum << "\n";


                Graph* temp = new Graph(n);

                temp->clearVisited();
                //n.clearVisited();
                printf("iter number %d , thread num %d , val value %d ,\n",sol,omp_get_thread_num(), var);
                if (temp->DFS(from, to))
                {
                    #pragma omp atomic
                    sum += temp->calculate_state();
                }
                delete temp;
                //if (message) { cout << " number of threads" << omp_get_num_threads() << "\n"; message = false; }
            }
    }
    return sum;
}



std::vector<GState> calculateAllStates(Graph n)
{
    int graphsize = n.edges.size();
    long sizeOfIter = std::pow(2, graphsize);
    std::vector<GState> vectorState = std::vector<GState>();
//    #pragma omp parallel
    {
        
//        #pragma omp for
        for (long i = 0; i < sizeOfIter; i++)
        {
            GState state;
            Graph g(n);
            state.graph = g;
            string charbit;

            for (int j = graphsize - 1; j >= 0; j--) {
                int k = i >> j;
                if (k & 1)
                    charbit.push_back('1');
                else
                    charbit.push_back('0');
            }

            state.charState = charbit;
            double probState = 0;

            for (int j = 0; j < charbit.size(); j++)
            {
                if (charbit[j] == '1')
                {
                    state.graph.edges[j].state = true;
                }
            }


            for (std::vector<int>::iterator j = state.graph.nodes.begin(); j < std::prev(state.graph.nodes.end()); j++)
            {
                for (std::vector<int>::iterator k = j+1; k < state.graph.nodes.end(); k++)
                {
                    int start = *j;
                    int end = *k;
                    bool isPathExst = state.graph.DFS(start, end);
                    if (isPathExst)
                    {
                        probState += state.graph.calculate_state();
                    }
                }
            }
            double allit = state.graph.nodes.size() * (state.graph.nodes.size() - 1);
            probState = probState / allit;
            state.probabil = probState;
//            #pragma omp critical
            {
                vectorState.push_back(state);
            }
        }

    }
    return vectorState;

}

// Driver code
int main(int argc, char* argv[])
{
    // Plug graph here
    Graph g;
    {
        //example e
    g.addEdge(1, 2);
    g.addEdge(1, 3);

    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 2);
    g.addEdge(3, 4);
    g.nodes.push_back(1);
    g.nodes.push_back(2);
    g.nodes.push_back(3);
    g.nodes.push_back(4);
//
//    g.addEdge(2, 5);
//    g.addEdge(3, 7);
//    g.addEdge(3, 4);
//    g.addEdge(4, 3);
//    g.addEdge(4, 5);
//    g.addEdge(5, 2);
//    g.addEdge(5, 6);
//    g.addEdge(5, 8);
//    g.addEdge(5, 4);
//    g.addEdge(6, 5);
//    g.addEdge(6, 7);
//    g.addEdge(7, 6);
//    g.addEdge(7, 3);
//    g.addEdge(7, 8);
//    g.addEdge(7, 9);
//    g.addEdge(8, 5);
//    g.addEdge(8, 7);
//    g.addEdge(8, 9);
//
}

    std::cout << "Here we go ";

    int value = 32;
   

    std::vector<GState> allStates = calculateAllStates(g);
    // calculation are done in methods
    double sum = 0;
    for (auto state : allStates)
    {
        sum += state.probabil;
    }
    

    std::cout << " final score is " << sum << "\n";

    return 0;
}

// improved by Vishnudev C
