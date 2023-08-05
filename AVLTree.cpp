#include "AVL.h"
using namespace std;

Node* AVLTree::breadthFirstSearch(const vector<string>& targetIngredients)
{
	queue<Node*> q;
	Node* foundRecipe = nullptr;
	int ingredient_matches = 0;
	q.push(root);

	while (!q.empty())
	{
		Node* node = q.front();
		q.pop();
		int currMatches = 0;
		for (int i = 0; i < targetIngredients.size(); i++)
		{
			if (find(node->ingredients.begin(), node->ingredients.end(), targetIngredients[i]) != node->ingredients.end())
				currMatches++;
		
		}
		if (currMatches > ingredient_matches)
		{
			foundRecipe = node;
			ingredient_matches = currMatches;
		}
				if (node->left) {
					q.push(node->left);
				}

				if (node->right) {
					q.push(node->right);
				}
	

	}
	cout << foundRecipe->name << endl;
	for (int i = 0; i < foundRecipe->ingredients.size(); i++)
	{
		cout << foundRecipe->ingredients[i] << endl;
	}
	cout << "Matches: " << ingredient_matches << endl;
	return foundRecipe;
}


Node* AVLTree::depthFirstSearch(const vector<string>& targetIngredients) {
	stack<Node*> stk;
	stk.push(root);
	Node* foundRecipe = nullptr;
	int ingredient_matches = 0;


	while (!stk.empty()) {
		Node* current = stk.top();
		stk.pop();
		int currMatches = 0;
		for (int i = 0; i < targetIngredients.size(); i++)
		{
			if (find(current->ingredients.begin(), current->ingredients.end(), targetIngredients[i]) != current->ingredients.end())
				currMatches++;

		}
		if (currMatches > ingredient_matches)
		{
			foundRecipe = current;
			ingredient_matches = currMatches;
		}

		if (current->right) {
			stk.push(current->right);
		}
		if (current->left) {
			stk.push(current->left);
		}
	}
	cout << foundRecipe->name << endl;
	for (int i = 0; i < foundRecipe->ingredients.size(); i++)
	{
		cout << foundRecipe->ingredients[i] << endl;
	}
	cout <<"Matches: "<<  ingredient_matches << endl;
	return foundRecipe;
}


int AVLTree::height(Node* node) {
	if (node == nullptr) {
		return -1;  // Height of an empty subtree is -1
	}
	int leftHeight = height(node->left);
	int rightHeight = height(node->right);
	return 1 + max(leftHeight, rightHeight);  // Height of the node is 1 + max height of its subtrees
}


// Checks balance of the tree Left - Right
int AVLTree::balance(Node* node) {
	if (node == nullptr) {
		return 0;
	}
	return height(node->left) - height(node->right);
}


// Performs Left Rotation
Node* AVLTree::rotateLeft(Node* node) {


	Node* newRoot = node->right;
	node->right = newRoot->left;
	newRoot->left = node;
	// Update heights
	node->height = max(height(node->left), height(node->right)) + 1;
	newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
	return newRoot;


}


// Performs Right Rotation
Node* AVLTree::rotateRight(Node* node) {
	Node* newRoot = node->left;
	if (newRoot == nullptr)
		return node;
	node->left = newRoot->right;
	newRoot->right = node;
	// Update heights
	node->height = max(height(node->left), height(node->right)) + 1;
	newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
	return newRoot;
}


Node* AVLTree::findMinNode(Node* node) {
	Node* current = node;
	while (current && current->left != nullptr) {
		current = current->left;
	}
	return current;
}


void AVLTree::insert(int id, string name, string description, vector<string> ingredients, int size_ingredients, vector<string> ingredients_amount, string serving_size, string servings, string steps) {
	root = insertHelper(root, id, name, description, ingredients, size_ingredients, ingredients_amount, serving_size, servings, steps);
}

Node* AVLTree::insertHelper(Node* node, int id, string name, string description, vector<string> ingredients, int size_ingredients, vector<string> ingredients_amount, string serving_size, string servings, string steps) {
	// Steps:
	// 1. Traverse through tree starting with root and insert node in correct spot
	// 2. Get the balance factor of this ancestor node to check whether this node becomes unbalanced (if unbalanced, rotate)
	// 3. Update the height of the ancestor node, and perform rotations on the tree as necessary ***After recursion
	// 4. Print "successful" if insertion is successful and "unsuccessful" otherwise

	if (node == nullptr) {     // First Node
		cout << count++ << ". successful" << endl;
		return new Node(id, name, description, ingredients, size_ingredients, ingredients_amount, serving_size, servings, steps);

	}

	if (id < node->id) {     // Insert Left
		node->left = insertHelper(node->left, id, name, description, ingredients, size_ingredients, ingredients_amount, serving_size, servings, steps);
	}
	else if (id > node->id) {     // Insert Right    
		node->right = insertHelper(node->right, id, name, description, ingredients, size_ingredients, ingredients_amount, serving_size, servings, steps);
	}

	// Update Height
	node->height = 1 + max(height(node->left), height(node->right));

	// Check balance and rotate if necessary
	// Left-Left case
	if (balance(node) > 1 && id < node->left->id) {
		return rotateRight(node);
	}

	// Right-Right case
	if (balance(node) < -1 && id > node->right->id) {
		return rotateLeft(node);
	}

	// Left-Right case
	if (balance(node) > 1 && id > node->left->id) {
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}

	// Right-Left case
	if (balance(node) < -1 && id < node->right->id) {
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}

	return node;



}

