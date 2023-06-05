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


int isGraphTree(const IGraph& graph) {
    // Граф не содержит циклов и мы посетили все вершины
    int vertexCount = graph.VerticesCount();

    // Проверка на посещение всех
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

    // Проверка наличия циклов
    std::vector<int> parent(vertexCount, -1);

    for (int i = 0; i < vertexCount; ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int vertex : nextVertices) {
            if (parent[i] != vertex && parent[vertex] != i) {
                if (parent[vertex] != -1) {
                    return 0; // Наблюдаем цикл
                }
                parent[vertex] = i;
            }
        }
    }

    return 1;  // Граф является деревом
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

    output << isGraphTree(*graph) << std::endl;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2\n0 1\n0 2";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n2\n0 1\n0 2";
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