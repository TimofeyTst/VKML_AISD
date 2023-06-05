#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <queue>
#include <utility>
#include <memory>
#include <limits>

struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


class ListGraph : public IGraph {
public:
    ListGraph(int vertexCount);
    ListGraph(const IGraph& graph);

    // IGraph
    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjLists;
    std::vector<std::vector<int>> prevAdjLists;
};


ListGraph::ListGraph(int vertexCount)
{
    adjLists.resize(vertexCount);
    prevAdjLists.resize(vertexCount);
}

ListGraph::ListGraph(const IGraph& graph)
{
    adjLists.resize(graph.VerticesCount());
    prevAdjLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        adjLists[i] = graph.GetNextVertices(i);
        prevAdjLists[i] = graph.GetPrevVertices(i);
    }
}

void ListGraph::AddEdge(int from, int to)
{
    int vertexCount = VerticesCount();
    assert(from >= 0 && from < vertexCount);
    assert(to >= 0 && to < vertexCount);

    adjLists[from].push_back(to);
    prevAdjLists[to].push_back(from);
}

int ListGraph::VerticesCount() const
{
    return adjLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < adjLists.size());
    return prevAdjLists[vertex];
}


int isGraphEulerian(const IGraph& graph) {
    // Граф связный + все вершины имеют четную степень
    int vertexCount = graph.VerticesCount();

    // Проверка на связность графа
    std::vector<bool> visited(vertexCount, false);
    std::queue<int> bfsQ;

    visited[0] = true;
    bfsQ.push(0);

    while (!bfsQ.empty()) {
        int currentVertex = bfsQ.front();
        bfsQ.pop();

        std::vector<int> nextVertices = graph.GetNextVertices(currentVertex);
        for (int vertex : nextVertices) {
            if (!visited[vertex]) {
                visited[vertex] = true;
                bfsQ.push(vertex);
            }
        }
    }

    for (bool v : visited) {
        if (!v) {
            return 0;  // Посетили не все
        }
    }

    // Проверка степеней вершин
    for (int i = 0; i < vertexCount; ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        if (nextVertices.size() % 2 != 0) {
            return 0;  // Нечетная степень
        }
    }

    return 1;
}

void run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;
    auto graph = std::make_unique<ListGraph>(n);

    int adjCount = 0;
    input >> adjCount;
    int from = 0;
    int to = 0;
    for (size_t i = 0; i < adjCount; i++)
    {
        input >> from >> to;
        graph->AddEdge(from, to);
        graph->AddEdge(to, from);
    }

    output << isGraphEulerian(*graph) << std::endl;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n4\n0 1\n1 2\n2 3\n3 0";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n6\n0 1\n1 2\n2 3\n3 0\n0 2\n1 3";
        run(input, output);
        assert(output.str() == "0\n");
    }
}

int main()
{
    //test();
    run(std::cin, std::cout);
    return 0;
}