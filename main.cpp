#include "mainwindow.h"
#include "AVL.h"
#include "RecipeManager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <QDir>
using namespace std;



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PantryPal_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }



    AVLTree tree;
    RecipeManager recipeManager;
    vector<string> searchList;
    //string file = "recipes_w_search_terms_edit.csv";
    QString currentDir = QDir::currentPath();
    QString filePath = currentDir + "/Recipes.csv";

    recipeManager.ReadFromCSVFile(tree, filePath.toStdString(), searchList);


    //vector<string> search{"water", "onion", "red bell pepper", "butter", "salt"};
    //tree.printInorder();

    //tree.breadthFirstSearch(search);
    //tree.depthFirstSearch(search);



    MainWindow w;
    w.show();
    return a.exec();
}
