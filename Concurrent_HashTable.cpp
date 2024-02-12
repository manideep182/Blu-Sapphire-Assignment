#include <iostream>
#include <vector>
#include <mutex>
#include <iomanip>
#include <unistd.h>
using namespace std;

template<typename Key, typename Value, size_t BucketSize = 10>
class ConcurrentHashMap {
private:
    // Creating HashMap structure
    struct Entry {
        Key key;
        Value value;
        Entry* next;
        //Constructor to initialize Entry object with  provided key and value 
        Entry(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };

    vector<mutex> mutexes;
    vector<Entry*> buckets;

    //This method calculates the hash value for the given key using 
    //hash function and % BucketSize to determine which bucket to place the entry into
    size_t hash(const Key& key) const {
        return std::hash<Key>{}(key) % BucketSize;
    }

public:
    //Constructor initializing the buckets and mutexes vectors with the specified bucket size
    ConcurrentHashMap() : buckets(BucketSize), mutexes(BucketSize) {}

    //Function to insert key-value pair into the map
    void put(const Key& key, const Value& value) {
        size_t idx = hash(key);                 //Hashing the given key using hash Function
        lock_guard<mutex> lock(mutexes[idx]);   //locking the mutex associated with the bucket at index idx
        Entry* curr = buckets[idx];             //Duplicating the head of the Linked List in buckets List
        while (curr) {
            //Checking if there is a key present in the List
            if (curr->key == key) {
                curr->value = value;            //Updating the existing value
                return;
            }
            curr = curr->next;
        }
        //If there is no entry
        curr = new Entry(key, value);   //Creating new entry object
        curr->next = buckets[idx];      //Linking the new entry to the head of bucket List
        buckets[idx] = curr;            //Updating the head
    }
    
    //Function to get the value associated with given Key
    bool get(const Key& key, Value& value) {
        size_t idx = hash(key);                 //Hashing the given key using hash Function
        lock_guard<mutex> lock(mutexes[idx]);   //locking the mutex associated with the bucket at index idx
        Entry* curr = buckets[idx];             //Duplicating the head of the Linked List in buckets List
        while (curr) {
            //Checking if there is a key present in the List
            if (curr->key == key) {
                value = curr->value;            //Storing the value of the given key 
                return true;
            }
            curr = curr->next;
        }
        return false;                           //If key not found return false
    }

    //Function to delete a key-value pair from the map
    bool remove(const Key& key) {
        size_t idx = hash(key);                 //Hashing the given key using hash Function
        lock_guard<mutex> lock(mutexes[idx]);   //locking the mutex associated with the bucket at index idx
        Entry* curr = buckets[idx];             //Duplicating the head of the Linked List in buckets List
        Entry* prev = nullptr;
        while (curr) {
            //Checking if there is a key present in the List
            if (curr->key == key) {
                //If key found removing the entry by adjusting the links
                if (prev) {                     //If prev is not nullptr, adjust the prev and next links of curr 
                    prev->next = curr->next;
                } 
                else {                          //If prev is nullptr, it means remove head
                    buckets[idx] = curr->next;
                }
                delete curr;                    //deallocating the memory
                return true;
            }
            //Updating the prev and curr pointers for traversal
            prev = curr;
            curr = curr->next;
        }
        return false;
    }
    
    //Function to display map contents
    void print() {
        //Iterating through each bucket in the HashMap
        for (size_t i = 0; i < BucketSize; ++i) {
            lock_guard<mutex> lock(mutexes[i]);             //locking the mutex associated with the current bucket at index i
            Entry* curr = buckets[i];                       //initializing curr pointer to the head of list in current bucket
            cout << "Bucket " << setw(2) << i << ": ";      //Printing the label of the buckets
            
            //Iterating through the list of the curent bucket
            while (curr) {
                cout << "{" << curr->key << ": " << curr->value << "}"; //Printing current node key-value pair
                curr = curr->next;                                      //Moving current pointer to the next node
                if (curr) cout << " -> ";                               //Prints an array if there is a next node present
            }
            cout << endl;
        }
    }
};

int main() {
    ConcurrentHashMap<string, string> map;

    //Displaying the menu
    while (true) {
        
        system("clear");    // Clearing the console for better readability
        
        cout << "Select an operation:\n"
                "1. Insert\n"
                "2. Remove\n"
                "3. Get\n"
                "4. Print Map Contents\n"
                "5. Exit\n";

        int choice;
        cout<<"Enter your choice: ";
        cin >> choice;
        cout<<endl;
        
        //To handle input other than int data type.
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the invalid input
            cout << "Invalid input. Please enter an integer.\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            continue;
        }
        
        //Calling the functions according to the choice using nested-if
        if (choice == 1) {
            string key;
            string value;
            cout << "Enter key and value to insert: ";
            cin >> key >> value;
            map.put(key, value);
        } else if (choice == 2) {
            string key;
            cout << "Enter key to remove: ";
            cin >> key;
            if (map.remove(key)) {
                cout << "\nKey '" << key << "' removed successfully.\n";
            } else {
                cout << "\nKey '" << key << "' not found.\n";
            }
        } else if (choice == 3) {
            string key;
            cout << "Enter key to get value: ";
            cin >> key;
            string value;
            if (map.get(key, value)) {
                cout << "\nValue of key '" << key << "': " << value << "\n";
            } else {
                cout << "\nKey '" << key << "' not found.\n";
            }
        } else if (choice == 4) {
            cout << "Map Contents:\n";
            map.print();
        } else if (choice == 5) {
            cout << "Exiting...\n";
            break;
        }  else {
            cout << "Invalid choice. Please enter a number between 1 and 5.\n";
        }
        
        
        cout<<"Press Enter to continue...";
        cin.ignore();
        cin.get();
        
    }

    return 0;
}




