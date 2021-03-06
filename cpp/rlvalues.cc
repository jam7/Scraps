#include <iostream>

const int GLOB = 0;

class T {
public:
  T(int t) : _t{t} { std::cout << "T()\n"; }
  ~T(void) { std::cout << "~T()\n"; }

  T(const T &t) : _t{t._t} { std::cout << "T(&)\n"; }
  T & operator=(const T &t) { std::cout << "=(T&)\n"; return *this; }
  // T(const T &t) = delete;
  // T & operator=(const T &t) = delete;

  T(T &&t) : _t{t._t} { std::cout << "T(&&)\n"; }
  T & operator=(T &&t) { std::cout << "=(T&&)\n"; return *this; }
  // T(T &&) = delete;
  // T & operator=(T &&) = delete;

private:
  int _t;
};

T getT(void)
{
  T t{1};
  return t;
}

void fun(T && t)
{
  // You want to use an rvalue reference argument when moving the value inside
  // the function, you can get away with a normal reference, but the type is
  // less descriptive (i.e., doesn't communicate the move to the caller).
  // T t2 = std::move(t);
  std::cout << "fun(T&&)\n";
  return;
}

// Note! RValue references must not be named! point of move( x ) is to remove
// name from scope to force a rvalue reference.
int main(void)
{
  // lvalue
  int a; // cell (s1) + capability to access it ('a').



  // rvalue
  (void) 1; // literal value 1, no capability to access it / identify it beyond
            // the semicolumn.



  // lvalue + rvalue
  a = 1; // store rvalue (1) into cell (s1) using capability 'a'.

  // Notes: values (rvalues) can be stored into a cell through a capability
  // (lvalue).



  // lvalue (pointers)
  int *b; // cell (s2 [for pointer val] + capability to access it ('b')
          // + 'derefernce' operation on capability
          // best to think of pointers as a type that stores a capability.
          // I.e., it reifies as a value a capability. So while 'a' is a
          // capabilty to access the cell s1, 'b' is a capability to access the
          // cell s2, which itself can store a capability.
          //
          // 'dereference' operation, invokes the capability stored in s2,
          // access the cell it identifies.

  // *b = 0 // error as we have yet to store a capability in cell s2.
  (void) &a; // reify capability 'a' into a rvalue.
  b = &a; // store capability 'a' into cell s2 using capability 'b'.

  // &9; // error as can't derive a new capability (lvalue) from an rvalue....
         // what cell would it be for?



  // lvalue references 
  int &c = a; // creates a new capability 'a' to access the cell s1
              // identified by capability 'a'.
  // int &c; // error as need to refer to an existing capability when creating
             // new one.
  int &c1 = c; // create another capability 'c1' to access cell s1

  c = 0; // update cell (s1) to store the rvalue 0 using capability 'c'.
  a = 0; // equivalent to above, but using capability 'a'.

  // Note: the cell that a capability identifies can't be changed after
  // declaration. Pointers provide an ability to achieve operations that
  // utilize this feature, but they do so through a layer of indirection where
  // a capability has been reified into a rvalue stored in the cell that the
  // pointer type capability identifies.
  
  int d = a; // creates cell (s3) with capability 'd' to access it + stores a
             // copy of the rvalue in cell s1 referenced by capability 'a'.

  // lvalue references (more)  
  // int &e = 0; // error as 0 is an rvalue, and we need an lvalue to create a
                 // new capability, since what cell would 'e' be a capability
                 // for in this situation?
  
  const int &e = 0; // OK as since 'e' is a capability to only access (but not
                    // modify) a cell, we can create it from an rvalue.
                    // Why want? If the type was expensive to copy, this gives
                    // us a constant reference to a literal.

  // (int&) e = 1; // may work or fail depending on how the compiler allocated '0'.
  // const int &e = GLOB;
  // (int&) e = 1; // extremely likely to segfault as GLOB allocated in
                   // read-only text section.



  // rvalue references
  int &&f = 0; // create cell (s4) and capability to access it 'f' + store 0;
  f = 1;

  // rvalue references (move)
  int &&g = 1;
  int &h = g;
  int &&i = std::move(g);
  int &&j = 2;

  std::cout << "&g: " << &g << std::endl;
  std::cout << "&h: " << &h << std::endl;
  std::cout << "&i: " << &i << std::endl;

  std::cout << "j: " << j << std::endl;
  std::cout << "&j: " << &j << std::endl;

  j = std::move(g);

  std::cout << "j: " << j << std::endl;
  std::cout << "&j: " << &j << std::endl;

  // why rvalue references?
  T&& t = getT();

  std::cout << "end...\n";

  fun(std::move(t));

  return 0;
}

