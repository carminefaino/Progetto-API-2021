#include <stdio.h>
#include <stdlib.h>

// Maximum possible integer value to represent infinity
#define INFINITY 2147483647

// Global variables to track heap sizes
int rankSize = 0;    // Size of the top-k ranking
int queueSize = 0;   // Size of the current processing queue

// Structure to represent a node in the graph with its ID and path cost
typedef struct {
    int nodeId;      // Unique identifier for the node
    int pathCost;    // Cost of the path to this node
} GraphNode;

// Utility function to swap two graph nodes
void swapNodes(GraphNode *a, GraphNode *b) {
    GraphNode temp = *b;
    *b = *a;
    *a = temp;
}

// Builds a max heap for the given array of graph nodes
// Used to maintain the top-k smallest path costs
void buildMaxHeap(GraphNode array[], int heapSize, int index) {
    // Base case: single element heap
    if (heapSize == 1) {
        return;
    }
    
    // Find the largest element among root, left and right children
    int largest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    
    // Check if left child exists and is larger than root
    if (leftChild < heapSize && array[leftChild].pathCost > array[largest].pathCost) {
        largest = leftChild;
    }
    
    // Check if right child exists and is larger than current largest
    if (rightChild < heapSize && array[rightChild].pathCost > array[largest].pathCost) {
        largest = rightChild;
    }
    
    // If largest is not the root, swap and recursively heapify
    if (largest != index) {
        swapNodes(&array[index], &array[largest]);
        buildMaxHeap(array, heapSize, largest);
    }
}

// Builds a min heap for the given array of graph nodes
// Used in finding the minimum path cost
void buildMinHeap(GraphNode array[], int heapSize, int index) {
    // Base case: single element heap
    if (heapSize == 1) {
        return;
    }
    
    // Find the smallest element among root, left and right children
    int smallest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    
    // Check if left child exists and is smaller than root
    if (leftChild < heapSize && array[leftChild].pathCost < array[smallest].pathCost) {
        smallest = leftChild;
    }
    
    // Check if right child exists and is smaller than current smallest
    if (rightChild < heapSize && array[rightChild].pathCost < array[smallest].pathCost) {
        smallest = rightChild;
    }
    
    // If smallest is not the root, swap and recursively heapify
    if (smallest != index) {
        swapNodes(&array[index], &array[smallest]);
        buildMinHeap(array, heapSize, smallest);
    }
}

// Removes the minimum node from the queue and re-heapifies
void removeMinNode(GraphNode array[]) {
    // Swap first (minimum) and last elements
    swapNodes(&array[0], &array[queueSize - 1]);
    
    // Reduce queue size
    queueSize--;
    
    // Rebuild min heap
    for (int i = queueSize / 2 - 1; i >= 0; i--) {
        buildMinHeap(array, queueSize, i);
    }
}

// Inserts a new node into a full top-k ranking
void insertIntoFullTopK(GraphNode array[], int nodeId, int pathCost) {
    // Replace the root (maximum) with new node
    array[0].nodeId = nodeId;
    array[0].pathCost = pathCost;
    
    // Rebuild max heap to maintain top-k smallest
    for (int i = rankSize / 2 - 1; i >= 0; i--) {
        buildMaxHeap(array, rankSize, i);
    }
}

// Calculate power using iterative method
// Optimized for integer exponentiation
int calculatePower(int base, int exponent) {
    // Special cases for 0 and 1 exponents
    if (exponent == 0) return 1;
    if (exponent == 1) return base;
    
    // Calculate power iteratively
    int result = base * base;
    for (int i = 3; i < (exponent + 1); i++) {
        result *= base;
    }
    return result;
}

// Custom number reading function optimized for input parsing
int readNumber() {
    char numberStr[10];
    int numberValue = 0;
    char c;
    int digitCount = 0;
    
    // Read characters until comma or newline
    c = getchar_unlocked();
    
    // Capture digits
    for (int i = 0; c != ',' && c != '\n'; i++) {
        numberStr[i] = c;
        digitCount++;
        c = getchar_unlocked();
    }
    
    // Convert to integer using positional notation
    for (int j = digitCount - 1, multiplier = 0; j >= 0; j--, multiplier++) {
        numberValue += (numberStr[j] - '0') * calculatePower(10, multiplier);
    }
    
    return numberValue;
}

// Find minimum path cost using a Dijkstra-like algorithm
int findMinimumPathCost(int graphSize) {
    int totalPathCost = 0;
    int graph[graphSize][graphSize];
    GraphNode nodeQueue[graphSize];
    
    // Read graph adjacency matrix
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            graph[i][j] = readNumber();
        }

        // Initialize node queue
        // First node starts with 0 cost, others with infinite cost
        if (i == 0) {
            nodeQueue[i].nodeId = i;
            nodeQueue[i].pathCost = 0;
        } else {
            nodeQueue[i].nodeId = i;
            nodeQueue[i].pathCost = INFINITY;
        }
    }

    queueSize = graphSize;

    // Path finding algorithm
    for (int i = 0; i < graphSize; i++) {
        int currentNodeId = nodeQueue[0].nodeId;
        int currentNodeCost = nodeQueue[0].pathCost;
        
        // Process node if cost is finite
        if (currentNodeCost < INFINITY) {
            totalPathCost += currentNodeCost;
            
            // Update path costs for neighboring nodes
            for (int x = 0; x < (graphSize - i); x++) {
                int newPathCost = currentNodeCost + graph[currentNodeId][nodeQueue[x].nodeId];
                if (newPathCost < nodeQueue[x].pathCost && graph[currentNodeId][nodeQueue[x].nodeId] > 0) {
                    nodeQueue[x].pathCost = newPathCost;
                }
            }
        } else {
            break;  // Stop if no valid path exists
        }
        
        // Remove current node and re-heapify
        removeMinNode(nodeQueue);
    }

    return totalPathCost;
}

// Process graphs for small k values (less than 100,000)
void processSmallTopK(int graphSize, int k) {
    char input[15];
    int pathCost = 0, graphCount = -1, isFirstTime = 1;
    GraphNode topKGraphs[k];

    // Process input lines
    while (fgets(input, 15, stdin) != NULL) {
        if (input[0] == 'A') {
            graphCount++;
            // Find minimum path cost for current graph
            pathCost = findMinimumPathCost(graphSize);
            queueSize = 0;
            
            // Fill initial k graphs
            if (graphCount < k) {
                topKGraphs[graphCount].nodeId = graphCount;
                topKGraphs[graphCount].pathCost = pathCost;
            } else {
                // Initialize max heap for top-k
                if (isFirstTime) {
                    rankSize = k;
                    for (int i = rankSize / 2 - 1; i >= 0; i--) {
                        buildMaxHeap(topKGraphs, rankSize, i);
                    }
                    isFirstTime = 0;
                }
                
                // Replace if current path cost is smaller
                if (pathCost < topKGraphs[0].pathCost) {
                    insertIntoFullTopK(topKGraphs, graphCount, pathCost);
                }
            }
        } else {
            // Print results
            int printLimit = (graphCount < k) ? graphCount + 1 : k;
            for (int i = 0; i < printLimit; i++) {
                printf("%s%d", (i == 0) ? "" : " ", topKGraphs[i].nodeId);
            }
            printf("\n");
        }
    }
}

// Process graphs for large k values (greater than or equal to 100,000)
void processLargeTopK(int graphSize, int k) {
    char input[15];
    int pathCost = 0, graphCount = -1, isFirstTime = 1, previousPathCost = 0;
    int isDescending = 1;
    int currentIndex = 0;
    GraphNode topKGraphs[k];

    // Process input lines with different strategy for large k
    while (fgets(input, 15, stdin) != NULL) {
        if (input[0] == 'A') {
            graphCount++;
            previousPathCost = pathCost;
            pathCost = findMinimumPathCost(graphSize);
            queueSize = 0;
            
            // Fill initial k graphs
            if (graphCount < k) {
                topKGraphs[graphCount].nodeId = graphCount;
                topKGraphs[graphCount].pathCost = pathCost;
            } else {
                // Check if path costs are strictly decreasing
                if (pathCost > previousPathCost) {
                    isDescending = 0;
                }
                
                // Special handling for descending path costs
                if (isDescending) {
                    topKGraphs[currentIndex].nodeId = graphCount;
                    topKGraphs[currentIndex].pathCost = pathCost;
                    currentIndex++;
                    
                    // Circular buffer for k elements
                    if (currentIndex == k) {
                        currentIndex = 0;
                    }
                } else {
                    // Initialize max heap for top-k
                    if (isFirstTime) {
                        rankSize = k;
                        for (int i = rankSize / 2 - 1; i >= 0; i--) {
                            buildMaxHeap(topKGraphs, rankSize, i);
                        }
                        isFirstTime = 0;
                    }
                    
                    // Replace if current path cost is smaller
                    if (pathCost < topKGraphs[0].pathCost) {
                        insertIntoFullTopK(topKGraphs, graphCount, pathCost);
                    }
                }
            }
        } else {
            // Print results
            int printLimit = (graphCount < k) ? graphCount + 1 : k;
            for (int i = 0; i < printLimit; i++) {
                printf("%s%d", (i == 0) ? "" : " ", topKGraphs[i].nodeId);
            }
            printf("\n");
        }
    }
}

// Main entry point of the program
int main() {
    int graphSize, k;
    
    // Read graph size and k value
    scanf("%d %d", &graphSize, &k);
    getchar_unlocked();

    // Choose processing method based on k value
    if (k < 100000) {
        processSmallTopK(graphSize, k);
    } else {
        processLargeTopK(graphSize, k);
    }

    return 0;
}
