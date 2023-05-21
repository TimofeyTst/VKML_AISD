#include "arc_graph.hpp"
#include "i_graph.hpp"
#include <vector>

ArcGraph::ArcGraph(int vertexCount)
{
    // Граф пуст, пока в нем нет ребер
    edges.reserve(0);
}

ArcGraph::ArcGraph(const IGraph& graph)
{
    int vertexCount = graph.VerticesCount();
    edges.reserve(vertexCount);

    for (int i = 0; i < vertexCount; ++i) {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int j : nextVertices) {
            edges.emplace_back(i, j);
        }
    }
}

void ArcGraph::AddEdge(int from, int to)
{
    edges.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const
{
    int maxVertex = -1;
    for (const auto& edge : edges) {
        maxVertex = std::max(maxVertex, std::max(edge.first, edge.second));
    }
    return maxVertex + 1;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    std::vector<int> nextVertices;
    for (const auto& edge : edges) {
        if (edge.first == vertex) {
            nextVertices.push_back(edge.second);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
    std::vector<int> prevVertices;
    for (const auto& edge : edges) {
        if (edge.second == vertex) {
            prevVertices.push_back(edge.first);
        }
    }
    return prevVertices;
}