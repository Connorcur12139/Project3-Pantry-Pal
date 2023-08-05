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
	int height(Node* node);
	int balance(Node* node);
	Node* rotateLeft(Node* node);
	Node* rotateRight(Node* node);
	Node* findMinNode(Node* node);

	Node* insertHelper(Node* node, int id, string name, string description, vector<string> ingredients, int size_ingredients, vector<string> ingredients_amount, string serving_size, string servings, string steps);
	



	




public:
	Node* root;


	Node* breadthFirstSearch(const vector<string>& targetIngredients);
	Node* depthFirstSearch(const vector<string>& targetIngredients);
	//void depthFirstSearchInOrderHelper(Node* node);

	void insert(int id, string name, string description, vector<string> ingredients, int size_ingredients, vector<string> ingredients_amount, string serving_size, string servings, string steps);
	AVLTree() : root() {}  // ?
};

