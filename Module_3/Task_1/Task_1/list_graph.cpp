#include "list_graph.hpp"
#include "i_graph.hpp"
#include <cassert>
#include <vector>

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