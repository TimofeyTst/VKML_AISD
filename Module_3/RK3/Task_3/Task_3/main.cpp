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
    int n = graph.VerticesCount();

    std::vector<int> minCost(n, std::numeric_limits<int>::max()); // Массив минимальных стоимостей пути до каждой вершины
    std::vector<int> flightCount(n, max_count + 1); // Массив количества перелетов до каждой вершины
    minCost[from] = 0; // Стоимость пути из начальной вершины равна 0

    // Очередь с приоритетом, в которой хранятся пары {стоимость, вершина}
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({ 0, from });

    while (!pq.empty()) {
        int cost = pq.top().first;
        int v = pq.top().second;
        pq.pop();

        // Если текущая вершина - целевая и количество перелетов меньше или равно max_count,
        // то возвращаем минимальную стоимость пути до целевой вершины
        if (v == to && flightCount[v] <= max_count) {
            return minCost[v];
        }

        // Если количество перелетов до текущей вершины равно 0, пропускаем ее
        if (flightCount[v] == 0) {
            continue;
        }

        for (const auto& edge : graph.GetNextVertices(v)) {
            int u = edge.first;
            int weight = edge.second;

            // Если количество перелетов до следующей вершины будет меньше, обновляем его
            int nextFlightCount = flightCount[v] - (v != from);
            if (nextFlightCount < flightCount[u]) {
                flightCount[u] = nextFlightCount;
            }

            // Вычисляем стоимость нового пути до следующей вершины
            int newCost = cost + weight;

            // Если новая стоимость пути меньше текущей минимальной стоимости пути до следующей вершины,
            // обновляем минимальную стоимость и добавляем вершину в очередь с приоритетом
            if (newCost < minCost[u] && flightCount[u] >= 0) {
                minCost[u] = newCost;
                pq.push({ newCost, u });
            }
        }
    }

    // Если не удалось достичь целевой вершины за указанное количество перелетов, возвращаем -1
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
    output << getShortestWay(*graph, aim_from-1, aim_to-1, max_count) << std::endl;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 7 2 4 1\n1 2 6\n5 1 1\n4 1 9\n4 5 3\n4 3 2\n2 5 7\n3 5 1\n";
        run(input, output);
        assert(output.str() == "10\n");
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
    //test();
    run(std::cin, std::cout);
    return 0;
}