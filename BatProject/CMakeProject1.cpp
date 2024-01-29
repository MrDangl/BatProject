// CMakeProject1.cpp: определяет точку входа для приложения.
//


// C++ program to print DFS traversal from
// a given vertex in a  given graph
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <omp.h>
#include "lib/pugiXML/src/pugixml.hpp"

#define MaxNvar  42
using namespace std;

// Graph class represents a directed graph
// using adjacency list representation
class Edge {
    string start, end;
    double probability = 0.9;
    bool state;
    public:
    string getStart()
    {
        return start;
    }
    string getEnd()
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
    Edge(string nodeStart, string nodeEnd)
    {
        start = nodeStart; end = nodeEnd; state = false;
    }
    Edge(string nodeStart, string nodeEnd,bool statebool)
    {
        start = nodeStart; end = nodeEnd; state = statebool;
    }
};
// TODO: rearrange cycles for OpenMP integration

class Graph {
public:
    map<string, bool> visited;
    map<string, list<Edge*> > adj;
    vector<string> nodes;
    vector<Edge*> edges;
    double extProbabil;
    // Function to add an edge to graph
    void clearVisited();
    void addEdge(string v, string w);
    void addEdge(string v, string w,bool state);
    double calculate_state();
    double calculate_state(double probabil);
   
    void setExternalProbabil(double probabil);

    double getExternalProbabil();
    
    Graph(string graphMlPath);
    Graph();

    // DFS traversal of the vertices
    // reachable from v
    bool DFS(string v, string m);
    vector<Edge*> getEdges();
};
void Graph::clearVisited()
{
    visited.clear();
}

Graph::Graph(string graphMlPath)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(graphMlPath.c_str());
    pugi::xml_node graph = doc.child("graphml").child("graph");
    nodes = vector<string>();
    edges.clear();
    auto c = graph.first_child();
    for(pugi::xml_node elem : graph)
    {
        string str = elem.name();
        if (str == "node")
        {
            nodes.push_back(elem.attribute("id").as_string());
        }
        if (str == "edge")
        {
            addEdge(elem.attribute("source").as_string(), elem.attribute("target").as_string());
            
            if (elem.attribute("directed"))
            {
            addEdge(elem.attribute("target").as_string(), elem.attribute("source").as_string());
            }
        }
    }
}

Graph::Graph()
{
}

void Graph::addEdge(string v, string w)
{
    // Add w to v’s list.
    Edge *edge = new Edge(v, w);
    adj[v].push_back(edge);
    edges.push_back(edge);
}
void Graph::addEdge(string v, string w,bool state)
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

double Graph::calculate_state(double probabil)
{
    double graphprobabil = 0;
    //    #pragma omp parallel for reduction(+:graphprobabil)
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i]->getState()) graphprobabil *= probabil;
        else graphprobabil *= 1 - probabil;
        if (graphprobabil == 0)
        {
            if (edges[i]->getState()) graphprobabil = probabil;
            else graphprobabil = 1 - probabil;
        }
    }
    //std::cout << graphprobabil<<"\n";
    return graphprobabil;
}

void Graph::setExternalProbabil(double probabil)
{
    extProbabil = probabil;
}

double Graph::getExternalProbabil()
{
    return extProbabil;
}

// recursive altered deep first search that sends bool in return found m
// TODO: redo function without recursive for open MP
bool Graph::DFS(string v, string m)
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



// Do while метод, работает на 2^n+1 дольше делает абсолютно одно и то же что и фор, работает одновременно только один
    // так как необходимо менять состояния на выключенные для каждого набора
    //Идет перебор по состояниям,код выполняется если у нас не существует дуги
    // начинаем с пустого состояния всех дуг, потом потом по одной включаем 
    // и проверяем каждое состояние через DFS есть ли в данном состоянии указаный узел
    // если узел есть считаем состояние как произведение вероятностей для каждой дуги в схеме , где 1-вер для выключенной
    // 1+вер для включенной, потом мы суммируем все полученные вероятности состояний и получаем оценку для всей сети
double calculateGraphReliabilityWhile(Graph n, string from, string to){
    
    double sum = 0;
    int var = 0;
    int numberOfIter = std::pow(2, n.getEdges().size());
    int sol = 0;
    auto pointeredges = n.getEdges();

    do {
        sol++;
        if (!pointeredges[var]->getState()) {
            
            pointeredges[var]->changeState(true);
            
            /*Debug slow*/
            for (var = 0; var < pointeredges.size(); var++)
            {
                std::cout << pointeredges[var]->getState() << " ";
                
                //sol++;
            }
            if (var > pointeredges.size() / 2) std::cout << "Half way there\n";
            std::cout << sum << "\n";

            n.clearVisited();
            
            if (n.DFS(from, to))
            {
                sum += n.calculate_state();
                //sum += n.calculate_state(n.getExternalProbabil());
            }
            var = 0;
           // printf("%d", sol); printf("\n");
        }
        else { pointeredges[var]->changeState(false);var++; }
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
double calculateGraphReliabilityFor(Graph n, string from, string to)
{
    double sum = 0;
    int var = 0;
    int numberOfIter = std::pow(2, n.getEdges().size());
    int sol = 0;
    auto pointeredges = n.getEdges();
    for (int i = 0; i < numberOfIter - 1; i++)
    {
        sol++;
        if (var < pointeredges.size())
        {
            if (pointeredges[var]->getState()) {

                while (pointeredges[var]->getState() != false)
                {
                    pointeredges[var]->changeState(false);
                    if (var < pointeredges.size() - 1) var++;
                }
                // printf("%d", sol); printf("\n");
            }
            if (!pointeredges[var]->getState())
            {
                pointeredges[var]->changeState(true);
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

        n.clearVisited();

        if (n.DFS(from, to))
        {
            //sum += n.calculate_state();
            sum += n.calculate_state(n.getExternalProbabil());
        }
        //if (message) { cout << " number of threads" << omp_get_num_threads() << "\n"; message = false; }
    }

    return sum;
}

// Driver code
int main()
{
    // Plug graph here
    Graph g("gas2003.graphml");
    double probabil;
    string pathml;
    string start;
    string finish;
    string temp;
    
    // calculation are done in methods
    std::cout << "Type path to graphml file \n";
    std::cin >> pathml;
    Graph e(pathml);
    std::cout << "\n Type probabil for edges \n";
    std::cin >> probabil;
    std::cout << "\n  + Type Begining node \n";
    std::cin >> start;
    std::cout << "\n Type Finish node \n";
    std::cin >> finish;
    double sum = calculateGraphReliabilityFor(e, start, finish);

    std::cout << " final score is " << sum << "\n";
    return 0;
}

// improved by Vishnudev C
