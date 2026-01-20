#pragma once
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

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
        : text(std::move(text)), type(type) {};

    MessagePart(MessageType type) : type(type) {};

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

  static std::string generateHr(const std::string a, const int len);
  static void box();
  static void processLine();

public:
  struct Question
  {
    std::string prompt;
    std::vector<std::string> options;
  };
  static void show();
  static void clear();
  static void ask(const std::initializer_list<std::string> anwsers);
  static void ask(const std::vector<std::string> &anwsers);
  static void add(std::unique_ptr<std::string> s);
  static void ask(std::string question);

  inline static bool isBox;

  static std::vector<std::string>
  multiQuestion(const std::vector<Question> &questions);
  static void sectionBreak();
};
