#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
using namespace std;

// ============================================================
// LINKED LIST - COMPLETE C++ IMPLEMENTATION
// Covers: Singly, Doubly and Circular Linked Lists
// ============================================================

// ============================================================
// 1. SINGLY LINKED LIST
// ============================================================
class SinglyLinkedList {
    struct Node {
        int data;
        Node* next;
        explicit Node(int value) : data(value), next(nullptr) {}
    };

    Node* head = nullptr;

public:
    ~SinglyLinkedList() { clear(); }

    bool empty() const { return head == nullptr; }

    int size() const {
        int count = 0;
        for (Node* cur = head; cur; cur = cur->next) ++count;
        return count;
    }

    void display() const {
        if (!head) {
            cout << "NULL\n";
            return;
        }
        for (Node* cur = head; cur; cur = cur->next)
            cout << cur->data << " -> ";
        cout << "NULL\n";
    }

    void insertFront(int value) {
        Node* node = new Node(value);
        node->next = head;
        head = node;
    }

    void insertBack(int value) {
        Node* node = new Node(value);
        if (!head) {
            head = node;
            return;
        }
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }

    // 0-based index. index == size() is allowed.
    bool insertAt(int index, int value) {
        if (index < 0 || index > size()) return false;
        if (index == 0) {
            insertFront(value);
            return true;
        }
        Node* cur = head;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        Node* node = new Node(value);
        node->next = cur->next;
        cur->next = node;
        return true;
    }

    bool deleteFront() {
        if (!head) return false;
        Node* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    bool deleteBack() {
        if (!head) return false;
        if (!head->next) return deleteFront();
        Node* cur = head;
        while (cur->next->next) cur = cur->next;
        delete cur->next;
        cur->next = nullptr;
        return true;
    }

    bool deleteAt(int index) {
        if (index < 0 || index >= size()) return false;
        if (index == 0) return deleteFront();
        Node* cur = head;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        Node* temp = cur->next;
        cur->next = temp->next;
        delete temp;
        return true;
    }

    bool deleteValue(int value) {
        if (!head) return false;
        if (head->data == value) return deleteFront();
        Node* cur = head;
        while (cur->next && cur->next->data != value) cur = cur->next;
        if (!cur->next) return false;
        Node* temp = cur->next;
        cur->next = temp->next;
        delete temp;
        return true;
    }

    int search(int value) const {
        int index = 0;
        for (Node* cur = head; cur; cur = cur->next, ++index)
            if (cur->data == value) return index;
        return -1;
    }

    bool update(int index, int value) {
        if (index < 0) return false;
        Node* cur = head;
        for (int i = 0; cur && i < index; ++i) cur = cur->next;
        if (!cur) return false;
        cur->data = value;
        return true;
    }

    void reverseIterative() {
        Node *prev = nullptr, *cur = head;
        while (cur) {
            Node* next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        head = prev;
    }

private:
    Node* reverseRecursive(Node* node) {
        if (!node || !node->next) return node;
        Node* newHead = reverseRecursive(node->next);
        node->next->next = node;
        node->next = nullptr;
        return newHead;
    }

public:
    void reverseRecursive() { head = reverseRecursive(head); }

    int middle() const {
        if (!head) throw runtime_error("List is empty");
        Node *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow->data; // second middle for even length
    }

    int nthFromEnd(int n) const {
        if (n <= 0) throw invalid_argument("n must be positive");
        Node *fast = head, *slow = head;
        for (int i = 0; i < n; ++i) {
            if (!fast) throw out_of_range("n is larger than list size");
            fast = fast->next;
        }
        while (fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow->data;
    }

    void removeDuplicatesUnsorted() {
        unordered_set<int> seen;
        Node* cur = head;
        Node* prev = nullptr;
        while (cur) {
            if (seen.count(cur->data)) {
                Node* temp = cur;
                cur = cur->next;
                prev->next = cur;
                delete temp;
            } else {
                seen.insert(cur->data);
                prev = cur;
                cur = cur->next;
            }
        }
    }

    // Assumes the list is sorted.
    void removeDuplicatesSorted() {
        Node* cur = head;
        while (cur && cur->next) {
            if (cur->data == cur->next->data) {
                Node* temp = cur->next;
                cur->next = temp->next;
                delete temp;
            } else {
                cur = cur->next;
            }
        }
    }

    void sortList() {
        vector<int> values;
        for (Node* cur = head; cur; cur = cur->next) values.push_back(cur->data);
        sort(values.begin(), values.end());
        Node* cur = head;
        for (int value : values) {
            cur->data = value;
            cur = cur->next;
        }
    }

    bool isPalindrome() const {
        vector<int> values;
        for (Node* cur = head; cur; cur = cur->next) values.push_back(cur->data);
        return equal(values.begin(), values.begin() + values.size() / 2, values.rbegin());
    }

    void clear() {
        while (head) deleteFront();
    }

    // Floyd's cycle detection. This demo helper intentionally creates a cycle.
    void createCycle(int position) {
        if (!head || position < 0) return;
        Node* cycleNode = nullptr;
        Node* tail = nullptr;
        int index = 0;
        for (Node* cur = head; cur; cur = cur->next, ++index) {
            if (index == position) cycleNode = cur;
            tail = cur;
        }
        if (tail && cycleNode) tail->next = cycleNode;
    }

    bool hasCycle() const {
        Node *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }

    void removeCycle() {
        Node *slow = head, *fast = head;
        bool found = false;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                found = true;
                break;
            }
        }
        if (!found) return;
        slow = head;
        if (slow == fast) {
            while (fast->next != slow) fast = fast->next;
        } else {
            while (slow->next != fast->next) {
                slow = slow->next;
                fast = fast->next;
            }
        }
        fast->next = nullptr;
    }
};

// ============================================================
// 2. DOUBLY LINKED LIST
// ============================================================
class DoublyLinkedList {
    struct Node {
        int data;
        Node *prev, *next;
        explicit Node(int value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    ~DoublyLinkedList() { clear(); }

    int size() const {
        int count = 0;
        for (Node* cur = head; cur; cur = cur->next) ++count;
        return count;
    }

    void displayForward() const {
        for (Node* cur = head; cur; cur = cur->next) cout << cur->data << " <-> ";
        cout << "NULL\n";
    }

    void displayBackward() const {
        for (Node* cur = tail; cur; cur = cur->prev) cout << cur->data << " <-> ";
        cout << "NULL\n";
    }

    void insertFront(int value) {
        Node* node = new Node(value);
        node->next = head;
        if (head) head->prev = node;
        else tail = node;
        head = node;
    }

    void insertBack(int value) {
        Node* node = new Node(value);
        node->prev = tail;
        if (tail) tail->next = node;
        else head = node;
        tail = node;
    }

    bool insertAt(int index, int value) {
        if (index < 0 || index > size()) return false;
        if (index == 0) { insertFront(value); return true; }
        if (index == size()) { insertBack(value); return true; }
        Node* cur = head;
        for (int i = 0; i < index; ++i) cur = cur->next;
        Node* node = new Node(value);
        node->prev = cur->prev;
        node->next = cur;
        cur->prev->next = node;
        cur->prev = node;
        return true;
    }

    bool deleteFront() {
        if (!head) return false;
        Node* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        return true;
    }

    bool deleteBack() {
        if (!tail) return false;
        Node* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        return true;
    }

    bool deleteAt(int index) {
        if (index < 0 || index >= size()) return false;
        if (index == 0) return deleteFront();
        if (index == size() - 1) return deleteBack();
        Node* cur = head;
        for (int i = 0; i < index; ++i) cur = cur->next;
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        delete cur;
        return true;
    }

    bool deleteValue(int value) {
        Node* cur = head;
        while (cur && cur->data != value) cur = cur->next;
        if (!cur) return false;
        if (cur == head) return deleteFront();
        if (cur == tail) return deleteBack();
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        delete cur;
        return true;
    }

    int search(int value) const {
        int index = 0;
        for (Node* cur = head; cur; cur = cur->next, ++index)
            if (cur->data == value) return index;
        return -1;
    }

    void reverse() {
        Node* cur = head;
        while (cur) {
            swap(cur->prev, cur->next);
            cur = cur->prev;
        }
        swap(head, tail);
    }

    void clear() {
        while (head) deleteFront();
    }
};

// ============================================================
// 3. CIRCULAR SINGLY LINKED LIST
// ============================================================
class CircularLinkedList {
    struct Node {
        int data;
        Node* next;
        explicit Node(int value) : data(value), next(nullptr) {}
    };

    Node* tail = nullptr;

public:
    ~CircularLinkedList() { clear(); }

    bool empty() const { return tail == nullptr; }

    int size() const {
        if (!tail) return 0;
        int count = 0;
        Node* cur = tail->next;
        do {
            ++count;
            cur = cur->next;
        } while (cur != tail->next);
        return count;
    }

    void display() const {
        if (!tail) {
            cout << "EMPTY\n";
            return;
        }
        Node* cur = tail->next;
        do {
            cout << cur->data << " -> ";
            cur = cur->next;
        } while (cur != tail->next);
        cout << "(back to head)\n";
    }

    void insertFront(int value) {
        Node* node = new Node(value);
        if (!tail) {
            tail = node;
            tail->next = tail;
        } else {
            node->next = tail->next;
            tail->next = node;
        }
    }

    void insertBack(int value) {
        insertFront(value);
        tail = tail->next;
    }

    bool deleteFront() {
        if (!tail) return false;
        Node* head = tail->next;
        if (head == tail) tail = nullptr;
        else tail->next = head->next;
        delete head;
        return true;
    }

    bool deleteBack() {
        if (!tail) return false;
        if (tail->next == tail) return deleteFront();
        Node* cur = tail->next;
        while (cur->next != tail) cur = cur->next;
        cur->next = tail->next;
        delete tail;
        tail = cur;
        return true;
    }

    bool deleteValue(int value) {
        if (!tail) return false;
        Node* prev = tail;
        Node* cur = tail->next;
        do {
            if (cur->data == value) {
                if (cur == tail && cur == tail->next) tail = nullptr;
                else {
                    prev->next = cur->next;
                    if (cur == tail) tail = prev;
                }
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        } while (cur != tail->next);
        return false;
    }

    void clear() {
        while (tail) deleteFront();
    }
};

// ============================================================
// DEMO / TEST
// ============================================================
int main() {
    cout << "===== SINGLY LINKED LIST =====\n";
    SinglyLinkedList sll;
    sll.insertBack(10);
    sll.insertBack(20);
    sll.insertBack(30);
    sll.insertFront(5);
    sll.insertAt(2, 15);
    sll.display();
    cout << "Size: " << sll.size() << "\n";
    cout << "Search 20: index " << sll.search(20) << "\n";
    cout << "Middle: " << sll.middle() << "\n";
    cout << "2nd from end: " << sll.nthFromEnd(2) << "\n";
    sll.update(1, 7);
    sll.deleteValue(30);
    sll.display();
    sll.reverseIterative();
    cout << "Reversed: ";
    sll.display();
    cout << "Palindrome: " << (sll.isPalindrome() ? "Yes" : "No") << "\n";

    cout << "\n===== DOUBLY LINKED LIST =====\n";
    DoublyLinkedList dll;
    dll.insertBack(10);
    dll.insertBack(20);
    dll.insertFront(5);
    dll.insertAt(2, 15);
    dll.displayForward();
    dll.displayBackward();
    dll.deleteAt(1);
    dll.reverse();
    cout << "After delete + reverse: ";
    dll.displayForward();

    cout << "\n===== CIRCULAR LINKED LIST =====\n";
    CircularLinkedList cll;
    cll.insertBack(10);
    cll.insertBack(20);
    cll.insertFront(5);
    cll.display();
    cll.deleteValue(20);
    cll.display();

    cout << "\nAll linked-list implementations executed successfully.\n";
    return 0;
}
