#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <memory>
#include <utility>
#include <queue>
#include <set>

struct IWeightedGraph {
    virtual ~IWeightedGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
};

class WeightedGraph : public IWeightedGraph {
public:
    WeightedGraph(int vertexCount)
    {
        adjList.resize(vertexCount);
    }

    void AddEdge(int from, int to, int weight) override {
        int vertexCount = VerticesCount();
        assert(from >= 0 && from < vertexCount);
        assert(to >= 0 && to < vertexCount);

        adjList[from].push_back({ to, weight });
        adjList[to].push_back({ from, weight });
    }

    int VerticesCount() const override {
        return static_cast<int>(adjList.size());
    }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override {
        return adjList[vertex];
    }

private:
    std::vector<std::vector<std::pair<int, int>>> adjList;
};

int getMinTree(const IWeightedGraph& graph) {
    int vertexCount = graph.VerticesCount();

    std::vector<bool> visited(vertexCount, false);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    std::set<int> includedVertices;

    // Начинаем с первого
    int startVertex = 0;
    visited[startVertex] = true;
    includedVertices.insert(startVertex);

    // Добавляем начальные вершины в очередь с приоритетом
    for (const auto& edge : graph.GetNextVertices(startVertex)) {
        int to = edge.first;
        int weight = edge.second;
        pq.push({ weight, to });
    }

    int minWeight = 0;

    while (!pq.empty()) {
        auto currentEdge = pq.top();
        pq.pop();
        int weight = currentEdge.first;
        int vertex = currentEdge.second;

        // Пропускаем если уже был
        if (includedVertices.count(vertex) > 0)
            continue;

        // Иначе добавляем
        includedVertices.insert(vertex);
        minWeight += weight;
        visited[vertex] = true;

        for (const auto& edge : graph.GetNextVertices(vertex)) {
            int to = edge.first;
            int weight = edge.second;
            if (!visited[to]) {
                pq.push({ weight, to });
            }
        }
    }

    return minWeight;
}

void run(std::istream& input, std::ostream& output) {
    int size = 0;
    int adjCount = 0;
    input >> size >> adjCount;
    auto graph = std::make_unique<WeightedGraph>(size);

    int from = 0;
    int to = 0;
    int weight = 0;
    for (size_t i = 0; i < adjCount; i++)
    {
        input >> from >> to >> weight;
        from -= 1;
        to -= 1;
        graph->AddEdge(from, to, weight);
    }

    output << getMinTree(*graph) << std::endl;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 10\n4 3 3046\n4 5 90110\n5 1 57786\n3 2 28280\n4 3 18010\n4 5 61367\n4 1 18811\n4 2 69898\n3 5 72518\n3 1 85838\n";
        run(input, output);
        assert(output.str() == "107923\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 4\n1 2 1\n2 3 2\n3 4 5\n4 1 4";
        run(input, output);
        assert(output.str() == "7\n");
    }
}

int main()
{
    //test();
    run(std::cin, std::cout);
    return 0;
}