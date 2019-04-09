#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>
#include <map>

using namespace std;

// V keeps track of all of the nodes and their links to other nodes
struct V;
struct V {
    V(string node_) : edges(), node(node_) {} // makes V obj; takes in node; sets edge and node
    set<V*> edges,backEdges; // for creating edges and their links; also set is used so value and key are same
    string node;
};

class Graph {
public:
    Graph() {} // constructor
    virtual ~Graph() {} // deconstructor virtual for mem allocation
    
    // getNode adds and gets nodes
    V* getNode(string name) {
        // searches for node by name
        list<V>::iterator iter = adjList.begin();
        while (iter != adjList.end()) {
            if (iter->node == name) {
                V* retVal = &(*iter);
                return retVal; // location of node
            }
            iter++;
        }
        // if node not found it adds the node and edge to the back of the list
        adjList.push_back(V(name));
        return &adjList.back(); //returns the node and edge
    }
    
    // createEdge creates the link between two nodes
    void createEdge(V* v1, V* v2) {
        v1->edges.insert(v2); // Sets the connection edge from the main edge (ex: in Tree it shows it connecting to Leaf)
        v2->backEdges.insert(v1); // Sets the connection on back edge to main edge (ex: in Leaf it shows it connecting to Tree)
    }
    // removeNode deletes the node and its edges from adjList
    void removeNode(string name) {
        for (list<V>::iterator d = adjList.begin(); d != adjList.end(); ++d) {
            if (d->node == name) {
                V* v = &(*d);
                // First remove backEdges
                for (set<V*>::iterator be = d->backEdges.begin(); be != d->backEdges.end(); ++be) {
                    (*be)->edges.erase(v);
                }
                // Then remove node
                adjList.erase(d);
            }
        }
    }
    
    // saveGraph writes over the file with the current nodes and links existing in the session
    void saveGraph()
    {
        ofstream newFile;
        newFile.open("/Users/charlie/Desktop/Xcode_Projects/Assignment1/Assignment1/resource.txt");
            // writes out every node and its edge in the file in the format "node1 node2"
            for (list<V>::iterator d = adjList.begin(); d != adjList.end(); ++d) {
                if (d->node.empty()) continue;
                for (set <V*>::iterator e = d->edges.begin(); e != d->edges.end(); ++e) {
                    newFile << d->node << " " << (*e)->node << endl;
                }
            }
            newFile.close();
      
    }
    // print prints Directed Graph and Usability List
    void print() {
        cout << "----------------------------" << endl;
        cout << "       Directed Graph" << endl;
        cout << "----------------------------" << endl;
        // prints all nodes and their links
        for (list<V>::iterator d = adjList.begin(); d != adjList.end(); ++d) {
            if (d->node.empty()) continue;
            for (set<V*>::iterator e = d->edges.begin(); e != d->edges.end(); ++e) {
                cout << d->node << " --> " << (*e)->node << endl;
            }
        }
        cout << endl;
        cout << "----------------------------" << endl;
        cout << "    Node Usability List" << endl;
        cout << "----------------------------" << endl;
        // prints all nodes and their usability
        for (list<V>::iterator d = adjList.begin(); d != adjList.end(); ++d) {
            if (d->node.empty()) continue;
                string usable = "usable";
                if (d->edges.size() == 0) {
                    usable = "not usable";
                }
                cout << d->node << " [" << usable << "]" << endl;
        }
        cout << endl;
    }
private:
    list<V> adjList;        // adjList has node name and list of edges
};


int main(int arc, char* argv[]) {
    Graph graph;
    string main_node;
    string other_node;
    
    // opens file and reads from it
    ifstream File;
    File.open("/Users/charlie/Desktop/Xcode_Projects/Assignment1/Assignment1/resource.txt");
    if (File.is_open()) {
        while (File >> main_node >> other_node) {
            // checks if node exists, if not add it
            V* node1 = graph.getNode(main_node);
            V* node2 = graph.getNode(other_node);
            // adds links
            graph.createEdge(node1,node2);
       }
    } else {
        cout << "How dare you not open up to me File!" << endl;
    }
    File.close();
    graph.print();    // prints out current graph
    cout << endl;
    
    // we've completed reading the file. Now do interactive adding/deleting.
    bool res = true;
    string input = " ";
    cout << "Welcome to the Resource Manager!"  << endl;
    // while "q" is not entered continue receiving input
    while (input != "q") {
        // checks if res is true and if it is, provides options for user
        if (res == true) {
            cout << "Type one of the letters below to access a feature:" << endl;
            cout << "a - add a node and its link" << endl;
            cout << "d - delete a node" << endl;
            cout << "s - save graph for next session" << endl;
            cout << "q - quit the program" << endl;
        }
        cin >> input;
        cout << endl;
        string n1;
        string n2;
        // if 'a' adds node to graph, if 'd' removes node from graph, if 's' saves graph to txt,
        // if 'q' quits session, else if anything else typed then sends a message to user
        if (input == "a") {
            cout << "Enter first node, a space, and then the node it is linked to..." << endl;
            cout << "(ex: Tree Leaf)" << endl;
            cin >> n1 >> n2;
            V* nodeId1 = graph.getNode(n1);
            V* nodeId2 = graph.getNode(n2);
            graph.createEdge(nodeId1,nodeId2);
            graph.print();
            res = true;
        } else if (input == "d") {
            cout << "Enter the node you wish to delete..." << endl;
            cout << endl;
            cin >> n1;
            graph.removeNode(n1);
            graph.print();
            res = true;
        } else if (input == "s") {
            graph.saveGraph();
        }
        else if (input == "q") {
            break;
        } else {
            cout << "Please type either a, d, or q." << endl;
            res = false;
        }
        cout << endl;
    }
    
    graph.print();
    return 0;
}

