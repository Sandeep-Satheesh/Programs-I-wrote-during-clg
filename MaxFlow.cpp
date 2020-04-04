#include<iostream>
#include<list>
#include<vector>
#include<stack>
#include<mem.h>
#include<iomanip>

using namespace std;

vector<list<int> > paths;

void displayPath(list<int>& path) {
for (list<int>::iterator j = path.begin(); j != path.end(); j++)
            cout<<*j<<(char)26;
        cout<<"\b ";
}

bool findAllPathsFromSourceToSink(int**& G, int& n, int& source, int& sink, int& current, list<int>*& traversedPath) {

    if (current == sink) {
        return true; //we've found our way to the sink; this is a valid path.
    }

    stack<int> traversableNodes;
    for (int i = 0; i < n; i++) {
        if (i == current) continue; //no self loops
        if (G[current][i] != 0) {
            traversableNodes.push(i);
        }
    }
    if (traversableNodes.empty()) {
        return false; //we've hit a dead-end, as we have no possible paths to anywhere from here. Need to backtrack...
    }
    if (!traversedPath) {
        traversedPath = new list<int>;
        current = source;
        traversedPath->push_front(source+1);
    }

    for (int j = traversableNodes.top(); !traversableNodes.empty(); ) {
        traversedPath->push_back(j+1);

        bool flg;
        if (j == sink) {
            paths.push_back(*traversedPath);
            list<int>* anotherPath = new list<int> (*traversedPath);
            anotherPath->pop_back();
            traversedPath = anotherPath;
            flg = false;
        }
        else if ((flg = findAllPathsFromSourceToSink(G, n, source, sink, j, traversedPath)) == false) {
            traversedPath->pop_back();
        }
        if (traversableNodes.size() > 1) {
            traversableNodes.pop();
            j = traversableNodes.top();
        }
        else {
            if (!flg) traversedPath->pop_back();
            return !flg;
        }
    }

    return true;
}

int calculateBottleneckCapacity(int**& G, int**& flowMatrix, int& n, list<int>& path) {
    int bottleneckCap = INT_MAX;
    for (list<int>::iterator i = path.begin(); i != path.end(); ) {
        int v1 = *i, v2;
        if (++i != path.end()) v2 = *i;
        else break;

        if (G[v1-1][v2-1] - flowMatrix[v1-1][v2-1] < bottleneckCap) {
            bottleneckCap = G[v1-1][v2-1] - flowMatrix[v1-1][v2-1];
        }
    }
    return bottleneckCap;
}

void MaxFlow(int**& G, int& n, int& source, int& sink) {
    list<int>* dummy = NULL;
    cout<<"\n**********************************************************";
    cout<<"\n* PART 2: Finding all possible paths from source to sink *";
    cout<<"\n**********************************************************";

    findAllPathsFromSourceToSink(G,n,source,sink,source,dummy);

    cout<<"\n\nFound Paths:\n";
    cout<<"----------------\n";
    for (vector<list<int> >::iterator i = paths.begin(); i != paths.end(); i++) {
        displayPath(*i);
        cout<<'\n';
    }
    cout<<"\n\n\n****************************************************";
    cout<<"\n* PART 3: Finding the max flow from source to sink *";
    cout<<"\n****************************************************";

    //Step 1: Initialize zero flow for all edges...
        //Step 1a: Prepare a nxn "flow matrix" that maintains the flow through each edge in the graph...
        //Step 1b: Set everything to 0 in it.

    int** flowMatrix = new int*[n];
    for (int i = 0; i < n; i++) {
        flowMatrix[i] = new int[n];
        memset(flowMatrix[i], 0, n*sizeof(int));
    }
    int totalFlowIntoSink = 0;
    int excludedPaths[n], ct = 0; //list of paths to exclude, because one (or more) of their edges are already at max capacity. ct is the number of such paths.

    //Step 2: Repeating step: Select each valid path we've found earlier, find the bottleneck capacity, and update the flow in each iteration.

    cout<<"\n\n\t\t\t\tINITIAL FLOW: 0\n\n    \t\t\t\tBottleneck\t\tCurrent Flow";
    cout<<"\nPath\t\t\t\t Capacity\t\t  into Sink";
    cout<<"\n------------------------------------------------------------------------\n";
    for (vector<list<int> >::iterator i = paths.begin(); i != paths.end(); i++) {
        list<int>* currentPath = &(*i); //I prefer to deal with pointers to the list.

        bool flg = false; int v1, v2;
        for (list<int>::iterator i = currentPath->begin(); i != currentPath->end(); ) {
            v1 = *i;
            if (++i != currentPath->end()) v2 = *i;
            else break;

            if (G[v1-1][v2-1] == flowMatrix[v1-1][v2-1]) {//it means that this edge is the bottleneck edge, and all paths using it should be excluded.
                flg = true;
                break;
            }
        }
        displayPath(*currentPath);
        if (flg) { cout<<"  [REJECTED BECAUSE EDGE "<<v1<<(char)26<<v2<<" IS ALREADY FULL!]\n"; continue; }
        //Calculating the bottleneck capacity for the path... (min flow possible, considering flow from previous iterations)
        int bottleNeckCapacity = calculateBottleneckCapacity(G, flowMatrix, n, *currentPath), V1, V2;

        //Update the flow matrix with the new flows.
        for (list<int>::iterator i = currentPath->begin(); i != currentPath->end(); ) {
            int v1 = *i, v2;
            if (++i != currentPath->end()) v2 = *i;
            else break;
            flowMatrix[v1-1][v2-1] += bottleNeckCapacity;
        }

        //Update the total flow into the sink.
        totalFlowIntoSink += bottleNeckCapacity;

        //Display whatever you have done.
        cout<<"\t\t\t  "<<setw(3)<<bottleNeckCapacity<<"\t\t\t"<<setw(8)<<totalFlowIntoSink<<'\n';

    }

    //Clean up...
    for (int i = 0; i < n; i++)
        delete flowMatrix[i];
    delete flowMatrix;
    cout<<"\n________________________________________________________________________\n";
    cout<<"\n\n\t\t\t\tFINAL FLOW INTO SINK: "<<totalFlowIntoSink;
}

/* void generateGraphPreview(int**& G, int& n) {
 *
 *     ofstream fo ("generatedGraph.dot");
 *     fo<<"digraph {\n\trankdir=LR\n";
 *     for (int i = 0; i < n; i++)
 *         for (int j = 0; j < n; j++)
 *             if (i == j) continue; //we're not considering self-loops
 *             else if (G[i][j] != 0)
 *                 fo<<"\n\t"<<i+1<<" -> "<<j+1<<" [label = \""<<G[i][j]<<"\"];";
 *     fo<<"\n}\n";
 *     if (spawnl(0, "dot -Tpng generatedGraph.dot -o output.png", NULL))
 *         cout<<"\nDone!";
 *     else cout<<"\nERROR: dot did not execute successfully!";
 * }
 */

int main() {
    cout<<"\n*******************************";
    cout<<"\n* PART 1: Inputting the graph *";
    cout<<"\n*******************************";
cout<<"\n\nEnter the number of vertices: ";
    int n; cin>>n;
    while (n <= 1) {
        cout<<"\nInvalid number of vertices! Choose 2 or more!\nEnter the number of vertices: ";
        cin>>n;
    }
    //Initialize the graph.
    int** G;
    G = new int*[n];
    for (int i = 0; i < n; i++)
        G[i] = new int[n];
    cout<<"\nEnter the cost matrix of the graph: ";
    for (int i = 0; i < n; i++) {
        cout<<"\nEnter row "<<i+1<<": ";
        for (int j = 0; j < n; j++)
            cin>>G[i][j];
    }
/*  To be implemented later; generates the DOT file and calls dot.exe properly, but output image file is not getting generated...
     cout<<"\nWould you like to see a graphical preview of the graph you just entered? NOTE: The GraphViz home directory must be in your PATH variable.\nType y for yes, any other key for no: ";
 *     char ch; cin>>ch;
 *     if (ch == 'y' || ch == 'Y')
 *         generateGraphPreview(G, n);
 */
    cout<<"\nChoose source vertex (1 to "<<n<<"): "; int source; cin>>source;
    while (source <= 0 || source > n) {
        cout<<"\nInvalid source vertex! Value must lie from 1 to "<<n<<"!\nChoose source vertex (1 to "<<n<<"): ";
        cin>>source;
    }
    cout<<"\nChoose sink vertex: "; int sink; cin>>sink;
    while (sink == source || sink <= 0 || sink > n) {
        cout<<"\nInvalid sink vertex! Try again!"<<n<<"!\nChoose sink vertex: ";
        cin>>sink;
    }
    source--; sink--;
    MaxFlow(G, n, source, sink);

    //Cleanup vars...
    for (vector<list<int> >::iterator i = paths.begin(); i != paths.end(); i++)
        (*i).clear();
    paths.clear();
    for (int i = 0; i < n; i++)
        delete[] G[i];
    delete G;

    cout<<"\n\n\nINFORMATION: Application terminated successfully.\n\n";
    return 0;
}
