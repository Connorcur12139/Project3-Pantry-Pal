#ifndef RECIPEMANAGER_H
#define RECIPEMANAGER_H
#pragma once
#include "AVL.h"

#include <QCompleter>
#include <QStringListModel>
#include <QListWidgetItem>
#include <algorithm>
enum class SearchAlgorithm;

class RecipeManager {
public:
    RecipeManager();
    void initializeTree(const QString& filePath); // Load recipes from the CSV file and insert them into the AVLTree
    void findBestMatchingRecipe(const QStringList& pantryItems); // Find the recipe with the most matching ingredients
    QString getBestMatchingRecipeDetails() const; // Get the details of the best matching recipe
    static void ReadFromCSVFile(AVLTree& Tree, string filepath, vector<string>& uniqueIngredients);
    static vector<string> parseIngredients(string ingredientsStr);
    std::vector<std::string> getUniqueIngredients();
    void setUseBFS(bool useBFS);
    std::chrono::duration<double> getBFSDuration() const {
        return bfsDuration;
    }

    std::chrono::duration<double> getDFSDuration() const {
        return dfsDuration;
    }
    QString filename;


    // Add other methods if needed

private:
    AVLTree avlTree; // Your AVLTree instance to manage the recipes
    QString bestMatchingRecipeDetails; // Store the details of the best matching recipe
    std::vector<std::string> uniqueIngredients;
    std::chrono::duration<double> bfsDuration;
    std::chrono::duration<double> dfsDuration;

    bool useBFS = false;
};
#endif // RECIPEMANAGER_H
