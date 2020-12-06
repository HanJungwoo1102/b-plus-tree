// Please implement most of your source codes here. 
#include "BTree.h"

BTreeNode::BTreeNode() {
	for (int i = 0; i < NUM_KEYS; i++) {
		BTreeNode::keys[i] = NO_KEY;
	} 	
}

BTreeNode::BTreeNode(NodeType type) : BTreeNode() {
	BTreeNode::type = type;
}

NodeType BTreeNode::getNodeType() {
	return BTreeNode::type;
}

Key* BTreeNode::getKeys() {
	return BTreeNode::keys;
}

BTreeInternalNode::BTreeInternalNode() : BTreeNode(NodeType::INTERNAL) {
	for (int i = 0; i < NUM_KEYS + 1; i++) {
		BTreeInternalNode::child[i] = nullptr;
	}
}

BTreeInternalNode::~BTreeInternalNode() {}

BTreeNode** BTreeInternalNode::getPointers() {
	return BTreeInternalNode::child;	
}

BTreeLeafNode::BTreeLeafNode() : BTreeNode(NodeType::LEAF) {
	BTreeLeafNode::right_sibling = nullptr;
}

BTreeLeafNode::~BTreeLeafNode() {

}

void BTreeLeafNode::printLeafNode() {

}

BTreeLeafNode* BTreeLeafNode::getNextBTreeLeafNode() {
	return BTreeLeafNode::right_sibling; 
}

void BTreeLeafNode::setNextBTreeLeafNode(BTreeLeafNode* bTreeLeafNode) {
	BTreeLeafNode::right_sibling = bTreeLeafNode;
}

BTree::BTree() {

}

BTree::~BTree() {

}

void BTree::insert(long long value) {
	BTreeLeafNode* leafNode;

	if (BTree::isEmpty()) {
		leafNode = new BTreeLeafNode();
		BTree::root = leafNode;
	} else {
		leafNode = BTree::findLeafNode(value);
	}	

	Key* keys = leafNode->getKeys();
	int length = BTree::getLength(keys);

	if (length < NUM_KEYS) {
		BTree::insertInLeaf(keys, value);
	} else {
		BTreeLeafNode* newLeafNode = new BTreeLeafNode();	
		Key* newLeafKeys = newLeafNode->getKeys();

		Key tempKeys[NUM_KEYS + 1];

		for (int i = 0; i < NUM_KEYS + 1; i++) {
			tempKeys[i] = NO_KEY;
		}

		BTree::copyKeys(keys, tempKeys, 0, NUM_KEYS - 1);

		BTree::insertInLeaf(tempKeys, value);

		BTree::eraseKeys(keys);

		int splitedIndex = floor((NUM_KEYS + 1) / 2);	

		BTree::copyKeys(tempKeys, keys, 0, splitedIndex);
		BTree::copyKeys(tempKeys, newLeafKeys, splitedIndex + 1, NUM_KEYS);

		newLeafNode->setNextBTreeLeafNode(leafNode->getNextBTreeLeafNode());
		leafNode->setNextBTreeLeafNode(newLeafNode);

		BTree::insertInParent(leafNode, newLeafKeys[0], newLeafNode);
	}
}

void BTree::printLeafNode(long long value) {
	BTreeNode* currentNode = BTree::root;

	while (currentNode->getNodeType() != NodeType::LEAF) {
		BTreeInternalNode* currentInternalNode = (BTreeInternalNode*)currentNode;
		BTreeNode** pointers = currentInternalNode->getPointers();

		currentNode = pointers[0];
	}

	BTreeLeafNode* leafNode = (BTreeLeafNode*) currentNode;
	while (leafNode != nullptr) {
		Key* keys = leafNode->getKeys();
		for (int i = 0; i < NUM_KEYS; i++) {
			std::cout << keys[i] << " ";
		}
		leafNode = leafNode->getNextBTreeLeafNode();
		std::cout << " | ";
	}
	std::cout << std::endl;
}

void BTree::pointQuery(long long value) {
	BTreeLeafNode* leafNode = BTree::findLeafNode(value);

	Key* keys = leafNode->getKeys();

	for (int i = 0; i < NUM_KEYS; i++) {
		std::cout << keys[i] << " ";
	}
	std::cout << std::endl;

}

void BTree::rangeQuery(long long low, long long high) {

}

BTreeLeafNode* BTree::findLeafNode(Key key) {
	BTreeNode* currentNode = BTree::root;
	while(currentNode->getNodeType() != NodeType::LEAF)	{
		BTreeInternalNode* currentInternalNode = (BTreeInternalNode*)currentNode;
		Key* keys = currentInternalNode->getKeys();
		
		int findedIndex = BTree::findSmallestIndexBiggerThanKey(keys, key);	

		BTreeNode** pointers = currentInternalNode->getPointers();

		if (findedIndex == -1) {
			currentNode = BTree::getLastNonNullPointer(pointers);	
		} else if (key == keys[findedIndex]) {
			currentNode = pointers[findedIndex + 1];	
		} else {
			currentNode = pointers[findedIndex];
		}
	}
	return (BTreeLeafNode*)currentNode;
};

void BTree::insertInLeaf(Key* keys, Key key) {
	if (key < keys[0]) {
		BTree::shiftRightKeys(keys, 0);
		keys[0] = key;
	} else {
		int findedIndex = BTree::findHighestIndexSmallerThanOrEqual(keys, key);
		BTree::shiftRightKeys(keys, findedIndex + 1);
		keys[findedIndex + 1] = key;
	}
}

void BTree::insertInParent(BTreeNode* node, Key key, BTreeNode* insertedNode) {
	if (node == BTree::root) {
		BTreeInternalNode* newNode = new BTreeInternalNode();		
		
		Key* newNodeKeys = newNode->getKeys();
		BTreeNode** pointers = newNode->getPointers();

		newNodeKeys[0] = key;
		pointers[0] = node;
		pointers[1] = insertedNode;

		BTree::root = newNode;
		return;
	}
	BTreeInternalNode* parent = BTree::findParent(node);
}

bool BTree::isEmpty() {
	return BTree::root == NULL;
}

int BTree::getLength(Key* keys) {
	int i = 0;
	for (; i < NUM_KEYS; i++) {
		if (keys[i] == NO_KEY) {
			break;
		}
	}
	return i;
}

int BTree::findSmallestIndexBiggerThanKey(Key* keys, Key key) {
	int result = -1;
	for (int i = 0;i < NUM_KEYS; i++) {
		if (keys[i] > key) {
			result = i;
			break;
		}
	}
	return result;
}

int BTree::findHighestIndexSmallerThanOrEqual(Key* keys, Key key) {
	int result = -1;
	for (int i = NUM_KEYS; i > -1; i--) {
		Key currentKey = keys[i];
		if (currentKey != NO_KEY) {
			if (currentKey <= key) {
				result = i;
				break;
			}
		}
	}
	return result;
}

BTreeNode* BTree::getLastNonNullPointer(BTreeNode** pointers) {
	for (int i = NUM_KEYS - 1; i > -1; i--) {
		if (pointers[i] != nullptr) {
			return pointers[i];
		}
	}
}

void BTree::shiftRightKeys(Key* keys, int fromIndex) {
	for (int i = NUM_KEYS - 2; i > fromIndex - 1; i--) {
		keys[i + 1] = keys[i];
	}
}

void BTree::copyKeys(Key* sourceKeyAddress, Key* targetKeyAddress, int startIndex, int endIndex) {
	Key* startAddress = sourceKeyAddress + startIndex;
	size_t copySize = (endIndex - startIndex + 1) * sizeof(Key);
	
	memcpy(startAddress, targetKeyAddress, copySize);	
}

void BTree::eraseKeys(Key* keys) {
	for (int i = 0; i < NUM_KEYS; i++) {
		keys[i] = NO_KEY;
	}	
}

BTreeInternalNode* BTree::findParent(BTreeNode* node) {
	return BTree::findParentNodeHavingThisChild(BTree::root, node);	
}

BTreeInternalNode* BTree::findParentNodeHavingThisChild(BTreeNode* node, BTreeNode* childNode) {
	if (node->getNodeType() == NodeType::LEAF) {
		return nullptr;	
	} else {
		BTreeInternalNode* internalNode = (BTreeInternalNode*) node;

		BTreeNode** pointers = internalNode->getPointers();

		for (int i = 0; i < NUM_KEYS + 1; i++) {
			if (pointers[i] == childNode) {
				return internalNode;
			} 
		}

		for (int i = 0; i < NUM_KEYS + 1; i++) {
			BTreeInternalNode* findedNode = BTree::findParentNodeHavingThisChild(pointers[i], childNode);

			if (findedNode != nullptr) {
				return findedNode;
			}
		}
	}
}

