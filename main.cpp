#include <iostream>
#include <random>
#include <vector>
#include "WordFileReader.h"

const unsigned short int WORD_LENGTH = 5;
const char validWordsFilePath[] = "/Users/johan/CLionProjects/WordleClone/WordData/valid-wordle-words.txt";
const char wordleWordsFilePath[] = "/Users/johan/CLionProjects/WordleClone/WordData/wordle-wordlist.txt";

class TextFormatter {
private:
    static const std::string resetColor;

public:
    static std::string formatGreenText(const std::string &str) {
        return "\033[32m" + str + TextFormatter::resetColor;
    }

    static std::string formatYellowText(const std::string &str) {
        return "\033[33m" + str + TextFormatter::resetColor;
    }
};

const std::string TextFormatter::resetColor = "\033[0m";

class WordleGame {
private:
    std::vector<std::string> wordleWords;
    std::vector<std::string> allValidWords;
    static std::string roundWord;
    int attemptsLeft = 6;
    bool isSolved = false;

    struct FormattedGuess {
        std::string guess;
        std::string formattedGuess;
    };

    std::vector<FormattedGuess> previousGuesses;

    static int getRandomNumberInRange() {
        std::random_device randomDevice;
        std::mt19937 randomNumberGenerator(randomDevice());
        std::uniform_int_distribution<int> distribution(0, 2309);
        return distribution(randomNumberGenerator);
    }

    std::string selectRandomWord() {
        return wordleWords[getRandomNumberInRange()];
    }

    bool isValidGuess(const std::string &guess) {
        if (std::find(allValidWords.begin(), allValidWords.end(), guess) != allValidWords.end()) return true;
        std::cout << "Invalid word. Try again" << std::endl;
        return false;
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
        return TextFormatter::formatGreenText(std::string(1, letter));
    }

    static std::string formatLetterAsYellow(char letter) {
        return TextFormatter::formatYellowText(std::string(1, letter));
    }


public:
    WordleGame() : wordleWords(getWordData(wordleWordsFilePath)), allValidWords(getWordData(validWordsFilePath)) {}

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
};

std::string WordleGame::roundWord;

int main() {
    WordleGame game;
    bool isGameRunning = true;

    while (isGameRunning) {
        game.resetGame();
        while (!game.isGameOver()) {
            game.processGuessAttempt();
            game.displayPreviousGuesses();
        }
        isGameRunning = WordleGame::askForNewGame();
    }
    return 0;
}
