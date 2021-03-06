#pragma once
#include "Node.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

class Unzip {
private:
	std::string path;
	std::string treeStr = "";
	std::string vocab = "";
public:
	Unzip(std::string pathh) {
		path = pathh + "\\";
	}
	void unzipping(std::string fl);
	void BuildTree(Node *p);
	void Unzip::print_Tree(Node * p, int level);
};

void Unzip::unzipping(std::string fl) {
	std::ifstream inp(fl.c_str(), std::ios::out | std::ios::binary);
	char info_block_size[5];
	inp.read(info_block_size, 5);
	int size = atoi(info_block_size);
	char* infoBlock = new char[size];
	inp.read(infoBlock, size);
	std::vector<std::string> tokens;
	char *tok = strtok(infoBlock, "||");

	while (tok) {
		if (strlen(tok) == 0) break;
		tokens.push_back(tok);
		tok = strtok(NULL, "||");
	}


	std::string name = tokens[0];
	vocab = tokens[1];
	treeStr = tokens[2];

	std::string fullPath = path + name;
	Node *root = new Node();

	BuildTree(root);

	//std::ifstream unpack(fl, std::ios::out | std::ios::binary);
	std::ofstream unzip(fullPath, std::ios::out | std::ios::binary);
	//unpack.seekg();
	//TODO unzipping

	//ifstream unpack("C:\\test\\output.zipp", ios::out | ios::binary);
	//ofstream output("C:\\test\\unzipped.txt", ios::out | ios::binary);
	Node *p = root;
	int count = 0;
	char byte = inp.get();
	while (!inp.eof()) {
		bool b = byte & (1 << (7 - count));
		if (b) p = p->right; else p = p->left;
		if (p->left == NULL && p->right == NULL) {
			unzip << p->c;
			p = root;
		}
		count++;
		if (count == 8) {
			count = 0;
			byte = inp.get();
		}
	}
	//std::cout << std::endl;
	inp.close();
	unzip.close();

	unzip.close();
}

void Unzip::BuildTree(Node *p) {
	if (treeStr[0] == 'D') {
		treeStr.erase(0, 1);
		p->left = new Node();
		BuildTree(p->left);
	}
	if (treeStr[0] == 'U' || treeStr.length() == 0) {
		if (p->left == NULL) {
			p->c = vocab[0];
			vocab.erase(0, 1);
		}
		if (p->left != NULL && p->right == NULL) {
			treeStr.erase(0, 1);
			p->right = new Node();
			BuildTree(p->right);
		}
	}
}

void Unzip::print_Tree(Node * p, int level)
{
	if (p)
	{
		print_Tree(p->left, level + 1);
		for (int i = 0; i < level; i++) std::cout << "   ";
		std::cout << "Nd" << std::endl;
		print_Tree(p->right, level + 1);
	}
}