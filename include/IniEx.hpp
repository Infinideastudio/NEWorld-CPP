//
// Copyright 2015 infinideas
//

#ifndef NEWORLD_INI_EX_HPP
#define NEWORLD_INI_EX_HPP

#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <unordered_map>

namespace iniex {

class ParseException: public std::exception {
public:
    ParseException(int line,
                   const std::string &message) noexcept {
        m_line = line;
        m_message = message;
    }
    ~ParseException() noexcept = default;

    virtual const char *what() const noexcept {
        return (std::string("At line ")
                + std::to_string(m_line) + ": " + m_message).c_str();
    }

private:
    int m_line;
    std::string m_message;
};  // class ParseException

// template <typename T>
// class Value {
// public:
//     Value() = default;
//     Value(const Value &other): m_value(other.GetValue()) {}
//     Value(Value &&other): m_value(other.GetValue()) {}
//     explicit Value(const T &value): m_value(value) {}
//     explicit Value(T && value): m_value(value) {}
//     ~Value() = default;

//     Value &operator=(const Value &other) {
//         m_value = other.GetValue();

//         return *this;
//     }
//     Value &operator=(const T &value) {
//         m_value = value;

//         return *this;
//     }
//     Value &operator=(T &&value) {
//         m_value = value;

//         return *this;
//     }

//     T GetValue() {
//         return m_value;
//     }

// private:
//     T m_value;
// };  // class Value

// typedef Value<bool> Bool;
// typedef Value<int> Integer;
// typedef Value<float> Float;
// typedef Value<std::string> String;

constexpr char CommentSign = '#';

constexpr char TableLeftSign = '[';
constexpr char TableRightSign = ']';
constexpr char TableDotSign = '.';

constexpr char StringLeftSign = '\"';
constexpr char StringRightSign = '\"';
constexpr char StringConvertSign = '/';
constexpr char StringConvertNewLine = 'n';
constexpr char StringConvertTab = 't';
constexpr char StringAsciiSign = 'x';
constexpr int StringAsciiLength = 3;
constexpr char StringNoConvertSign = '@';

constexpr char BoolTrue[] = "true";
constexpr char BoolFalse[] = "false";

constexpr char NumberSeperater = '\'';

constexpr char ArrayLeftSign = '{';
constexpr char ArrayRightSign = '}';

constexpr char EqualSign = '=';

static std::unordered_map<std::string, std::shared_ptr<void>> GlobalMap;

}  // namespace iniex

#endif  // NEWORLD_INI_EX_HPP
