#include <iostream>
#include <string>
#include "../src/ds/Graph.h"
using namespace std;

void check(bool condition, string name)
{
    if (condition) cout << "PASS: " << name << endl;
    else cout << "FAIL: " << name << endl;
}

bool listContains(GraphNode* head, int v)
{
    while (head != nullptr)
    {
        if (head->vertex == v) return true;
        head = head->next;
    }
    return false;
}

int main()
{
    Graph g;

    // 1 & 9. Adding vertices and vertex count
    check(g.vertexCount() == 0, "new Graph has 0 vertices");
    int a = g.addVertex();
    int b = g.addVertex();
    int c = g.addVertex();
    int d = g.addVertex();
    check(a == 0 && b == 1 && c == 2 && d == 3, "addVertex returns sequential indices");
    check(g.vertexCount() == 4, "vertexCount is 4 after 4 addVertex calls");
    int e = g.addVertex();
    check(e == 4 && g.vertexCount() == 5, "graph grows past initial capacity");

    // 7. Isolated vertices
    check(g.neighbors(4) == nullptr, "isolated vertex has no neighbours");
    check(g.neighborCount(4) == 0, "isolated vertex neighbourCount is 0");
    check(!g.hasEdge(4, 0), "isolated vertex has no edges");

    // 2 & 6. Adding and checking edges
    g.addEdge(0, 1);
    check(g.hasEdge(0, 1), "hasEdge(0,1) true after addEdge(0,1)");
    check(g.hasEdge(1, 0), "edge is undirected: hasEdge(1,0) true");
    check(!g.hasEdge(0, 2), "hasEdge false for missing edge");

    // 3. Duplicate edges
    g.addEdge(0, 1);
    g.addEdge(1, 0);
    check(g.neighborCount(0) == 1, "duplicate edge not added to u's list");
    check(g.neighborCount(1) == 1, "duplicate edge not added to v's list");

    // Self loop
    g.addEdge(2, 2);
    check(!g.hasEdge(2, 2), "self-loop is ignored");
    check(g.neighborCount(2) == 0, "self-loop adds no neighbour");

    // 10. Neighbour information
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    check(g.neighborCount(0) == 3, "vertex 0 has 3 neighbours");
    check(listContains(g.neighbors(0), 1), "neighbors(0) contains 1");
    check(listContains(g.neighbors(0), 2), "neighbors(0) contains 2");
    check(listContains(g.neighbors(0), 3), "neighbors(0) contains 3");
    check(!listContains(g.neighbors(0), 4), "neighbors(0) does not contain 4");
    check(listContains(g.neighbors(3), 0), "neighbors(3) contains 0 (both directions)");

    // 8. Invalid vertex indices
    g.addEdge(-1, 0);
    g.addEdge(0, 99);
    g.addEdge(99, 100);
    check(g.vertexCount() == 5, "invalid addEdge does not change vertexCount");
    check(g.neighborCount(0) == 3, "invalid addEdge does not change neighbours");
    check(!g.hasEdge(-1, 0), "hasEdge false for negative index");
    check(!g.hasEdge(0, 99), "hasEdge false for too-large index");
    check(g.neighbors(-1) == nullptr, "neighbors(-1) is nullptr");
    check(g.neighbors(99) == nullptr, "neighbors(99) is nullptr");
    check(g.neighborCount(99) == -1, "neighborCount(99) is -1");

    // 4. Removing existing edges
    g.removeEdge(0, 1);
    check(!g.hasEdge(0, 1) && !g.hasEdge(1, 0), "removeEdge removes both directions");
    check(g.neighborCount(0) == 2, "u's neighbourCount drops after remove");
    check(g.neighborCount(1) == 0, "v's neighbourCount drops after remove");
    check(g.hasEdge(0, 2) && g.hasEdge(0, 3), "other edges survive removeEdge");

    g.removeEdge(0, 2);
    check(!g.hasEdge(0, 2) && g.hasEdge(0, 3), "removing a middle/head node keeps the list intact");

    // 5. Removing missing edges and invalid removals
    g.removeEdge(0, 1);
    g.removeEdge(3, 4);
    g.removeEdge(-1, 5);
    g.removeEdge(0, 99);
    check(g.neighborCount(0) == 1, "removing missing edge changes nothing");
    check(g.hasEdge(0, 3), "removing missing edge keeps existing edges");
    check(g.vertexCount() == 5, "removeEdge never changes vertexCount");

    // Re-add after remove
    g.addEdge(0, 1);
    check(g.hasEdge(0, 1) && g.hasEdge(1, 0), "edge can be re-added after removal");

    // Stress test: many vertices and edges (forces multiple resizes)
    Graph big;
    for (int i = 0; i < 20; i++) big.addVertex();
    check(big.vertexCount() == 20, "20 vertices added");
    for (int i = 0; i < 19; i++) big.addEdge(i, i + 1);
    bool chainOk = true;
    for (int i = 0; i < 19; i++)
    {
        if (!big.hasEdge(i, i + 1) || !big.hasEdge(i + 1, i)) chainOk = false;
    }
    check(chainOk, "chain of 19 edges stored correctly after resizes");
    check(big.neighborCount(0) == 1 && big.neighborCount(10) == 2, "chain end/middle neighbour counts correct");

    return 0;
}