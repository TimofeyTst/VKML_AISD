#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>

struct IWeightedGraph {
    virtual ~IWeightedGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int, std::pair<int, int>>> GetEdges() const = 0;
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

    // Возвращаем weight: { from, to }
    std::vector<std::pair<int, std::pair<int, int>>> GetEdges() const override {
        std::vector<std::pair<int, std::pair<int, int>>> edges;
        for (int from = 0; from < adjList.size(); ++from) {
            for (const auto& edge : adjList[from]) {
                int to = edge.first;
                int weight = edge.second;
                // Так как ребра хранятся в обе стороны
                if (from < to) {
                    edges.push_back({ weight, {from, to} });
                }
            }
        }
        return edges;
    }

private:
    std::vector<std::vector<std::pair<int, int>>> adjList;
};

class DSU {
public:
    DSU(int size) : parent(size) {
        for (int i = 0; i < size; ++i) {
            parent[i] = i;
        }
    }

    int FindParent(int v) {
        if (v == parent[v]) {
            return v;
        }
        return FindParent(parent[v]);
    }

    void Union(int v1, int v2) {
        int p1 = FindParent(v1);
        int p2 = FindParent(v2);
        if (p1 != p2) {
            parent[p2] = p1;
        }
    }

private:
    std::vector<int> parent;
};

int getMinTree(const IWeightedGraph& graph) {
    int vertexCount = graph.VerticesCount();
    std::vector<std::pair<int, std::pair<int, int>>> edges = graph.GetEdges();

    // Сортируем ребра по весу
    std::sort(edges.begin(), edges.end());

    DSU ds(vertexCount);
    int minSpanningTreeWeight = 0;

    // Проходим по отсортированным ребрам и добавляем их в остовное дерево, если они не создают цикл
    for (const auto& edge : edges) {
        int weight = edge.first;
        int from = edge.second.first;
        int to = edge.second.second;

        if (ds.FindParent(from) != ds.FindParent(to)) {
            ds.Union(from, to);
            minSpanningTreeWeight += weight;
        }
    }

    return minSpanningTreeWeight;
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
        input << "4 4\n1 2 1\n2 3 2\n3 4 5\n4 1 4";
        run(input, output);
        assert(output.str() == "7\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 10\n4 3 3046\n4 5 90110\n5 1 57786\n3 2 28280\n4 3 18010\n4 5 61367\n4 1 18811\n4 2 69898\n3 5 72518\n3 1 85838\n";
        run(input, output);
        assert(output.str() == "107923\n");
    }
}

int main()
{
    //test();
    run(std::cin, std::cout);
    return 0;
}