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

  elementNode<T>* findNode(unsigned row, unsigned column) const	{
    elementNode<T> *tempNode = nullptr;
    tempNode = (elementNode<T>*)hRows[row] -> ptrNext;
    while (!tempNode) {
      if (tempNode -> posX == row && tempNode -> posY == column) return tempNode;
      tempNode = (elementNode<T>*)tempNode -> ptrNext; 
    }
    return nullptr;
  }

  void set(unsigned posX, unsigned posY, T data ) {
    //if (posX > root -> numColumns ||  posY > root -> numColumns) throw "OUT OF RANGE";
    if (posX > hRows.size() ||  posY > hColumns.size()) throw "OUT OF RANGE";
    if (findNode(posX, posY) == nullptr) return;
    elementNode<T> element(data, posX,posY);
    headNode<T> *header = hRows[posX];
    elementNode<T> *currentNode = (elementNode<T>*)hRows[posX] -> ptrNext;
    elementNode<T> *prevNode = (elementNode<T>*) hRows[posX] -> ptrNext ;
    while (1) {
      if (!(header -> ptrNext)) {
        hRows[posX] -> ptrNext = &element;
        break;
      }  
      currentNode = (elementNode<T>*)currentNode -> ptrNext;
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
        header = hRows[posY];
        prevNode = (elementNode<T>*)currentNode;
        currentNode = (elementNode<T>*)currentNode -> ptrNext;
      }
    }  
    currentNode = (elementNode<T>*)hColumns[posY] -> ptrNext;
    prevNode = (elementNode<T>*)hColumns[posY] -> ptrNext;
    while (1) {
      if (!(currentNode -> ptrDown)) hRows[posX] -> ptrDown = &element;
      currentNode = (elementNode<T>*)currentNode -> ptrDown;
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
        currentNode = (elementNode<T>*)currentNode -> ptrDown;
      }
    }

  }

  T operator()(unsigned posX, unsigned posY) const {
    if (posX > hRows.size() ||  posY > hColumns.size()) throw "OUT OF RANGE";
    elementNode<T> *tempNode = nullptr;
    tempNode = findNode(posX, posY);
    if (!tempNode) return 0;
    return tempNode->data;   
  }

  Matrix<T> operator*(T scalar) const {
    Matrix<T> matrixResult(numRows, numColumns);
    elementNode<T> *tempNode = nullptr;
    for (int row = 0; row < hRows.size(); row++) {
      tempNode =(elementNode<T>*)hRows[row] -> ptrNext;
      unsigned column = 0;
      while (!tempNode) {
        matrixResult.set(row, column, (tempNode -> data) * scalar);
        tempNode = (elementNode<T>*)tempNode -> ptrNext;  
        column++;
      }
    }
    return matrixResult;
  }

 Matrix<T> operator*(Matrix<T> other) const  {
    if (numRows != other.numColumns) throw "THEY CAN'T BE OPERATED ";
    Matrix<T> matrixResult(numRows, other.numColumns);

    return  matrixResult;
  }

  Matrix<T> operator+(Matrix<T> other) const {
    Matrix<T> matrixResult(numRows, numColumns);
    if (hRows.size() == other.hRows.size() && hColumns.size() == other.hColumns.size()) throw "THEY CAN'T BE OPERATED ";
    elementNode<T> *tempM1 = nullptr;
    elementNode<T> *tempM2 = nullptr; 
    for (int row = 0; row < hRows.size(); row++) {
      tempM1 = (elementNode<T>*)hRows[row] -> ptrNext;
      tempM2 = (elementNode<T>*)other.hRows[row] -> ptrNext;
      unsigned column = 0;
      while (!tempM1){
        matrixResult.set(row, column, tempM1 -> data + tempM2 -> data);
        tempM1 = (elementNode<T>*)tempM1 -> ptrNext;
        tempM2 = (elementNode<T>*)tempM2 -> ptrNext;
        column++; 
      }
    }
    return matrixResult;
  }

  Matrix<T> operator-(Matrix<T> other) const {
    Matrix<T> matrixResult(numRows, numColumns);
    if (hRows.size() == other.hRows.size() && hColumns.size() == other.hColumns.size()) throw "THEY CAN'T BE OPERATED ";
    elementNode<T> *tempM1 = nullptr;
    elementNode<T> *tempM2 = nullptr; 
    for (int row = 0; row < hRows.size(); row++) {
      tempM1 = (elementNode<T>*)hRows[row] -> ptrNext;
      tempM2 = (elementNode<T>*)other.hRows[row] -> ptrNext;
      unsigned column = 0;
      while (!tempM1){
        matrixResult.set(row, column, tempM1 -> data - tempM2 -> data);
        tempM1 = (elementNode<T>*)tempM1 -> ptrNext;
        tempM2 = (elementNode<T>*)tempM2 -> ptrNext;
        column++; 
      }
    }
    return matrixResult;
  }
  Matrix<T> transpose() const {
    Matrix<T> matrixResult(numColumns, numRows);
    elementNode<T> *tempNode = nullptr;
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
    
  void clearList(elementNode<T> *&tempNode)  {
    if(tempNode->ptrNext)  clearList((elementNode<T>*&)tempNode->ptrNext);
    delete tempNode;
  }
    
  ~Matrix() {
    elementNode<T> *tempNode = nullptr;
    for (int row = 0; row < numRows; row++){
      tempNode = (elementNode<T>*)hRows[row] -> ptrNext;
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