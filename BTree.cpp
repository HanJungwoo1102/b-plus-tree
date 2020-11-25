// Please implement most of your source codes here. 
#include "BTree.h"

BTreeNode::BTreeNode() {
	
}

BTreeNode::BTreeNode(NodeType type) {
	BTreeNode::type = type;
}

NodeType BTreeNode::getNodeType() {
	return BTreeNode::type;
}

BTreeInternalNode::BTreeInternalNode() : BTreeNode(NodeType::INTERNAL) {
}

BTreeInternalNode::~BTreeInternalNode() {

}

BTreeLeafNode::BTreeLeafNode() : BTreeNode(NodeType::LEAF) {
}

BTreeLeafNode::~BTreeLeafNode() {

}

void BTreeLeafNode::printLeafNode() {

}

BTree::BTree() {

}

BTree::~BTree() {

}

void BTree::insert(long long value) {
	std::cout << value << std::endl;
}

void BTree::printLeafNode(long long value) {

}

void BTree::pointQuery(long long value) {

}

void BTree::rangeQuery(long long low, long long high) {

}
