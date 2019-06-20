#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include <iostream>
#include "node.h"

using namespace std;

template <typename T>
class Matrix {   
public:
  Matrix(unsigned rows, unsigned columns)	{
    //root = new rootNode<T>(rows, columns);
    numRows = rows;
    numColumns = columns;
    for (unsigned it = 0; it < rows || it < columns; it++) {        
      if (it < rows)  {
        headNode<T> *row = new headNode<T>(it);
        hRows.push_back(row);
      }  
      if (it < columns) {
        headNode<T> *column = new headNode<T>(it);  
        hColumns.push_back(column);
      }
    }
  }

  Node<T>* findNode(unsigned row, unsigned column)	{
    Node<T> *tempNode = nullptr;
    tempNode = hRows[row]->ptrNext;
    while (!tempNode) {
      if (tempNode -> posX == row && tempNode -> posY == column) return tempNode;
      tempNode = tempNode -> ptrNext; 
    }
    return nullptr;
  }

  void set(unsigned posX, unsigned posY, T data ) {
    //if (posX > root -> numColumns ||  posY > root -> numColumns) throw "OUT OF RANGE";
    if (posX > hRows.size() ||  posY > hColumns.size()) throw "OUT OF RANGE";
    if (findNode(posX, posY) == nullptr) return;
    elemenNode<T> element(data, posX,posY);
    Node<T> *currentNode = hRows[posX];
    Node<T> *prevNode = hRows[posX];
    while (1) {
      if (!(currentNode -> ptrNext)) {
        hRows[posX] -> ptrNext = &element;
        break;
      }  
      currentNode = currentNode -> ptrNext;
      while (!currentNode) {
        if (currentNode -> posY < posY && currentNode -> ptrNext == nullptr)  {
          currentNode -> ptrNext = &element;
          break;
        }  
        if (currentNode -> posY > posY) {
          element.ptrNext = currentNode;
          prevNode -> ptrNext = &element;
          break;    
        }
        prevNode = currentNode;
        currentNode = currentNode -> ptrNext;
      }
    }  
    currentNode = hColumns[posY];
    prevNode = hColumns[posY];
    while (1) {
      if (!(currentNode -> ptrDown)) hRows(posX) -> ptrDown = &element;
      currentNode = currentNode -> ptrDown;
      while (!currentNode) {
        if (currentNode -> posY < posY && currentNode -> ptrDown == nullptr)  {
          currentNode -> ptrDown = &element;
          break;
        }  
        if (currentNode -> posY > posY) {
          element.ptrDown = currentNode;
          prevNode -> ptrDown = &element;
          break;    
        }
        prevNode = currentNode;
        currentNode = currentNode -> ptrDown;
      }
    }

  }

  T operator()(unsigned posX, unsigned posY) const {
    if (posX > hRows.size() ||  posY > hColumns.size()) throw "OUT OF RANGE";
    Node<T> *tempNode = nullptr;
    tempNode = findNode(posX, posY);
    if (!tempNode) return 0;
    return tempNode->data;   
  }

  Matrix<T> operator*(T scalar) const {
    Matrix<T> matrixResult(numRows, numColumns);
    Node<T> *tempNode = nullptr;
    for (int row = 0; row < hRows.size(); row++) {
      tempNode = hRows[row] -> ptrNext;
      unsigned column = 0;
      while (!tempNode) {
        matrixResult.set(row, column, (tempNode -> data) * scalar);
        tempNode = tempNode -> ptrNext;  
        column++;
      }
    }
    return matrixResult;
  }

 /* Matrix<T> operator*(Matrix<T> other) const  {
    if (numRows !=  )
  }*/

  Matrix<T> operator+(Matrix<T> other) const {
    Matrix<T> matrixResult(numRows, numColumns);
    if (hRows.size() == other.hRows.size() && hColumns.size() == other.hColumns.size()) throw "THEY CAN'T BE OPERATED ";
    Node<T> *tempM1 = nullptr;
    Node<T> *tempM2 = nullptr; 
    for (int row = 0; row < hRows.size(); row++) {
      tempM1 = hRows[row] -> ptrNext;
      tempM2 = other[row].hRows -> ptrNext;
      unsigned column = 0;
      while (!tempM1){
        matrixResult.set(row, column, tempM1 -> data + tempM2 -> data);
        tempM1 = tempM1 -> ptrNext;
        tempM2 = tempM2 -> ptrNext;
        column++; 
      }
    }
    return matrixResult;
  }

  Matrix<T> operator-(Matrix<T> other) const {
    Matrix<T> matrixResult(numRows, numColumns);
    if (hRows.size() == other.hRows.size() && hColumns.size() == other.hColumns.size()) throw "THEY CAN'T BE OPERATED ";
    Node<T> *tempM1 = nullptr;
    Node<T> *tempM2 = nullptr; 
    for (int row = 0; row < hRows.size(); row++) {
      tempM1 = hRows[row] -> ptrNext;
      tempM2 = other.hRows[row] -> ptrNext;
      unsigned column = 0;
      while (!tempM1){
        matrixResult.set(row, column, tempM1 -> data - tempM2 -> data);
        tempM1 = tempM1 -> ptrNext;
        tempM2 = tempM2 -> ptrNext;
        column++; 
      }
    }
    return matrixResult;
  }
  Matrix<T> transpose() const {
    Matrix<T> matrixResult(numColumns, numRows);
    Node<T> *tempNode = nullptr;
    for (int column = 0; column < hRows.size(); column++) {
      tempNode = hColumns -> ptrNext;
      unsigned row = 0;
      while (!tempNode) {
        matrixResult.set(row, column, tempNode -> data);
        tempNode = tempNode -> ptrNext;  
        column++;
      }
    }
    return matrixResult;
  }

  void print() const  {
    for (int row = 0; row < hRows.size(); row++){
      for (int column = 0; column < hColumns.size(); column++){
        cout << (*this)(row,column)<<" ";
      }
      cout << "\n";
    }
  }
    
  void clearList(Node<T> *&tempNode)  {
    if(tempNode->ptrNext)  clearList(tempNode->ptrNext);
    delete tempNode;
  }
    
  ~Matrix() {
    Node<T> *tempNode = nullptr;
    for (int row = 0; row < numRows; row++){
      tempNode = hColumns[row] -> ptrNext;
      if (!tempNode -> ptrNext) break;
      clearList(tempNode);
    }
    hRows.clear();
    hColumns.clear();
  } 

private:
  //rootNode<T> *root;
  vector<headNode<T>*> hRows, hColumns;
  unsigned numRows, numColumns;
};

#endif //SPARSE_MATRIX_MATRIX_H