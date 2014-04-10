#include <iostream>
#include <new>
#include <cassert>

using namespace std;


/** \brief Template class to represent a Set of elements of type T
 *
 * Set is implemented as a sorted doubly linked list
 * \invariant Sets should not contain repeatitions, i.e.
 * two objects with the same pos cannot belong to a Set
 *
 * \param T is the type of the objects (elements) stored in the set
 * \note All Set operations have a linear complexity, in the worst case
 */

template<typename T>
class Set
{
private:
    
    /** \brief Class Node
     *
     * This class represents an internal node of a doubly linked list storing objects of type T
     * \param T is the type of the object (element) stored in a Node
     * \note All members of class Node are public
     * but only class Set can access them, since Node is declared in the private part of class Set
     *
     */
    

    
    
public:

    class Node
    {
    public:
        /** \brief Constructor
         *
         * \param nodeVal an object of type T whose copy will be stored in the Node
         * \param nextPtr a pointer to the next Node in the list
         * \param prevPtr a pointer to the previous Node in the list
         * \pre Type T should support the following operations:
         *  default constructor T(), operator<<, operator<=,
         *
         */
        //Constructor
        Node (T nodeVal = T(), Node* nextPtr = nullptr, Node* prevPtr = nullptr)
        : pos (nodeVal), next (nextPtr), prev(prevPtr)
        {  };
        
        //Data members
        T pos;    /**< Object (element) stored in the Node */
        Node* next; /**< Pointer to the next Node */
        Node* prev; /**< Pointer to the previous Node */
    };
    
    /** \brief Default constructor
     *
     * Create an empty Set
     * \param No arguments
     *
     */
    //Default constructor
    Set ();
    
    
    /** \brief Conversion constructor
     *
     * Convert an object val into a singleton {val}
     * \param val an object of type T
     *
     */
    //Conversion constructor
    Set (T val);
    
    
    
    /** \brief Copy constructor
     *
     * Create a new Set as a copy of Set b
     * \param b Set to be copied
     * \note This function does not modify Set b in any way
     *
     */
    //Copy constructor
    Set (const Set& b);
    
    
    /** \brief Destructor
     *
     * Deallocate all memory (Nodes) allocated by the constructor
     * \param No arguments
     *
     */
    //Destructor
    ~Set ();
    

    
    /** \brief Test whether the Set is empty
     *
     * \note This function does not modify the Set in any way
     * \return true if the set is empty, otherwise false
     *
     */
    //Test whether a set is empty
    bool is_empty () const;
    
    

    //Make the set empty
    void clear();
    

    void add(T pos);
    void remove();
    Node *head;  /**< Pointer to the dummy header Node */
    Node *tail;  /**< Pointer to the dummy tail Node */
    int counter; /**< Count number of elements in the Set */
    
    
private:
    
    
    //Private member functions
    
    /** \brief Insert a new Node storing val before the Node pointed by p
     *
     * \param p pointer to a Node
     * \param val pos to be inserted  before position p
     * \return *this
     *
     */
    //Insert a new Node storing val before the Node pointed by p
    Set& insert(Node *p, T val);
    
    
    /** \brief Remove the Node pointed by p
     *
     * \param p pointer to a Node
     * \return *this
     *
     */
    //Remove the Node pointed by p
    Set& erase(Node *p);
    
    
    /** \brief Initialize the Set to an empty Set
     *
     * \return None
     *
     */
    //Initialize the Set to an empty Set
    void init();
    
    
    /** \brief Display the Set to ostream os
     *
     * \note This function does not modify *this in any way
     * \return None
     *
     */
    //Display the Set to ostream os
    void print(ostream& os) const;
  
    
    /** \brief Overloaded operator<<
     *
     * \param os ostream object where characteres are inserted
     * \param b Set to be displayed
     * \return The same as parameter os
     *
     */
    friend ostream& operator<< (ostream& os, const Set<T>& b)
    {
        if (b.is_empty ())
            os << "Set is empty!" << endl;
        else
        {
            b.print(os); //call the private function Set<T>::print
        }
        
        return os;
    }
    
    
};


/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

#ifndef DOXYGEN_SHOULD_SKIP_THIS

//Default constructor
template<typename T>
Set<T>::Set ()
{
    init();
}


//conversion constructor
template<typename T>
Set<T>::Set (T n)
{
    init();
    Node *p = head->next;
    
    while(p != tail && p->pos < n)
        p = p->next;
    
    if(p == tail || p->pos != n)
        insert(p,n);
    
    //ADD CODE
}



//Copy constructor
template<typename T>
Set<T>::Set (const Set& b)
{
    init();
    Node *foo = b.head->next;
    
    while(foo != b.tail)
    {
        insert(tail, foo->pos);
        foo = foo->next;
    }
}


//Destructor
template<typename T>
Set<T>::~Set ()
{
    
    //ADD CODE

    Node* temp;
    while (head != NULL)
    {
        temp = head->next;
        delete head;
        head = temp;
    }
}


//Test whether a set is empty
template<typename T>
bool Set<T>::is_empty () const
{
    return(head->next==tail);
}



//Make the set empty
template<typename T>
void Set<T>::clear()
{
    Node *p = head->next;
    while(p != tail)
    {
        head->next = p->next;
        erase(p);
        p = head->next;
    }
}



/****************************************************
 * Private member functions                          *
 *****************************************************/

//Insert a new Node storing val before the Node pointed by p
template<typename T>
Set<T>& Set<T>::insert(Node *p, T val)
{
    Node* newNode = new (nothrow) Node(val, p, p->prev);
    p->prev = p->prev->next = newNode;
    assert(p->prev != nullptr);
    
    counter++;
    return *this;
}


//Delete the Node pointed by p
template<typename T>
Set<T>& Set<T>::erase(Node *p)
{
    //ADD CODE
    p->prev->next = p->next;
    p->next->prev = p->prev;
    
    delete p;
    counter--;
    return *this;
}

//Create an empty Set
template<typename T>
void Set<T>::init()
{
    //ADD CODE
    
    head = new Node();
    tail = new Node();
    
    assert (head!= nullptr && tail != nullptr);
    
    head->next = tail;
    tail->prev = head;
    
    counter = 0;
}


//Display all elements in the Set
template<typename T>
void Set<T>::print(ostream& os) const
{
    for(Node *p = head->next; p != tail; p = p->next)
        os << "X: " << p->pos.x << " Y: " << p->pos.y << " Z: " << p->pos.z << endl;
    
    os << endl;
    //ADD CODE
}





template<typename T>
void Set<T>::add(T pos)
{
    insert(tail, pos);
}

template<typename T>
void Set<T>::remove()
{
    erase(head->next);
}


#endif /* DOXYGEN_SHOULD_SKIP_THIS */

