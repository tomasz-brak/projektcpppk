#include "Display.h"
#include <cstdio>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <iostream>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#endif

enum Key
{
  NONE,
  UP,
  DOWN,
  QUIT,
  ENTER,
  BACKSPACE,
  CHAR_INPUT
};

size_t utf8_len(const std::string &str)
{
  size_t length = 0;
  for(char c : str)
    {
      if((c & 0xC0) != 0x80)
        {
          length++;
        }
    }
  return length;
}

void Display::clearConsole()
{
#if defined(_WIN32) || defined(_WIN64)
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if(hOut == INVALID_HANDLE_VALUE)
    return;

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if(!GetConsoleScreenBufferInfo(hOut, &csbi))
    {
      return;
    }

  DWORD cellCount
    = static_cast<DWORD>(csbi.dwSize.X) * static_cast<DWORD>(csbi.dwSize.Y);
  COORD homeCoords = {0, 0};
  DWORD count;

  if(!FillConsoleOutputCharacterA(hOut, ' ', cellCount, homeCoords, &count))
    {
      return;
    }

  if(!FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, homeCoords,
                                 &count))
    {
      return;
    }

  SetConsoleCursorPosition(hOut, homeCoords);
#else
  std::cout << "\x1b[2J\x1b[H" << std::flush;
#endif
}

#ifdef _WIN32
/**
 * @brief captures keyboard input on Windows.
 * @param charCode Reference to store the character code for text input.
 * @return Key enum representing the action or input type.
 */
Key getKey(int &charCode)
{
  charCode = 0;
  int ch = _getch();
  if(ch == 13)
    return ENTER;
  if(ch == 8)
    return BACKSPACE;

  if(ch == 0 || ch == 224)
    {
      ch = _getch();
      if(ch == 72)
        return UP;
      if(ch == 80)
        return DOWN;
    }

  if(ch >= 32 && ch <= 126)
    {
      charCode = ch;
      return CHAR_INPUT;
    }

  if(ch == 'q' || ch == 'Q')
    return QUIT;

  return NONE;
}
#else

/**
 * @brief Configures terminal raw mode on Linux/Unix.
 * @param enable True to enable raw mode, False to disable.
 */
void setRawMode(bool enable)
{
  static struct termios oldt, newt;
  if(enable)
    {
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
  else
    {
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

/**
 * @brief Captures keyboard input on Linux/Unix.
 * @param charCode Reference to store the character code for text input.
 * @return Key enum representing the action or input type.
 */
Key getKey(int &charCode)
{
  charCode = 0;
  std::cout << "" << std::flush;
  char ch;
  if(read(STDIN_FILENO, &ch, 1) <= 0)
    return NONE;

  if(ch == '\n' || ch == '\r')
    return ENTER;

  if(ch == 127 || ch == 8)
    return BACKSPACE;

  if(ch == 27)
    {
      char seq[2];
      if(read(STDIN_FILENO, &seq[0], 1) > 0
         && read(STDIN_FILENO, &seq[1], 1) > 0)
        {
          if(seq[0] == '[')
            {
              if(seq[1] == 'A')
                {
                  return UP;
                }
              if(seq[1] == 'B')
                {
                  return DOWN;
                }
            }
        }
    }
  else
    {
      if(ch >= 32 && ch != 127)
        {
          charCode = (int)ch;
          return CHAR_INPUT;
        }
    }
  return NONE;
}
#endif

/**
 * @brief Helper to process substrings split by a delimiter.
 */
void operateOnSubStrings(const std::string &input,
                         const std::string &delimiter,
                         std::function<void(const std::string &)> func)
{
  size_t start = 0;
  size_t end = input.find(delimiter);

  while(end != std::string::npos)
    {
      func(input.substr(start, end - start));

      start = end + delimiter.length();
      end = input.find(delimiter, start);
    }

  func(input.substr(start));
}

std::string Display::generateHr(const std::string a, const int len)
{
  std::string s;
  for(int x = 0; x < len; ++x)
    {
      s += a;
    }
  return s;
}

void Display::show()
{
#ifndef _WIN32
  if(isQuestion)
    setRawMode(true);
#endif

  while(true)
    {
      if(isBox)
        {
          std::cout << "╭" << generateHr("─", longestLine) << "╮" << std::endl;
        }

      for(auto &section : Display::sections)
        {
          if(section->type == TEXT)
            {
              if(isBox)
                {
                  operateOnSubStrings(
                    *section->text, "\n",
                    [lLine = longestLine](const std::string substr) {
                      int padding = lLine - (int)utf8_len(substr);
                      if(padding < 0)
                        padding = 0;
                      std::cout << "│" << substr << std::string(padding, ' ')
                                << "│" << std::endl;
                    });
                }
              else
                {
                  std::cout << *section->text;
                }
            }
          else if(section->type == SECTION_BREAK)
            {
              if(isBox)
                {
                  std::cout << "├" << generateHr("─", longestLine) << "┤"
                            << std::endl;
                }
              else
                {
                  std::cout
                    << generateHr(std::to_string(DELIMETER), longestLine)
                    << std::endl;
                }
            }
        }

      if(!isQuestion)
        {
          if(isBox)
            std::cout << "╰" << generateHr("─", longestLine) << "╯"
                      << std::endl;
          break;
        }

      if(anwsered)
        {
          clearConsole();
          break;
        }

      if(isInput)
        {
          std::string indicator = "> " + current_input_text + "_";
          if(isBox)
            {
              int padding = longestLine - (int)utf8_len(indicator);
              if(padding < 0)
                padding = 0;

              std::cout << "│" << indicator << std::string(padding, ' ') << "│"
                        << std::endl;
              std::cout << "╰" << generateHr("─", longestLine) << "╯"
                        << std::endl;
            }
          else
            {
              std::cout << indicator << std::endl;
            }
        }
      else
        {
          if(isBox)
            {
              std::string out = std::to_string(current_anwser + 1) + ") "
                                + options[current_anwser];
              int padding = longestLine - (int)utf8_len(out);
              if(padding < 0)
                padding = 0;

              std::cout << "│" << out << std::string(padding, ' ') << "│"
                        << std::endl;
              std::cout << "╰" << generateHr("─", longestLine) << "╯"
                        << std::endl;
            }
          else
            {
              std::cout << (current_anwser + 1) << " "
                        << options[current_anwser] << std::endl;
            }
        }

      std::cout << std::flush;

      // Handle Input
      int charCode = 0;
      auto k = getKey(charCode);

      if(isInput)
        {
          if(k == CHAR_INPUT)
            {
              current_input_text += (char)charCode;
            }
          else if(k == BACKSPACE)
            {
              if(!current_input_text.empty())
                {
                  current_input_text.pop_back();
                }
            }
          else if(k == ENTER)
            {
              anwsered = true;
            }
        }
      else
        {
          if(k == UP)
            {
              current_anwser++;
            }
          else if(k == DOWN)
            {
              current_anwser--;
            }
          else if(k == ENTER)
            {
              anwsered = true;
            }

          if(current_anwser < 0)
            {
              current_anwser = options.size() - 1;
            }
          if(current_anwser >= (int)options.size())
            {
              current_anwser = 0;
            }
        }

      clearConsole();
    }
}

void Display::add(std::unique_ptr<std::string> s)
{
  if(Display::sections.empty()
     || Display::sections.back()->type == SECTION_BREAK)
    {
      operateOnSubStrings(*s, "\n", [](std::string substr) {
        if(utf8_len(substr) > (size_t)Display::longestLine)
          Display::longestLine = utf8_len(substr);
      });
      Display::sections.emplace_back(
        std::make_unique<MessagePart>(std::move(s), MessageType::TEXT));
    }
  else
    {
      *Display::sections.back()->text += *s;

      operateOnSubStrings(*Display::sections.back()->text, "\n",
                          [](std::string substr) {
                            if(utf8_len(substr) > (size_t)Display::longestLine)
                              Display::longestLine = utf8_len(substr);
                          });
    }
}

void Display::box() { isBox = !isBox; }

void Display::sectionBreak()
{
  Display::sections.emplace_back(std::make_unique<MessagePart>(SECTION_BREAK));
}

void Display::clear()
{
  Display::sections.clear();
  Display::options.clear();
  isBox = false;
  isQuestion = false;
  isInput = false;
  anwsered = false;
  current_input_text = "";
  clearConsole();
}

void Display::ask(const std::initializer_list<std::string> anwsers)
{
  Display::options = anwsers;
  isQuestion = true;
  isInput = false;
  current_anwser = 0;
  int n = 1;
  for(auto &anwser : anwsers)
    {
      std::string s = std::to_string(n) + ") " + anwser;
      if(utf8_len(s) > (size_t)longestLine)
        {
          longestLine = (int)utf8_len(s);
        }
      n++;
    }
}

void Display::ask(const std::vector<std::string> &anwsers)
{
  Display::options = anwsers;
  isQuestion = true;
  isInput = false;
  current_anwser = 0;
  int n = 1;
  for(auto &anwser : anwsers)
    {
      std::string s = std::to_string(n) + ") " + anwser;
      if(utf8_len(s) > (size_t)longestLine)
        {
          longestLine = (int)utf8_len(s);
        }
      n++;
    }
}

void Display::ask(std::string question)
{
  if(!question.empty())
    {
      add(std::make_unique<std::string>(question));
    }
  isQuestion = true;
  isInput = true;
  current_input_text = "";
  anwsered = false;
}

std::vector<std::string>
Display::multiQuestion(const std::vector<Question> &questions)
{
  std::vector<std::string> results;
  bool wasBox = isBox;
  clear();
  isBox = wasBox;

  for(const auto &q : questions)
    {
      if(q.options.empty())
        {
          ask(q.prompt);
        }
      else
        {
          add(std::make_unique<std::string>(q.prompt));
          ask(q.options);
        }

      show();

      std::string ans;
      if(q.options.empty())
        {
          ans = current_input_text;
        }
      else
        {
          ans = options[current_anwser];
        }
      results.push_back(ans);

      std::string formattedAnswer = " -> " + ans + "\n";
      add(std::make_unique<std::string>(formattedAnswer));

      isQuestion = false;
      isInput = false;
      anwsered = false;
      options.clear();
      current_input_text = "";
    }
  return results;
}
