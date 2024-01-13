#include "main.h"

struct cusInfo {
    string name;
    int NUM;
    int RESULT;
    int ID;
    cusInfo(int RESULT, int ID, string name, int NUM = 1) {
        this->RESULT = RESULT;
        this->ID = ID;
        this->name = name;
        this->NUM = NUM;
    }
    ~cusInfo() {
        RESULT = 0;
        ID = 0;
        name = "";
        NUM = 0;
    }
};

class Code {
public:
    char character;
    string enCode;
public:
    Code(char ch, string c) {
        this->character = ch;
        this->enCode = c;
    }
    ~Code() {
        character = '\0';
        enCode = "";
    }
};

class HuffTreeNode {
public:
    char character;
    int freq;
    int appearance;
    HuffTreeNode* pLeft;
    HuffTreeNode* pRight;
    vector<Code*> info;
public:
    HuffTreeNode(char ch, int f, int appearance = 0, HuffTreeNode* pLeft = 0, HuffTreeNode* pRight = 0) {
        this->character = ch;
        this->freq = f;
        this->appearance = appearance;
        this->pLeft = pLeft;
        this->pRight = pRight;
    }
    ~HuffTreeNode() {
        pLeft = NULL;
        pRight = NULL;
        character = '\0';
        freq = 0;
        appearance = 0;
        for (int i = 0; i < info.size(); i++)
        {
            delete info[i];
            info[i] = NULL;
        }
    }

    void characterCodes(HuffTreeNode* root, string str)
    {
        HuffTreeNode* pRoot = root;
        if (!pRoot) return;

        if (pRoot->appearance == 0) {
            info.push_back(new Code(pRoot->character, str));
            //cout << root->character << " : " << str << endl;
        }

        characterCodes(pRoot->pLeft, str + "0");
        characterCodes(pRoot->pRight, str + "1");
    }

    int countNode(HuffTreeNode* root) {
        if (!root) return 0;
        int count = 0;
        queue<HuffTreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            HuffTreeNode* cur = q.front();
            q.pop();
            count++;

            if (cur->pLeft) q.push(cur->pLeft);
            if (cur->pRight) q.push(cur->pRight);
        }
        return count;
    }
};

template<class T>
class myMinHeap {
    // This class will be use for huffman tree(instead of using priority queue)
public:
    vector<T> elements;
public:
    myMinHeap() {}
    ~myMinHeap() {
        for (int i = 0; i < elements.size(); i++)
        {
            if (elements[i]) {
                delete elements[i];
            }
        }
    }

    void reheapUpHuff(int position) {
        // RE-HEAP UP FUNCTION
        if (position <= 0 || position >= int(elements.size())) return;
        int parent = (position - 1) / 2;
        if (compareHuff(position, parent)) {
            swap(elements[position], elements[parent]);
            reheapUpHuff(parent);
        }
    }

    void reheapDownHuff(int position) {
        // RE-HEAP DOWN FUNCTION
        if (position < 0 || position >= int(elements.size())) return;
        int leftChild = 2 * position + 1;
        int rightChild = 2 * position + 2;
        int last = int(elements.size()) - 1;

        if (leftChild <= last) {
            int smaller = leftChild;
            if (rightChild <= last)
                if (compareHuff(rightChild, leftChild)) smaller = rightChild;

            if (compareHuff(smaller, position)) {
                swap(elements[smaller], elements[position]);
                reheapDownHuff(smaller);
            }
        }
    }

    bool compareHuff(int child_1, int child_2) {
        // Case 1 : different frequency
        if (elements[child_1]->freq < elements[child_2]->freq) return 1;

        // Case 2: same frequency but different character
        if (elements[child_1]->freq == elements[child_2]->freq &&
            elements[child_1]->character < elements[child_2]->character) return 1;

        // Case 3: same frequency, same character but different appearance
        if (elements[child_1]->freq == elements[child_2]->freq &&
            elements[child_1]->character == elements[child_2]->character &&
            elements[child_1]->appearance < elements[child_2]->appearance) return 1;

        // Otherwise, return false
        return 0;
    }

    void pushHuff(T newNode) {
        // This function will add new element and then reheapUp
        elements.push_back(newNode);
        int last = int(elements.size()) - 1;
        reheapUpHuff(last);
    }

    T popHuff() {
        // This function will return the head
        T result = elements[0];
        elements[0] = elements[int(elements.size()) - 1];
        elements.pop_back();
        reheapDownHuff(0);
        return result;
    }

    int size() {
        // This function will return size of elements
        return int(elements.size());
    }
};

class myHash {
protected:
    cusInfo** table;
public:
    myHash() {
        this->table = new cusInfo * [MAXSIZE / 2];
        // Assigning every elements as NULL
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            table[i] = NULL;
        }

    }

    ~myHash() {
        this->clear();
    }

    bool isEmpty() {
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            if (table[i] != NULL) return 0;
        }
        return 1;
    }

    bool findName(string name) {
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            if (table[i] && table[i]->name == name) {
                return 1;
            }
        }
        return 0;
    }

    bool isFull() {
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            if (table[i] == NULL) return 0;
        }
        return 1;
    }


    void put(int key, cusInfo* newCustomer) {
        int index = key;
        if (table[index] == NULL) {
            // Assign table infomation
            table[index] = newCustomer;
        }
        // Linear probing
        else {
            while (table[index] != NULL) {
                index++;
                if (index >= (MAXSIZE / 2)) {
                    index = 0;
                }
            }
            table[index] = newCustomer;
        }
    }

    bool containsID(int customerID) {
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            if (table[i] && table[i]->ID == customerID) return 1;
        }
        return 0;
    }

    bool containsResult(int result) {
        if (table[result] != NULL) return 1;
        return 0;
    }

    void deleteInfo(int ID) {
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            if (table[i] && table[i]->ID == ID) {
                delete table[i];
                table[i] = NULL;
                break;
            }
        }
    }

    void clear() {
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            if (table[i] != NULL) {
                delete table[i];
            }
        }
        delete[] table;
    }

    void printHT() {
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            if (table[i] != NULL) {
                cout << table[i]->ID << "-" << table[i]->RESULT << "-" << table[i]->NUM << "\n";
            }
        }
    }

    void updateNUM(string name) {
        for (int i = 0; i < (MAXSIZE / 2); i++)
        {
            if (table[i] && table[i]->name == name) {
                table[i]->NUM++;
                return;
            }
        }
        return;
    }
};

class myAVL {
public:
    class AVLNode;
public:
    AVLNode* root;
    int count;
public:
    myAVL() {
        root = NULL;
        count = 0;
    }
    ~myAVL() {
        this->clear(this->root);
    }

    AVLNode* recursiveAdd(AVLNode* pRoot, AVLNode* newNode) {
        if (!pRoot) return newNode;
        if (newNode->customerInfo->RESULT < pRoot->customerInfo->RESULT)
            pRoot->pLeft = recursiveAdd(pRoot->pLeft, newNode);
        else
            pRoot->pRight = recursiveAdd(pRoot->pRight, newNode);
        // rebalance it
        return balance(pRoot);
    }

    void add(int result, int ID, string customerName) {
        // Function to add newNode
        if (count <= MAXSIZE / 2) {
            AVLNode* newNode = new AVLNode(new cusInfo(result, ID, customerName));
            this->root = this->recursiveAdd(root, newNode);
            count++;
        }
        return;
    }

    bool isFull() {
        return count == (MAXSIZE / 2);
    }

    int getHeight(AVLNode* pRoot) {
        // Function return height of the tree
        if (!pRoot) return 0;
        else {
            int lh = getHeight(pRoot->pLeft);
            int rh = getHeight(pRoot->pRight);
            return 1 + ((lh > rh) ? lh : rh);
        }
    }

    AVLNode* rotateLeft(AVLNode* pRoot)
    {
        AVLNode* newRoot = pRoot->pRight;
        pRoot->pRight = newRoot->pLeft;
        newRoot->pLeft = pRoot;
        return newRoot;
    }

    AVLNode* rotateRight(AVLNode* pRoot)
    {
        AVLNode* newRoot = pRoot->pLeft;
        pRoot->pLeft = newRoot->pRight;
        newRoot->pRight = pRoot;
        return newRoot;
    }

    int getBalance(AVLNode* pRoot) {
        return getHeight(pRoot->pLeft) - getHeight(pRoot->pRight);
    }

    AVLNode* balance(AVLNode* root)
    {
        // LH
        if (getBalance(root) > 1)
        {
            // RH of LH
            if (getBalance(root->pLeft) < 0) root->pLeft = rotateLeft(root->pLeft);
            // LH of LH and RH of LH
            return rotateRight(root);
        }
        // RH
        else if (getBalance(root) < -1)
        {
            // LH of RH
            if (getBalance(root->pRight) > 0) root->pRight = rotateRight(root->pRight);
            // RH of RH or RH of LH
            return rotateLeft(root);
        }
        return root;
    }

    AVLNode* removeHelper(AVLNode* pRoot, int value, int ID)
    {
        if (pRoot == NULL) return pRoot;
        if (pRoot->customerInfo->RESULT > value) pRoot->pLeft = removeHelper(pRoot->pLeft, value, ID);
        else if (pRoot->customerInfo->RESULT < value) pRoot->pRight = removeHelper(pRoot->pRight, value, ID);
        else
        {
            if (pRoot->customerInfo->ID == ID) {
                // Case 0 : leaf
                if (pRoot->pLeft == NULL && pRoot->pRight == NULL)
                {
                    delete pRoot;
                    pRoot = NULL;
                }
                // Case 1 : 1 child
                else if (pRoot->pLeft == NULL || pRoot->pRight == NULL)
                {
                    AVLNode* newRoot = NULL;
                    if (pRoot->pLeft != NULL)
                        newRoot = pRoot->pLeft;
                    else
                        newRoot = pRoot->pRight;

                    delete pRoot;
                    pRoot = NULL;
                    pRoot = newRoot;
                }
                else
                {
                    AVLNode* leastRight = pRoot->pRight;
                    while (leastRight->pLeft != NULL) leastRight = leastRight->pLeft;
                    // Swap data
                    pRoot->customerInfo->RESULT = leastRight->customerInfo->RESULT;
                    pRoot->customerInfo->ID = leastRight->customerInfo->ID;
                    pRoot->customerInfo->name = leastRight->customerInfo->name;
                    pRoot->customerInfo->NUM = leastRight->customerInfo->NUM;
                    pRoot->pRight = removeHelper(pRoot->pRight, leastRight->customerInfo->RESULT, leastRight->customerInfo->ID);
                }
            }
            else {
                pRoot->pLeft = removeHelper(pRoot->pLeft, value, ID);
                pRoot->pRight = removeHelper(pRoot->pRight, value, ID);
            }
        }
        if (pRoot == NULL) return pRoot;
        return balance(pRoot);
    }

    void remove(int RESULT, int ID) {
        root = removeHelper(root, RESULT, ID);
        count--;
    }

    bool containsID(int ID) {
        // Traverse by using BFT
        if (!root) return 0;
        queue<AVLNode*> q;
        q.push(root);
        while (!q.empty()) {
            AVLNode* cur = q.front();
            q.pop();
            // Checking...
            if (cur->customerInfo->ID == ID) return 1;

            if (cur->pLeft) q.push(cur->pLeft);
            if (cur->pRight) q.push(cur->pRight);
        }
        return 0;
    }

    bool containsResult(AVLNode* pRoot, int result) {
        if (!pRoot) return 0;
        if (pRoot->customerInfo->RESULT == result) return 1;
        else if (pRoot->customerInfo->RESULT < result) return containsResult(pRoot->pLeft, result);
        else return containsResult(pRoot->pRight, result);
    }

    void printAVL() {
        // Function to print
        if (!root) return;
        queue<AVLNode*> q;
        q.push(root);
        while (!q.empty()) {
            AVLNode* cur = q.front();
            q.pop();
            // print node
            cout << cur->customerInfo->ID << "-" << cur->customerInfo->RESULT << "-" << cur->customerInfo->NUM << "\n";

            if (cur->pLeft) q.push(cur->pLeft);
            if (cur->pRight) q.push(cur->pRight);
        }
    }

    bool findName(AVLNode* pRoot, int RESULT, string name) {
        if (!pRoot) return 0;
        if (pRoot->customerInfo->RESULT == RESULT) {
            if (pRoot->customerInfo->name == name) { return 1; }
            else {
                // Duyet tiep ben trai va ben phai
                if(findName(pRoot->pLeft, RESULT, name)) return 1;
                if(findName(pRoot->pRight, RESULT, name)) return 1;
                return 0;
            }
        }
        else if (pRoot->customerInfo->RESULT < RESULT) return findName(pRoot->pRight, RESULT, name);
        else return findName(pRoot->pLeft, RESULT, name);
        return 0;
    }

    void clear(AVLNode* pRoot) {
        if (!pRoot) return;
        if (pRoot->pLeft) clear(pRoot->pLeft);
        if (pRoot->pRight) clear(pRoot->pRight);
        delete pRoot;
        pRoot = NULL;
    }

    void updateNUM(AVLNode* pRoot, int RESULT, int ID) {
        if (!pRoot) return;
        if (pRoot->customerInfo->RESULT == RESULT) {
            if (pRoot->customerInfo->ID == ID) {
                pRoot->customerInfo->NUM++;
                return;
            }
            else {
                updateNUM(pRoot->pLeft, RESULT, ID);
                updateNUM(pRoot->pRight, RESULT, ID);
            }
        }
        else if (pRoot->customerInfo->RESULT < RESULT) updateNUM(pRoot->pRight, RESULT, ID);
        else return updateNUM(pRoot->pLeft, RESULT, ID);
    }

public:
    class AVLNode {
    public:
        cusInfo* customerInfo;
        AVLNode* pLeft;
        AVLNode* pRight;
    public:
        AVLNode(cusInfo* customer, AVLNode* pLeft = 0, AVLNode* pRight = 0) {
            this->customerInfo = customer;
            this->pLeft = pLeft;
            this->pRight = pRight;
        }
        ~AVLNode() {
            delete customerInfo;
        }
    };
};

class Node {
public:
    cusInfo* data;
    Node* next;
public:
    Node(cusInfo* data, Node* next = 0) {
        this->data = data;
        this->next = next;
    }
    ~Node() {
        next = NULL;
    }
};

class myList {
public:
    Node* head;
    Node* tail;
public:
    myList() {
        this->head = NULL;
        this->tail = NULL;
    }
    ~myList() {
        this->clear();
        this->head = NULL;
        this->tail = NULL;
    }

    // Add head
    void add(cusInfo* data) {
        Node* newNode = new Node(data);
        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    Node* pop() {
        Node* del = head;
        head = head->next;
        return del;
    }

    bool contains(cusInfo* item) {
        if (!head) return 0;
        Node* ptr = head;
        while (ptr) {
            if (ptr->data = item) return 1;
            ptr = ptr->next;
        }
        return 0;
    }

    void clear() {
        if (this->head) {
            Node* ptr = head;
            while (head) {
                ptr = head;
                head = head->next;
                if (ptr->data != 0) delete ptr->data;
                delete ptr;
            }
        }
    }

    Node* remove(string name) {
        if (!head) return NULL;
        Node* prev = NULL;
        Node* cur = head;
        while (cur) {
            if (cur->data->name == name) {
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        if (!cur) return NULL;
        // case 0: head
        if (!prev) {
            head = head->next;
            return cur;
        }
        // case 1: others
        prev->next = cur->next;
        if (cur == this->tail) this->tail = prev;
        return cur;
    }

    void addHead(cusInfo* data) {
        Node* newHead = new Node(data);
        if (!head) {
            head = tail = newHead;
        }
        else {
            newHead->next = head;
            head = newHead;
        }
    }

    void updateNUM(string name) {
        if (!head) return;
        Node* ptr = head;
        while (ptr) {
            if (ptr->data->name == name) {
                ptr->data->NUM++;
                break;
            }
            ptr = ptr->next;
        }
        return;
    }

    void print() {
        cout << "\nPRINT LIST :" << endl;
        Node* ptr = head;
        while (ptr) {
            cout << ptr->data->name << "-" << ptr->data->ID << "-" << ptr->data->RESULT << "-" << ptr->data->NUM << "\n";
            ptr = ptr->next;
        }
    }

    int indexOf(string name) {
        if (!head) return -1;
        Node* ptr = head;
        int idx = 0;
        while (ptr) {
            if (ptr->data->name == name) {
                return idx;
            }
            idx++;
            ptr = ptr->next;
        }
        return -1;
    }

    Node* pop_back() {
        Node* result = this->tail;
        Node* ptr = head;
        while (ptr->next->next) {
            ptr = ptr->next;
        }
        tail = ptr;
        ptr->next = NULL;
        return result;
    }

    bool isSooner(string name1, string name2) {
        if (indexOf(name1) == -1 || indexOf(name2) == -1) return 0;
        return indexOf(name1) < indexOf(name2);
    }

    int size() {
        int count = 0;
        if (!head) return count;
        Node* ptr = head;
        while (ptr) {
            count++;
            ptr = ptr->next;
        }
        return count;
    }
};

// Commands
bool checkREG(string s, string* customerName);
bool checkCLE(string s, int* customerNUM);
bool checkPrintHT(string s);
bool checkPrintAVL(string s);
bool checkPrintMH(string s);

// Others Function
vector<char> diffChar(string customerName);
vector<int> findFreq(vector<char> diff, string customerName);
HuffTreeNode* makeTree(vector<pair<int, char>> combination);
void returnEncode(HuffTreeNode* tree, string customerName, string* afterCode);
void returnDecimal(string binary, int* decimal);
int findingArea(int RESULT);
void deleteHuff(HuffTreeNode* root);
bool containInArea_1(myList* FIFO, myHash* table);
bool containInArea_2(myList* FIFO, myAVL* table);
bool differentChar(string customerName);

// Base information
string getCommand = "";
int customerID = 0;
string customerName = "";
int customerNUM = 0;

// Assigning Hash Table and AVL Table.
myHash* hashTable = new myHash();
myAVL* AVLTable = new myAVL();
myList* findFIFO = new myList();
myList* findLRCO = new myList();

class MHNode {
public:
    cusInfo* data;
    MHNode* pLeft;
    MHNode* pRight;
public:
    MHNode(cusInfo* data, MHNode* pLeft = 0, MHNode* pRight = 0) {
        this->data = data;
        this->pLeft = pLeft;
        this->pRight = pRight;
    }
    ~MHNode() {
        if (data != 0) delete data;
    }
};

class myMinHeap_v2 {
public:
    vector<cusInfo*> elements;
public:
    myMinHeap_v2() {}
    ~myMinHeap_v2() {
        for (int i = 0; i < elements.size(); i++)
        {
            delete elements[i];
        }
    }

    bool compare(int child_1, int child_2) {
        if (elements[child_1]->NUM < elements[child_2]->NUM) return 1;
        if (elements[child_1]->NUM == elements[child_2]->NUM &&
            findFIFO->isSooner(elements[child_1]->name, elements[child_2]->name)) return 1;
        return 0;
    }

    void reheapUp(int position) {
        if (position <= 0 || position >= int(elements.size())) return;
        int parent = (position - 1) / 2;
        if (compare(position, parent)) {
            swap(elements[position], elements[parent]);
            reheapUp(parent);
        }
    }

    void reheapDown(int position) {
        // RE-HEAP DOWN FUNCTION
        if (position < 0 || position >= int(elements.size())) return;
        int leftChild = 2 * position + 1;
        int rightChild = 2 * position + 2;
        int last = int(elements.size()) - 1;

        if (leftChild <= last) {
            int smaller = leftChild;
            if (rightChild <= last)
                if (compare(rightChild, leftChild)) smaller = rightChild;

            if (compare(smaller, position)) {
                swap(elements[smaller], elements[position]);
                reheapDown(smaller);
            }
        }
    }

    void push(cusInfo* newNode) {
        // This function will add new element and then reheapUp
        if (elements.size() > MAXSIZE) return;
        elements.push_back(newNode);
        int last = int(elements.size()) - 1;
        reheapUp(last);
    }

    cusInfo* pop() {
        // This function will return the head
        cusInfo* result = elements[0];
        elements[0] = elements[int(elements.size()) - 1];
        elements.pop_back();
        reheapDown(0);
        return result;
    }

    cusInfo* remove(string name) {
        cusInfo* result = elements[0];
        for (int i = 0; i < elements.size(); i++)
        {
            if (elements[i]->name == name) {
                result = elements[i];
                int last = int(elements.size()) - 1;
                elements[i] = elements[last];
                elements.pop_back();
                reheapDown(i);
                reheapUp(i);
                break;
            }
        }
        return result;
    }

    void printVec() {
        cout << "\nVector: \n";
        for (int i = 0; i < elements.size(); i++)
        {
            cout << elements[i]->RESULT << "-" << elements[i]->ID << "-" << elements[i]->NUM << endl;
        }
    }
};

class myBTree {
public:
    MHNode* root;
public:
    myBTree(){
        root = NULL;
    }
    ~myBTree() {
        this->deleteHTree(this->root);
    }

    void deleteHTree(MHNode* root) {
        // Function for deletion
        if (!root) return;
        if (root->pLeft) deleteHTree(root->pLeft);
        if (root->pRight) deleteHTree(root->pRight);
        delete root;
    }

    void printMH(MHNode* root) {
        // Print pre-order
        if (root) {
            cout << root->data->ID << "-" << root->data->NUM << "\n";
            printMH(root->pLeft);
            printMH(root->pRight);
        }
    }

    void buildTree(vector<cusInfo*> elements) {
        for (int i = 0; i < elements.size(); i++)
        {
            // Should create new instead of taking elements[i] (easy to delete, otherwise can cause error in deletion)
            cusInfo* newCustomer = new cusInfo(elements[i]->RESULT, elements[i]->ID, elements[i]->name, elements[i]->NUM);
            MHNode* newNode = new MHNode(newCustomer);
            // Adding nodes in BFT
            if (root == NULL) root = newNode;
            else {
                queue<MHNode*> q;
                q.push(root);
                while (!q.empty()) {
                    MHNode* cur = q.front();
                    q.pop();
                    // if cur->left is null, insert the newNode. Otherwise save it into queue
                    if (!cur->pLeft) {
                        cur->pLeft = newNode;
                        break;
                    }
                    else q.push(cur->pLeft);
                    // if cur->right is null, insert the newNode. Otherwise save it into queue
                    if (!cur->pRight) {
                        cur->pRight = newNode;
                        break;
                    }
                    else q.push(cur->pRight);
                }
            }
        }
    }
};

myMinHeap_v2* findLFCO = new myMinHeap_v2();

void simulate(string filename)
{
    ifstream myFile;
    myFile.open(filename);
    while (getline(myFile, getCommand)) {
        customerID = 0;
        customerName = "";
        customerNUM = 0;
        // REG command
        if (checkREG(getCommand, &customerName)) {
            // Get different characters and its frequency
            vector<char> diff = diffChar(customerName);
            vector<int> freq = findFreq(diff, customerName);

            // Create a vector contains pair of those
            vector<pair<int, char>> combine;
            for (int i = 0; i < int(diff.size()); i++)
                combine.push_back(make_pair(freq[i], diff[i]));
            sort(combine.begin(), combine.end());

            // Create a huff tree
            HuffTreeNode* tree = makeTree(combine);

            // Encoding characters
            if (differentChar(customerName)) {
                tree->characterCodes(tree, "1");
            }
            else {
                tree->characterCodes(tree, "");
            }

            // Encoding customer's name into decimal
            string afterEncode = "";
            returnEncode(tree, customerName, &afterEncode);
            int RESULT = 0;
            returnDecimal(afterEncode, &RESULT);
            // Case 1 character

            // Case khong goi mon
            if (!hashTable->findName(customerName) && !AVLTable->findName(AVLTable->root, RESULT, customerName)) {
                // Find ID and assign customer table
                customerID = RESULT % MAXSIZE + 1;
                // Process customerID
                while (1) {
                    if (customerID > MAXSIZE) customerID = 1;
                    if (!hashTable->containsID(customerID) && !AVLTable->containsID(customerID)) break;
                    if (hashTable->isFull() && AVLTable->isFull()) break;
                    else customerID++;
                }

                // Restaurant is not full
                if (!hashTable->isFull() || !AVLTable->isFull()) {
                    // Select area for customer
                    if (RESULT % 2 != 0) {
                        // Hash Table
                        int key = RESULT % (MAXSIZE / 2);
                        // Case this area is not full
                        if (!hashTable->isFull()) {
                            hashTable->put(key, new cusInfo(RESULT, customerID, customerName));
                            // Add into queue
                            findFIFO->add(new cusInfo(RESULT, customerID, customerName));
                            findLRCO->addHead(new cusInfo(RESULT, customerID, customerName));
                            findLFCO->push(new cusInfo(RESULT, customerID, customerName));
                        }
                        // If this area is full and other is not, move to other.
                        else if (hashTable->isFull() && !AVLTable->isFull()) {
                            AVLTable->add(RESULT, customerID, customerName);
                            findFIFO->add(new cusInfo(RESULT, customerID, customerName));
                            findLRCO->addHead(new cusInfo(RESULT, customerID, customerName));
                            findLFCO->push(new cusInfo(RESULT, customerID, customerName));
                        }
                    }
                    else {
                        // AVL Table
                        // Case this area is not full
                        if (!AVLTable->isFull()) {
                            AVLTable->add(RESULT, customerID, customerName);
                            findFIFO->add(new cusInfo(RESULT, customerID, customerName));
                            findLRCO->addHead(new cusInfo(RESULT, customerID, customerName));
                            findLFCO->push(new cusInfo(RESULT, customerID, customerName));
                        }
                        else if (AVLTable->isFull() && !hashTable->isFull()) {
                            int key = RESULT % (MAXSIZE / 2);
                            hashTable->put(key, new cusInfo(RESULT,customerID, customerName));
                            findFIFO->add(new cusInfo(RESULT, customerID, customerName));
                            findLRCO->addHead(new cusInfo(RESULT, customerID, customerName));
                            findLFCO->push(new cusInfo(RESULT, customerID, customerName));
                        }
                    }
                }
                // Restaurant is full -> find OPT
                else {
                    int OPT = RESULT % 3;
                    // Case 1 : FIFO
                    if (OPT == 0) {
                        Node* oldestCustomer = findFIFO->pop();
                        Node* del_1 = findLRCO->remove(oldestCustomer->data->name);
                        cusInfo* del_2 = findLFCO->remove(oldestCustomer->data->name);
                        if (findingArea(oldestCustomer->data->ID) == 1) {
                            // Case oldest in hashTable
                            int key = RESULT % (MAXSIZE / 2);
                            hashTable->deleteInfo(oldestCustomer->data->ID);
                            hashTable->put(key, new cusInfo(RESULT, oldestCustomer->data->ID, customerName));
                        }
                        else {
                            // Case oldest in AVLTable
                            AVLTable->remove(oldestCustomer->data->RESULT, oldestCustomer->data->ID);
                            AVLTable->add(RESULT, oldestCustomer->data->ID, customerName);
                        }
                        // add new customer into queue
                        findFIFO->add(new cusInfo(RESULT, oldestCustomer->data->ID, customerName));
                        findLRCO->addHead(new cusInfo(RESULT, oldestCustomer->data->ID, customerName));
                        findLFCO->push(new cusInfo(RESULT, oldestCustomer->data->ID, customerName));
                        // Free memory
                        if (oldestCustomer->data != 0) delete oldestCustomer->data;
                        delete(oldestCustomer);
                        if (del_1->data != 0) delete del_1->data;
                        delete del_1;
                        delete del_2;
                    }
                    // Case 2 : LRCO
                    else if (OPT == 1) {
                        // delete in queue
                        Node* oldestCustomer = findLRCO->pop_back();
                        Node* del_1 = findFIFO->remove(oldestCustomer->data->name);
                        cusInfo* del_2 = findLFCO->remove(oldestCustomer->data->name);
                        if (findingArea(oldestCustomer->data->ID) == 1) {
                            int key = RESULT % (MAXSIZE / 2);
                            hashTable->deleteInfo(oldestCustomer->data->ID);
                            hashTable->put(key, new cusInfo(RESULT, oldestCustomer->data->ID, customerName));
                        }
                        else {
                            AVLTable->remove(oldestCustomer->data->RESULT, oldestCustomer->data->ID);
                            AVLTable->add(RESULT, oldestCustomer->data->ID, customerName);
                        }
                        // add new customer into queue
                        findFIFO->add(new cusInfo(RESULT, oldestCustomer->data->ID, customerName));
                        findLRCO->addHead(new cusInfo(RESULT, oldestCustomer->data->ID, customerName));
                        findLFCO->push(new cusInfo(RESULT, oldestCustomer->data->ID, customerName));
                        if (oldestCustomer->data != 0) delete oldestCustomer->data;
                        delete(oldestCustomer);
                        if (del_1->data != 0) delete del_1->data;
                        delete del_1;
                        delete del_2;
                    }
                    // Case 3 : LFCO
                    else {
                        cusInfo* oldestCustomer = findLFCO->pop();
                        // delete in queue
                        Node* del_1 = findFIFO->remove(oldestCustomer->name);
                        Node* del_2 = findLRCO->remove(oldestCustomer->name);
                        if (findingArea(oldestCustomer->ID) == 1) {
                            int key = RESULT % (MAXSIZE / 2);
                            hashTable->deleteInfo(oldestCustomer->ID);
                            hashTable->put(key, new cusInfo(RESULT, oldestCustomer->ID, customerName));
                        }
                        else {
                            AVLTable->remove(oldestCustomer->RESULT, oldestCustomer->ID);
                            AVLTable->add(RESULT, oldestCustomer->ID, customerName);
                        }
                        // add new customer into queue
                        findFIFO->add(new cusInfo(RESULT, oldestCustomer->ID, customerName));
                        findLRCO->addHead(new cusInfo(RESULT, oldestCustomer->ID, customerName));
                        findLFCO->push(new cusInfo(RESULT, oldestCustomer->ID, customerName));
                        delete(oldestCustomer);
                        if (del_1->data != 0) delete del_1->data;
                        delete del_1;
                        if (del_2->data != 0) delete del_2->data;
                        delete del_2;
                    }
                }
            }
            // Case goi mon
            else {
                // Delete in findLRCO and then push back to head
                Node* nNode = findLRCO->remove(customerName);
                findLRCO->addHead(nNode->data);
                findFIFO->updateNUM(customerName);
                cusInfo* updateHeap = findLFCO->remove(customerName);
                updateHeap->NUM++;
                findLFCO->push(updateHeap);
                // Update NUM
                if (hashTable->findName(customerName)) {
                    hashTable->updateNUM(customerName);
                }
                else {
                    AVLTable->updateNUM(AVLTable->root, RESULT, nNode->data->ID);
                }
                delete nNode;
            }
            // Free huff tree.
            deleteHuff(tree);
        }
        else if (checkCLE(getCommand, &customerNUM)) {
            // Case 1 : NUM < 1 -> delete all in area 1
            // 1. Check if hash is empty.
            // 2. Delete 1 by 1 by using the FIFO list.
            // 3. Free memory each time delete information.
            if (customerNUM < 1) {
                if (!hashTable->isEmpty()) {
                    while (containInArea_1(findFIFO, hashTable)) {
                        Node* ptr = findFIFO->head;
                        while (ptr) {
                            if (hashTable->containsID(ptr->data->ID)) break;
                            ptr = ptr->next;
                        }
                        if (ptr) {
                            // delete
                            hashTable->deleteInfo(ptr->data->ID);
                            Node* del_1 = findFIFO->remove(ptr->data->name);
                            Node* del_2 = findLRCO->remove(ptr->data->name);
                            cusInfo* del_3 = findLFCO->remove(ptr->data->name);

                            // Free memory
                            if (del_1->data != 0) delete del_1->data;
                            delete del_1;
                            if (del_2->data != 0) delete del_2->data;
                            delete del_2;
                            delete del_3;
                        }
                    }
                }
            }
            // Case 2 : NUM > MAXSIZE -> delete all in area 2
            // 1. Check if hash is empty.
            // 2. Delete 1 by 1 by using the FIFO list.
            // 3. Free memory each time delete information.
            else if (customerNUM > MAXSIZE) {
                if (AVLTable->count != 0) {
                    while (containInArea_2(findFIFO, AVLTable)) {
                        Node* ptr = findFIFO->head;
                        while (ptr) {
                            if (AVLTable->containsID(ptr->data->ID)) break;
                            ptr = ptr->next;
                        }
                        if (ptr) {
                            // delete
                            AVLTable->remove(ptr->data->RESULT, ptr->data->ID);
                            Node* del_1 = findFIFO->remove(ptr->data->name);
                            Node* del_2 = findLRCO->remove(ptr->data->name);
                            cusInfo* del_3 = findLFCO->remove(ptr->data->name);
                            // Free memory
                            if (del_1->data != 0) delete del_1->data;
                            delete del_1;
                            if (del_2->data != 0) delete del_2->data;
                            delete del_2;
                            delete del_3;
                        }
                    }
                }
            }
            // Case 3: Delete 1 table
            else {
                if (hashTable->containsID(customerNUM)) {
                    hashTable->deleteInfo(customerNUM);
                    Node* ptr = findFIFO->head;
                    while (ptr) {
                        if (ptr->data->ID == customerNUM) break;
                        ptr = ptr->next;
                    }
                    Node* del_1 = findFIFO->remove(ptr->data->name);
                    Node* del_2 = findLRCO->remove(ptr->data->name);
                    cusInfo* del_3 = findLFCO->remove(ptr->data->name);
                    // Free memory
                    if (del_1->data != 0) delete del_1->data;
                    delete del_1;
                    if (del_2->data != 0) delete del_2->data;
                    delete del_2;
                    delete del_3;
                }
                else if (AVLTable->containsID(customerNUM)) {
                    Node* ptr = findFIFO->head;
                    while (ptr) {
                        if (ptr->data->ID == customerNUM) break;
                        ptr = ptr->next;
                    }
                    AVLTable->remove(ptr->data->RESULT, ptr->data->ID);
                    Node* del_1 = findFIFO->remove(ptr->data->name);
                    Node* del_2 = findLRCO->remove(ptr->data->name);
                    cusInfo* del_3 = findLFCO->remove(ptr->data->name);
                    // Free memory
                    if (del_1->data != 0) delete del_1->data;
                    delete del_1;
                    if (del_2->data != 0) delete del_2->data;
                    delete del_2;
                    delete del_3;
                }
            }
        }
        else if (checkPrintHT(getCommand)) {
            hashTable->printHT();
        }
        else if (checkPrintAVL(getCommand)) {
            AVLTable->printAVL();
        }
        else if (checkPrintMH(getCommand)) {
            myBTree* myTree = new myBTree();
            myTree->buildTree(findLFCO->elements);
            myTree->printMH(myTree->root);
            delete myTree;
            myTree = NULL;
        }
    }
    myFile.close();
    delete findFIFO;
    delete findLRCO;
    delete AVLTable;
    delete hashTable;
    delete findLFCO;
}

// REG Functions
bool checkREG(string s, string* customerName) {
    string correctCommand = "REG ";
    if (s.find(correctCommand) != 0) return 0;
    int i = 4;
    while (s[i] != '\0') {
        if (s[i] == ' ' || s[i] < 'A' || (s[i] > 'Z' && s[i] < 'a') || (s[i] > 'z')) return 0;
        *customerName += s[i];
        i++;
    }
    return 1;
}

vector<char> diffChar(string customerName) {
    vector<char> result;
    int i = 0;
    bool flag = 1;
    while (customerName[i] != '\0') {
        bool flag = 1;
        for (int j = 0; j < int(result.size()); j++)
        {
            if (result[j] == customerName[i]) {
                flag = 0;
            }
        }
        if (flag) {
            result.push_back(customerName[i]);
        }
        i++;
    }
    return result;
}

vector<int> findFreq(vector<char> diff, string customerName) {
    int length = int(customerName.length());
    vector<int> freqOfChar;
    for (int i = 0; i < int(diff.size()); i++)
    {
        int count = 0;
        for (int j = 0; j < length; j++)
        {
            if (customerName[j] == diff[i]) {
                count++;
            }
        }
        freqOfChar.push_back(count);
    }
    return freqOfChar;
}

HuffTreeNode* makeTree(vector<pair<int, char>> combination) {
    // Assigning an minHeap and push the combination into it
    myMinHeap<HuffTreeNode*>* pq = new myMinHeap<HuffTreeNode*>();
    for (int i = 0; i < int(combination.size()); i++)
    {
        pq->pushHuff(new HuffTreeNode(combination[i].second, combination[i].first));
    }
    // A counter for '~' to determine which come first
    int characterCounter = 0;
    while (pq->size() != 1) {
        // Take the 2 minimum characters with least frequency
        HuffTreeNode* left = pq->popHuff();
        HuffTreeNode* right = pq->popHuff();
        // Assign new Node the its pointers will link to the previous nodes. Push it back to the pq
        pq->pushHuff(new HuffTreeNode('~', left->freq + right->freq, ++characterCounter, left, right));
    }
    HuffTreeNode* root = pq->popHuff();
    // Free priority queue
    delete pq;
    return root;
}

void returnEncode(HuffTreeNode* tree, string customerName, string* afterCode) {
    for (int i = 0; i < int(customerName.length()); i++)
    {
        for (int j = 0; j < tree->info.size(); j++)
        {
            // Encoding...
            if (customerName[i] == tree->info[j]->character) {
                *afterCode += tree->info[j]->enCode;
            }
        }
    }
    // Process afterCode (by taking 15 bits)
    int lengthOfCode = int((*afterCode).length());
    string copy = *afterCode;
    // Reset the afterCode
    *afterCode = "";
    if (lengthOfCode > 15) {
        *afterCode = copy.substr(copy.length() - 15, copy.length());
    }
    else {
        int needMore = 15 - lengthOfCode;
        for (int i = 0; i < needMore; i++)
        {
            *afterCode += '0';
        }
        *afterCode += copy;
    }
}

void returnDecimal(string binary, int* decimal) {
    int k = int(binary.length() - 1);
    for (int i = 0; i < binary.length(); i++)
    {
        if (binary[k] == '1') {
            *decimal += int(pow(2, i));
        }
        k--;
    }
}

int findingArea(int customerID) {
    // We must find by ID because the RESULT may be the same
    if (hashTable->containsID(customerID)) return 1;
    else if (AVLTable->containsID(customerID)) return 2;
    return -1;
}

bool checkPrintHT(string s) {
    string correctCommand = "PrintHT";
    if (s != correctCommand) return 0;
    return 1;
}

bool checkPrintAVL(string s) {
    string correctCommand = "PrintAVL";
    if (s != correctCommand) return 0;
    return 1;
}

bool checkPrintMH(string s) {
    string correctCommand = "PrintMH";
    if (s != correctCommand) return 0;
    return 1;
}

void deleteHuff(HuffTreeNode* root) {
    if (!root) return;
    if (root->pLeft) deleteHuff(root->pLeft);
    if (root->pRight) deleteHuff(root->pRight);
    delete root;
    root = NULL;
}

bool checkCLE(string s, int* customerNUM) {
    string correctCommand = "CLE ";
    if (s.find(correctCommand) != 0) return 0;
    int i = 4;
    string str_NUM = "";
    while (s[i] != '\0') {
        if (s[i] == ' ' || s[i] < '0' || s[i] > '9') return 0;
        str_NUM += s[i];
        i++;
    }
    *customerNUM = stoi(str_NUM);
    return 1;
}

bool containInArea_1(myList* FIFO, myHash* table) {
    Node* ptr = FIFO->head;
    while (ptr) {
        if (table->containsID(ptr->data->ID)) return 1;
        ptr = ptr->next;
    }
    return 0;
}

bool containInArea_2(myList* FIFO, myAVL* table) {
    Node* ptr = FIFO->head;
    while (ptr) {
        if (table->containsID(ptr->data->ID)) return 1;
        ptr = ptr->next;
    }
    return 0;
}

bool differentChar(string customerName) {
    int length = int(customerName.length());
    for (int i = 0; i < (length -1); i++)
    {
        if (customerName[i] != customerName[i + 1]) return 0;
    }
    return 1;
}