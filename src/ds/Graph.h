#pragma once

class GraphNode
{
public:
    int vertex;
    GraphNode* next;

    GraphNode(int v)
    {
        vertex = v;
        next = nullptr;
    }
};

class Graph
{
public:
    GraphNode** adj;
    int vertices;
    int capacity;

    Graph()
    {
        vertices = 0;
        capacity = 4;
        adj = new GraphNode*[capacity];
        for (int i = 0; i < capacity; i++)
        {
            adj[i] = nullptr;
        }
    }

    ~Graph()
    {
        for (int i = 0; i < vertices; i++)
        {
            GraphNode* current = adj[i];
            while (current != nullptr)
            {
                GraphNode* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] adj;
    }

    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    int addVertex()
    {
        if (vertices == capacity)
        {
            int newCapacity = capacity * 2;
            GraphNode** newAdj = new GraphNode*[newCapacity];

            for (int i = 0; i < vertices; i++)
            {
                newAdj[i] = adj[i];
            }
            for (int i = vertices; i < newCapacity; i++)
            {
                newAdj[i] = nullptr;
            }

            delete[] adj;
            adj = newAdj;
            capacity = newCapacity;
        }

        adj[vertices] = nullptr;
        vertices++;
        return vertices - 1;
    }

    bool hasEdge(int u, int v) const
    {
        if (u < 0 || u >= vertices || v < 0 || v >= vertices)
        {
            return false;
        }

        GraphNode* current = adj[u];
        while (current != nullptr)
        {
            if (current->vertex == v)
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void addEdge(int u, int v)
    {
        if (u < 0 || u >= vertices || v < 0 || v >= vertices)
        {
            return;
        }
        if (u == v)
        {
            return;
        }
        if (hasEdge(u, v))
        {
            return;
        }

        GraphNode* nodeForU = new GraphNode(v);
        nodeForU->next = adj[u];
        adj[u] = nodeForU;

        GraphNode* nodeForV = new GraphNode(u);
        nodeForV->next = adj[v];
        adj[v] = nodeForV;
    }

    void removeEdge(int u, int v)
    {
        if (u < 0 || u >= vertices || v < 0 || v >= vertices)
        {
            return;
        }
        if (!hasEdge(u, v))
        {
            return;
        }

        removeFromList(u, v);
        removeFromList(v, u);
    }

    int vertexCount() const
    {
        return vertices;
    }

    // Returns the head of u's neighbour list (walk it with ->next).
    // Returns nullptr if u is invalid or has no neighbours.
    GraphNode* neighbors(int u) const
    {
        if (u < 0 || u >= vertices)
        {
            return nullptr;
        }
        return adj[u];
    }

    // Returns how many neighbours u has, or -1 if u is invalid.
    int neighborCount(int u) const
    {
        if (u < 0 || u >= vertices)
        {
            return -1;
        }

        int count = 0;
        GraphNode* current = adj[u];
        while (current != nullptr)
        {
            count++;
            current = current->next;
        }
        return count;
    }

private:
    // Removes v from u's list only (one direction). Returns true if found.
    bool removeFromList(int u, int v)
    {
        GraphNode* current = adj[u];
        GraphNode* previous = nullptr;

        while (current != nullptr)
        {
            if (current->vertex == v)
            {
                if (previous == nullptr)
                {
                    adj[u] = current->next;
                }
                else
                {
                    previous->next = current->next;
                }
                delete current;
                return true;
            }
            previous = current;
            current = current->next;
        }
        return false;
    }
};