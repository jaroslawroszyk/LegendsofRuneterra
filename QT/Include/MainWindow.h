#pragma once

#include <QDesktopWidget>
#include <QMainWindow>
#include <memory>
#include <fstream>

#include "CardsContainer.h"
#include "CardWindow.h"
#include "DeckBuilder.hpp"
#include "searchEngine.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;

    void createCards();
    void displayCards();
    void readSaveIfExist();
    void refreshDeckDisplay();
    void setLogo();

    std::vector<std::string> convertCheckbox(std::string);
    std::unique_ptr<CardsContainer> cardContainer;
    std::unique_ptr<CardApi::Filters> currentRequest;
    DeckBuilder deckbuilder;
    SqliteHelper::unique_sqlite3 dataBase;
    std::fstream saveFile;

private slots:

    void on_Search_B_clicked();
    void on_Health_B_clicked();
    void on_Cost_B_clicked();
    void on_Attack_B_clicked();
    void on_Type_B_clicked();
    void on_Rarity_B_clicked();
    void on_Region_B_clicked();
    void cardClicked();
    void displayCardWindow(std::string cardId);

    void on_GoNext_B_clicked();
    void on_GoBack_B_clicked();

    void on_NumberOfPage_editingFinished();
    void on_ShowDeck_B_clicked();
    void on_ImpExpDeck_B_clicked();
    void on_Export_B_clicked();
    void on_Import_B_clicked();
};

