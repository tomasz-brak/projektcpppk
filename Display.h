#pragma once
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Static class for managing terminal display, output formatting,
 * and interactive user input (questions/options).
 */
class Display
{
private:
  static constexpr auto DELIMETER = '-';
  enum MessageType
  {
    SECTION_BREAK,
    TEXT,
  };

  class MessagePart
  {
  public:
    MessagePart(std::unique_ptr<std::string> text, MessageType type)
        : text(std::move(text)), type(type){};

    MessagePart(MessageType type) : type(type){};

    const MessageType type;
    std::unique_ptr<std::string> text;
  };

  inline static std::vector<std::unique_ptr<MessagePart>> sections;
  inline static std::vector<std::string> options;

  inline static int longestLine;
  inline static bool isQuestion;

  inline static bool anwsered = false;
  inline static int current_anwser = 0;
  inline static bool isInput = false;
  inline static std::string current_input_text = "";

  /**
   * @brief Generates a horizontal rule string of a specific character.
   * @param a The character/string to repeat.
   * @param len The total length of the generated string.
   * @return The generated horizontal rule string.
   */
  static std::string generateHr(const std::string a, const int len);

  /**
   * @brief Toggles the internal boxed display state.
   */
  static void box();

  static void processLine();

  /**
   * @brief Clears the console screen using system specific commands.
   */
  static void clearConsole();

public:
  struct Question
  {
    std::string prompt;
    std::vector<std::string> options;
  };

  /**
   * @brief Renders the accumulated content to the terminal.
   *
   */
  static void show();

  /**
   * @brief Resets the display.
   *
   * Clears all sections, options, resets flags, and clears the console screen.
   */
  static void clear();

  /**
   * @brief Configures a multiple-choice question using an initializer list.
   * @param anwsers List of options to display to the user.
   */
  static void ask(const std::initializer_list<std::string> anwsers);

  /**
   * @brief Configures a multiple-choice question using a vector.
   * @param anwsers Vector of options to display to the user.
   */
  static void ask(const std::vector<std::string> &anwsers);

  /**
   * @brief Adds a new text section to the display.
   * @param s Unique pointer to the string content.
   */
  static void add(std::unique_ptr<std::string> s);

  /**
   * @brief Configures a free-text input question.
   * @param question The prompt text to display.
   */
  static void ask(std::string question);

  /**
   * @brief Controls whether the output is wrapped in a decorative box.
   */
  inline static bool isBox;

  /**
   * @brief Ask a series of questions. Helper function, repeats ask().
   *
   * @param questions A vector of Question objects.
   * @return A vector of strings containing the answers.
   */
  static std::vector<std::string>
  multiQuestion(const std::vector<Question> &questions);

  /**
   * @brief Adds a horizontal section break to the display.
   */
  static void sectionBreak();
};
