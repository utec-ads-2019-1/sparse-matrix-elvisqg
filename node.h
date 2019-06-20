#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

#include <vector>

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *ptrNext, *ptrDown;
    
public:
  explicit Node() {
    ptrNext = nullptr;
   	ptrDown = nullptr;
  }
  friend class Matrix<T>;
};

/*template <typename T>
class rootNode : public Node<T> {
protected:
	unsigned numRows, numColumns;
  vector<Node<T>*> *rows, *columns;

public:
	rootNode(unsigned rows, unsigned columns)	{
		numRows = rows;
		numColumns = columns;
		rows = nullptr;
		columns = nullptr;
	}
};*/

template <typename T>
class headNode : public Node<T> {
protected:
	unsigned pos;
  unsigned numElemnts;
public:
	headNode(unsigned pos_p)	{ 
    pos = pos_p;
    numElemnts = 0;
  }
};

template <typename T>
class elementNode : public Node<T> {
protected:
	
  unsigned posX, posY;
  T data;
public:
	elementNode(T data_p, unsigned posX_p, unsigned posY_p)	{ 
    data = data_p;
    posX = posX_p;
    posY = posY_p;
  }
};
#endif //SPARSE_MATRIX_NODE_Htemplate <typename T>
