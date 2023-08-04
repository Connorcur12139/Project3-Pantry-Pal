#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCompleter>
#include <QButtonGROUP>
#include <QStringListModel>
#include <QListWidgetItem>
#include <QLineEdit>
#include "RecipeManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void updateFruitList(const QString& searchText);
    void searchButtonClicked();
    void addSelectedItemToPantry();
    void deletePantryItem(QListWidgetItem *item);
    void on_clearAllButton_clicked();
    void onDfsCheckBoxClicked();
    void onBfsCheckBoxClicked();

private:
    Ui::MainWindow *ui;
    std::vector<QString> availableFruits;
    QStringList pantryItems; // QStringList to store the pantry items
    RecipeManager* recipeManager;



};
#endif // MAINWINDOW_H
