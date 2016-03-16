#ifndef skei_associative_array_included
#define skei_associative_array_included
//----------------------------------------------------------------------

/*
  based on:
    http://www.codeproject.com/Articles/149879/Associative-Array-in-C
    by Arnav Mukhopadhyay
*/

//----------------------------------------------------------------------

/*

#include <iostream>
#include <vector>
using namespace std;

//----------

template <class _T>
class SAssociativeArray {
  private:
    typedef struct _Data {
      T data;
      string name;
    } Data ;
    vector<Data> stack;
  public:
    long Size() {
      return stack.size();
    }
    bool IsItem(string name) {
      for(int i=0; i<Size(); i++) {
        if(stack[i].name == name) return true;
      }
      return false;
    }
    bool AddItem(string name, T data) {
      if (IsItem(name)) return false;
      Data d;
      d.name = name;
      d.data = data;
      stack.push_back(d);
      return true;
    }
    T& operator [] (string name) {
      for(int i=0; i<Size(); i++) {
        if (stack[i].name == name) return stack[i].data;
      }
      long idx = Size();
      Data d;
      d.name = name;
      stack.push_back(d);
      return stack[idx].data;
    }
    string GetItemName(long index) {
      if (index<0) index = 0;
      for(int i=0; i<Size(); i++) if (i == index) return stack[i].name;
      return "";
    }
    T& operator [] (long index) {
      if (index < 0) index = 0;
      for (int i=0; i<Size(); i++) {
        if (i == index) return stack[i].data;
      }
      return stack[0].data;
    }
};

//----------------------------------------------------------------------

// usage

//  Declare the class to be used with int data type
// AssocArray<int>   arr;

// Now add an element to the array using function
// arr.AddItem("firstElement", 100);

// Now add an element using [] operator
// arr["secondElement"] = 25;

// Now add another element
// arr["otherElement"] = 56;

// Here is how to access the element "firstElement" for any other purpose
// cout<<"firstElement = " << arr["firstElement"] << endl;

// Here is how to access using index
// cout<<"Element 1= " << arr[1] << endl;

// Other functions
// arr.IsItem("some element name") will return true if that item is on the array list or else false
// arr.GetItemName(2) will return the name of the element corresponding to index 2
// arr.Size()   will return the number of items saved on the list

*/

//----------------------------------------------------------------------
#endif
