//
// Created by tomasz on 11/15/2025.
//

#include "test_Serializer.h"

#include <cstring>
#include <stack>

#include "test_util.h"
#include "../src/Serializer/storeClassList.h"

RUN_TEST(storeClassList_Token$scope) {
    std::string testString = "{{{"; // at the end scope is 3
    auto status = std::make_shared<textStatus>();

    for (auto it = testString.begin(); it != testString.end(); ++it) {
        std::stack<std::size_t> ts{};
        ParseToken(it, status, testString.begin(), ts);
    }
    T_ASSERT(status->dict_scope == 3);
}

RUN_TEST(storeClassList_Token$many) {
    std::string testString = "{}{}{}{}"; // at the end scope is 3
    auto status = std::make_shared<textStatus>();

    for (auto it = testString.begin(); it != testString.end(); ++it) {
        std::stack<std::size_t> ts{};
        ParseToken(it, status, testString.begin(), ts);
    }
    T_ASSERT(status->dict_scope == 0);
}

RUN_TEST(storeClassList_Token$multilevel) {
    std::string testString = R"({{{[[,"x[]")"; // at the end scope is 3
    auto status = std::make_shared<textStatus>();

    for (auto it = testString.begin(); it != testString.end(); ++it) {
        std::stack<std::size_t> ts{};
        ParseToken(it, status, testString.begin(), ts);
    }
    T_ASSERT(status->dict_scope == 3);
    T_ASSERT(status->array_scope == 2);
    T_ASSERT(status->string_since == -1);
}

RUN_TEST(storeClassList_Token$stringToken) {
    std::string testString = R"("{[\"{["{[]})"; // at the end scope is 0
    auto status = std::make_shared<textStatus>();
    for (auto it = testString.begin(); it != testString.end(); ++it) {
        std::stack<std::size_t> ts{};
        ParseToken(it, status, testString.begin(), ts);
    }
    T_ASSERT(status->dict_scope == 0);
    T_ASSERT(status->array_scope == 0);
    T_ASSERT(status->string_since == -1);
}

RUN_TEST(storeClassList_removeWhitespace$escaped) {
    const std::string testString = R"(abc"\" cd" ef")"; // String wit space with an escaped quote
    T_ASSERT(removeWhitespacePreserveStrings(testString).compare(R"(abc"\" cd"ef)") == true);
}