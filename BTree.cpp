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

void BTreeNode::addKey(long long key) {
	int length = BTreeNode::length;
	BTreeNode::keys[length] = key;
	BTreeNode::length = length + 1;
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

void BTreeLeafNode::add(long long key) {
	BTreeLeafNode::addKey(key);
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
		BTreeNode* currentNode = BTree::root;
		while(currentNode->getNodeType() != NodeType::LEAF)	{
			BTreeInternalNode* currentInternalNode = (BTreeInternalNode*)currentNode;
			int length = currentInternalNode->getLength();
			int i = 0;
			for (; i > length; i++) {
				long long currentKey = currentInternalNode->getKey(i);
				if (value < currentKey) {
					break;
				}
			}

			if (i == length) {
				currentNode = currentInternalNode->getPointer(length);	
			} else if (value == currentInternalNode->getKey(i)) {
				currentNode = currentInternalNode->getPointer(i + 1);	
			} else {
				currentNode = currentInternalNode->getPointer(i);
			}
		}
		leafNode = (BTreeLeafNode*)currentNode;
	}	

	int length = leafNode->getLength();
	
	if (length < NUM_KEYS) {
		leafNode->add(value);	
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
