#include <iostream>
#include <random>
#include <vector>
#include <cctype>
#include "WordFileReader.h"

const unsigned short int WORD_LENGTH = 5;
const std::string GREEN_TEXT = "\033[32m";
const std::string YELLOW_TEXT = "\033[33m";
const std::string RESET_COLOR = "\033[0m";

class TextFormatter {
public:
    static std::string formatTextToColor(const std::string &str, const std::string& color) {
        return color + str + RESET_COLOR;
    }

    static void toLowercase(std::string &str) {
        for (char &c: str)
            c = tolower(c);
    }
};

class WordleGame {
public:
    // constructor; get both word lists
    WordleGame(const std::string& wordleFilePath, const std::string& validWordsFilePath)
            : wordleWords(getWordData(wordleFilePath)),
              allValidWords(getWordData(validWordsFilePath)) {}

    void resetGame() {
        attemptsLeft = 6;
        isSolved = false;
        roundWord = selectRandomWord();
        previousGuesses.clear();
    }

    [[nodiscard]] bool isGameOver() const {
        return isSolved || attemptsLeft <= 0;
    }

    void processGuessAttempt() {
        attemptsLeft--;
        std::string guess = getUserGuess();
        isSolved = guess == roundWord;
        recordAndFormatGuess(guess);
    }

    void recordAndFormatGuess(const std::string &guess) {
        std::string formattedGuess;
        for (int i = 0; i < WORD_LENGTH; i++) {
            formattedGuess += getColorFormattedLetter(guess, i);
        }
        previousGuesses.push_back({guess, formattedGuess});
    }

    std::string getUserGuess() {
        std::string guess;
        do {
            std::cout << "Enter guess: ";
            std::getline(std::cin, guess);
            TextFormatter::toLowercase(guess);
        } while (!isValidGuess(guess));
        return guess;
    }

    void displayPreviousGuesses() const {
        for (const auto &prevGuess: previousGuesses) {
            std::cout << prevGuess.formattedGuess << std::endl;
        }
        std::cout << std::endl;
    }

    static bool askForNewGame() {
        std::string playAgain;
        std::cout << "Play again? (y/N): ";
        std::getline(std::cin, playAgain);
        return playAgain == "y";
    }

    static void displayGameOverMessage(const WordleGame &game) {
        if (game.isSolved) {
            std::cout << "Congratulations! You guessed the word correctly." << std::endl;
        } else {
            std::cout << "Almost! The correct word was " << WordleGame::roundWord << "." << std::endl;
        }
    }

    static std::string roundWord;
    bool isSolved = false;

private:
    std::vector<std::string> wordleWords;
    std::vector<std::string> allValidWords;
    int attemptsLeft = 6;

    struct FormattedGuess {
        std::string guess;
        std::string formattedGuess;
    };

    std::vector<FormattedGuess> previousGuesses;

    static int getRandomNumberInRange(int max) {
        std::random_device randomDevice;
        std::mt19937 randomNumberGenerator(randomDevice());
        std::uniform_int_distribution<int> distribution(0, max);
        return distribution(randomNumberGenerator);
    }

    std::string selectRandomWord() {
        return wordleWords[getRandomNumberInRange(wordleWords.size() - 1)];
    }

    [[nodiscard]] bool isValidGuess(const std::string &guess) const {
        return std::find(allValidWords.begin(), allValidWords.end(), guess) != allValidWords.end();
    }

    static std::string getColorFormattedLetter(const std::string &guess, int currentIndex) {
        char currentLetter = guess[currentIndex];
        if (isLetterInRoundWord(currentLetter)) {
            return applyColorToLetter(guess, currentIndex);
        }
        return {currentLetter};
    }

    static std::string applyColorToLetter(const std::string &guess, int currentIndex) {
        char currentLetter = guess[currentIndex];

        if (isCorrectPosition(currentLetter, currentIndex)) {
            return formatLetterAsGreen(currentLetter);
        }

        return formatLetterAsYellow(currentLetter);
    }

    static bool isCorrectPosition(char letter, int index) {
        return letter == roundWord[index];
    }

    static bool isLetterInRoundWord(char letter) {
        return roundWord.find(letter) != std::string::npos;
    }

    static std::string formatLetterAsGreen(char letter) {
        return TextFormatter::formatTextToColor(std::string(1, letter), GREEN_TEXT);
    }

    static std::string formatLetterAsYellow(char letter) {
        return TextFormatter::formatTextToColor(std::string(1, letter), YELLOW_TEXT);
    }
};

std::string WordleGame::roundWord;

int main() {
    const char validWordsFilePath[] = "/Users/johan/CLionProjects/WordleClone/WordData/valid-wordle-words.txt";
    const char wordleFilePath[] = "/Users/johan/CLionProjects/WordleClone/WordData/wordle-wordlist.txt";

    WordleGame game(wordleFilePath, validWordsFilePath);
    bool isGameRunning = true;

    while (isGameRunning) {
        game.resetGame(); // init
        while (!game.isGameOver()) {
            game.processGuessAttempt();
            game.displayPreviousGuesses();
        }

        WordleGame::displayGameOverMessage(game);

        isGameRunning = WordleGame::askForNewGame();
    }
    return 0;
}
