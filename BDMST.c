#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define ROWS 250
#define COLUMNS 2
#define infi 9999
#define D 15
float cost = infi;

void print2DArray(float** arr);
float calculateDistance(float x1, float x2, float y1, float y2);
float** create_2d_array(int rows, int cols, float X[ROWS][COLUMNS]);

int calculateDepthWithCriteria(int parent[], int targetNode) {
    int depth = 0;
    int currentNode = targetNode;

    // Traverse the parent array from the target node towards the root
    while (currentNode != -1) {
        depth++;

        // Check if the depth exceeds the given criteria
        if (depth > D)
            return -1;

        currentNode = parent[currentNode];
    }

    return depth;
}

float prims(float** adjMatrix, int numNodes, int start_node) {
        int c=0;
    int parent[ROWS];
    float key[ROWS];
    int visited[ROWS];
    int parent_MST[ROWS];
    int depth[ROWS];
    int i, j, u, v;
    float minCost = 0;
    float** MST = (float**)malloc(ROWS * sizeof(float*));

    for (int j = 0; j < ROWS; j++) {
        MST[j] = (float*)malloc(ROWS * sizeof(float));
    }

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < ROWS; j++) {
            MST[i][j] = -1;
        }
    }

    // Initialize key values and visited array
    for (i = 0; i < numNodes; i++) {
        key[i] = infi;
        visited[i] = 0;
        depth[i] = 0;
    }

    // Start with the first node
    key[start_node] = 0;
    parent[start_node] = -1;
    parent_MST[start_node] = -1;
    depth[start_node] = 0;


    // Find the MST
    for (i = 0; i < numNodes - 1; i++) {
        // Find the vertex with the minimum key value
        float minKey = infi;
        for (j = 0; j < numNodes; j++) {
            if (visited[j] == 0 && key[j] < minKey) {
                minKey = key[j];
                u = j;
            }
        }

        visited[u] = 1; // Mark the vertex as visited
        // Update key values and parent for the adjacent vertices
        for (v = 0; v < numNodes; v++) {
            if ( visited[v] == 0 && adjMatrix[u][v] < key[v]) {
                depth[v] = depth[u] + 1;
                key[v] = adjMatrix[u][v];
                parent[v] = u;

                if (calculateDepthWithCriteria(parent, v) != -1) {
                    parent_MST[v] = u;
                }
                else{visited[u]=0;
                key[v]=infi;}
                
                
            }
        }
    }

    // Calculate the total cost of the MST
    for (i = 0; i < numNodes; i++) {
        // if(parent_MST[i]!=-1)
        {minCost += adjMatrix[i][parent_MST[i]];}
    }

    printf("Minimum spanning tree starting from node %d\n", start_node);
    if(minCost<cost)
    {   cost=minCost;
    for (i = 0; i < numNodes; i++) {
        
        
            if (parent_MST[i] != -1) {
            printf("%d - %d -> %f\n", parent_MST[i], i, adjMatrix[i][parent_MST[i]]);
        }
        
    }}

    printf("Total cost: %f\n\n", minCost);

    // Free the dynamically allocated memory
    for (i = 0; i < ROWS; i++) {
        free(MST[i]);
    }
    free(MST);
    return minCost;
}

int main(void) {
    int numNodes = ROWS;
    int start_node;
    float array[ROWS][COLUMNS];
    FILE* file;
    file = fopen("dataSet250-5.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int row = 0;
    while (!feof(file)) {
        if (ferror(file)) {
            printf("Error reading file.\n");
            return 1;
        }
        for (int i = 0; i < COLUMNS; i++) {
            if (fscanf(file, "%f", &array[row][i]) == EOF)
                break;
        }
        row++;
        if (row == ROWS)
            break;
    }
    fclose(file);

    float** edges = create_2d_array(ROWS, ROWS, array);
    float total_sum = 0, all_cost[ROWS];
    float t;

    for (int j = 0; j < ROWS; j++) {
        t = prims(edges, numNodes, j);
        all_cost[j] = t;
        total_sum += t;
        if (cost > t) {
            cost = t;
        }
    }

    printf("***************************************************************************************************\n");
    float mean = total_sum / ROWS;
    float sum = 0;

    for (int j = 0; j < ROWS; j++) {
        sum +=((all_cost[j] - mean)*(all_cost[j] - mean));
    }

    float stdev = sqrt(sum / ROWS);
    printf("No. of nodes: %d\n", ROWS);
    printf("Best Cost: %f, Mean Cost: %f, Standard Deviation: %f\n", cost, mean, stdev);
    printf("***************************************************************************************************\n");

    // Free the dynamically allocated memory
    for (int i = 0; i < ROWS; i++) {
        free(edges[i]);
    }
    free(edges);

    return 0;
}

void print2DArray(float** arr) {
    int count = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            printf("%1f ", arr[i][j]);
            if (arr[i][j] != -1) {
                count++;
            }
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
    printf("%d", count);
}

float calculateDistance(float x1, float x2, float y1, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);
    return distance;
}

float** create_2d_array(int rows, int cols, float X[ROWS][COLUMNS]) {
    float** array = (float**)malloc(rows * sizeof(float*));

    for (int i = 0; i < rows; i++) {
        array[i] = (float*)malloc(cols * sizeof(float));
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            array[i][j] = 0;
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            array[i][j] = calculateDistance(X[i][0], X[j][0], X[i][1], X[j][1]);
        }
    }

    return array;
}


