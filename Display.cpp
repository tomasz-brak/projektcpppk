#include "Display.h"
#include <atomic>
#include <cstdio>
#include <functional>
#include <initializer_list>
#include <memory>
#include <print>
#include <string>

#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
// #define NOMINMAX
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
  ENTER
};

std::string DEBUG_last_string;

void clearConsole()
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
Key getKey()
{
  int ch = _getch();
  if(ch == 13)
    return ENTER;
  if(ch == 0 || ch == 224)
    {
      ch = _getch();
      if(ch == 72)
        return UP;
      if(ch == 80)
        return DOWN;
    }
  if(ch == 'q' || ch == 'Q')
    return QUIT;
  return NONE;
}
#else

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

Key getKey()
{
  std::print("Wating for input: ");
  char ch;
  if(read(STDIN_FILENO, &ch, 1) <= 0)
    return NONE;

  if(ch == '\n' || ch == '\r')
    return ENTER;

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
                  DEBUG_last_string = "ArrA";
                  return UP;
                }
              if(seq[1] == 'B')
                {
                  DEBUG_last_string = "arrB";
                  return DOWN;
                }
            }
        }
    }
  else if(ch == 'q' || ch == 'Q')
    {
      return QUIT;
    }
  return NONE;
}
#endif

/**
 * Splits a string by a delimiter and executes a callback for each substring.
 *
 * @param input The original string to split.
 * @param delimiter The string to split by.
 * @param func A lambda or function that takes a const std::string&.
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

/**
 * @brief Wypisuje "widok w konsoli"
 */
void Display::show()
{
  if(isBox)
    {
      std::print("╭{}╮\n", generateHr("─", longestLine));
    }

  for(auto &section : Display::sections)
    {
      if(section->type == TEXT)
        {
          if(isBox)
            {
              operateOnSubStrings(
                *section->text, "\n", [](const std::string substr) {
                  std::print("│{}{}│\n", substr,
                             std::string(longestLine - substr.length(), ' '));
                });
            }
          else
            {
              std::print("{}", *section->text);
            }
        }
      else if(section->type == SECTION_BREAK)
        {
          if(isBox)
            {
              std::print("├{}┤\n", generateHr("─", longestLine));
            }
          else
            {
              std::print("{}\n",
                         generateHr(std::to_string(DELIMETER), longestLine));
            }
        }
    }

  if(isBox)
    {
      std::print("╰{}╯\n", generateHr("─", longestLine));
    }

  if(isQuestion)
    {
#ifndef _WIN32
      setRawMode(true);
#endif
      {
        if(anwsered)
          {
            clearConsole();
            return;
          }
        std::print("{}) Użyj 🔼 🔽 by wybrać: {}\n", current_anwser + 1,
                   options[current_anwser]);
        std::fflush(stdout);

        auto k = getKey();
        if(k == UP)
          {
            current_anwser++;
            std::print("Kup");
          }
        else if(k == DOWN)
          {
            current_anwser--;
            std::print("Kdown ");
          }

        if(k == ENTER)
          {
            anwsered = true;
          }

        if(current_anwser < 0)
          {
            current_anwser = options.size() - 1;
          }
        if(current_anwser >= options.size())
          {
            current_anwser = 0;
          }
        clearConsole();
        show();
      }
    }
  clearConsole();
}
/**
 * @brief Dodaje linijkę/linijki do teksku
 *
 * @param s należy zakończyć \n
 */
void Display::add(std::unique_ptr<std::string> s)
{
  operateOnSubStrings(*s, "\n", [](std::string substr) {
    if(substr.length() > Display::longestLine)
      Display::longestLine = substr.length();
  });

  if(Display::sections.empty()
     or Display::sections.back()->type == SECTION_BREAK)
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
  clearConsole();
}

void Display::ask(const std::initializer_list<std::string> anwsers)
{
  Display::options = anwsers;
  isQuestion = true;
}
