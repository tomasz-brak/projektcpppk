//
// Created by tomasz on 11/14/2025.
//
#pragma once

#include <any>
#include <cassert>
#include <functional>
#include <iostream>
#include <ostream>
#include <iomanip>

struct TestFailure : public std::runtime_error {
    explicit TestFailure(const std::string& msg) : std::runtime_error(msg) {}
};

struct TestInfo {
    std::function<void()> func;
    std::string name;
    int index;
};

inline std::vector<TestInfo>& get_test_registry() {
    static std::vector<TestInfo> tests;
    return tests;
}

struct TestRegistrar {
    TestRegistrar(const std::function<void()> &func, const char* name, int index) {
        get_test_registry().push_back({func, name, index});
    }
};

inline void test_assert_fail(const char* expr, const char* file, int line) {
    throw TestFailure(std::string(expr) + " at " + file + ":" + std::to_string(line));
}


#define T_ASSERT(expr) ((expr) ? (void)0 : test_assert_fail(#expr, __FILE__, __LINE__))

#define RUN_TEST(name) TEST_LOGIC(__COUNTER__, name)

#define TEST_LOGIC(n, name) \
void name(); \
void name##_impl(); \
void name() { \
    try { \
        name##_impl(); \
        std::cout << std::setw(3) << std::left << ( n + 1 ) << std::setw(40) << #name << "✅  Passed!" << std::endl; \
    } catch (const TestFailure& e) { \
        std::cout << std::setw(3) << std::left << ( n + 1 ) << std::setw(40) << #name << "⛔  Failed!" << e.what() << std::endl; \
    } \
} \
static TestRegistrar registrar_##name(name, #name, n);\
void name##_impl()


#ifndef PROJEKT_UTIL_H
#define PROJEKT_UTIL_H
#endif //PROJEKT_UTIL_H