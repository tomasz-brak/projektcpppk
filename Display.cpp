#include "Display.h"
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <iostream>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <conio.h>
#else
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#endif

enum Key
{
  NONE,
  UP,
  DOWN,
  QUIT,
  ENTER,
};

std::string DEBUG_last_string;

/**
 * @brief Splits a string by a delimiter and executes a callback for each
 * substring.
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

#ifndef _WIN32
/**
 * @brief Enables or disables raw mode for terminal input on Linux.
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
#endif

void Display::clearConsole()
{
#if defined(_WIN32) || defined(_WIN64)
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if(hOut == INVALID_HANDLE_VALUE)
    return;

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if(!GetConsoleScreenBufferInfo(hOut, &csbi))
    return;

  DWORD cellCount
    = static_cast<DWORD>(csbi.dwSize.X) * static_cast<DWORD>(csbi.dwSize.Y);
  COORD homeCoords = {0, 0};
  DWORD count;

  if(!FillConsoleOutputCharacterA(hOut, ' ', cellCount, homeCoords, &count))
    return;

  if(!FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, homeCoords,
                                 &count))
    return;

  SetConsoleCursorPosition(hOut, homeCoords);
#else
  std::cout << "\x1b[2J\x1b[H" << std::flush;
#endif
}

std::string Display::generateHr(const std::string a, const int len)
{
  std::string s;
  s.reserve(len * a.length());
  for(int x = 0; x < len; ++x)
    {
      s += a;
    }
  return s;
}

void Display::show()
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
                  std::cout << "│" << substr
                            << std::string(lLine - substr.length(), ' ') << "│"
                            << std::endl;
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
              std::cout << generateHr(std::to_string(DELIMETER), longestLine)
                        << std::endl;
            }
        }
    }

  if(isQuestion)
    {
      Key k = NONE;

#ifndef _WIN32
      char c = 0;
      if(read(STDIN_FILENO, &c, 1) > 0)
        {
          if(c == '\n' || c == '\r')
            {
              k = ENTER;
            }
          else if(c == 27) // Escape char
            {
              char seq[2];
              // Try to read arrow keys
              if(read(STDIN_FILENO, &seq[0], 1) > 0
                 && read(STDIN_FILENO, &seq[1], 1) > 0)
                {
                  if(seq[0] == '[' && seq[1] == 'A')
                    k = UP;
                  else if(seq[0] == '[' && seq[1] == 'B')
                    k = DOWN;
                }
            }
          else if(c == 127 || c == 8)
            {
              if(!Display::userInput.empty())
                Display::userInput.pop_back();
            }
          else if(c >= 32 && c < 127)
            {
              Display::userInput += c;
            }
        }
#else
      int c = _getch();
      if(c == 13)
        {
          k = ENTER;
        }
      else if(c == 0 || c == 224)
        {
          int arrow = _getch();
          if(arrow == 72)
            k = UP;
          else if(arrow == 80)
            k = DOWN;
        }
      else if(c == 8)
        {
          if(!Display::userInput.empty())
            Display::userInput.pop_back();
        }
      else if(c >= 32 && c <= 126)
        {
          Display::userInput += static_cast<char>(c);
        }
#endif

      if(isBox)
        {
          if(!options.empty())
            {
              std::string out = std::to_string(current_anwser + 1) + ") "
                                + options[current_anwser];
              std::cout << "│" << out
                        << std::string(longestLine - out.length(), ' ') << "│"
                        << std::endl;
              std::cout << "╰" << generateHr("─", longestLine) << "╯"
                        << std::endl;
            }
          else
            {
              std::cout << "╰" << generateHr("─", longestLine) << "╯"
                        << std::endl;
            }
        }
      else
        {
          if(!options.empty())
            std::cout << (current_anwser + 1) << " " << options[current_anwser]
                      << std::endl;
        }

      std::cout << std::flush;

      // Removed secondary read (getKey) and used the 'k' determined above
      if(k == UP)
        {
          current_anwser++;
        }
      else if(k == DOWN)
        {
          current_anwser--;
        }

      if(k == ENTER)
        {
          anwsered = true;
          // Note: userInput clearing moved to caller if needed,
          // or handle it here if it was intended to clear on enter.
          // Original code cleared it only on Windows inside the loop.
          // Keeping it as is (accumulating) for singleQuestion to work.
        }

      if(current_anwser < 0)
        {
          current_anwser = options.size() - 1;
        }

      if(!options.empty() && current_anwser >= (int)options.size())
        {
          current_anwser = 0;
        }

      clearConsole();
    }

  if(isBox && !isQuestion)
    {
      std::cout << "╰" << generateHr("─", longestLine) << "╯" << std::endl;
    }
  if(isQuestion && !anwsered)
    {
      show();
    }
}

void Display::add(std::unique_ptr<std::string> s)
{
  operateOnSubStrings(*s, "\n", [](std::string substr) {
    if(substr.length() > Display::longestLine)
      Display::longestLine = substr.length();
  });

  if(Display::sections.empty()
     || Display::sections.back()->type == SECTION_BREAK)
    {
      Display::sections.emplace_back(
        std::make_unique<MessagePart>(std::move(s), MessageType::TEXT));
      return;
    }

  *Display::sections.back()->text += *s;
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
  anwsered = false;
  longestLine = 0;
  userInput = "";
  clearConsole();
}

void Display::ask(const std::vector<std::string> anwsers)
{
  Display::options = anwsers;
  isQuestion = true;
  int n = 1;
  for(auto &anwser : anwsers)
    {
      std::string s = std::to_string(n) + ") " + anwser;
      if(s.length() > (size_t)longestLine)
        {
          longestLine = (int)s.length();
        }
      n++;
    }
}

void Display::ask() { isQuestion = true; }

std::string Display::singleQuestion(std::string q)
{
  auto display = std::make_unique<Display>();
  display->clear();
  display->box();
  display->add(std::make_unique<std::string>(q));
  display->sectionBreak();
  display->ask();
  display->show();
  return display->userInput;
}
