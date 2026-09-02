#include <iostream>

int main() {
    std::cout << "Hello, C++!" << std::endl;
    return 0;
}

/***
 * pointers in c++
 * a pointer = variable that stores the memory address what does it mean?  of another variable
 * normal variable => int a = 10; // a is a variable that stores the value 10
 * pointer variable => int* p = &a; // p is a pointer that stores the address of a  and if i have added (* so thats mean this is a pointer
 * and type of pointer will remain same as that variable ) (& = operator hai used for address of VARiable)
 * for eg address of a = 0 x 100(this is just an example address)
 * 
 * now 
 * count << a // 10 
 * count << &a // 0 x 100 (address of a)
 * count << p // 0 x 100
 * count << *p // 10 // value at address
 * 
 * 
 * changing the value of a using pointer
 *  
 * int x = 10 
 * int* p = &x; // p is a pointer that stores the address of x
 * *p = 20; // changing the value of x using pointer p
 * count << x // 20
 * 
 * --- p+1 or p+2 means next integer address, it doesn't mean add 1 or 2 to the value
 * int a[] = {10,20,30}
 * int* p = &a[0]; // p is a pointer that stores the address of the first element of array a
 * count << *p // 10
 * count << *(p+1) // 20
 * count << *(p+2) // 30
 * 
 * a[i] == *(a + i)
 * 
 * 
 * call by value and call by reference 
 * 
 * call by value = copies the values from actual parameters to the formal parameters 
 * void test (inta, intt b){ //a and b are formal parameters
 * } 
 *
 * 
 * int main(){
 * int x = 10, y = 20;
 * test(x,y); // copies the values of x and y to a and b  // here x and y are actual parameter
 * } 
 * call by value - original parameter ki value change nahi hoti hai
 * call by reference = copies the address of actual parameters to the formal parameters and actual parameter ki 
 * value change ho jati hai
 * 
 * for example 
 * 
 * void test(int& a, int b){ // a and b are formal parameters
 * a = a + 10;
 * b = b + 20;
 * cout<< "a = " << a << " b = " << b << endl; // a= 20 b= 40
 * }
 *  
 * int main(){
 * int x = 10, y = 20;
 * test(x,y); // copies the address of x and y to a and b  // here x and y are actual parameter
 * cout << "x = " << x << " y = " << y << endl; // x= 20 y= 20 //y remains same because y copy by value horh 
 * }
 * 
 * //now how to do using pointers - so we opass the address of variable
 * void test(int* a, int b){ // a and b are formal parameters
 *  *a = *a + 10; // dereferencing the pointer to get the value and add 10
 *  b = b + 20;
 *  cout<< "a = " << *a << " b = " << b << endl; // a= 20 b= 40
 * }
 * 
 * int main(){
 * int x = 10, y = 20;
 * test(&x,y); // copies the address of x and y to a and b  // here x and y are actual parameter
 * cout << "x = " << x << " y = " << y << endl; // x= 20 y= 20 //y remains same because y copy by value horh 
 * }
 * 
 * 
 * 
 * 
 * lets quickly complete first programming fundamentals and oop
 * 
 * 
 * int x = 5
 * int *p = &x 
 * *p += 3
 * cout << x // 8 
 * 
 * 
 * important key points 
 * *p + 1      // value + 1
*(p + 1)    // next element's value
(*p)++      // increment VALUE
p++         // increment POINTER

Expression	Reads as	What it does	Result	Changes?
*p + 1	(*p) + 1	Dereference, then add 1 to the value	11	nothing modified
*(p + 1)	explicit parens	Add 1 to the pointer (moves sizeof(int) bytes), then dereference — same as p[1]	20	nothing modified
(*p)++	parens force deref first	Post-increment the pointed-to value	yields 10 (old), then a[0] becomes 11	a[0] changes

Post-increment means:
Use current p
Then move p
p++	—	Post-increment the pointer itself — now points at a[1]	yields old p	p changes, data untouched
*(p++); // Post-increment the pointer, then dereference the old address — same as *(p); p++;	yields 10 (old), then p points at a[1]	p changes, data untouched

Expression	1st prints	2nd prints	Combined
*(p++)	10	20	1020
*(++p)	20	20	2020

 */


