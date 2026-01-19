#include "Display.h"
#include <atomic>
#include <cstdio>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <iostream>

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
  std::cout << "Waiting for input: " << std::flush;
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
              // Zmienione [this] na [lLine = longestLine]
              operateOnSubStrings(
                *section->text, "\n", [lLine = longestLine](const std::string substr) {
                  std::cout << "│" << substr 
                            << std::string(lLine - substr.length(), ' ') 
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
              std::cout << "├" << generateHr("─", longestLine) << "┤" << std::endl;
            }
          else
            {
              std::cout << generateHr(std::to_string(DELIMETER), longestLine) << std::endl;
            }
        }
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

        if(isBox)
          {
            std::string out = std::to_string(current_anwser + 1) + ") " + options[current_anwser];
            std::cout << "│" << out 
                      << std::string(longestLine - out.length(), ' ') 
                      << "│" << std::endl;
            std::cout << "╰" << generateHr("─", longestLine) << "╯" << std::endl;
          }
        else
          {
            std::cout << (current_anwser + 1) << " " << options[current_anwser] << std::endl;
          }
        
        std::cout << std::flush;
        auto k = getKey();
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
          }

        if(current_anwser < 0)
          {
            current_anwser = options.size() - 1;
          }
        if(current_anwser >= (int)options.size())
          {
            current_anwser = 0;
          }
        clearConsole();
      }
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
  clearConsole();
}

void Display::ask(const std::initializer_list<std::string> anwsers)
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