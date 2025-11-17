//
// Created by Tomasz on 11/04/2025.
//

#include "Serializable.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <memory>

#include "../util.h"
#include "storeClassList.h"

#include <stack>
#include <valarray>

#include "Token.h"

constexpr char SEPARATOR_LINE = '=';

std::string getSourceDir() {
    const std::string path = __FILE__;
    const auto pos = path.find_last_of("/\\");
    return path.substr(0, pos + 1);
}

bool writeStr(const std::string &filename, const std::string &str) {
    const std::string filePath = filename;
    std::cout << "Opening: " << filePath << std::endl;
    if (std::ofstream outFile(filePath); outFile.is_open()) {
        outFile << str;
        outFile.close();
        std::cout << "Dane dla " << filename << " zostaly zapisane pomyslnie !!!" << std::endl;
        return true;
    }
    std::cout << "Nieudalo sie otworzyc pliku !!!" << std::endl;
    return false;
}

std::string readFile(const std::string &filename) {
    const std::string filePath = filename;
    std::ifstream file(filePath);
    std::string result;
    std::string t;
    while (std::getline(file, t)) {
        result += t + "\n";
    }
    return result;
}

bool isEscaped(const std::string::const_iterator &begin, std::string::const_iterator it) {
    std::size_t backslashes = 0;
    while (it != begin) {
        --it;
        if (*it == '\\')
            ++backslashes;
        else
            break;
    }
    return (backslashes % 2) == 1;
}

void separateParenthesis(std::string &data) {
    std::string result;
    for (auto it = data.begin(); it != data.end(); ++it) {
        result += *it;
        if ((result == "[" or result == "]" or result == "{" or result == "}") and *(it + 1) != '\n') {
            result += "\n";
        }
    }
    data = result;
}

std::string removeWhitespacePreserveStrings(const std::string &s) {
    std::string out;
    out.reserve(s.size());
    bool in_string = false;

    for (auto it = s.begin(); it != s.end(); ++it) {
        const char c = *it;
        if (c == '"') {
            // determine if escaped
            if (!isEscaped(s.begin(), it)) {
                in_string = !in_string;
            }
            out += c;
            continue;
        }
        if (in_string) {
            out += c;
        } else {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                out += c;
            }
        }
    }
    return out;
}

void format(const std::string &filename) {
    const std::string path = filename + ".json";
    std::string src = readFile(path);
    if (src.empty()) {
        std::cerr << "Could not read file or file is empty: " << path << "\n";
        return;
    }

    std::string out;

    int indent = 0;
    bool in_string = false;

    auto appendIndent = [&](int levels) {
        if (levels > 0) out.append(static_cast<size_t>(levels * 4), ' ');
    };

    for (size_t i = 0; i < src.size(); ++i) {
        char ch = src[i];

        if (ch == '"') {
            // If " we enter string
            size_t j = i;
            int backslashes = 0;
            while (j > 0 && src[--j] == '\\') ++backslashes;
            if ((backslashes % 2) == 0) {
                in_string = !in_string;
            }
            out += ch;
            continue;
        }

        if (in_string) {
            out += ch;
            continue;
        }

        if (std::isspace(static_cast<unsigned char>(ch))) {
            continue;
        }

        switch (ch) {
            case '{':
            case '[':
                out += ch;
                out += '\n';
                ++indent;
                appendIndent(indent);
                break;

            case '}':
            case ']':
                out += '\n';
                --indent;
                if (indent < 0) {
                    std::cerr << "Format Error! negative indent. Not saving!\n";
                    return;
                }
                appendIndent(indent);
                out += ch;
                break;

            case ',':
                out += ch;
                out += '\n';
                appendIndent(indent);
                break;

            case ':':
                out += ch;
                out += ' ';
                break;

            default:
                out += ch;
                break;
        }
    }

    if (in_string) {
        std::cerr << "Format Error! unterminated string literal. Not saving!\n";
        return;
    }
    if (indent != 0) {
        std::cerr << "Format Error! mismatched braces/brackets (indent=" << indent << "). Not saving!\n";
        return;
    }

    out += '\n';
    if (!writeStr(path, out)) {
        std::cerr << "Could not write formatted JSON to: " << path << "\n";
    }
}


bool storeClass(const std::vector<std::shared_ptr<ISerializable> > &data, const std::string &dataDir) {
    std::string result = "[";
    std::string filename;

    for (auto it = data.begin(); it != data.end(); ++it) {
        createDirectories(dataDir);
        filename = dataDir;
        filename += "/";
        filename += (*it)->name;
        std::cout << filename << std::endl;
        result += (*it)->serialize();
        if (it != data.end() - 1) {
            result += ",\n";
            continue;
        }
        result += "\n";
    }

    result += "]";
    constexpr auto succes = true;
    if (succes) {
        format(filename);
    }
    return succes;
}

std::shared_ptr<std::vector<std::function<bool(std::shared_ptr<textStatus>)> > > getWorkers() {
    static auto workers = std::make_shared<std::vector<std::function<bool(std::shared_ptr<textStatus>)> > >();
    return workers;
}

std::optional<std::size_t> ParseToken(const std::string::const_iterator ch,
                                      const std::shared_ptr<textStatus> &status,
                                      const std::string::const_iterator stringBegin,
                                      std::stack<std::size_t> &tokenStack) {

    // Only care about escaping for quote characters:
    if (*ch == '"' && isEscaped(stringBegin, ch)) {
        return std::nullopt;
    }

    // If we are currently inside a string, handle string characters / closing quote
    if (status->string_since >= 0 && *ch == '\"') {
        // closing quote
        status->string_since = -1;
        if (!tokenStack.empty()) {
            auto opened_by = tokenStack.top();
            tokenStack.pop();
            return opened_by;
        } else {
            // malformed string (no opener) — ignore
            return std::nullopt;
        }
    }

    if (status->string_since >= 0) {
        // still inside string content
        status->string_since += 1;
        return std::nullopt; // Stop parsing other tokens while inside string
    }

    // Not inside string: check for opening quote
    if (*ch == '\"') {
        status->string_since = 0;
        tokenStack.emplace(std::distance(stringBegin, ch));
        return std::nullopt;
    }

    // Non-string tokens
    switch (*ch) {
        case '{':
            getWorkers()->emplace_back([](const std::shared_ptr<textStatus> &status) {
                status->dict_scope++;
                return true; // one-shot worker
            });
            tokenStack.emplace(std::distance(stringBegin, ch));
            break;
        case '[':
            getWorkers()->emplace_back([](const std::shared_ptr<textStatus> &status) {
                status->array_scope++;
                return true;
            });
            tokenStack.emplace(std::distance(stringBegin, ch));
            break;
        case '}':
            status->dict_scope--;
            if (!tokenStack.empty()) {
                auto opened_by = tokenStack.top();
                tokenStack.pop();
                return opened_by;
            }
            break;

        case ']':
            status->array_scope--;
            if (!tokenStack.empty()) {
                auto opened_by = tokenStack.top();
                tokenStack.pop();
                return opened_by;
            }
            break;

        default: ;
    }
    return std::nullopt;
}


std::vector<std::shared_ptr<charData> > parseChars(const std::string &input) {
    std::vector<std::shared_ptr<charData> > parsedChars;

    std::stack<std::size_t> tokenStack{};

    auto parserState = std::make_shared<textStatus>();
    parserState->ch = input.begin();

    for (auto it = input.begin(); it != input.end(); ++it) {
        {
            auto &workers = *getWorkers();
            std::erase_if(workers,
                          [&parserState](const auto &worker) { return worker(parserState); });
        }

        auto chStatus = std::make_shared<textStatus>(*parserState);
        chStatus->opened_by = std::nullopt;
        chStatus->closed_by = std::nullopt;
        chStatus->ch = it;

        if (auto closes = ParseToken(it, parserState, input.begin(), tokenStack); closes.has_value()) {
            const auto opened_idx = closes.value();
            if (opened_idx < parsedChars.size()) {
                parsedChars[opened_idx]->status->closed_by = std::distance(input.begin(), it) - opened_idx;
            }
            chStatus->opened_by = opened_idx;
        }

        auto chData = std::make_shared<charData>();
        chData->ch = *it;
        chData->status = chStatus;

        parsedChars.emplace_back(std::move(chData));
    }

    int i = 0;
    for (const auto& ch : parsedChars) {
        std::cout << i++ << ". ";
        std::cout << ch->ch << ": " << (ch->status->opened_by.has_value() ? std::to_string(ch->status->opened_by.value()) : "X") << std::endl;
    }

    i = 0;
    for (const auto& ch : parsedChars) {
        std::cout << i++ << ". ";
        std::cout << ch->ch << ": " << (ch->status->closed_by.has_value() ? std::to_string(ch->status->closed_by.value()) : "X") << std::endl;
    }

    return parsedChars;
}



tokenType whatTokenIs(const char ch) {
    switch (ch) {
        case '{':
            return DICT;
            break;
        case '[':
            return ARRAY;
            break;
        case '"':
            return STRING;
        default:
            return UNKNOWN;
    }
}

[[nodiscard]] std::shared_ptr<Token> assembleTokenTree(std::span<std::shared_ptr<charData> > chars) {
    auto root = std::make_shared<Token>();
    for (size_t i = 0; i < chars.size(); ++i) {
        if (chars[i]->status->closed_by.has_value()){
            root->content.emplace_back(chars[i]);
            root->type = whatTokenIs(chars[i]->ch);
            auto inner_span = chars.subspan(i + 1, chars[i]->status->closed_by.value() - 1);
            auto c = assembleTokenTree(inner_span);
            std::cout << "calling assembleTokenTree with; " << [inner_span]() {
                std::string out; for (const auto& sp : inner_span) {
                    out += sp->ch;
                }
                return out;
            }() << std::endl;
            c->parent = root;
            if (root->type == DICT) {
                // Find all keys if
            }
            root->children.emplace_back(std::move(c));
            i += chars[i]->status->closed_by.value()-1;
        } else {
            root->content.emplace_back(chars[i]);
        }
    }


    return root;
}

std::shared_ptr<Token> assembleTokenTree(std::vector<std::shared_ptr<charData>>& vec) {
    return assembleTokenTree(std::span(vec));
}


bool readClass(const char *name, const std::vector<std::shared_ptr<ISerializable> > &data, const std::string &dataDir) {
    const std::string filename = dataDir + "/" + name + ".json";
    std::string fileContent;

    if (std::ifstream file(filename); !file.is_open()) {
        std::cout << "Nie udalo sie otworzyc pliku do odczytu: " << filename << std::endl;
        return false;
    } else {
        fileContent = {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }
    fileContent = removeWhitespacePreserveStrings(fileContent);
    auto parsedChars = parseChars(fileContent);
    auto tokenTree = assembleTokenTree(parsedChars);
    std::cout << tokenTree->toString(0) << std::endl;
    return true;
}