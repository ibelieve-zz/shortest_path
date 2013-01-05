/*************************************************
 * File Name: shortestPath.c
 * Dependent file: function.h
 *
 * Author: Bing Lu
 * Contact: http://compiledreams.com
 *
 * Program function: user inputs vertex number and edges of undirected graph
 * and then input two vertexs,this program find out the shortest path between them 
 * if there is no path, reporting error
 *
 * Sample input:
 *
 * $ ./shortestPath 
 * V 5
 * E {<1,2>,<2,3>,<2,4>}
 * s 1 3
 * 1-2-3
 *
 ***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "function.h"

/*
 * Takes input till we see an EOF. Input lines may be:
 * 1. V int -- specifies a (new) set of vertices. We assume that the
 *               identities of the vertices are 0..<int>-1.
 * 2. E {<int,int>,<int,int>...} -- specifies edges in undirected graph
 * 3. s int int --specifies the origin and end point of a shortest path that user wants to know
 */
int main() {
    struct listType **adjList = NULL; 
	/* v is an array. Each entry in the array contains a list of
	 * type struct listType.
	 */

    int state = 0;
	/* To keep track of the current state of our state-machine.
	 * 0 -- means we are ready to read the 1st char of a line.
	 * 1 -- means we have read V, and are looking for # of vertices.
	 * 2 -- means we have read E, and are looking for 1st vert. in edge.
	 * 3 -- means we have read 1st vert of edge, and are looking for 2nd.
	 * 4 -- means we have read s, and looking for origin point the shortest path user wants to know
	 * 5 -- means we have read origin point and looking for the end point
	 */

    char c = '\0'; /* Input character */
    int nVerts = 0; /* To store # of vertices */
    int nEdges = 0; /* to store number of edges in graph*/
    int v[4] = {-1}; /* v[0] and v[1] to store two vertices that are to be an edge */
			/*v[3] and v[4] to store two vertices that are to be origin and end point of route*/
    int in_brace=0; //flag to show if we are in a pair of { }
    int in_bracket=0; //flag to show if we are in a pair fo < >
    int init_done=0; //flag to show if the initialization of graph is finished
    while(scanf("%c", &c) >= 1) 
	{
		if(state == 0) 
		{
			if(c == 'V') 
			{
				freeAllLists(adjList, nVerts);
				init_done = 0;
				if(adjList != NULL) 
				{
			    	free(adjList);
			    	adjList = NULL;
				}
				state = 1;
				nVerts = 0;
				continue;
		    }
		    
			else if(c == 'E') 
			{
				/* Have we read vertices yet? */
				if(adjList == NULL) 
				{
					printf("Error: need \'V\' specification first.\n"); fflush(stdout);
					skipToNextLineOfInput();
					continue;
				}
				init_done = 0;
				bzero(adjList,nVerts*sizeof(struct listType *));
				state = 2; continue;
			}
		    
			else if(c == 's') 
			{
				//have we read vertices yet?
				if(adjList == NULL)
				{
					printf("Error: need \'V\' specification first.\n"); fflush(stdout);
					skipToNextLineOfInput();
					continue;
				}
				//judge whether vertex and edges have been initiated				
				if(init_done!=1)
				{
					printf("Error: specifying edge first.\n"); fflush(stdout);
					state=2;
					continue;
				}
				state = 4;
				continue;
			}
			else 
			{
				/* Error! */
				printf("Error: Unrecognized command. Rejecting line.\n"); fflush(stdout);
				skipToNextLineOfInput();
				continue;
			}
		}
		else if(state == 1) 
		{
			if(readInteger(c, &nVerts, 0) < 0) 
			{
				printf("Error: Erroneous line. Rejecting it.\n"); fflush(stdout);
				skipToNextLineOfInput();
				state = 0;
				continue;
		    	}
	
			/* Allocate an nVerts sized array of (struct listType *) */
		 	adjList = (struct listType **)malloc(nVerts * sizeof(struct listType *));
		 	if(adjList == NULL) 
			{
				printf("Error: malloc() returned null.\n"); fflush(stdout);
				break;
		 	}
	
		 	bzero(adjList, nVerts*sizeof(struct listType *));
	
		 	state = 0; 
		 	continue;
		}
		else if(state == 2 || state == 3) 
		{
			//the ',' in '>,<',ignore it,do nothing until next loop	
			if(c==' '||c=='\t')
				continue;
			//when '{' appears, in_brace=1 to suggest we are in a { }
			if(c=='{')
			{
				in_brace=1;
				continue;
			}
			//when '}' appears, in_brace=0 to suggest we are out of the { }
			if(c=='}')
			{
				in_brace=0;
				continue;
			}
			//if in_brace=0 and c='\n', we arrive at the end of E command, 
			//so, transfer to state 0 and wait for new command
			if(in_brace==0&&c=='\n')
			{
				init_done=1; //the initialization of graph finishd
				state=0;
				continue;
			}
			if(in_brace&&c=='<')
			{
				in_bracket=1;
				continue;
			}
			if(in_bracket)
			{
				//if(readInteger(c, &(v[state - 2]), !(state - 2)) < 0) 
				int t=0;

				t=readInteger(c,&(v[state-2]), !(state-2));
				if(t<0)
				{
					printf("Error: Erroneous line. Rejecting it.\n"); fflush(stdout);
					skipToNextLineOfInput();
					state = 0;
					continue;
				}
				if(state == 2) 
				{
					state = 3; 
					continue;
				}
	
				/* Else -- state == 3 */
				if(v[0] == v[1]) 
				{
					printf("Error: Cannot add self-edge.\n"); fflush(stdout);
					state = 0;
					continue;
				}
		
				if(addEdges(v, adjList, nVerts) < 0) 
				{
					printf("Error: could not add edges.\n"); fflush(stdout);
				}
				
				if(t==1)
				{
					in_bracket=0;
					state=2;
					continue;
				}
			}
		}
		else if(state == 4 || state == 5) 
		{
			
			int t = readInteger(c, &(v[state - 2]), !(state - 4));
			if(t==-1)
			{
				printf("Error: command s needs two integers as begin and end point\n"); fflush(stdout);
				state = 0;
				continue;
			}
			if(t<0) 
			{
				printf("Error: Erroneous line. Rejecting it.\n"); fflush(stdout);
				skipToNextLineOfInput();
				state = 0;
				continue;
		    	}
	
			if(state == 4) 
			{
				state = 5; continue;
			}

			/* Else -- state == 5 */
			if(v[2] < 0 || v[3] < 0 || v[2] >= nVerts || v[3] >= nVerts) 
			{
				/* Error! */
				printf("Error: specifying invalid vertex\n"); fflush(stdout);
				state=0;
				continue;
			}
			//if begin and end are same vertice, obvious result
			if(v[2] == v[3]) 
			{
				printf("%d\n",v[2]);fflush(stdout);
				state = 0;
				continue;
			}
			
			
			NodeType *nodes=NULL;
			nodes=(NodeType *)malloc(sizeof(NodeType)*nVerts);

		    if(nodes == NULL) 
			{
				printf("Error: malloc() returned NULL.\n"); fflush(stdout);
				continue;
			}	
			
			EdgeType *edges=NULL;
			edges=(EdgeType *)malloc(sizeof(EdgeType)*(nVerts*(nVerts-1)/2));

			if(edges == NULL) 
			{
				printf("Error: malloc() returned NULL.\n"); fflush(stdout);
				continue;
			}
			//nEdges is the number of edges in the graph
			nEdges = GenEdge(adjList, nVerts, edges);
			
			//tempflag is a flag to show if the begin and end point are exchanged
			int tempflag=-1;
			if(v[2]>v[3])
			{	
				tempflag=v[2];
				v[2]=v[3];
				v[3]=tempflag;
			}
			
			//implementing Bellman-Ford algorithm
			BellmanFord(v[2], nVerts, nEdges, nodes, edges);
			//finding out the shortest path, if it exists
			GenPath(nodes, nVerts, v[2], v[3], tempflag);
			
			//release this area of memory
			free(nodes);
			nodes=NULL;
			free(edges);
			edges=NULL;
				
	    	state = 0;
		    continue;
		}
		else 
		{
		    /* Weird -- state should never be anything else */
		    /* Indicates a serious bug in the program */
	
		    printf("Error: state == %d unexpected.\n", state); fflush(stdout);
		    break;
		}
    }

    /* We read EOF, or there was some other input that made scanf() fail.
     * Exit.
     */

    //printf("Exiting...\n");
    return 0;
}

/*****************************END OF FILE*************************************/
