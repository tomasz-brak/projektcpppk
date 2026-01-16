#include "Display.h"
#include <functional>
#include <memory>
#include <print>
#include <string>

#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#define NOMINMAX
#include <windows.h>
#else
#include <unistd.h>
#endif

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
  clearConsole();
}
