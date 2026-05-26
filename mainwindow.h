#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QStackedWidget>
#include "WordLadderGame.h"

/**
 * @class MainWindow
 * @brief The main graphical user interface for the Word Ladder Game.
 * * This class inherits from QMainWindow and manages the three main tabs of the application:
 * Automatic Mode (finding paths automatically), Playing Mode (interactive gameplay),
 * and Analytics Mode (viewing player statistics).
 */
class MainWindow : public QMainWindow //MainWindow inherits from QMainWindow
{
    Q_OBJECT //needed for the signals and slots mechanism

public:
    /**
     * @brief Constructor for the MainWindow class.
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    MainWindow(QWidget *parent = nullptr); //constructor

    /**
     * @brief Destructor for the MainWindow class.
     */
    ~MainWindow(); //destructor

private slots: //these functions need to receive a signal
    // this will run when the button is clicked
    /**
     * @brief Slot triggered when the "Show transformation" button is clicked in Automatic Mode.
     * Computes and displays the shortest path between the start and target words.
     */
    void onSolveButtonClicked();

    //needed for game modes
    /**
     * @brief Slot triggered when the "Start Game" button is clicked in Playing Mode.
     * Validates input, generates a valid game path, and switches to the gameplay screen.
     */
    void onStartGameClicked();

    /**
     * @brief Slot triggered when the user submits a new word during gameplay.
     * Validates the move (same length, single letter difference, dictionary presence)
     * and checks for the win condition.
     */
    void onSubmitWordClicked();

    /**
     * @brief Slot triggered when the user requests a hint.
     * Calculates the optimal next move using BFS and highlights the letter to change.
     */
    void onHintClicked();

    /**
     * @brief Slot triggered when the "Load analytics" button is clicked.
     * Reads the player's CSV file and calculates unique words and total games played.
     */
    void onShowAnalyticsClicked();

private:
    // interface elements for Automatic Mode
    QLineEdit *startWordEdit; //the box where the starting word will be typed in
    QLineEdit *targetWordEdit; //the box where the target word will be typed in
    QListWidget *pathList; //the box where the list will be outputted

    // interface elements for Game Mode
    QStackedWidget *gameStackedWidget; // Will keep the 2 widgets
    QWidget *gameSetupWidget; // Widget 1: Name and length of the word
    QWidget *gameplayWidget; // Widger 2: The actual game

    QLineEdit *playerNameEdit; //the box where the user's name will be typed in
    QLineEdit *wordLengthEdit; //the box where the length of the word will be typed in

    QLabel *currentWordLbl; //a label that indicates the current word
    QLabel *targetWordLbl; //a label that indicates the target word
    QLineEdit *nextWordInput; //the box where the user will type the next input
    QListWidget *historyList; // the box with the list of the words inputted so far

    QLineEdit *analyticsNameEdit; // box for name
    QListWidget *statsList; // list to output stats

    // variables that remember the state of the current game
    QString playerName; //save the username
    std::string currentPlayingWord; // rememeber the word reached by the user
    std::string targetPlayingWord; // remember the target word
    int hintsUsed; // the no of hints used by the user
    int movesUsed; // the no of steps done by the user
    int optimalMoves; // this will keep the number of moves given by the BFS (no of steps in the shortest path)
    std::vector<std::string> wordsUsedHistory;
    /**
     * @brief An instance of the backend game logic.
     */
    WordLadderGame game; //an instance of the game
};

#endif // MAINWINDOW_H