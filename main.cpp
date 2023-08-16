#include <iostream>
#include <random>
#include <unordered_map>
#include "WordFileReader.h"
#include "TextFormatter.h"

// Constants
const unsigned short int totalWordleWords = 2309;
const unsigned short int wordLength = 5;
const char validWordsFilePath[] = "/Users/johan/CLionProjects/WordleClone/WordData/valid-wordle-words.txt";
const char wordleWordsFilePath[] = "/Users/johan/CLionProjects/WordleClone/WordData/wordle-wordlist.txt";

// Struct to hold formatted guess information
struct FormattedGuess {
    std::string guess;
    std::string formattedGuess;
};

// Function declarations
std::string selectRandomWord();
int getRandomNumberInRange(int minValue, int maxValue);
std::string getUserGuess();
bool isValidGuess(std::string &guess);
bool isStringInCollection(const std::string &target, const std::vector<std::string> &vec);
bool askForNewGame();

bool evaluateGuess(const std::string &guess);
bool isGuessCorrect(const std::string &guess);
bool doesRoundWordContainLetter(const char &letter);
std::string matchLetterColors(const std::string &guess);
std::string colorFormatGuess(const std::string &guess, std::string &coloredGuess, std::string &letter);
std::string &getColorFormattedLetter(const std::string &guess, std::string &letter, int i);
std::string &applyColorToLetter(const std::string &guess, std::string &letter, int i);
std::string formatColoredLetter(int i, const std::string &guess, const std::string &color);

void updatePreviousGuesses(const std::string &guess, const std::string &formattedGuess,
                           std::vector<FormattedGuess> &previousGuesses);
void displayPreviousGuesses(const std::vector<FormattedGuess> &previousGuesses);
void processGuessAttempt(std::vector<FormattedGuess> &previousGuesses);
void runMainGameLoop();

// Global variables
bool isGameRunning = true;
bool isSolved = false;
std::string roundWord;
int attemptsLeft;
std::vector<std::string> wordleWords = getWordData(wordleWordsFilePath);
std::vector<std::string> allValidWords = getWordData(validWordsFilePath);


// Main function
int main() {
    while (isGameRunning) {
        runMainGameLoop();
    }

    return 0;
}

void runMainGameLoop() {
    attemptsLeft = 6;

    roundWord = selectRandomWord();
    std::vector<FormattedGuess> previousGuesses;

    while (!isSolved && attemptsLeft > 0) {
        processGuessAttempt(previousGuesses);
    }

    std::cout << "Congrats! You guessed the word correct!" << std::endl;

    isGameRunning = askForNewGame();
}

bool askForNewGame() {
    std::string playAgain;

    std::cout << "Play again? (y/N): ";
    std::getline(std::cin, playAgain);

    if (playAgain == "y") {
        return true;
    }
    return false;
}

void processGuessAttempt(std::vector<FormattedGuess> &previousGuesses) {
    attemptsLeft -= 1;

    std::string guess = getUserGuess();
    isSolved = evaluateGuess(guess);

    // Color format the current guess
    std::string formattedGuess = matchLetterColors(guess);

    updatePreviousGuesses(guess, formattedGuess, previousGuesses);
    displayPreviousGuesses(previousGuesses);

    std::cout << "Attempts left: " << attemptsLeft << std::endl;
}

void updatePreviousGuesses(const std::string &guess, const std::string &formattedGuess,
                           std::vector<FormattedGuess> &previousGuesses) {
    FormattedGuess formattedGuessObj;
    formattedGuessObj.guess = guess;
    formattedGuessObj.formattedGuess = formattedGuess;
    previousGuesses.push_back(formattedGuessObj);
}

void displayPreviousGuesses(const std::vector<FormattedGuess> &previousGuesses) {
    for (const auto &prevGuess: previousGuesses) {
        std::cout << prevGuess.formattedGuess << std::endl;
    }
    std::cout << std::endl;
}

std::string selectRandomWord() {
    return wordleWords[getRandomNumberInRange(0, totalWordleWords)];
}

int getRandomNumberInRange(const int minValue, const int maxValue) {
    std::random_device randomDevice;
    std::mt19937 randomNumberGenerator(randomDevice());

    std::uniform_int_distribution<int> distribution(minValue, maxValue);

    return distribution(randomNumberGenerator);
}

std::string getUserGuess() {
    std::string guess;
    bool isGuessValid;

    do {
        std::cout << "Enter guess: ";
        std::getline(std::cin, guess);

        isGuessValid = isValidGuess(guess);
    } while (!isGuessValid);


    return guess;
}

bool isValidGuess(std::string &guess) {
    bool isWordValid = isStringInCollection(guess, allValidWords);

    if (isWordValid)
        return true;

    std::cout << "Invalid word. Try again" << std::endl;
    return false;
}

bool isStringInCollection(const std::string &target, const std::vector<std::string> &vec) {
    return std::find(vec.begin(), vec.end(), target) != vec.end();
}

bool evaluateGuess(const std::string &guess) {
    if (isGuessCorrect(guess))
        return true;

    std::string coloredGuess = matchLetterColors(guess);
    return false;
}

bool isGuessCorrect(const std::string &guess) {
    return guess == roundWord;
}

std::string matchLetterColors(const std::string &guess) {
    std::string coloredGuess;
    std::string letter;

    coloredGuess = colorFormatGuess(guess, coloredGuess, letter);

    return coloredGuess;
}

std::string colorFormatGuess(const std::string &guess,  std::string &coloredGuess, std::string &letter) {
    for (int i = 0; i < wordLength; i++) {
        letter = getColorFormattedLetter(guess, letter, i);
        coloredGuess += letter;
    }
    return coloredGuess;
}

std::string &getColorFormattedLetter(const std::string &guess, std::string &letter, int currentIndex) {
    if (doesRoundWordContainLetter(guess[currentIndex])) {
        letter = applyColorToLetter(guess, letter, currentIndex);
    } else {
        letter = guess[currentIndex];
    }
    return letter;
}

std::string &applyColorToLetter(const std::string &guess, std::string &letter, int currentIndex) {
    if (guess[currentIndex] == roundWord[currentIndex]) {
        letter = formatColoredLetter(currentIndex, guess, "green");
    } else {
        if (doesRoundWordContainLetter(guess[currentIndex])) {
            letter = formatColoredLetter(currentIndex, guess, "yellow");
        } else {
            letter = guess[currentIndex];
        }
    }
    return letter;
}

bool doesRoundWordContainLetter(const char &letter) {
    return roundWord.find(letter) != std::string::npos;
}

std::unordered_map<std::string, std::function<std::string(const std::string &)>> colorFormats = {
        {"green",  TextFormatter::formatGreenText},
        {"yellow", TextFormatter::formatYellowText}
};

std::string formatColoredLetter(int currentIndex, const std::string &guess, const std::string &color) {
    std::string coloredLetter;
    std::string currentLetter;
    currentLetter += guess[currentIndex];

    coloredLetter = colorFormats[color](currentLetter);

    return coloredLetter;
}