/**
 * @file mainwindow.cpp
 * @brief Implementation of the MainWindow class.
 * * This file contains the logic for the Graphical User Interface, including setting up
 * the widgets, layouts, handling button clicks (slots), and interacting with the
 * WordLadderGame backend.
 */
#include "mainwindow.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <set>

/**
 * @brief Constructs the MainWindow, setting up all tabs, layouts, and UI elements.
 * @param parent Pointer to the parent widget.
 */
//constructor of the window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Word Ladder Game");
    this->resize(600, 450);

    QTabWidget *tabWidget = new QTabWidget(this); //create the system who holds the tabs
    this->setCentralWidget(tabWidget);

    //Automat Mode
    QWidget *autoTab = new QWidget(); //create a new widged where we will draw all the elements
    QVBoxLayout *autoLayout = new QVBoxLayout(autoTab); //add all elements vertically from up to down
    QFormLayout *formLayout = new QFormLayout(); //create form-based layout which associates labels with input fields

    // Le instanțiem folosind pointerii din clasa MainWindow
    startWordEdit = new QLineEdit(); //create first box for writing
    targetWordEdit = new QLineEdit(); //create second box for writing
    pathList = new QListWidget(); //create the big list

    //set labels on the rows
    formLayout->addRow(new QLabel("Start word:"), startWordEdit);
    formLayout->addRow(new QLabel("Target word:"), targetWordEdit);

    //create a classic button that can be clicked
    QPushButton *solveBtn = new QPushButton("Show transformation");

    //put the pieces together and name the whole
    autoLayout->addLayout(formLayout);
    autoLayout->addWidget(solveBtn);
    autoLayout->addWidget(pathList);
    tabWidget->addTab(autoTab, "Automatic Mode");

    // Game Mode
    QWidget *gameTab = new QWidget(); //create the main widget
    QVBoxLayout *gameLayout = new QVBoxLayout(gameTab); //assign to it a vertical layout

    // Create the "deck of cards" that holds the widgets
    gameStackedWidget = new QStackedWidget();

    // Widget 1
    gameSetupWidget = new QWidget();//create the main widget
    QVBoxLayout *setupLayout = new QVBoxLayout(gameSetupWidget); //assign to it a vertical layout
    QFormLayout *setupForm = new QFormLayout(); //assign to it a explicative form

    //create the two boxes where the user will type
    playerNameEdit = new QLineEdit();
    playerNameEdit->setPlaceholderText("eg: Amalia");
    wordLengthEdit = new QLineEdit();
    wordLengthEdit->setPlaceholderText("eg: 4");

    //populate the form: place the label next to the writing box
    setupForm->addRow(new QLabel("Player Name:"), playerNameEdit);
    setupForm->addRow(new QLabel("Word Length:"), wordLengthEdit);

    //create the starting button
    QPushButton *startGameBtn = new QPushButton("Start Game");

    //put together the pieces for the first widget
    setupLayout->addLayout(setupForm);
    setupLayout->addWidget(startGameBtn);
    setupLayout->addStretch(); // Pushes elements to the top (estetic reasons only)

    // Widget 2
    gameplayWidget = new QWidget();//create the second widget
    QVBoxLayout *playLayout = new QVBoxLayout(gameplayWidget);//assign to it a vertical layout

    //create 2 labels that will output permanently the current and target words
    targetWordLbl = new QLabel("Target: -");
    currentWordLbl = new QLabel("Current Word: -");

    // horizontal row for the input and the 2 buttons
    QHBoxLayout *inputLayout = new QHBoxLayout();
    nextWordInput = new QLineEdit();
    nextWordInput->setPlaceholderText("Enter next word...");
    //the 2 action buttons
    QPushButton *submitWordBtn = new QPushButton("Submit");
    QPushButton *hintBtn = new QPushButton("Hint");
    //these 3 elements will stay next to each other on the horizontal line created previously
    inputLayout->addWidget(nextWordInput);
    inputLayout->addWidget(submitWordBtn);
    inputLayout->addWidget(hintBtn);

    //create the list with the valid words inputted by the user
    historyList = new QListWidget();

    //put together the pieces for the second widget
    playLayout->addWidget(targetWordLbl);
    playLayout->addWidget(currentWordLbl);
    playLayout->addLayout(inputLayout);
    playLayout->addWidget(new QLabel("History:"));
    playLayout->addWidget(historyList);

    // add widgets to the stack
    gameStackedWidget->addWidget(gameSetupWidget); // index 0 (shown first)
    gameStackedWidget->addWidget(gameplayWidget);  // index 1 (shown during game)

    //the widgets are introduced in the main layout of the tab
    gameLayout->addWidget(gameStackedWidget);
    tabWidget->addTab(gameTab, "Playing Mode");

    // connect the buttons to their functions
    connect(startGameBtn, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    connect(submitWordBtn, &QPushButton::clicked, this, &MainWindow::onSubmitWordClicked);
    connect(hintBtn, &QPushButton::clicked, this, &MainWindow::onHintClicked);

    //Analytics Mode
    QWidget *analyticsTab = new QWidget(); //create a new widget
    QVBoxLayout *analyticsLayout = new QVBoxLayout(analyticsTab); //add all elements vertically from up to down

    analyticsNameEdit = new QLineEdit(); //create a text box for the username
    analyticsNameEdit->setPlaceholderText("Enter username ..."); //create a guidance text
    QPushButton *showStatsBtn = new QPushButton("Load analystics"); //create a guidance text
    statsList = new QListWidget(); //create a widget for the list of analystics

    ////put the pieces together and name the whole
    analyticsLayout->addWidget(new QLabel("Enter player name:"));
    analyticsLayout->addWidget(analyticsNameEdit);
    analyticsLayout->addWidget(showStatsBtn);
    analyticsLayout->addWidget(statsList);
    analyticsLayout->addStretch();
    tabWidget->addTab(analyticsTab, "Analytics Mode");
    // connect the buttons to their functions
    connect(showStatsBtn, &QPushButton::clicked, this, &MainWindow::onShowAnalyticsClicked);

    // load the dictionary
    game.loadDictionary("/Users/amaliatanase/CLionProjects/WordLadder/dictionary.txt");
    game.buildGraph();

    // connection (Signal & Slot)
    // when solveBtn emits the semnal 'clicked', function 'onSolveButtonClicked' is being called
    connect(solveBtn, &QPushButton::clicked, this, &MainWindow::onSolveButtonClicked);
}

/**
 * @brief Destructor for the MainWindow class.
 */
MainWindow::~MainWindow() {}

/**
 * @brief Executes the pathfinding algorithm when the "Show transformation" button is clicked.
 */
// function that is being executed when the button is clicked
void MainWindow::onSolveButtonClicked()
{
    // clear the list
    pathList->clear();

    // take the text from the interface
    // Qt uses QString, but my function uses std::string => convert it
    std::string startWord = startWordEdit->text().toUpper().toStdString();
    std::string targetWord = targetWordEdit->text().toUpper().toStdString();

    // traverse with the BFS algorithm
    std::vector<std::string> path = game.findShortestPath(startWord, targetWord);

    // output the result in the interface
    if (path.empty()) {
        pathList->addItem("No path has been found.");
    } else {
        pathList->addItem(QString("Path with %1 steps has been found:").arg(path.size()));
        for (const std::string& word : path) {
            // convert std::string back to QString to put it in the list
            pathList->addItem(QString::fromStdString(word));
        }
    }
}

/**
 * @brief Initiates a new game session, generating target words and switching the UI view.
 */
void MainWindow::onStartGameClicked()
{
    // take the inputted data
    playerName = playerNameEdit->text();
    QString lengthStr = wordLengthEdit->text();

    // did both fields been completed?
    if (playerName.isEmpty() || lengthStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the username and the length of the word.");
        return;
    }

    int length = lengthStr.toInt();

    if (game.generateValidGame(length, currentPlayingWord, targetPlayingWord)) {
        // initialize the atributes
        hintsUsed = 0;
        movesUsed = 0;
        wordsUsedHistory.clear();
        wordsUsedHistory.push_back(currentPlayingWord);

        // compute the optimum path with BFS
        std::vector<std::string> bestPath = game.findShortestPath(currentPlayingWord, targetPlayingWord);
        optimalMoves = bestPath.size() - 1; // no steps = no words - start

        // update the game widget with the new words
        targetWordLbl->setText("Target: <b>" + QString::fromStdString(targetPlayingWord) + "</b>");
        currentWordLbl->setText("Current word: <b>" + QString::fromStdString(currentPlayingWord) + "</b>");

        historyList->clear();
        historyList->addItem(QString::fromStdString(currentPlayingWord));

        // Pass to the game widget
        gameStackedWidget->setCurrentIndex(1); // 1 = gameplayWidget

    } else {
        QMessageBox::warning(this, "Error", "No game of this length could have been generated.");
    }
}

/**
 * @brief Processes the word submitted by the user, validates it, and updates game progress.
 */
void MainWindow::onSubmitWordClicked() {
    // take the inputted word
    std::string newWord = nextWordInput->text().toUpper().toStdString();
    nextWordInput->clear(); // clear the box for the next move

    // check if it has the same length
    if (newWord.length() != currentPlayingWord.length()) {
        QMessageBox::warning(this, "Invalid move", "The word needs to have the same length!");
        return;
    }

    // check if only a letter is different
    int diffCount = 0;
    for (size_t i = 0; i < newWord.length(); ++i) {
        if (newWord[i] != currentPlayingWord[i]) {
            diffCount++;
        }
    }
    if (diffCount != 1) {
        QMessageBox::warning(this, "Invalid move", "Only a letter needs to be different from the last word!");
        return;
    }

    // check if the word exists in the dictionary
    if (!game.isValidWord(newWord)) {
        QMessageBox::warning(this, "Invalid move", "The word does not exist in the dictionary!");
        return;
    }

    // the move is valid
    // update the stats
    currentPlayingWord = newWord;
    movesUsed++;
    wordsUsedHistory.push_back(newWord);
    // update the visual interface
    currentWordLbl->setText("Current word: <b>" + QString::fromStdString(currentPlayingWord) + "</b>");
    historyList->addItem(QString::fromStdString(newWord));
    // Check the winning confition
    if (currentPlayingWord == targetPlayingWord) {
        QString winMessage = QString("Congratulations, %1!\nYou have reached the target with %2 moves. \nThe optimal path was %3 moves.\nYou used %4 hints.")
                                 .arg(playerName)
                                 .arg(movesUsed)
                                 .arg(optimalMoves)
                                 .arg(hintsUsed);

        QMessageBox::information(this, "You won!", winMessage);

        //save data to CSV
        QString filename = "/Users/amaliatanase/WordLadderGUI/" + playerName + ".csv"; // the file will be named after the user
        QFile file(filename);

        // open the file in append mode
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);

            // if the file is new, write the header of it
            if (file.size() == 0) {
                out << "Date,Time,Start,Target,Moves,Optimal,Hints,History\n";
            }

            QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd");
            QString currentTime = QDateTime::currentDateTime().toString("HH:mm:ss");

            // create the string with the history of the moves
            QString wordsUsedStr;
            for (size_t i = 0; i < wordsUsedHistory.size(); ++i) {
                wordsUsedStr += QString::fromStdString(wordsUsedHistory[i]);
                if (i < wordsUsedHistory.size() - 1) wordsUsedStr += "->";
            }

            // write the data into the file, separated by commas
            out << currentDate << ","
                << currentTime << ","
                << QString::fromStdString(wordsUsedHistory.front()) << ","
                << QString::fromStdString(targetPlayingWord) << ","
                << movesUsed << ","
                << optimalMoves << ","
                << hintsUsed << ","
                << "\"" << wordsUsedStr << "\"\n";
            file.close();
        }

        // go back to the setUp widget after clicking OK
        gameStackedWidget->setCurrentIndex(0);
        playerNameEdit->clear();
        wordLengthEdit->clear();
        historyList->clear();
    }
}

/**
 * @brief Provides a hint to the user by highlighting the optimal next letter to change.
 */
void MainWindow::onHintClicked() {
    // check if the game is not finished yet
    if (currentPlayingWord == targetPlayingWord) return;
    // increase the hints contor
    hintsUsed++;

    // find the optimal path with BFS starting from current word
    std::vector<std::string> path = game.findShortestPath(currentPlayingWord, targetPlayingWord);

    // if a path exists, path[0] is the current word and path[1] is the next best move
    if (path.size() > 1) {
        std::string optimalNextWord = path[1];
        QString hintHtml = "To reach the target faster, try the word:<br><br><span style='font-size:18px;'><b>";
        for (size_t i = 0; i < optimalNextWord.length(); ++i) {
            if (currentPlayingWord[i] != optimalNextWord[i]) {
                // use red font for the different letter
                hintHtml += "<font color='red'>" + QString(optimalNextWord[i]) + "</font>";
            } else {
                // letter is the same
                hintHtml += QString(optimalNextWord[i]);
            }
        }
        hintHtml += "</b></span>";
        //output the box with the hint
        //information will output a drawing
        QMessageBox::information(this, "Hint", hintHtml);

    } else {
        QMessageBox::warning(this, "Oops", "Unfortunately you got stuck, there is no valid path from this word to the target!");
    }
}

/**
 * @brief Parses the user's CSV file to calculate and display gameplay statistics.
 */
void MainWindow::onShowAnalyticsClicked()
{
    //find and open the file for the given user
    QString name = analyticsNameEdit->text();
    QString filename = "/Users/amaliatanase/WordLadderGUI/" + name + ".csv";
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No file found for this user!");
        return;
    }

    QTextStream in(&file);
    std::set<std::string> uniqueWords;
    int totalGames = 0;

    // skip the header
    in.readLine();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        // history of the words is the last column
        QString history = fields.last();
        // clean the ""
        history = history.replace("\"", "");

        // split the words by "->"
        QStringList words = history.split("->");
        for (int i = 0; i < words.size(); ++i) {
            uniqueWords.insert(words[i].toStdString());
        }
        totalGames++;
    }
    file.close();

    // output the stats
    statsList->clear();
    statsList->addItem(QString("Total games played: %1").arg(totalGames));
    statsList->addItem(QString("No of unique words used: %1").arg(uniqueWords.size()));

    statsList->addItem("Used words:");
    for (const auto &w : uniqueWords) {
        statsList->addItem(QString::fromStdString(w));
    }
}