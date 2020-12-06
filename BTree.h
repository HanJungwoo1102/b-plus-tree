// Note: You need to add additional member variables and functions as you need.
#include <iostream>
#include <cstring>
#include <cmath>

#define NUM_KEYS 10 
// NUM_KEYS should be set to make each tree node as large as 4KB. 
// But for this assignment, we will set the degree of node small to make debugging and grading easy
// Please do not change this number. 
// In practice, DBMS employs 'slotted page structure' to store variable-length records in B+tree.
// But the purpose of this assignment is to make students familiar with B-tree itself. 

const long long NO_KEY = 0;

enum NodeType {
	ROOT,
	INTERNAL,
	LEAF
};

typedef long long Key;

class BTreeNode{   
   protected:
   	NodeType type;
	Key keys[NUM_KEYS];
   public:
   	BTreeNode();
	BTreeNode(NodeType type);
	virtual ~BTreeNode() {}
	NodeType getNodeType();
	Key* getKeys();	
};

class BTreeInternalNode:public BTreeNode{
   private:
	BTreeNode* child[NUM_KEYS+1];
   public:
	BTreeInternalNode();
	~BTreeInternalNode();
	BTreeNode** getPointers();
};

class BTreeLeafNode:public BTreeNode{
   private:
	BTreeLeafNode* right_sibling;
   public:
	BTreeLeafNode();
	~BTreeLeafNode();
	void printLeafNode(); // print all keys in the current leaf node, separated by comma.
	BTreeLeafNode* getNextBTreeLeafNode();
	void setNextBTreeLeafNode(BTreeLeafNode* bTreeLeafNode);
};

class BTree{  
    private:
	BTreeNode *root;
	bool isEmpty();
	BTreeLeafNode* findLeafNode(long long key);
	void insertInLeaf(Key* keys, Key key);
	void insertInParent(BTreeNode* node, Key key, BTreeNode* insertedNode);
	int findSmallestIndexBiggerThanKey(Key* keys, Key key);
	int findHighestIndexSmallerThanOrEqual(Key* keys, Key key);
	BTreeNode* getLastNonNullPointer(BTreeNode** pointers);
	int getLength(Key* keys);
	void shiftRightKeys(Key* keys, int fromIndex);
	void copyKeys(Key* sourceKeyAddress, Key* targetKeyAddress, int fromIndex, int toIndex);
	void eraseKeys(Key* keys);
	BTreeInternalNode* findParent(BTreeNode* node);
	BTreeInternalNode* findParentNodeHavingThisChild(BTreeNode* node, BTreeNode* findingNode);
    public:
	// You are not allowed to change the interfaces of these public methodu.
	BTree(); 
	~BTree(); 
	void insert(long long value);  
//	void remove(long long value);
	void printLeafNode(long long value);
	// find the leaf node that contains 'value' and print all values in the leaf node.
	void pointQuery(long long value); 
	// print the found value or "NOT FOUND" if there is no value in the index
	void rangeQuery(long long low, long long high); 
	// print all found keys (low <= keys < high), separated by comma (e.g., 10, 11, 13, 15\n) 
};

