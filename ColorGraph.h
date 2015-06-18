#ifndef __team_project__ColorGraph__
#define __team_project__ColorGraph__
#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "LinkedGraph.h"
#include "BinarySearchTree.h"
#include "Vertex.h"
#include "Edge.h"
#include "LinkedStack.h"

template <class LabelType>
class ColorGraph: public LinkedGraph<LabelType>
{
private:
    LinkedStack<LabelType> removal_stack; // the linked stack that keeps track of removals (for undoing)
    vector<Edge<LabelType>> colorGraph;
    int color_count = 4; // the number of colors to try to solve the color map problem (WILL BE UPDATED LATER)
    bool isSafe(Vertex<LabelType> currVertex, int c);
    bool colorCheck(Vertex<LabelType> currVertex, int i);
public:
    ColorGraph(){} // not sure what is needed here during construction
 //   ~ColorGraph(){}
    bool graphColoring(); // backtracking algorithm answers problem better
  //  bool add(LabelType start, LabelType end, int edgeWeight = 0);
    bool remove(LabelType start, LabelType end);
    bool undoRemove();
  //  void printColorMap();
  //  void setNumColor(int c){color_count = c;}
  //  int getNumColor(){return color_count;}
};

/* A utility function to check if the current color assignment
 is safe for vertex v */
template <class LabelType>
bool ColorGraph<LabelType>::isSafe(Vertex<LabelType> currVertex, int c)
{
    for (int i = 0; i < this->numberOfVertices; i++){
      //  if (/*vertex*/ && c == color[i]) // color[i] is not used in this program,
        // possibly change to check if the same color is used for any adjacent vertices
            return false;
    }
    return true;
}
                 
/* A recursive utility function to solve m coloring problem */
template <class LabelType>
bool ColorGraph<LabelType>::colorCheck(Vertex<LabelType> currVertex, int i)
{
        /* base case: If all vertices are assigned a color then
         return true */
       // if (currVertex == this->numberOfVertices) // not sure what this checks exactly, possibly change to check if no vertex has a color of 0
            return true;
        
        /* Consider this vertex v and try different colors */
        for (int c = 1; c <= color_count; c++)
        {
            /* Check if assignment of color c to v is fine*/
            if (isSafe(currVertex, c))
            {
               // color[/*vertex*/] = c;
                currVertex.setColor(c);
                
                /* recur to assign colors to rest of the vertices */
                if ((i+1) < colorGraph.size()){ // make sure it doesn't go past the amount of vertices
                if (colorCheck (colorGraph[i+1], (i+1)) == true)
                    return true;
                }
                
                /* If assigning color c doesn't lead to a solution
                 then remove it */
               // color[/*vertex*/] = 0;
                currVertex.setColor(0);
            }
        }
        
        /* If no color can be assigned to this vertex then return false */
        return false;
}
                 
/* This function solves the m Coloring problem using Backtracking.
It mainly uses graphColoringUtil() to solve the problem. It returns
false if the m colors cannot be assigned, otherwise return true and
prints assignments of colors to all vertices. Please note that there
may be more than one solutions, this function prints one of the
feasible solutions.*/
template <class LabelType>
bool ColorGraph<LabelType>::graphColoring()
{
    this->unvisitVertices(); // reset this graph
    while (*this->pvertexIterator.hasNext()){
        colorGraph.push(*this->pvertexIterator->next());
    }
        // Initialize all color values as 0. This initialization is needed
        // correct functioning of isSafe()
      /*  int *color = new int[this->numberOfVertices];
        for (int i = 0; i < this->numberOfVertices; i++)
            color[i] = 0;*/
    for (int i = 0; i < colorGraph.size(); i++){
        colorGraph[i].setColor(0);
    }
    
    // ***** iterate through vector to apply intial color to each vertex
    
//  Vertex<LabelType> currVertex = *this->pvertexIterator->next();
    // change this so that each vertex in the map gets a color of 0 initially
    
    // **** use a for loop to color check each vertex
    
        // Call graphColoringUtil() for vertex 0
        if (colorCheck(colorGraph[0], 0) == false)
        {
            cout << "Problem cannot be solved.\n";
            return false;
        }
        return true;
}

/*
 template <class LabelType>
 void Kruskal<LabelType>::applyKruskal()
 {
	if (minSpanTree.size() > 0)
	{
 minSpanTree.clear();
	}
	unvisitVertices(); // reset this graph
	int numEdges = orderedEdges.size();
	int edgeCount = 0;
	for (int i = 0; i < numEdges && edgeCount < numberOfVertices-1; ++i)
	{
 for (int i = 0; i < minSpanTree.size(); ++i){ // reset for each edge
 minSpanTree[i].setChecked(false);
 }
 
 KruskalEdge<LabelType> *currEdge = &orderedEdges[i];
 LabelType start = currEdge->getStart(); // get name of start of edge
 LabelType end = currEdge->getEnd(); // get name of end of edge
 Vertex<LabelType>* startVertex = 0, *endVertex=0;
 
 startVertex = vertices.getItem(start); // get start Vertex from graph
 endVertex = vertices.getItem(end); // get end Vertex from graph
 if ( REMOVED )
{
    // NOT SHOWN
    
}

}
}
*/

template<class LabelType>
bool ColorGraph<LabelType>::remove(LabelType start, LabelType end)
{
    bool successful = false;
    Vertex<LabelType>* startVertex = this->vertices.getItem(start);
    Vertex<LabelType>* endVertex   = this->vertices.getItem(end);
    
    
    successful = startVertex->disconnect(end);
    if (successful)
    {
        successful = endVertex->disconnect(start);
        if (successful)
        {
            this->numberOfEdges--;
            removal_stack.push(start);
            removal_stack.push(end);
            // If either vertex no longer has a neighbor, remove it
            if (startVertex->getNumberOfNeighbors() == 0)
            {
                this->vertices.remove(start);
                --this->numberOfVertices; // UPDATED 06/11/2015
                delete startVertex; // UPDATED 06/08/2015
            }
            
            if (endVertex->getNumberOfNeighbors() == 0)
            {
                this->vertices.remove(end);
                --this->numberOfVertices; // UPDATED 06/11/2015
                delete endVertex;  // UPDATED 06/08/2015
            }
        }
        else
            successful = false; // Failed disconnect from endVertex
    }
    else
        successful = false;    // Failed disconnect from startVertex
    
    return successful;
}  // end remove

template <class LabelType>
bool ColorGraph<LabelType>::undoRemove()
{
    if (!removal_stack.isEmpty()){
    LabelType start = removal_stack.peek();
        removal_stack.pop();
    LabelType end = removal_stack.peek();
        removal_stack.pop();
    this->add(start, end);
        return true;
    }
    else
        return false;
}

/*template <class LabelType>
void ColorGraph<LabelType>::printColorMap()
{
     use iterator here?
}*/

#endif
