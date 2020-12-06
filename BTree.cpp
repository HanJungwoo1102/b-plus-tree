// Please implement most of your source codes here. 
#include "BTree.h"

BTreeNode::BTreeNode() {
	BTreeNode::length = 0;	
}

BTreeNode::BTreeNode(NodeType type) : BTreeNode() {
	BTreeNode::type = type;
}

NodeType BTreeNode::getNodeType() {
	return BTreeNode::type;
}

void BTreeNode::insertKey(long long key) {
	int length = BTreeNode::length;
	long long * keys = BTreeNode::keys;

	int insertedKeyIndex = length;

	for (int i = 0; i < length; i++) {
		if (keys[i] > key) {
			insertedKeyIndex = i;
			break;
		}
	}

	for (int i = length - 1; i >= insertedKeyIndex; i--) {
		keys[i + 1] = keys[i];
	}	

	keys[insertedKeyIndex] = key;
	BTreeNode::length = length + 1;
}

void BTreeNode::removeKey(long long key) {
	int length = BTreeNode::length;
	long long * keys = BTreeNode::keys;

	int removedKeyIndex = -1;

	for (int i =0; i < length; i++) {
		if (keys[i] == key) {
			removedKeyIndex = i;
			break;
		}
	}

	if (removedKeyIndex != -1) {
		for (int i = removedKeyIndex; i < length - 1; i++) {
			keys[i] = keys[i + 1];
		}
	}
}

int BTreeNode::getLength() {
	return BTreeNode::length;
}

long long BTreeNode::getKey(int keyIndex) {
	return BTreeNode::keys[keyIndex];
}

BTreeInternalNode::BTreeInternalNode() : BTreeNode(NodeType::INTERNAL) {
}

BTreeInternalNode::~BTreeInternalNode() {}

BTreeNode* BTreeInternalNode::getPointer(int pointerIndex) {
	return BTreeInternalNode::child[pointerIndex];	
}

BTreeLeafNode::BTreeLeafNode() : BTreeNode(NodeType::LEAF) {
}

BTreeLeafNode::~BTreeLeafNode() {

}

void BTreeLeafNode::printLeafNode() {

}

void BTreeLeafNode::insert(long long key) {
	BTreeLeafNode::insertKey(key);
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
	std::cout << value << std::endl;

	BTreeLeafNode* leafNode;

	if (BTree::isEmpty()) {
		leafNode = new BTreeLeafNode();
	} else {
		leafNode = BTree::findLeafNode(value);
	}	

	int length = leafNode->getLength();
	
	if (length < NUM_KEYS) {
		leafNode->insert(value);	
	} else {
		BTreeLeafNode* newLeafNode = new BTreeLeafNode();	

		long long keyArray[length + 1];
		int findedIndex = -1;
		for (int i = 0; i < length + 1; i++) {
			long long key = leafNode->getKey(i);
			if (key > value) {
				if (findedIndex == -1) {
					findedIndex = i;
				}
				keyArray[i + 1] = key;
			} else {
				keyArray[i] = key;
			}
		}

		if (findedIndex == -1) {
			keyArray[length + 1] = value;
		} else {
			keyArray[length] = value;
		}

	}
}

void BTree::printLeafNode(long long value) {

}

void BTree::pointQuery(long long value) {

}

void BTree::rangeQuery(long long low, long long high) {

}

bool BTree::isEmpty() {
	return BTree::root == NULL;
}

BTreeLeafNode* BTree::findLeafNode(long long key) {
	BTreeNode* currentNode = BTree::root;
	while(currentNode->getNodeType() != NodeType::LEAF)	{
		BTreeInternalNode* currentInternalNode = (BTreeInternalNode*)currentNode;
		int length = currentInternalNode->getLength();
		int i = 0;
		for (; i > length; i++) {
			long long currentKey = currentInternalNode->getKey(i);
			if (key < currentKey) {
				break;
			}
		}

		if (i == length) {
			currentNode = currentInternalNode->getPointer(length);	
		} else if (key == currentInternalNode->getKey(i)) {
			currentNode = currentInternalNode->getPointer(i + 1);	
		} else {
			currentNode = currentInternalNode->getPointer(i);
		}
	}
	return (BTreeLeafNode*)currentNode;
};

