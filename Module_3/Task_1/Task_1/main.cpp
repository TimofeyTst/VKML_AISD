#include <iostream>
#include <cassert>
#include <sstream>
#include <queue>
#include <vector>
#include <functional>

#include "i_graph.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"
#include "set_graph.hpp"
#include "arc_graph.hpp"


void BFS(const IGraph& graph, int vertex, std::function<void(const int)> visit)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> bfsQ;
    bfsQ.push(vertex);
    visited[vertex] = true;
    while (bfsQ.size() > 0) {
        int current = bfsQ.front();
        bfsQ.pop();
        visit(current);
        std::vector<int> adjList = graph.GetNextVertices(current);
        for (int i = 0; i < adjList.size(); ++i) {
            if (!visited[adjList[i]]) {
                bfsQ.push(adjList[i]);
                visited[adjList[i]] = true;
            }
        }
    }
}

int getLastWave(const IGraph& graph, int vertex) {
    int LastWave = -1;

    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> bfsQ;
    bfsQ.push(vertex);
    visited[vertex] = true;
    while (bfsQ.size() > 0) {
        int current = bfsQ.front();
        bfsQ.pop();
        LastWave++;
        std::vector<int> adjList = graph.GetNextVertices(current);
        for (int i = 0; i < adjList.size(); ++i) {
            if (!visited[adjList[i]]) {
                bfsQ.push(adjList[i]);
                visited[adjList[i]] = true;
            }
        }
    }
    return LastWave;
}

void run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;
    std::vector<IGraph*> graphs { new ListGraph(n) };
    int from = 0;
    int to = 0;
    for (size_t i = 0; i < n; i++)
    {
        input >> from >> to;
        graphs[0]->AddEdge(from, to);
    }
    int startAdj = 4;
    output << getLastWave(*graphs[0], startAdj) << std::endl;

    graphs.emplace_back(new MatrixGraph(*graphs.back()));
    graphs.emplace_back(new SetGraph(*graphs.back()));
    graphs.emplace_back(new ArcGraph( *graphs.back()));
    for (size_t i = 1; i < graphs.size(); i++)
    {
        output << getLastWave(*graphs[i], startAdj) << std::endl;
    }

    for (auto graph : graphs) { delete graph; }
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5\n1 2\n2 3\n3 4\n4 3\n4 1";
        run(input, output);
        assert(output.str() == "3\n3\n3\n3\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7\n1 2\n2 3\n3 4\n4 3\n4 1\n2 5\n5 6";
        run(input, output);
        assert(output.str() == "5\n5\n5\n5\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "8\n1 2\n2 3\n3 4\n4 3\n4 1\n2 5\n5 6\n6 7";
        run(input, output);
        assert(output.str() == "6\n6\n6\n6\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "9\n7 0\n1 2\n2 3\n3 4\n4 3\n4 1\n2 5\n5 6\n6 7";
        run(input, output);
        assert(output.str() == "7\n7\n7\n7\n");
    }
}

int main()
{
    test();
    //run(std::cin, std::cout);
    return 0;
}