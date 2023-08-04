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



// Find and Remove the account with the specified ID from the tree; Balancing isn't required after
void AVLTree::removeId(int id) {
	root = removeIdHelper(root, id);

}

Node* AVLTree::removeIdHelper(Node* node, int id) {
	// Steps:
	// 1. Ensure ID exists, use search()
	// 2. Traverse recursively through tree to reach desired node to delete
	//    ex. if (id < root->id) root->left = remove(root->left,id);
	// 3. Three scenarios ( no children, one child, two children) to handle removing the node
	// 4. Make sure to return the correct node (or null) at the end to reconnect subtrees at ancestor level
	if (node == nullptr) {
		cout << "unsuccessful" << endl;  // ID not found
		return nullptr;
	}

	if (id < (node->id)) {
		node->left = removeIdHelper(node->left, id);  // Search in the left subtree
	}
	else if (id > (node->id)) {
		node->right = removeIdHelper(node->right, id);  // Search in the right subtree
	}
	else {
		// Node to be deleted is found

		// Case 1: Node has no children or only one child
		if (node->left == nullptr) {
			Node* temp = node->right;
			delete node;
			cout << "successful" << endl;
			return temp;
		}
		else if (node->right == nullptr) {
			Node* temp = node->left;
			delete node;
			cout << "successful" << endl;
			return temp;
		}

		// Case 2: Node has two children
		Node* successor = findMinNode(node->right);  // Find the inorder successor
		node->size_ingredients = successor->size_ingredients;  // Replace the current node with its inorder successor
		node->name = successor->name;
		//cout << "successful" << endl;
		node->right = removeIdHelper(node->right, successor->size_ingredients);  // Remove the inorder successor from the right subtree

		// Update height 
		node->height = 1 + max(height(node->left), height(node->right));

		//cout << "successful" << endl;
		return node;
	}

	// Update height
	//node->height = 1 + max(height(node->left), height(node->right));

	//return node;
}






// Removes Nth node from inorder traversal of the tree 
// print successful/unsuccessful (e.g. N outside range), balance after remove not required
void AVLTree::removeInorderN(int n) {
	int count = 0; // Counter for tracking the Nth ID
	bool success = false; // Flag to indicate if removal is successful


	removeInorderNHelper(root, n, count, success);

	if (!success) {
		cout << "unsuccessful" << endl;
	}
}


Node* AVLTree::removeInorderNHelper(Node*& node, int n, int& count, bool& success) {
	// Steps:
	// 1. Find Nth node (n=0 means first node), could be an O(n) operation // (if you know its id??)
	//		a. check bounds on N
	//		b. Inorder traversal, keep track of which node you're at (slide 19 discussion 1, passing by ref)
	// 2. Delete it with remove() o(logn)
	//		a. Helper function (this)
	//		b. watch for memory leaks

	if (node == nullptr) {
		return node;
	}
	removeInorderNHelper(node->left, n, count, success);
	if (n == count) {
		success = true;
		return removeIdHelper(root, (node->id));
	}
	count++;
	removeInorderNHelper(node->right, n, count, success);
	return node;





}



// Search for the student with the specified Name from the tree.
void AVLTree::searchName(string name) {
	bool found = false;
	searchNameHelper(root, name, found);
	if (!found)
		cout << "unsuccessful" << endl;
}

void AVLTree::searchNameHelper(Node* node, string name, bool& found) {
	//If the student name was found, print the associated ID.
	//If the tree has more than one object with the same NAME, print each ID on a new line with no other spacesand in the same relative order as a pre - order traversal.
	//If the name does not exist within the tree, print unsuccessful.
	if (node != nullptr) {
		if (node->name == name) {
			cout << node->id << endl;
			found = true;
		}
		searchNameHelper(node->left, name, found);
		searchNameHelper(node->right, name, found);
	}
	return;
}



// Search for the student with the specified ID from the tree.
void AVLTree::searchId(int id) {
	return searchIdHelper(root, id);
}

void AVLTree::searchIdHelper(Node* node, int id) {
	// If the ID was found, print out their NAME.
	// If the ID does not exist within the tree, print unsuccessful.
	if (node == nullptr) {
		cout << "unsuccessful" << endl;  // Tree is empty
		return;
	}

	if (id == node->id) {
		cout << node->name << endl;  // Print the name of the student with the ID
		return;
	}
	else if (id < node->id) {
		searchIdHelper(node->left, id);  // Search in the left subtree
	}
	else if (id > node->id) {
		searchIdHelper(node->right, id);  // Search in the right subtree
	}
	else {
		cout << "unsuccessful" << endl;  // ID not found
	}

}



// Print out a comma separated inorder traversal of the names in the tree
void AVLTree::printInorder() {
	/*printInorderHelper(root);
	cout << endl;*/
	if (root == nullptr)  // NULL?
		return;
	//vector<string> names;
	printInorderHelper(root);
	//for (int i = 0; i < names.size() - 1; i++) {
	//	cout << i << "." << names[i] << endl;;
	//}
	//cout << names[names.size() - 1] << endl;
}

void AVLTree::printInorderHelper(Node* node) {
	/*static int counter = 1;
	if (node != nullptr) {
		printInorderHelper(node->left);
		cout << node->name;
		if (node->right != nullptr || counter == 1) {
			cout << ", ";
			counter++;
		}
		printInorderHelper(node->right);
	}*/
	if (node == nullptr)
		return;
	printInorderHelper(node->left);
	/*names.push_back(node->name);*/
	cout << node->name << endl;
	for (int i = 0; i < node->ingredients.size();i++)
	{
		cout << node->ingredients[i] << ',';
	}
	cout << endl << endl;

	printInorderHelper(node->right);
}



// Print out a comma separated preorder traversal of the names in the tree.
void AVLTree::printPreorder() {
	/*printPreorderHelper(root);
	cout << endl;*/
	if (root == nullptr)
		return;
	vector<string> names;
	printPreorderHelper(root, names);
	for (int i = 0; i < names.size() - 1; i++) {
		cout << names[i] << ", ";
	}
	cout << names[names.size() - 1] << endl;
}

void AVLTree::printPreorderHelper(Node* node, vector<string>& names) {
	/*if (node != nullptr) {
		cout << node->name;
		if (node->left != nullptr || node->right != nullptr) {
			cout << ", ";
		}
		printPreorderHelper(node->left);
		printPreorderHelper(node->right);
	}*/
	if (node != nullptr) {
		names.push_back(node->name);
		printPreorderHelper(node->left, names);
		printPreorderHelper(node->right, names);
	}
}



// Print out a comma separated postorder traversal of the names in the tree.
void AVLTree::printPostorder() {
	/*printPostorderHelper(root);
	cout << endl;*/
	if (root == nullptr)
		return;
	vector<string> names;
	printPostorderHelper(root, names);
	for (int i = 0; i < names.size() - 1; i++) {
		cout << names[i] << ", ";
	}
	cout << names[names.size() - 1] << endl;
}

void AVLTree::printPostorderHelper(Node* node, vector<string>& names) {
	/*if (node != nullptr) {
		printPostorderHelper(node->left);
		printPostorderHelper(node->right);
		cout << node->name;
		if (node != root) {
			cout << ", ";
		}
	}*/
	if (node == nullptr)
		return;
	printPostorderHelper(node->left, names);
	printPostorderHelper(node->right, names);
	names.push_back(node->name);
}



// Prints the number of levels that exist in the tree.
// Prints 0 if the head of the tree is null. For example, the tree in Fig. 1 has 4 levels.
void AVLTree::printLevelCount() {
	cout << height(root) + 1 << endl;
}

