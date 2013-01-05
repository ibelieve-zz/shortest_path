/*************************************************
 * File Name: function.h
 * Dependent file: No
 *
 * Author: Bing Lu
 * Contact: http://compiledreams.com
 *
 * Content: declare data structure and funciton used in main()function
 *
 *
 ***************************************************/

/* Data structure for an linked list of ints */
struct listType 
{
    int i;
    struct listType *next;
};

/*define a structure to save the edge of undirected graph
 *pt1 and pt2 are the end point of edge
 */
typedef struct 
{
    int pt1, pt2;
} EdgeType;

/*define a structure to save the node locates in each edge
 *dist stands for distance to begin point
 *pres stands for predecessor of current node
 */
typedef struct
{
	int dist, pres;
} NodeType;

//implementing Bellman-Ford algorithm
extern void BellmanFord(int origin, int nVerts, int nEdges, NodeType *nodes, EdgeType *edges);

//generating the edge from user inputing
extern int GenEdge (struct listType **al, int nVerts, EdgeType *edges);

//generating the shortest path and print it
extern void GenPath (NodeType *nodes, int nVerts, int origin, int end, int tempflag);

//find whether edge that user what to add to graph is already exited
extern int findInList(struct listType *l, int a);

//add the vertex to the list
extern void addToList(int a, struct listType **pl);

//remove vertex from list
extern void removeFromList(int i, struct listType **pl); 

//skiping and waiting for another input
extern void skipToNextLineOfInput();

//free the whole list
extern void freeAllLists(struct listType **al, int nVerts); 

//read integer from user input
extern int readInteger(char c, int *i, int permissive);

//add edge according to user input 
extern int addEdges(int v[2], struct listType **al, int nVerts);

/***********************END OF FILE*************************/
