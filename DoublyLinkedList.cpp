#include <iostream>
#include <vector>
#include <unistd.h>

using namespace std;

//Creating the node structure
struct Node {
    int data;
    Node* prev;
    Node* next;
};

//Creating DLL class
class DoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    //Constructor initializing head and tail
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    //Funtion to insert value at the end of the list
    void insertEnd(int value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->prev = nullptr;
        newNode->next = nullptr;

        //Checking if the List is empty or not
        if (head == nullptr) { 
            head = newNode;
            tail = newNode;
        } 
        //If the list is not empty, linking the new node to the tail
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    //Function to insert value at specific position in the list
    void insertAt(int position, int value) {
        //Checking whether the given position is valid or not
        if (position < 0) {
            cout << "Invalid position" << endl;
            return;
        }

        Node* newNode = new Node;
        newNode->data = value;
        newNode->prev = nullptr;
        newNode->next = nullptr;

        //If the position is '0', newNode to be inserted at the beginning
        if (position == 0) {
            newNode->next = head;
            //If the list is not empty, Link the prev of head to newNode
            if (head != nullptr) {
                head->prev = newNode;
            }
            head = newNode;     //If the list is empty, newNode is the head
            if (tail == nullptr) {
                tail = newNode;
            }
            return;
        }
        
        //Creating a duplication of head for traversing till given position
        Node* current = head;
        int currentPosition = 0;

        while (current != nullptr && currentPosition < position - 1) {
            current = current->next;
            currentPosition++;
        }
        
        //Checking whether the given position is valid
        if (current == nullptr) {
            cout << "Invalid position" << endl;
            return;
        }
        
        //Placing the newNode in between the current Node and its next
        newNode->next = current->next;
        if (current->next != nullptr) {
            current->next->prev = newNode;      //Linking prev pointer
        }
        current->next = newNode;
        newNode->prev = current;

        //Checking the current node is the tail and adding newNode at the end of the list
        if (current == tail) {
            tail = newNode;
        }
    }

    //Function to delete a value at specific position in the List
    void deleteAt(int position) {
        //Checking valid position
        if (position < 0) {
            cout << "Invalid position" << endl;
            return;
        }

        //Checking whether the List is empty
        if (head == nullptr) {
            cout << "List is empty" << endl;
            return;
        }

        //If the given position is head
        if (position == 0) {
            Node* temp = head;          //Creating a duplicate of head
            head = head->next;          //Changing the head 
            if (head != nullptr) {
                head->prev = nullptr;   //Changed head node's prev pointer to be nullptr
            }
            delete temp;                //Deleting the duplicate created
            return;
        }

        Node* current = head;
        int currentPosition = 0;

        //Traversing through the list till given position
        while (current != nullptr && currentPosition < position) {
            current = current->next;
            currentPosition++;
        }

        //Checking given position is null 
        if (current == nullptr) {
            cout << "Invalid position" << endl;
            return;
        }

        //Checking whether the given position is the tail of the list
        if (current == tail) {
            tail = current->prev;   //the tail pointer is updated to point to the previous node
            tail->next = nullptr;   //Updating next pointer of tail node
            delete current;         //Removing current from the list
        }
        //If the given postion is in between the list
        //Adjusting the pointers of adjacent nodes of current node
        else {
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
        }
    }

    // Function to display list in forward direction
    void displayForward() {
        Node* current = head;               //duplication of head Node for traversal
        while (current != nullptr) {
            cout << current->data << " ";   //Printing the current Node data
            current = current->next;        //Traversing to next point
        }
        cout << endl;
    }

    //Function to display list in backward direction
    void displayBackward() {
        Node* current = tail;               //duplication of tail Node for traversal
        while (current != nullptr) {
            cout << current->data << " ";   //Printing the current Node data
            current = current->prev;        //Traversing to previous point
        }
        cout << endl;
    }
};

int main() {
    DoublyLinkedList dll;

    int choice;
    int value, position;

    while (true) {
        system("clear");

        // Display the menu for the user in console
        cout << "Doubly Linked List Operations:" << endl;
        cout << "1. Insert at end" << endl;
        cout << "2. Insert at specific position" << endl;
        cout << "3. Delete at specific position" << endl;
        cout << "4. Display forward" << endl;
        cout << "5. Display backward" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value to insert at end: ";
                cin >> value;
                dll.insertEnd(value);
                cout<<"List : ";
                dll.displayForward();
                break;
            case 2:
                cout << "Enter value and position to insert: ";
                cin >> value >> position;
                dll.insertAt(position, value);
                cout<<"List : ";
                dll.displayForward();
                break;
            case 3:
                cout << "Enter position to delete: ";
                cin >> position;
                dll.deleteAt(position);
                cout<<"List : ";
                dll.displayForward();
                break;
            case 4:
                cout << "List forward: ";
                dll.displayForward();
                break;
            case 5:
                cout << "List backward: ";
                dll.displayBackward();
                break;
            case 6:
                cout << "Thank You & Visit Again" << endl;
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get(); //To make the console wait for the user to read
    }

    return 0;
}





