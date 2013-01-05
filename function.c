
/*************************************************
 * File Name: function.c
 * Dependent file: function.h
 *
 * Author: Bing Lu
 * Contact: http://compiledreams.com/

 * Content: define data structure and funciton used in main()function
 *
 *
 ***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "function.h"

/*This function implements Bellman-Ford algorithm 
 *origin is the begin point of path
 *nVerts stands for the total amount of vertex in graph
 *nEdges stands for the total amount of edges in graph
 *nodes is a pointer who points to NodeType
 *edges is a pointer who points to EdgeType
*/
void BellmanFord(int origin, int nVerts, int nEdges, NodeType *nodes, EdgeType *edges) 
{
    int i, j;
	//n vertex have n edges in one route at most
    for (i = 0; i < nVerts; ++i)
	{
		//initiating every nodes
        nodes[i].dist = nVerts;
		nodes[i].pres = -1;	//default negative value to represent no predecessor
	}
	//the distance to origin point itself is zero
    nodes[origin].dist = 0;
	nodes[origin].pres = -2;

	for (i = 0; i < nVerts - 1; ++i)
        for (j = 0; j < nEdges; ++j)
		{
			//relaxing precess of Bellman-Ford algorithm
        	if (nodes[edges[j].pt1].dist + 1 < nodes[edges[j].pt2].dist)
			{
                nodes[edges[j].pt2].dist = nodes[edges[j].pt1].dist + 1;
				nodes[edges[j].pt2].pres = edges[j].pt1;	//record its predecessor
			}
			if (nodes[edges[j].pt2].dist + 1 < nodes[edges[j].pt1].dist)
			{
				nodes[edges[j].pt1].dist = nodes[edges[j].pt2].dist + 1;
				nodes[edges[j].pt1].pres = edges[j].pt2;
			}
		}
}

/*This function generates edge from adjacency list after traversal
 *al is a pointer to the whole adjacency list
*/
int GenEdge (struct listType **al, int nVerts, EdgeType *edges)
{
    int k = 0;
	int j = 0;
   	while(k < nVerts) 
	{
		//tmp is pointer who points to the beginning address of each child list
		struct listType *tmp = al[k]; 
		while(tmp != NULL) 
		{
			//printf("%d ", tmp->i); fflush(stdout);
			if(tmp->i > k)
			{
				edges[j].pt1 = k;
				edges[j].pt2 = tmp->i;
				j++;
			}
			tmp = tmp->next; //scanning the current child list
		}
		k++; //scanning every child list
    	}
	return j+1;	//return the number of existing edges in graph
}

/*This function generate the shortest path if it exists
*tempflag is a variable to represent if the origin and end have been exchanged
*if not, it should be less than zero (should be -1)
*/
void GenPath (NodeType *nodes, int nVerts, int origin, int end, int tempflag)
{
	//if shortest path exists, finding each vertex backward in path from the end point
	int i = end;
	int j = 1; //j stands for the amount of vertex in the shortest path
	int *path = NULL;
	path = (int *)malloc(sizeof(int)*nVerts);
	if ( path==NULL)
	{
		printf("Error: malloc() returned NULL.\n");fflush(stdout);
	}
	else
	{
		path[0] = end;
		while(nodes[i].pres!=-1)
		{
			path[j]=nodes[i].pres;
			//finding out the origin
			if(path[j]==origin)
			{
				//if origin and end points never exchanges each other
				//print the path reversely in order to acquire the correct order
				//because they are reversed order when stored
				if(tempflag<0)
				{
					for(i=j;i>0;i--)
						printf("%d-",path[i]);
					printf("%d\n",path[0]);fflush(stdout);
				}
				else
				{
					for(i=0;i<j;i++)
						printf("%d-",path[i]);
					printf("%d\n",path[j]);fflush(stdout);
				}
				//release this area in memory
				free(path);
				path=NULL;
					
				return;
			}
			j++;
			i=nodes[i].pres;
		}
		//if not path between this two vertex, print error message
		printf("Error: There is no path between %d and %d\n",origin,end); fflush(stdout);
	}
	free(path);
	path=NULL;
	return;
}



/*
 * Name: findInList()
 *
 * Parameters:
 *   - l: list in which to find
 *   - a: entry to find
 *
 * Returns: int
 *   - 0 if a not found in l. 1 otherwise.
 *
 * Description: see 'Returns' above.
 */
int findInList(struct listType *l, int a) 
{
    	if(l == NULL) return 0;

    	struct listType *tmp = l;
    	while(tmp != NULL) 
		{
			if(tmp->i == a) return 1;
				tmp = tmp->next;
    	}

    	return 0;
}

/* 
 * Name: addToList()
 *
 * Parameters:
 *   - a: an integer to add to the list
 *   - pl: contains the address of the list
 *
 * Returns: void
 *
 * Description: creates a new entry for the list, and assigns a as
 * the (integer) key to it. Adds the entry to the head of the list.
 */
void addToList(int a, struct listType **pl) 
{
    	struct listType *newEntry = NULL;

    	newEntry = (struct listType *)malloc(sizeof(struct listType));

    	if(newEntry == NULL) 
		{
			printf("Error: malloc() returned NULL.\n");
			return;
    	}

    	newEntry->i = a; /* Same as (*newEntry).i = a; */

    	/* Add at head of list */
    	newEntry->next = *pl;    /* Same as (*newEntry).next =  ... */
    	*pl = newEntry;
}


/*
 * Name: removeFromList()
 *
 * Parameters:
 *   - i: index of the item to remove
 *   - pl: address of the list
 *
 * Returns: void
 *
 * Description: removes the ith entry in the list. The first entry is
 * at index 0. Returns -1 on error, and > -1 otherwise.
 */
void removeFromList(int i, struct listType **pl) 
{
    	if(*pl == NULL) 
		{
			/* printf("List is empty!\n"); */
			return;
    	}

    	if(i < 0) 
		{
			/* printf("Negative index!\n"); */
			return;
    	}

    	struct listType *toBeDel, *prev;

    	toBeDel = *pl;
    	prev = NULL;
    	int j = 0;

    	while(j < i && toBeDel != NULL) 
		{
			prev = toBeDel;
			toBeDel = toBeDel->next;
			j++;
    	}

    	if(toBeDel == NULL) 
		{
			/* printf("Index %d too high.\n", i); */
			return;
    	}

    	if(prev != NULL) 
		{
			prev->next = toBeDel->next;
    	}
    	else 
		{
			*pl = toBeDel->next;
    	}

    	free((void *)toBeDel);
    	return;
}

/*
 * Name: skipToNextLineOfInput()
 *
 * Parameters:
 *   - none.
 *
 * Returns: void
 *
 * Description: skips this line of input, onto next line.
 *
 * WARNING: this method is not robust.
 */
void skipToNextLineOfInput() 
{
    char c;

    while(scanf("%c", &c) > 0) 
	{
		if(c == '\n') return;
    	}
    
    /* Unexpected input. Return quietly */
}

/*
 * Name: freeAllLists()
 *
 * Paramaters:
 *   - al: adj list in which lists are to be freed.
 *   - nVerts: # vertices in al.
 *
 * Returns: void
 *
 * Description: frees all adjacency lists in al.
 */
void freeAllLists(struct listType **al, int nVerts) 
{
    	if(nVerts <= 0 || al == NULL) 
	{
	/* Nothing to do. */
	return;
    	}

    	int i = 0;
    	while(i < nVerts) 
	{
		while(al[i] != NULL) 
		{
	    		/*
	    		printf("Removing from list %d, item with key %d\n", 
		    		i, (al[i])->i); fflush(stdout);
	    		*/
	    		removeFromList(0, &(al[i]));
		}
		i++;
    	}
}

/*
 * Name: readInteger()
 *
 * Paramaters:
 *   - c: character already read from stdin.
 *   - i: pointer to integer into which we read.
 *   - permissive: whether integer being read can be terminated
 *                 by \n only (not permissive) or by \n, space or \t.
 *
 * Returns: int
 *   - -1 on error. >= 0 otherwise.
 *
 * Description: reads in an integer.
 */
int readInteger(char c, int *i, int permissive) {
    	*i = 0;

    	/* Skip leading white spaces */
		while(c == ' ' || c == '\t') 
		{
		if(scanf("%c", &c) > 0)
		{
			if(permissive && c == '\n')
				return -1; //command s needs two integers 
			continue;
		}
		else 
		{
			/* Error! */
			return -2;
		}
    	}	

    	while(c >= '0' && c <= '9') 
		{
		*i = 10*(*i) + (int)(c - '0');
		if(scanf("%c", &c) > 0) 
		{
			if(permissive && c == '\n')
				return -1; //command s needs two integers 

			if(c== '\n') 
				return 0;

			//represents out of a pair of <>
			if(c== '>')
				return 1; 
		}
		else 
		{
			/* Error! */
			return -3;
		}
    	}

    	if(permissive && (c == ' ' || c == '\t' || c==',')) 
		{
			return 0;
    	}
		else if(permissive && (c == '\n'))
			return -1;
    	else 
		{
			/* Error */
			return -4;
    	}
}

/*
 * Name: addEdges()
 *
 * Paramaters:
 *   - v: the two vertices that comprise the edge
 *   - al: adj list to which to add edges.
 *   - nVerts: total # of vertices in al
 *
 * Returns: int
 *   - -1 on error, >= 0 otherwise.
 *
 * Description: adds two edges, v[0] -> v[1] and v[1] -> v[0] to
 *              the graph represented by al.
 */
int addEdges(int v[2], struct listType **al, int nVerts) {
    if(v[0] < 0 || v[1] < 0 || nVerts < 0) {
		/* Error! */
		return -1;
    }

    if(v[0] >= nVerts || v[1] >= nVerts) {
		/* Error! */
		return -1;
    }

    if(findInList(al[v[0]], v[1]) || findInList(al[v[1]], v[0])) {
		printf("Error: Edge being added already exists.\n");
		return -1;
    }

    addToList(v[1], &(al[v[0]]));
    addToList(v[0], &(al[v[1]]));
}

/***********************END OF FILE*************************/
