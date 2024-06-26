﻿// CMakeProject1.cpp: определяет точку входа для приложения.
//


// C++ program to print DFS traversal from
// a given vertex in a  given graph

// omp_critical.cpp
// compile with: /openmp
#include <iostream>
#include <list>
#include <map>
#include <cmath>
#include <vector>
#include <bitset>
#include <omp.h>
#include "lib/cmd_line/src/cmd_line.hpp"
#include "lib/pugiXML/src/pugixml.hpp"

#define MaxNvar  42
using namespace std;

// Graph class represents a directed graph
// using adjacency list representation
class Edge {
    int start, end;
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
    }Edge(int nodeStart, int nodeEnd,double probabil)
    {
        start = nodeStart; end = nodeEnd; probability= probabil;
    }
};
// TODO: rearrange cycles for OpenMP integration

class Graph {
public:
    map<int, bool> visited;
    map<int, list<Edge*> > adj;
    vector<int> nodes;
    vector<Edge*> edges;

    Graph& operator=(const Graph& rhs) 
    {
        visited = rhs.visited;
        adj = rhs.adj;
        nodes = rhs.nodes;
        edges = rhs.edges;
    };


    double extProbabil;
    // Function to add an edge to graph
    void clearVisited();
    void addEdge(int v, int w);
    void addEdge(int v, int w,bool state);
    void addEdge(Edge& edge);
    double calculate_state();
    //double calculate_state(double probabil);
   
    void setExternalProbabil(double probabil);

    double getExternalProbabil();
    
    Graph(string graphMlPath);
    Graph();
    Graph* clone();

    // DFS traversal of the vertices
    // reachable from v
    bool DFS(int v, int m);
    vector<Edge*> getEdges();
    ~Graph();
};

class GState
{
    public :
        double probabil;
        std::string charState;
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
    nodes = vector<int>();
    edges.clear();
    auto c = graph.first_child();
    for(pugi::xml_node elem : graph)
    {
        string str = elem.name();
        if (str == "node")
        {
            nodes.push_back(elem.attribute("id").as_int());
        }
        if (str == "edge")
        {
            if (elem.append_attribute("prblty"))
            {
                addEdge(elem.attribute("source").as_int(), elem.attribute("target").as_int(),elem.attribute("prblty").as_double());
            
                if (elem.attribute("directed"))
                {
                    addEdge(elem.attribute("target").as_int(), elem.attribute("source").as_int(), elem.attribute("prblty").as_double());
                }
  
            }
            else
            {
                addEdge(elem.attribute("source").as_int(), elem.attribute("target").as_int(), graph.attribute("prbltydefault").as_double());

                if (elem.attribute("directed"))
                {
                    addEdge(elem.attribute("target").as_int(), elem.attribute("source").as_int(), graph.attribute("prbltydefault").as_double());
                }

            }
        }
    }
}

Graph::Graph()
{
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

void Graph::addEdge(Edge& edge)
{
    Edge* newedge = new Edge(edge);
    adj[newedge->getStart()].push_back(newedge);
    edges.push_back(newedge);
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

Graph* Graph::clone()
{
    Graph* clone = new Graph();
    for (int i = 0; i < edges.size(); i++)
        clone->addEdge (*edges[i]);
    clone->nodes = nodes;
    return clone;
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

Graph::~Graph()
{
    visited.clear();
    nodes.clear();
    for(auto edge : edges)
    {
        delete edge;
    }
    edges.clear();
    adj.clear();
}



// Do while метод, работает на 2^n+1 дольше делает абсолютно одно и то же что и фор, работает одновременно только один
    // так как необходимо менять состояния на выключенные для каждого набора
    //Идет перебор по состояниям,код выполняется если у нас не существует дуги
    // начинаем с пустого состояния всех дуг, потом потом по одной включаем 
    // и проверяем каждое состояние через DFS есть ли в данном состоянии указаный узел
    // если узел есть считаем состояние как произведение вероятностей для каждой дуги в схеме , где 1-вер для выключенной
    // 1+вер для включенной, потом мы суммируем все полученные вероятности состояний и получаем оценку для всей сети
double calculateGraphReliabilityWhile(Graph n, int from, int to){
    
    double sum = 0;
    int var = 0;
    int numberOfIter = std::pow(2, n.getEdges().size());
    int sol = 0;
    auto pointeredges = n.getEdges();

    do {
        sol++;
        if (!pointeredges[var]->getState()) {
            
            pointeredges[var]->changeState(true);
            
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
double calculateGraphReliabilityFor(Graph n, int from, int to)
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
                if (pointeredges[var]->getState()) {

                    while (pointeredges[var]->getState() != false)
                    {
                        pointeredges[var]->changeState(false);
                        if (var < pointeredges.size() - 1)
                        {

                            var++;
                        }
                    }

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


            Graph* temp = new Graph(n);

            if (n.DFS(from, to))
            {
                //sum += n.calculate_state();
                sum += n.calculate_state();
            }
            //if (message) { cout << " number of threads" << omp_get_num_threads() << "\n"; message = false; }
        }
    }
}



std::vector<GState> calculateAllStates(Graph n)
{
    int graphsize = n.edges.size();
    long sizeOfIter = std::pow(2, graphsize);
    std::vector<GState> vectorState;
    #pragma omp parallel
    {
        
        #pragma omp for
        for (long i = 0; i < sizeOfIter; i++)
        {
            GState state;
            Graph* graph = n.clone();
            graph->clearVisited();
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
                    graph->edges[j]->changeState(true);
                }
            }


            for (std::vector<int>::iterator j = graph->nodes.begin(); j < graph->nodes.end(); j++)
            {
                for (std::vector<int>::iterator k = graph->nodes.begin(); k < graph->nodes.end(); k++)
                {
                    bool isPathExst = graph->DFS(*j, *k);
                    if (isPathExst && j!=k)
                    {
                        probState += graph->calculate_state();
                    }
                }
            }
            double allit = graph->nodes.size() * (graph->nodes.size() - 1);
            probState = probState / allit;
            state.probabil = probState;
            #pragma omp critical
            {
                vectorState.push_back(state);
            }
            delete graph;
        }

    }
    return vectorState;

}

// Driver code
int main(int argc, char** argv)
{
    // Plug graph here
    double probabil = 0;
    std::string pathml;
    std::string start;
    std::string finish;
    std::string temp;
    Command_line args;
    args.add_argument({ "-pathMl" }, &pathml, "Path to GrapthMl.", false);
    args.add_argument({ "-probability" }, &probabil, "Probability of edge reliability",false);
    args.add_argument({ "-start" }, &start, "Start node",false);
    args.add_argument({ "-finish" }, &finish, "finish node",false);

    args.parse(argc, argv);
    // calculation are done in methods
    
    std::vector<GState> gstatevec;
    pathml = "gas2003.graphml";
    Graph e(pathml);
    if (probabil != 0) 
    {
        e.setExternalProbabil(probabil);
    }
    if (!start.empty() && !finish.empty())
    {
        probabil = calculateGraphReliabilityFor(e, atoi(start.c_str()), atoi(finish.c_str()));
    }
    else
    {
        gstatevec = calculateAllStates(e);
    }

    return 0;
}

