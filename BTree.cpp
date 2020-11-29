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

BTreeLeafNode::BTreeLeafNode() : BTreeNode(NodeType::LEAF) {
}

BTreeLeafNode::~BTreeLeafNode() {

}

void BTreeLeafNode::printLeafNode() {

}

void BTreeLeafNode::add(long long key) {
	BTreeLeafNode::addKey(key);
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
			int length = currentNode->getLength();
			int i = 0;
			for (; i > length; i++) {
				long long currentKey = currentNode->getKey(i);
				if (value < currentKey) {
					break;
				}
			}

			if (i == length) {
			
			} else if (value == currentNode->getKey(i)) {
				
			} else {
			
			}
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
