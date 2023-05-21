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


int getShortestWays(const IGraph& graph, int from, int to) {
    int n = graph.VerticesCount();

    std::vector<int> distances(n, std::numeric_limits<int>::max());
    std::vector<int> routeCounts(n, 0);

    distances[from] = 0;
    routeCounts[from] = 1;

    std::queue<int> bfsQ;
    bfsQ.push(from);

    while (!bfsQ.empty()) {
        int currEdje = bfsQ.front();
        bfsQ.pop();

        // Получаем следующие вершины из текущей вершины
        std::vector<int> edjeList = graph.GetNextVertices(currEdje);

        for (int edje : edjeList) {
            // Если найдено более короткое расстояние до следующей вершины,
            // обновляем расстояние и счетчик путей
            if (distances[edje] > distances[currEdje] + 1) {
                distances[edje] = distances[currEdje] + 1;
                routeCounts[edje] = routeCounts[currEdje];
                bfsQ.push(edje);
            }
            // Если найден еще один кратчайший путь до следующей вершины,
            // увеличиваем счетчик путей
            else if (distances[edje] == distances[currEdje] + 1) {
                routeCounts[edje] += routeCounts[currEdje];
            }
        }
    }

    return routeCounts[to];
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
    }
    

    input >> from >> to;
    output << getShortestWays(*graph, from, to) << std::endl;
}

void test() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n5\n0 1\n0 2\n1 2\n1 3\n2 3\n\n0 3";
        run(input, output);
        assert(output.str() == "2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n6\n0 1\n0 2\n1 2\n1 3\n2 3\n0 3\n\n0 3";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5\n6\n0 1\n0 2\n1 2\n1 3\n2 3\n0 3\n\n0 4";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n5\n0 1\n1 2\n1 3\n3 0\n3 2\n\n0 3";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6\n9\n0 1\n0 4\n0 2\n1 4\n1 5\n4 5\n4 2\n4 3\n2 3\n\n0 3";
        run(input, output);
        assert(output.str() == "2\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "1\n0\n\n0 0";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2\n1\n0 1\n\n0 1";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2\n1\n1 0\n\n0 1";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2\n0 1\n1 2\n\n0 2";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2\n0 1\n0 2\n\n0 2";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "3\n2\n1 0\n2 0\n\n0 2";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n3\n0 1\n1 2\n2 3\n\n0 3";
        run(input, output);
        assert(output.str() == "1\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "4\n3\n0 1\n1 2\n3 2\n\n0 3";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "50000\n200000\n";
        for (int i = 0; i < 50000; ++i) {
            for (int j = 0; j < 4; ++j) {
                int next = (i + j + 1) % 49999;
                input << i << " " << next << "\n";
            }
        }
        input << "\n0 49999";
        run(input, output);
        assert(output.str() == "0\n");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6\n10\n0 1\n0 2\n0 3\n1 3\n1 4\n2 4\n2 5\n3 5\n4 5\n5 3\n\n0 5";
        run(input, output);
        assert(output.str() == "2\n");
    }
}

int main()
{
    test();
    run(std::cin, std::cout);
    return 0;
}