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


int getShortestWay(const IWeightedGraph& graph, int from, int to, int max_count) {
    int vertexCount = graph.VerticesCount();


    std::vector<int> distances(vertexCount, std::numeric_limits<int>::max());
    // Очередь с приоритетом для хранения пар (расстояние, вершина)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    distances[from] = 0;
    pq.push({ 0, from });
    int current_count = 0;

    while (!pq.empty()) {
        int currentVertex = pq.top().second;
        pq.pop();
        current_count += 1;

        // Если мы достигли конечной вершины, то возвращаем расстояние до нее
        if (currentVertex == to) {
            return current_count + 1;
        }

        // Проходим по всем соседним вершинам и обновляем расстояния до них
        for (const auto& edge : graph.GetNextVertices(currentVertex)) {
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
    int aim_from = 0;
    int aim_to = 0;
    int max_count = 0;
    input >> aim_from >> aim_to >> max_count;

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

    input >> from >> to;
    output << getShortestWay(*graph, from, to, max_count) << std::endl;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 7 2 4 1\n1 2 6\n5 1 1\n4 1 9\n4 5 3\n4 3 2\n2 5 7\n3 5 1\n";
        run(input, output);
        assert(output.str() == "4\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 3 1 1 3\n1 2 4\n2 3 5\n3 1 6\n";
        run(input, output);
        assert(output.str() == "-1\n");
    }
}

int main()
{
    test();
    run(std::cin, std::cout);
    return 0;
}