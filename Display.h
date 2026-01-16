#pragma once
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
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

  inline static bool isBox;
  inline static int longestLine;

  static std::string generateHr(const std::string a, const int len);
  static void processLine();

public:
  static void show();
  static void clear();
  static void box();
  static std::string ask();
  static void add(std::unique_ptr<std::string> s);
  static void sectionBreak();
};
