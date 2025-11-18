//
// Created by tomasz on 11/15/2025.
//

#include "test_Serializer.h"

#include <cstring>
#include <stack>

#include "test_util.h"
#include "../src/Serializer/storeClassList.h"


RUN_TEST(storeClassList_Token$multilevel) {
    std::string testString = R"({{{[[,"x[]")"; // at the end scope is 3
    auto status = std::make_shared<textStatus>();

    for (auto it = testString.begin(); it != testString.end(); ++it) {
        std::stack<std::size_t> ts{};
        ParseToken(it, status, testString.begin(), ts);
    }
    T_ASSERT(status->string_since == -1);
}

RUN_TEST(storeClassList_Token$stringToken) {
    std::string testString = R"("{[\"{["{[]})"; // at the end scope is 0
    auto status = std::make_shared<textStatus>();
    for (auto it = testString.begin(); it != testString.end(); ++it) {
        std::stack<std::size_t> ts{};
        ParseToken(it, status, testString.begin(), ts);
    }
    T_ASSERT(status->string_since == -1);
}



RUN_TEST(classStorage) {
    class TestClass : public Serializable<TestClass> {
    public:
        int something = 0;
        std::string text = "hello";

        static const std::string &static_name() {
            static const std::string name = "TestClass";
            return name;
        }

        explicit TestClass() {
            this->name = static_name();

            registerField("something", &TestClass::something, defaultString<int>());
            registerField("text", &TestClass::text, defaultString<std::string>());
        }

    };
    auto instance = TestClass::create();
    instance->something = 42;
    instance->text = "world";
    std::vector<std::shared_ptr<ISerializable>> vec = {instance};
    const std::string dataDir = "./test_data/";
    storeClass(vec, dataDir);
    std::vector<std::shared_ptr<ISerializable>> loadedVec;
    readClass("TestClass", loadedVec, dataDir);
    T_ASSERT(loadedVec.size() == 1);
    auto loadedInstance = std::dynamic_pointer_cast<TestClass>(loadedVec[0]);
    T_ASSERT(loadedInstance != nullptr);
    T_ASSERT(loadedInstance->something == 42);
    T_ASSERT(loadedInstance->text == "world");
}

RUN_TEST(storeClassList_removeWhitespace$escaped) {
    const std::string testString = R"(abc"\" cd" ef")"; // String wit space with an escaped quote
    T_ASSERT(removeWhitespacePreserveStrings(testString).compare(R"(abc"\" cd"ef)") == true);
}