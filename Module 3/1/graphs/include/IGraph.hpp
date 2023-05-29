#pragma once
#include <vector>
#include <cassert>
#include <climits>
#include <iostream>

#define assertm(exp, msg) assert(((void)msg, exp))

struct IGraph {
    virtual ~IGraph() {}
	
	// Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;

    void PrintEdges(std::istream& input_stream = std::cin, std::ostream& output_stream = std::cout) {
        for (int from = 0; from < VerticesCount(); from++) {
            auto connected = GetNextVertices(from);
            for (int to: connected) {
                output_stream << "{" << from << ", " << to << "}" << std::endl;
            }
        }
    }
};