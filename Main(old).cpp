#include <iostream>
#include <sstream>
#include <algorithm> 
#include <cctype> 
#include "AVL.h"
#include <fstream>
#include <string>
#include <chrono>
using namespace std::chrono;

using namespace std;


vector<string> parseIngredients(string ingredientsStr) {
	vector<string> ingredients;
	istringstream iss(ingredientsStr);
	string ingredient;
	while (getline(iss, ingredient, ',')) {
		// Remove leading/trailing whitespaces and single quotes from the ingredient
		size_t firstPos = ingredient.find_first_not_of(" '");
		size_t lastPos = ingredient.find_last_not_of(" '");
		if (firstPos != string::npos && lastPos != string::npos) {
			ingredients.push_back(ingredient.substr(firstPos, lastPos - firstPos + 1));
		}
	}

	return ingredients;
}

void ReadFromCSVFile(AVLTree& Tree, string filepath, vector<string>& uniqueIngredients)
{
	ifstream csv(filepath);
	if (!csv.is_open())
	{
		cout << "Could not open: " << filepath << endl;

	}
	else
	{
		string line;
		getline(csv, line);

		//while(getline(csv,line))
		for(int i = 0; i < 10; i++)
		{
			getline(csv, line);
			istringstream stream(line);
			//initialize values to default
			int id = 0;
			string idstr;
			getline(stream, idstr, ',');
			id = stoi(idstr);

			string name = "";
			getline(stream, name, ',');

			string description = "";
			getline(stream, description, '[');

			//logic to parse the ingredients list and get the size of list
			int num_ingredients = 0;
			string ingredients = "";
			vector<string> ingredientList = {};
			getline(stream, ingredients, ']');// Might need to be changed to ']'
			ingredientList = parseIngredients(ingredients);
			num_ingredients = ingredientList.size();

			for (int i = 0; i < ingredientList.size(); i++)
			{
				if (find(uniqueIngredients.begin(), uniqueIngredients.end(), ingredientList[i]) == uniqueIngredients.end())
				{
					uniqueIngredients.push_back(ingredientList[i]);
				}
			}

			string dud = "";
			getline(stream, dud, '[');

			string ingredient_amounts = "";
			vector<string> ingredients_amountList = {};
			getline(stream, ingredient_amounts, ']');//Might need to be changed to ']'
			ingredients_amountList = parseIngredients(ingredient_amounts);


			//string dud = "";
			getline(stream, dud, ',');

			string serving_size = "";
			getline(stream, serving_size, ',');
			
			string servings = "";
			getline(stream, servings, ',');
			//servings = stoi(serv_string);

			string steps = "";
			getline(stream, steps, ']');

			//Insert into tree
			Tree.insert(id, name, description, ingredientList, num_ingredients, ingredients_amountList, serving_size, servings, steps);

		}

		csv.close();
	}


}



int main() {
	AVLTree tree;
	vector<string> searchList;
	string file = "recipes_w_search_terms_edit.csv";
	ReadFromCSVFile(tree, file, searchList);


	vector<string> search{"water", "onion", "red bell pepper", "butter", "salt"};
	//tree.printInorder();


	auto bfsStart = high_resolution_clock::now();
	tree.breadthFirstSearch(search);
	auto bfsStop = high_resolution_clock::now();

	auto bfsDuration = duration<double>(bfsStop - bfsStart);
	cout << "BFS time: " << bfsDuration.count() << " seconds" << endl;

	auto dfsStart = high_resolution_clock::now();
	tree.depthFirstSearch(search);
	auto dfsStop = high_resolution_clock::now();

	auto dfsDuration = duration<double>(dfsStop - dfsStart);

	cout << "DFS time: " << dfsDuration.count() << " seconds" << endl;

	//tree.printPreorder();
	return 0;
}








