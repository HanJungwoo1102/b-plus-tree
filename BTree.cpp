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
	std::cout << "insert: " << value << std::endl;
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
		BTree::insertInLeaf(keys, value, NUM_KEYS);
	} else {
		BTreeLeafNode* newLeafNode = new BTreeLeafNode();	
		Key* newLeafKeys = newLeafNode->getKeys();

		Key tempKeys[NUM_KEYS + 1];

		for (int i = 0; i < NUM_KEYS + 1; i++) {
			tempKeys[i] = NO_KEY;
		}

		BTree::copyKeys(keys, tempKeys, 0, NUM_KEYS - 1);
		
		BTree::insertInLeaf(tempKeys, value, NUM_KEYS + 1);

		BTree::eraseKeys(keys);

		int splitedNum = (NUM_KEYS + 1) / 2;	

		BTree::copyKeys(tempKeys, keys, 0, splitedNum - 1);
		BTree::copyKeys(tempKeys, newLeafKeys, splitedNum, NUM_KEYS);

		newLeafNode->setNextBTreeLeafNode(leafNode->getNextBTreeLeafNode());
		leafNode->setNextBTreeLeafNode(newLeafNode);

		BTree::insertInParent(leafNode, newLeafKeys[0], newLeafNode);
	}
	BTree::printAllLeaves();
}

void BTree::printLeafNode(long long value) {
	BTreeLeafNode* leafNode = BTree::findLeafNode(value);

	Key* keys = leafNode->getKeys();

	for (int i = 0; i < NUM_KEYS; i++) {
		std::cout << keys[i] << " ";
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
	std::cout << "low: " << low << ", hight: " << high << std::endl;
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

void BTree::insertInLeaf(Key* keys, Key key, int keysSize) {
	if (key < keys[0]) {
		BTree::shiftRightKeys(keys, 0, keysSize);
		keys[0] = key;
	} else {
		int findedIndex = BTree::findHighestIndexSmallerThanOrEqual(keys, key);
		BTree::shiftRightKeys(keys, findedIndex + 1, keysSize);
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
	BTreeNode** pointers = parent->getPointers();		
	Key* keys = parent->getKeys();
	int length = BTree::getPointersLength(pointers);

	if (length < NUM_KEYS + 1) {
		BTree::insertInternalNodeAfterNode(pointers, keys, insertedNode, key, node, NUM_KEYS + 1, NUM_KEYS);
	} else {
		BTreeNode* tempPointers[NUM_KEYS + 2];
		for (int i = 0; i < NUM_KEYS + 1; i++) {
			tempPointers[i] = nullptr;
		}

		Key tempKeys[NUM_KEYS + 1];
		for (int i = 0; i < NUM_KEYS; i++) {
			tempKeys[i] = NO_KEY;
		}
	
		BTree::copyKeys(keys, tempKeys, 0, NUM_KEYS - 1);
		BTree::copyPointers(pointers, tempPointers, 0, NUM_KEYS);

		BTree::insertInternalNodeAfterNode(tempPointers, tempKeys, insertedNode, key, node, NUM_KEYS + 2, NUM_KEYS + 1);

		BTree::erasePointers(pointers);		

		BTreeInternalNode* newInternalNode = new BTreeInternalNode();
		BTreeNode** newPointers = newInternalNode->getPointers();
		Key* newKeys = newInternalNode->getKeys();

		int splitedNum = (NUM_KEYS + 1) / 2;

		BTree::copyKeys(tempKeys, keys, 0, splitedNum - 1);
		BTree::copyPointers(tempPointers, pointers, 0, splitedNum);

		Key newKey = tempKeys[splitedNum];

		BTree::copyKeys(tempKeys, newKeys, splitedNum + 1, NUM_KEYS);
		BTree::copyPointers(tempPointers, newPointers, splitedNum + 1, NUM_KEYS + 1);
		
		BTree::insertInParent(parent, newKey, newInternalNode);
	}
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
	for (int i = NUM_KEYS; i > -1; i--) {
		if (pointers[i] != nullptr) {
			return pointers[i];
		}
	}
}

void BTree::shiftRightKeys(Key* keys, int fromIndex, int keysSize) {
	for (int i = keysSize - 2; i > fromIndex - 1; i--) {
		keys[i + 1] = keys[i];
	}
}

void BTree::copyKeys(Key* sourceKeyAddress, Key* targetKeyAddress, int startIndex, int endIndex) {
	Key* startAddress = sourceKeyAddress + startIndex;
	size_t copySize = (endIndex - startIndex + 1) * sizeof(Key);
	
	memcpy(targetKeyAddress, startAddress, copySize);	
}

void BTree::copyPointers(BTreeNode** sourcePointerAddress, BTreeNode** targetPointerAddress, int startIndex, int endIndex) {
	BTreeNode** startAddress = sourcePointerAddress + startIndex;
	size_t copySize = (endIndex - startIndex + 1) * sizeof(BTreeNode**);

	memcpy(targetPointerAddress, startAddress, copySize);
}

void BTree::eraseKeys(Key* keys) {
	for (int i = 0; i < NUM_KEYS; i++) {
		keys[i] = NO_KEY;
	}	
}

void BTree::erasePointers(BTreeNode** pointers) {
	for (int i = 0; i < NUM_KEYS + 1; i++) {
		pointers[i] = nullptr;
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

int BTree::getPointersLength(BTreeNode** pointers) {
	int i = 0;
	for (; i < NUM_KEYS + 1; i++) {
		if (pointers[i] == nullptr) {
			break;
		}
	}
	return i;
}

void BTree::insertInternalNodeAfterNode(BTreeNode** pointers, Key* keys, BTreeNode* insertedNode, Key insertedKey, BTreeNode* afterNode, int pointersSize, int keysSize) {
	int afterNodeIndex = -1;

	for (int i = 0; i < NUM_KEYS + 1; i++) {
		if (pointers[i] == afterNode) {
			afterNodeIndex = i;
			break;
		}
	}

	if (afterNodeIndex != -1) {
		for (int i = pointersSize - 2; i > afterNodeIndex; i--) {
			pointers[i + 1] = pointers[i];
		}
		for (int i = keysSize - 2; i > afterNodeIndex - 1; i--) {
			keys[i + 1] = keys[i];
		}

		pointers[afterNodeIndex + 1] = insertedNode;
		keys[afterNodeIndex] = insertedKey;
	}
}

void BTree::printAllLeaves() {
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

