#pragma once
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <vector>
#include <cctype> 
#include <queue>
#include <stack>

using namespace std;


struct Node {

	//Inputs from csv file

	int id;
	string name;
	string description;
	vector<string> ingredients;
	int size_ingredients;
	vector<string> ingredients_amount;
	string serving_size;
	string servings;
	string steps;

	Node* left;
	Node* right;
	int height;

	// default constructor
	Node() : name(""), id(0), description(""), ingredients{}, size_ingredients(0), ingredients_amount{}, serving_size(""), servings(""), steps(""), left(nullptr), right(nullptr) {}

	// constructor with parameters
	Node(int id, string name, string description, vector<string> ingredients, int size_ingredients, vector<string> ingredients_amount, string serving_size, string servings, string steps) :id(0), name(name), description(description), size_ingredients(size_ingredients), ingredients(ingredients), ingredients_amount(ingredients_amount), serving_size(serving_size), servings(servings), steps(steps), left(nullptr), right(nullptr) {}

};

class AVLTree {
private:
	int count = 0;
	int height(Node* node);
	int balance(Node* node);
	Node* rotateLeft(Node* node);
	Node* rotateRight(Node* node);
	Node* findMinNode(Node* node);

	Node* insertHelper(Node* node, int id, string name, string description, vector<string> ingredients, int size_ingredients, vector<string> ingredients_amount, string serving_size, string servings, string steps);
	



	
	
	Node* removeIdHelper(Node* node, int id);
	Node* removeInorderNHelper(Node*& node, int n, int& count, bool& success);
	void searchNameHelper(Node* node, string name, bool& found);
	void searchIdHelper(Node* node, int id);
	void printInorderHelper(Node* node);
	void printPreorderHelper(Node* node, vector<string>& names);
	void printPostorderHelper(Node* node, vector<string>& names);





public:
	Node* root;


	Node* breadthFirstSearch(const vector<string>& targetIngredients);
	Node* depthFirstSearch(const vector<string>& targetIngredients);
	//void depthFirstSearchInOrderHelper(Node* node);

	void insert(int id, string name, string description, vector<string> ingredients, int size_ingredients, vector<string> ingredients_amount, string serving_size, string servings, string steps);
	void removeId(int id);
	void removeInorderN(int n);
	void searchName(string name);
	void searchId(int id);
	void printInorder();
	void printPreorder();
	void printPostorder();
	void printLevelCount();
	AVLTree() : root() {}  // ?
};

