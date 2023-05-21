#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <utility>
#include <limits>
#include <queue>
#include <memory>

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

int getShortestWay(const IWeightedGraph& graph, int from, int to) {
    int vertexCount = graph.VerticesCount();


    std::vector<int> distances(vertexCount, std::numeric_limits<int>::max());
    // Очередь с приоритетом для хранения пар (расстояние, вершина)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    distances[from] = 0;
    pq.push({ 0, from });

    while (!pq.empty()) {
        int currentVertex = pq.top().second;
        pq.pop();

        // Если мы достигли конечной вершины, то возвращаем расстояние до нее
        if (currentVertex == to) {
            return distances[to];
        }

        // Проходим по всем соседним вершинам и обновляем расстояния до них
        for (const auto & edge : graph.GetNextVertices(currentVertex)) {
            int nextVertex = edge.first;
            int weight = edge.second;

            if (distances[currentVertex] + weight < distances[nextVertex]) {
                distances[nextVertex] = distances[currentVertex] + weight;
                pq.push({ distances[nextVertex], nextVertex });
            }
        }
    }

    // Если мы не достигли конечной вершины, то возвращаем -1
    return -1;
}

void run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;
    auto graph = std::make_unique<WeightedGraph>(n);

    int adjCount = 0;
    input >> adjCount;
    int from = 0;
    int to = 0;
    int weight = 0;
    for (size_t i = 0; i < adjCount; i++)
    {
        input >> from >> to >> weight;
        graph->AddEdge(from, to, weight);
    }


    input >> from >> to;
    output << getShortestWay(*graph, from, to) << std::endl;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "6\n9\n0 3 1\n0 4 2\n1 2 7\n1 3 2\n1 4 3\n1 5 3\n2 5 3\n3 4 4\n3 5 6\n0 2";
        run(input, output);
        assert(output.str() == "9\n");
    }
}

int main()
{
    //test();
    run(std::cin, std::cout);
    return 0;
}