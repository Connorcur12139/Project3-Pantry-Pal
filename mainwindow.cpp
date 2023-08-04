#include "mainwindow.h"
#include "QtWidgets/qcheckbox.h"
#include "ui_mainwindow.h"
#include "RecipeManager.h"


#include <QCompleter>
#include <QStringListModel>
#include <QListWidgetItem>
#include <algorithm>
#include <QDir>
#include <chrono>
#include <iomanip>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    recipeManager = new RecipeManager();

    // Get the current application directory
    QString currentDir = QDir::currentPath();

    // Append the relative file path to the CSV file
    QString filePath = currentDir + "/Recipes.csv";
    recipeManager->initializeTree(filePath);

    // Retrieve the unique ingredients from the RecipeManager
    const std::vector<std::string>& uniqueIngredients = recipeManager->getUniqueIngredients();

    QStringList uniqueIngredientsQString;
    for (const std::string& ingredient : uniqueIngredients) {
        uniqueIngredientsQString.append(QString::fromStdString(ingredient));
    }

    // Create a QStringListModel and set it as the model for the QCompleter
    QStringListModel* model = new QStringListModel(uniqueIngredientsQString, this);
    QCompleter* completer = new QCompleter(model, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->popup()->setStyleSheet("color: #F9F4F1; background-color: #673C4F;");
    completer->setFilterMode(Qt::MatchContains);

    // Set the completer for the search bar
    ui->searchLineEdit->setCompleter(completer);


    // default checked dfs
    ui->dfsCheckBox->setChecked(true);
    recipeManager->setUseBFS(false);
    // Connect the clicked signals of the checkboxes to their respective slots
    connect(ui->dfsCheckBox, &QCheckBox::clicked, this, &MainWindow::onDfsCheckBoxClicked);
    connect(ui->bfsCheckBox, &QCheckBox::clicked, this, &MainWindow::onBfsCheckBoxClicked);


    // Connect the clicked signal of the search button to the searchButtonClicked slot
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchButtonClicked);

    // Connect the returnPressed signal of the search bar to the addSelectedItemToPantry slot
    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, &MainWindow::addSelectedItemToPantry);

    // Connect the itemClicked signal to your custom slot
    connect(ui->pantryListWidget, &QListWidget::itemClicked, this, &MainWindow::deletePantryItem);
}



void MainWindow::addSelectedItemToPantry()
{

    // Get the current text from the search bar
    QString searchText = ui->searchLineEdit->text().trimmed();

    // Check if the entered ingredient is in the list of unique ingredients
    const std::vector<std::string>& uniqueIngredients = recipeManager->getUniqueIngredients();
    if (std::find(uniqueIngredients.begin(), uniqueIngredients.end(), searchText.toStdString()) != uniqueIngredients.end()) {
        // Check if the ingredient is not already in the pantry
        if (std::find(pantryItems.begin(), pantryItems.end(), searchText) == pantryItems.end()) {
            pantryItems.push_back(searchText);
            ui->pantryListWidget->addItem(searchText);
        }
    }
    // Clear the search bar
    ui->searchLineEdit->clear();
}

void MainWindow::deletePantryItem(QListWidgetItem *item)
{
    // Get the selected item and remove it from the list widget and the pantryItems list
    if (item != nullptr) {
        int row = ui->pantryListWidget->row(item);
        if (row >= 0 && row < pantryItems.size()) {
            pantryItems.removeAt(row);
            delete ui->pantryListWidget->takeItem(row);
        }
    }
}


void MainWindow::on_clearAllButton_clicked()
{
    // Clear all items from the virtual pantry and the QStringListModel
    ui->pantryListWidget->clear();
    pantryItems.clear();
}

void MainWindow::searchButtonClicked() {
    // Get the current pantry items from the QListWidget
    QStringList pantryItems;
    for (int i = 0; i < ui->pantryListWidget->count(); ++i) {
        pantryItems.append(ui->pantryListWidget->item(i)->text());
    }
    if (pantryItems.size() == 0){
        return;
    }

    recipeManager->findBestMatchingRecipe(pantryItems);



    // Retrieve the details of the best matching recipe from RecipeManager
    QString bestMatchingRecipeDetails = recipeManager->getBestMatchingRecipeDetails();

    // If no matching recipe found, show an appropriate message in the ingredientsTextEdit
    if (bestMatchingRecipeDetails.isEmpty()) {
        ui->ingredientsTextEdit->setPlainText("No matching recipe found.");
    } else {
        // Split the best matching recipe details using "|||" as the delimiter
        QStringList recipeDetailsList = bestMatchingRecipeDetails.split("|||");
        if (recipeDetailsList.size() == 4) {
            QString recipeName = recipeDetailsList[0];
            QString servings = recipeDetailsList[1];
            QString directions = recipeDetailsList[2];
            QString ingredients = recipeDetailsList[3]; // Retrieve the ingredients

            // Update the recipeNameLineEdit and servingsLineEdit
            ui->recipeNameLineEdit->setText(recipeName);
            ui->servingsLineEdit->setText("Servings: " + servings);

            // Update the directionsTextEdit to display the directions
            ui->directionsTextEdit->setPlainText(directions);

            // Update the ingredientsTextEdit to display the unique ingredients without comma
            ui->ingredientsTextEdit->setPlainText(ingredients);
        }

    }
    // Retrieve the BFS and DFS durations from RecipeManager
    QString bfsDurationStr = "BFS: " + QString::number(recipeManager->getBFSDuration().count()) + " seconds";
    QString dfsDurationStr = "DFS: " + QString::number(recipeManager->getDFSDuration().count()) + " seconds";

    // Display the BFS and DFS durations in the respective line edits
    ui->bfsLineEdit->setText(bfsDurationStr);
    ui->dfsLineEdit->setText(dfsDurationStr);
}


void MainWindow::onDfsCheckBoxClicked() {
    if (ui->dfsCheckBox->isChecked()) {
        ui->bfsCheckBox->setChecked(false);
        recipeManager->setUseBFS(false); // Set to false for DFS
    }
}

void MainWindow::onBfsCheckBoxClicked() {
    if (ui->bfsCheckBox->isChecked()) {
        ui->dfsCheckBox->setChecked(false);
        recipeManager->setUseBFS(true); // Set to true for BFS
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


