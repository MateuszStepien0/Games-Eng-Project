#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include <vector>
#include <functional>
#include <limits>
#include <cmath>
#include <mutex>

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

template <typename Data, typename Container, typename Predicate>
class MyPriorityQueue : public std::priority_queue<Data, Container, Predicate>
{
public:
	// std::priority_queue has two useful members:
	// 1. this->c is the underlying container of a priority_queue
	// 2. this->comp is the comparison predicate
	void reorder()
	{

		// std::make_heap rearranges the elements in the range [first,last] in such 
		//  a way that they form a heap.
		// std::begin() returns an iterator to the beginning of the given container c 		
		std::make_heap(std::begin(this->c), std::end(this->c), this->comp);
	}
};

template <class NodeType, class ArcType>
class NodeSearchCostComparer
{
public:
	typedef GraphNode<NodeType, ArcType> Node;
	bool operator()(Node* n1, Node* n2)
	{
		//return (n1->m_data.m_pathCost + n1->m_data.m_heuristicCost) 
		//		> (n2->m_data.m_pathCost + n2->m_data.m_heuristicCost);
		return n1->m_data.m_pathCost > n2->m_data.m_pathCost;
	}
};

// ---------------------------------------------------------------------
//  Name:           Graph
//  Description:    Manages the nodes and connections (arc) between them
// ---------------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph 
{
public:        
	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;

    // Constructor and destructor functions
    Graph( int size );
    ~Graph();

    // Accessors
    Node * nodeIndex(int index) const 
	{
		return m_nodes.at(index);
    }

    // Public member functions.
    bool addNode( NodeType data, int index );
    void removeNode( int index );
    bool addArc(int from, int to, ArcType weight);
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );        
    void clearMarks();
	void clearPrevious();
    void depthFirst( Node* node, std::function<void(Node *)> f_visit);
    void breadthFirst( Node* node, std::function<void(Node *)> f_visit);
	void adaptedBreadthFirst( Node* current, Node* goal );	
	void ucs(Node* start, Node* dest, std::function<void(Node*)> f_visit,
		std::vector<Node*>& path);
	void aStar(Node* start, Node* dest, std::vector<Node*>& path);
	int nodeDist(Node* node1, Node* node2);

	std::vector<Node*> m_nodes;
	std::mutex mutex;
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph( int maxNodes ) : m_nodes( maxNodes, nullptr) 
{  
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
   for( int index = 0; index < m_nodes.size(); index++ ) 
   {
        if( m_nodes[index] != nullptr ) 
		{
			delete m_nodes.at(index);
        }
   }
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode( NodeType data, int index ) 
{
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( nullptr == m_nodes.at(index) ) 
   {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
	  m_nodes.at(index) = new Node;
	  m_nodes.at(index)->m_data = data;
	  m_nodes.at(index)->setMarked(false);
    }
        
    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode( int index ) 
{
     // Only proceed if node does exist.
     if( nullptr != m_nodes.at(index) ) 
	 {
         // now find every arc that points to the node that
         // is being removed and remove it.        
         Arc* arc;

         // loop through every node
         for( int node = 0; node < m_nodes.size(); node++ ) 
		 {
              // if the node is valid...
              if( nullptr != m_nodes.at(node) ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_nodes.at(node)->getArc(m_nodes.at(index) );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_nodes.at(index);
		m_nodes.at(index) = nullptr;       
    }
}

// ----------------------------------------------------------------
//  Name:           addArd
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) {
     bool proceed = true; 

     // make sure both nodes exist.
     if( nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to) ) 
	 {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if( m_nodes.at(from)->getArc( m_nodes.at(to) ) != nullptr ) 
	 {
         proceed = false;
     }

     if (proceed == true) 
	 {
        // add the arc to the "from" node.
		 m_nodes.at(from)->addArc(m_nodes.at(to), weight );
     }
        
     return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc( int from, int to ) 
{
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to) ) 
	 {
         nodeExists = false;
     }

     if (nodeExists == true) 
	 {
        // remove the arc.
		 m_nodes.at(from)->removeArc(m_nodes.at(to) );
     }
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc( int from, int to ) 
{
     Arc* arc = 0;
     // make sure the to and from nodes exist
     if( nullptr != m_nodes.at(from) && nullptr != m_nodes.at(to) ) 
	 {
         arc = m_nodes.at(from)->getArc(m_nodes.at(to) );
     }
                
     return arc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() 
{
     for (int index = 0; index < m_nodes.size(); index++) 
	 {
          if (nullptr != m_nodes.at(index)) 
		  {
			  m_nodes.at(index)->setMarked(false);
          }
     }
}

template<class NodeType, class ArcType>
inline void Graph<NodeType, ArcType>::clearPrevious()
{
	for (int index = 0; index < m_nodes.size(); index++)
	{
		if (nullptr != m_nodes.at(index))
		{
			m_nodes.at(index)->setPrevious(nullptr);
		}
	}
}


// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst( Node* node, std::function<void(Node *)> f_visit ) 
{
     if( nullptr != node ) {
           // process the current node and mark it
           f_visit( node );
           node->setMarked(true);

           // go through each connecting node
           auto iter = node->arcList().begin();
           auto endIter = node->arcList().end();
        
		   for( ; iter != endIter; ++iter) 
		   {
			    // process the linked node if it isn't already marked.
                if ( (*iter).node()->marked() == false ) 
				{
                   depthFirst( (*iter).node(), f_visit);
                }            
           }
     }
}


// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a depth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst( Node* node, std::function<void(Node *)> f_visit) 
{
   if( nullptr != node ) 
   {
	  std::queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( node );
      node->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 ) 
	  {
         // process the node at the front of the queue.
		 f_visit( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         auto iter = nodeQueue.front()->arcList().begin();
         auto endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) 
		 {
              if ( (*iter).node()->marked() == false) 
			  {
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}


// ----------------------------------------------------------------
//  Name:           adaptedBreadthFirst
//  Description:    Performs a breadth-first traversal the starting node
//                  specified as an input parameter, terminating at the goal.
//  Arguments:      The first parameter is the starting node.
//                  The second parameter is the goal node.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::adaptedBreadthFirst( Node* current, Node *goal ) 
{
	bool goalReached = false;

	if (current != nullptr)
	{
		std::queue<Node*> nodeQueue;

		nodeQueue.push(current);
		current->setMarked(true);

		while (nodeQueue.size() != 0 && !goalReached)
		{
			auto iter = nodeQueue.front()->arcList().begin();
			auto endIter = nodeQueue.front()->arcList().end();

			for (; iter != endIter && !goalReached; iter++)
			{
				if ((*iter).node() == goal)
				{
					goalReached = true;
					(*iter).node()->setPrevious(nodeQueue.front());
				}
				else if ((*iter).node()->marked() == false)
				{
					(*iter).node()->setPrevious(nodeQueue.front());
					(*iter).node()->setMarked(true);
					nodeQueue.push((*iter).node());
				}
			}

			nodeQueue.pop();
		}
	}
}

template<class NodeType, class ArcType>
inline void Graph<NodeType, ArcType>::ucs(Node* start, Node* dest, std::function<void(Node*)> f_visit, std::vector<Node*>& path)
{
	std::priority_queue<Node*, std::vector<Node*>, NodeSearchCostComparer<NodeType, ArcType>> pq;

	for (int i = 0; i < m_nodes.size(); i++)
	{
		m_nodes.at(i)->m_data.m_pathCost = std::numeric_limits<int>::max();
	}

	start->m_data.m_pathCost = 0;
	pq.push(start);
	start->setMarked(true);

	while (!pq.empty() && pq.top() != dest)
	{
		auto iter = pq.top()->arcList().begin();
		auto endIter = pq.top()->arcList().end();

		for (; iter != endIter; iter++)
		{
			if ((*iter).node() != pq.top()->previous())
			{
				Arc arc = (*iter);
				int dist = arc.weight() + pq.top()->m_data.m_pathCost;

				if (dist < arc.node()->m_data.m_pathCost)
				{
					arc.node()->m_data.m_pathCost = dist;
					arc.node()->setPrevious(pq.top());
				}
				if (!arc.node()->marked())
				{
					arc.node()->setMarked(true);
					pq.push(arc.node());
				}
			}
		}
		pq.pop();
	}
}

template<class NodeType, class ArcType>
inline void Graph<NodeType, ArcType>::aStar(Node* start, Node* dest, std::vector<Node*>& path)
{
	std::priority_queue<Node*, std::vector<Node*>, NodeSearchCostComparer<NodeType, ArcType>> pq;

	for (int i = 0; i < m_nodes.size(); i++)
	{
		mutex.lock();
		m_nodes.at(i)->m_data.m_pathCost = std::numeric_limits<int>::max();
		m_nodes.at(i)->m_data.m_heuristicCost = Graph::nodeDist(m_nodes.at(i), dest);
		mutex.unlock();
	}

	mutex.lock();
	start->m_data.m_pathCost = 0;

	pq.push(start);

	start->setMarked(true);
	mutex.unlock();

	while (!pq.empty() && pq.top() != dest)
	{
		auto iter = pq.top()->arcList().begin();
		auto endIter = pq.top()->arcList().end();

		for (; iter != endIter; iter++)
		{
			if ((*iter).node() == pq.top()->previous())
			{
				continue;
			}

			Arc arc = (*iter);
			if (arc.node()->m_data.m_isWalkable == true || arc.node() == dest)
			{
				mutex.lock();
				int dist = arc.weight() + pq.top()->m_data.m_pathCost;
				mutex.unlock();

				if (dist < arc.node()->m_data.m_pathCost)
				{
					mutex.lock();
					arc.node()->m_data.m_pathCost = dist;
					arc.node()->setPrevious(pq.top());
					mutex.unlock();
				}
				if (!arc.node()->marked())
				{
					mutex.lock();
					arc.node()->setMarked(true);
					arc.node()->m_data.m_pathCost += 10;
					mutex.unlock();

					pq.push(arc.node());
				}
			}
		}
		pq.pop();
	}

	Node* currentNode = dest;

	while (currentNode->previous() != nullptr)
	{
		path.push_back(currentNode);
		currentNode = currentNode->previous();
	}

	clearMarks();
	clearPrevious();
}

template<class NodeType, class ArcType>
inline int Graph<NodeType, ArcType>::nodeDist(Node* node1, Node* node2)
{
	return std::abs(std::sqrt(
		((node2->m_data.m_pos.x - node1->m_data.m_pos.x) * (node2->m_data.m_pos.x - node1->m_data.m_pos.x)) +
		((node2->m_data.m_pos.y - node1->m_data.m_pos.y) * (node2->m_data.m_pos.y - node1->m_data.m_pos.y))));
}

#include "GraphNode.h"
#include "GraphArc.h"


#endif
