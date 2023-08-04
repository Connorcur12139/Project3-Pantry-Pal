#include "RecipeManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <QDir>

vector<string> RecipeManager::parseIngredients(string ingredientsStr) {
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




void RecipeManager::ReadFromCSVFile(AVLTree& Tree, string filepath, vector<string>& uniqueIngredients)
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

        while(getline(csv,line))
        //for(int i = 0; i < 100; i++)
        {
            getline(csv, line);
            istringstream stream(line);
            //initialize values to default
            int id = 0;
            string idstr;
            getline(stream, idstr, ',');

            try {
                id = std::stoi(idstr);
            } catch (const std::invalid_argument& e) {
                // Handle the error (e.g., print an error message or skip the line)
                std::cerr << "Error converting ID to integer: " << e.what() << std::endl;
                continue; // Skip this line and move to the next one
            }
            //id = stoi(idstr);

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

RecipeManager::RecipeManager() : useBFS(false) {}

void RecipeManager::initializeTree(const QString& filePath) {
    // Load recipes from the CSV file and insert them into the AVLTree
    //QString fullPath = QDir::current().filePath(filePath);
    ReadFromCSVFile(avlTree, "/Users/ameliareeves/Qt_Projects/PantryPal/Recipes.csv", uniqueIngredients);
    //std::cout << "CSV File Path: " << fullPath.toStdString() << std::endl;


}

void RecipeManager::findBestMatchingRecipe(const QStringList& pantryItems) {
    // Call the AVLTree's breadthFirstSearch method with the pantry items
    std::vector<std::string> pantryItemsStd;
    for (const QString& pantryItem : pantryItems) {
        pantryItemsStd.push_back(pantryItem.toStdString());
    }

    Node* foundRecipe = nullptr;
    // Perform the search and store the results in the bestMatchingRecipeIngredients vector
    if (useBFS) {
        auto bfsStart = std::chrono::high_resolution_clock::now();
        foundRecipe = avlTree.breadthFirstSearch(pantryItemsStd);
        auto bfsStop = std::chrono::high_resolution_clock::now();
        bfsDuration = bfsStop - bfsStart;

    } else {
        auto dfsStart = std::chrono::high_resolution_clock::now();
        foundRecipe = avlTree.depthFirstSearch(pantryItemsStd);
        auto dfsStop = std::chrono::high_resolution_clock::now();
        dfsDuration = dfsStop - dfsStart;
    }

    if (foundRecipe) {
        qDebug() << "Recipe Found: " << QString::fromStdString(foundRecipe->name);
    } else {
        qDebug() << "No Recipe Found.";
    }
    // Check if a matching recipe is found
    if (foundRecipe) {
        // Store the details of the best matching recipe in the bestMatchingRecipeDetails variable
        QString recipeDetails = QString::fromStdString(foundRecipe->name) + "\n\n|||" +
                                QString::fromStdString(foundRecipe->servings);

        recipeDetails += "\n\n|||Steps:\n";
        QString directions = QString::fromStdString(foundRecipe->steps);
        directions.replace("\"['", "");
        directions.replace("', '", "\n\n");
        recipeDetails += directions + "\n";

        // Append the ingredients to the recipeDetails
        recipeDetails += "\n\n|||Ingredients:\n";
        for (const std::string& ingredient : foundRecipe->ingredients_amount) {
            QString ingredientText = QString::fromStdString(ingredient);

            // Remove double quotes from the beginning and end of the ingredient
            ingredientText.replace("\"","");
            ingredientText.replace("   "," ");

            recipeDetails += QString::fromStdString(ingredientText.toStdString()) + "\n";
        }
    bestMatchingRecipeDetails = recipeDetails;

    } else {
        // No matching recipe found
        bestMatchingRecipeDetails.clear();
        bestMatchingRecipeDetails = "No matching recipe found.";
    }
}

void RecipeManager::setUseBFS(bool useBFS) {
    this->useBFS = useBFS;
}



QString RecipeManager::getBestMatchingRecipeDetails() const  {
    // Return the details of the best matching recipe
    return bestMatchingRecipeDetails;
}

std::vector<std::string> RecipeManager::getUniqueIngredients() {
    return uniqueIngredients;
}
