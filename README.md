# 🚇 Smart Metro Network System: Autocomplete & Infrastructure Optimization

An efficient C++ implementation for metro transit management combining **Prefix Tree (Trie)** search and **Kruskal's Minimum Spanning Tree (MST)** algorithm to solve real-world urban transit challenges.

---

## 🎯 Problem Statement & Motivation

1. **User Experience Problem:** Commuters need rapid, error-tolerant station lookup and autocompletion while searching for routes.
2. **Network Planning Problem:** Transit authorities require a minimal-cost infrastructure strategy to connect all stations while minimizing total track distance laid.
3. **Solution:** A dual-purpose Data Structures & Algorithms solution combining a **Trie** ($O(L + K)$ lookup time) and **Kruskal's Algorithm** with **Disjoint Set Union (DSU)** ($O(E \log E)$ complexity) for optimal network design.

---

## ✨ Key Features

* 🔍 **Station Autocomplete (Trie):** Fast prefix matching with integrated case-insensitivity handling.
* 🛣️ **Infrastructure Optimization (Kruskal's MST):** Calculates minimum total track length required to connect every station without introducing cycles.
* 🛠️ **Zero External Library Dependencies:** Built from basic primitives using standard static memory bounds without relying on STL containers or built-in algorithms (includes manual string casing, custom Bubble Sort, and explicit DSU parent tracking).

---

## 🏗️ System Architecture & Data Structures

```
               ┌────────────────────────┐
               │      MetroNetwork      │
               └───────────┬────────────┘
                           │
             ┌─────────────┴─────────────┐
             ▼                           ▼
    ┌─────────────────┐         ┌─────────────────┐
    │   StationTrie   │         │    RouteEdge    │
    └────────┬────────┘         └─────────────────┘
             │
             ▼
    ┌─────────────────┐
    │    TrieNode     │
    └─────────────────┘
```

### 1. `TrieNode` & `StationTrie`
* **Purpose:** Multi-way tree storing lowercase station names for prefix matching.
* **Complexity:**
  * **Insertion:** $O(L)$, where $L$ is length of station name.
  * **Autocomplete Search:** $O(L + K)$, where $L$ is length of the prefix and $K$ is the number of matching output nodes.

### 2. `RouteEdge`
* **Purpose:** Represents connections between stations via source ID ($u$), destination ID ($v$), and track distance ($w$).

### 3. `MetroNetwork`
* **Purpose:** Main controller executing station indexing, graph mapping, and MST construction.
* **Kruskal's Algorithm Flow:**
  1. Orders all available edges using distance sorting.
  2. Applies `findRoot` via **Disjoint Set Union (DSU)** to detect potential cycles.
  3. Selects $V - 1$ optimal edges to form the unified minimum-cost layout.

---

## 🚀 Getting Started

### Prerequisites
* C++11 compatible compiler (`g++`, `clang++`, or standard IDE).

### Build & Run Instructions

```bash
# Compile the program
g++ -o metro_system main.cpp

# Execute the application
./metro_system
```

---

## 📊 Example Output

```text
--- TESTING TRIE SEARCH ---
Stations matching prefix "Cen":
  - Central
  - CityCenter

Stations matching prefix "Air":
  - Airport

--- RUNNING KRUSKAL MST ---

============================================
 Optimal Metro Tracks (Kruskal's Algorithm)
============================================
Build Track: Central <---> CityCenter | Cost: 5 km
Build Track: Suburbs <---> IndustrialArea | Cost: 6 km
Build Track: CityCenter <---> Airport | Cost: 8 km
Build Track: CityCenter <---> Suburbs | Cost: 10 km
--------------------------------------------
Total Network Track Length: 29 km
============================================
```
