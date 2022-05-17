#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QRegularExpression>

#include "DeckWindow.h"


const std::string dataBaseParth = "database.sql";
constexpr size_t windowWightInPx = 1200;
constexpr size_t windowHeightInPx = 800;

void centerWindow(QWidget *widget);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Options->setCurrentIndex(0);

    setFixedSize(windowWightInPx,windowHeightInPx);
    centerWindow(this);

    this->setWindowTitle("Deckbuilder");

    cardContainer = std::make_unique<CardsContainer>(ui->Cards);
    for(auto& it : cardContainer->getCards()){
        connect(it, &QPushButton::clicked, this, &MainWindow::cardClicked);
    }

    currentRequest = std::make_unique<CardApi::Filters>(); 


    dataBase = SqliteHelper::open_db(dataBaseParth.c_str());

    cardContainer->displayCards(CardApi::searchCards(dataBase, *currentRequest));

    readSaveIfExist();
    setLogo();

}

MainWindow::~MainWindow()
{
    saveFile.open(".savesFile.txt", std::ios::trunc | std::ios::out);
    if(saveFile.is_open())
    {
        saveFile << deckbuilder.getEncodedDeck();
        saveFile.close();
    }else{
        ErrorWindow("Can't find saves file");
    }

    delete ui;  
}
void MainWindow::readSaveIfExist()
{
    std::string deckCode;
    saveFile.open(".savesFile.txt", std::ios::in);
    if(saveFile.is_open())
    {
        getline(saveFile, deckCode);
        deckbuilder.setFromEncoded(dataBase, deckCode);
        saveFile.close();
    }
    refreshDeckDisplay();
}

void MainWindow::refreshDeckDisplay()
{
    std::string deckText_ = "";
    for(const auto& [key, value] : deckbuilder.getCardCountMap()){

        deckText_ += std::to_string(value);
        deckText_ += "x ";
        deckText_ += key.name;
        deckText_ += '\n';
    }
    ui->OptionsAndDeck->findChild<QPlainTextEdit*>("DeckDisplay")->setPlainText(QString::fromStdString(deckText_));
}

void MainWindow::on_Search_B_clicked(){
    if (ui->HealthFrom->text().isEmpty())
    {
        currentRequest->minHealth.reset();
    }
    else
    {
       currentRequest->minHealth = ui->HealthFrom->text().toInt();
    }

    if (ui->HealthTo->text().isEmpty())
    {
        currentRequest->maxHealth.reset();
    }
    else
    {
       currentRequest->maxHealth = ui->HealthTo->text().toInt();
    }

    if (ui->CostFrom->text().isEmpty())
    {
        currentRequest->minCost.reset();
    }
    else
    {
       currentRequest->minCost = ui->CostFrom->text().toInt();
    }

    if (ui->CostTo->text().isEmpty())
    {
        currentRequest->maxCost.reset();
    }
    else
    {
       currentRequest->maxCost = ui->CostFrom->text().toInt();
    }

    if (ui->AttackFrom->text().isEmpty())
    {
        currentRequest->minAttack.reset();
    }
    else
    {
       currentRequest->minAttack = ui->AttackFrom->text().toInt();
    }

    if (ui->AttackTo->text().isEmpty())
    {
        currentRequest->maxAttack.reset();
    }
    else
    {
       currentRequest->maxAttack = ui->AttackTo->text().toInt();
    }
    currentRequest->cardName = ui->Name_T->text().toStdString();

    currentRequest->rarityNames = convertCheckbox("Rarity_?");
    //currentRequest->cardType_ = convertCheckbox("Typ_?"); //TODO
    currentRequest->regionNames = convertCheckbox("Region_??");

    cardContainer->displayCards(CardApi::searchCards(dataBase, *currentRequest));

}
std::vector<std::string> MainWindow::convertCheckbox(std::string regex){
    QRegularExpression chexbox_regex(regex.c_str());
    std::vector<std::string> checkboxNames;

    for(auto& it : ui->OptionsAndDeck->findChildren<QCheckBox*>(chexbox_regex)){
        if(it->isChecked()){
                checkboxNames.push_back(it->text().toUtf8().constData());
        }
    }
    return checkboxNames;
}
void MainWindow::cardClicked(){
    QPushButton *button = (QPushButton *)sender();
    displayCardWindow(button->property("Id").toString().toStdString());

}
void MainWindow::displayCardWindow(std::string cardId){
    CardWindow cardWindow(cardId, ui->OptionsAndDeck->findChild<QPlainTextEdit*>("DeckDisplay"), &deckbuilder, &dataBase, this);
    cardWindow.setModal(true);
    cardWindow.exec();
}
void MainWindow::setLogo()
{
    QLabel* icon = ui->SearchEngine->findChild<QLabel*>("Icon");
    QPixmap picture(":/img/icon");
    int wp = icon->width();
    int hp = icon->height();
    icon->setPixmap(picture.scaled(wp,hp,Qt::KeepAspectRatio));
}

void centerWindow(QWidget *widget){

    int x, y;
    int screenWidth;
    int screenHeight;

    QDesktopWidget *desktop = QApplication::desktop();

    screenWidth = desktop->width();
    screenHeight = desktop->height();


    x = (screenWidth - windowWightInPx) / 2;
    y = (screenHeight - windowHeightInPx) / 2;

    widget->setGeometry(x,y,windowWightInPx,windowHeightInPx);

}



void MainWindow::on_Health_B_clicked()
{
    ui->Options->setCurrentIndex(1);
}

void MainWindow::on_Cost_B_clicked()
{
     ui->Options->setCurrentIndex(2);
}

void MainWindow::on_Attack_B_clicked()
{
     ui->Options->setCurrentIndex(3);
}

void MainWindow::on_Type_B_clicked()
{
     ui->Options->setCurrentIndex(4);
}

void MainWindow::on_Rarity_B_clicked()
{
     ui->Options->setCurrentIndex(5);
}

void MainWindow::on_ImpExpDeck_B_clicked()
{
    ui->Options->setCurrentIndex(6);
}

void MainWindow::on_Region_B_clicked()
{
     ui->Options->setCurrentIndex(7);
}


void MainWindow::on_GoNext_B_clicked()
{
    int page = ui->NumberOfPage->text().toInt();
    if(page < 1){
        ui->NumberOfPage->setText("2");
        currentRequest->setPage(2);
    }else{
        ui->NumberOfPage->setText(QString::number(++page));
        currentRequest->setPage(page);
    }

    cardContainer->displayCards(CardApi::searchCards(dataBase, *currentRequest));
}

void MainWindow::on_GoBack_B_clicked()
{
    int page = ui->NumberOfPage->text().toInt();
    if(page > 1){
        ui->NumberOfPage->setText(QString::number(--page));
        currentRequest->setPage(page);
    }

    cardContainer->displayCards(CardApi::searchCards(dataBase, *currentRequest));
}


void MainWindow::on_NumberOfPage_editingFinished()
{
    currentRequest->setPage(ui->NumberOfPage->text().toInt());
    cardContainer->displayCards(CardApi::searchCards(dataBase, *currentRequest));
}

void MainWindow::on_ShowDeck_B_clicked()
{   
        this->hide();
        DeckWindow deckWindow(&deckbuilder,this->geometry(), &dataBase, this);
        deckWindow.setModal(true);
        deckWindow.exec();
        deckWindow.deleteLater();

        std::string deckText_ = "";
        for(const auto& [key, value] : deckbuilder.getCardCountMap()){

            deckText_ += std::to_string(value);
            deckText_ += "x ";
            deckText_ += key.name;
            deckText_ += '\n';
        }
        ui->OptionsAndDeck->findChild<QPlainTextEdit*>("DeckDisplay")->setPlainText(QString::fromStdString(deckText_));

        this->show();
}



void MainWindow::on_Export_B_clicked()
{
    ui->Options->findChild<QPlainTextEdit*>("CodeDisplay")->setPlainText(QString::fromStdString(deckbuilder.getEncodedDeck()));
}

void MainWindow::on_Import_B_clicked()
{
    deckbuilder.setFromEncoded(dataBase, ui->Options->findChild<QPlainTextEdit*>("CodeDisplay")->toPlainText().toStdString());    
    refreshDeckDisplay();
}
