#include <iostream>
#include <string>

using namespace std;

// Fixed array bounds to keep memory allocation simple and static
const int MAX_STATIONS = 100;
const int MAX_EDGES = 200;
const int ALPHABET_SIZE = 26;

// -------------------------------------------------------------
// 1. TRIE NODE STRUCTURE
// -------------------------------------------------------------
struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEnd;
    string name;

    // Constructor initializes everything manually
    TrieNode() {
        isEnd = false;
        name = "";
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};

// -------------------------------------------------------------
// 2. STATION TRIE CLASS (Prefix Search)
// -------------------------------------------------------------
class StationTrie {
private:
    TrieNode* root;

    // Helper function to print matching stations
    void collectStations(TrieNode* current) const {
        if (current == nullptr) return;

        // If this node marks the end of a station name, print it
        if (current->isEnd) {
            cout << " - " << current->name << "\n";
        }

        // Recursively check all 26 possible children
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current->children[i] != nullptr) {
                collectStations(current->children[i]);
            }
        }
    }

public:
    StationTrie() {
        root = new TrieNode();
    }

    // Insert station name into Trie
    void insert(string s) {
        TrieNode* current = root;

        for (int i = 0; i < s.length(); i++) {
            char ch = s[i];
            
            // Convert uppercase letters to lowercase manually
            if (ch >= 'A' && ch <= 'Z') {
                ch = ch + 32;
            }

            // Ignore non-alphabetic characters
            if (ch < 'a' || ch > 'z') continue;

            int index = ch - 'a';

            // Create new node if path does not exist
            if (current->children[index] == nullptr) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }

        current->isEnd = true;
        current->name = s; // Save full station name
    }

    // Search for all stations that start with 'pre'
    void autoSearch(string pre) const {
        TrieNode* current = root;

        for (int i = 0; i < pre.length(); i++) {
            char ch = pre[i];
            
            // Convert uppercase letters to lowercase manually
            if (ch >= 'A' && ch <= 'Z') {
                ch = ch + 32;
            }

            if (ch < 'a' || ch > 'z') continue;

            int index = ch - 'a';

            if (current->children[index] == nullptr) {
                cout << "No station found matching prefix: \"" << pre << "\"\n";
                return;
            }
            current = current->children[index];
        }

        cout << "Stations matching prefix \"" << pre << "\":\n";
        collectStations(current);
    }
};

// -------------------------------------------------------------
// 3. ROUTE EDGE STRUCT
// -------------------------------------------------------------
struct RouteEdge {
    int u; // Station ID 1
    int v; // Station ID 2
    int w; // Cost / Distance (Weight)
};

// -------------------------------------------------------------
// 4. METRO NETWORK CLASS (Kruskal's MST + Trie Integration)
// -------------------------------------------------------------
class MetroNetwork {
private:
    string stations[MAX_STATIONS];
    RouteEdge edges[MAX_EDGES];
    StationTrie trie;

    int totalStations;
    int totalEdges;

    // Helper: Returns existing ID or assigns a new ID to a station
    int getOrAddStationId(string name) {
        for (int i = 0; i < totalStations; i++) {
            if (stations[i] == name) {
                return i; // Station already exists
            }
        }
        
        // Add new station
        stations[totalStations] = name;
        trie.insert(name); // Add to Trie as well
        totalStations++;
        return totalStations - 1;
    }

    // Disjoint Set Union (DSU) - Basic Find Root function
    int findRoot(int parent[], int nodeIndex) {
        while (parent[nodeIndex] != nodeIndex) {
            nodeIndex = parent[nodeIndex];
        }
        return nodeIndex;
    }

    // Simple Bubble Sort to order edges by weight (smallest to largest)
    void sortEdges() {
        for (int i = 0; i < totalEdges - 1; i++) {
            for (int j = 0; j < totalEdges - i - 1; j++) {
                if (edges[j].w > edges[j + 1].w) {
                    // Manual Swap without std::swap
                    RouteEdge temp = edges[j];
                    edges[j] = edges[j + 1];
                    edges[j + 1] = temp;
                }
            }
        }
    }

public:
    MetroNetwork() {
        totalStations = 0;
        totalEdges = 0;
    }

    void addRoute(string stationA, string stationB, int distance) {
        if (totalEdges >= MAX_EDGES) {
            cout << "Cannot add more routes, maximum limit reached!\n";
            return;
        }

        int u = getOrAddStationId(stationA);
        int v = getOrAddStationId(stationB);

        edges[totalEdges].u = u;
        edges[totalEdges].v = v;
        edges[totalEdges].w = distance;
        totalEdges++;
    }

    void search(string pre) const {
        trie.autoSearch(pre);
    }

    // Kruskal's Algorithm
    void computeMST() {
        if (totalStations == 0) {
            cout << "No stations in network!\n";
            return;
        }

        // Step 1: Sort all edges manually by distance
        sortEdges();

        // Step 2: Initialize DSU parent array (everyone is their own parent initially)
        int parent[MAX_STATIONS];
        for (int i = 0; i < totalStations; i++) {
            parent[i] = i;
        }

        cout << "\n============================================\n";
        cout << " Optimal Metro Tracks (Kruskal's Algorithm)\n";
        cout << "============================================\n";

        int totalWeight = 0;
        int edgesSelected = 0;

        // Step 3: Loop through sorted edges and select valid ones
        for (int i = 0; i < totalEdges; i++) {
            int rootU = findRoot(parent, edges[i].u);
            int rootV = findRoot(parent, edges[i].v);

            // Cycle Check: If roots are different, no cycle is formed
            if (rootU != rootV) {
                cout << "Build Track: " << stations[edges[i].u] 
                     << " <---> " << stations[edges[i].v] 
                     << " | Cost: " << edges[i].w << " km\n";

                totalWeight += edges[i].w;
                parent[rootU] = rootV; // Union (Merge sets)
                edgesSelected++;

                // Stop if we have selected (V - 1) edges
                if (edgesSelected == totalStations - 1) break;
            }
        }

        cout << "--------------------------------------------\n";
        cout << "Total Network Track Length: " << totalWeight << " km\n";
        cout << "============================================\n";
    }
};

// -------------------------------------------------------------
// MAIN PROGRAM
// -------------------------------------------------------------
int main() {
    MetroNetwork metro;

    // Adding Routes (Station A, Station B, Distance in KM)
    metro.addRoute("Central", "Airport", 15);
    metro.addRoute("Central", "CityCenter", 5);
    metro.addRoute("CityCenter", "Airport", 8);
    metro.addRoute("CityCenter", "Suburbs", 10);
    metro.addRoute("Suburbs", "IndustrialArea", 6);
    metro.addRoute("Airport", "IndustrialArea", 20);

    // 1. Test Trie Search
    cout << "--- TESTING TRIE SEARCH ---\n";
    metro.search("Cen");
    cout << "\n";
    metro.search("Air");
    cout << "\n";

    // 2. Compute Kruskal's MST
    cout << "--- RUNNING KRUSKAL MST ---\n";
    metro.computeMST();

    return 0;
}
