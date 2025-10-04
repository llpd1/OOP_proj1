#include "StudentValidator.hpp"
#include <cctype>

bool StudentValidator::isDigits(const std::string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool StudentValidator::isAlphaSpace(const std::string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) {
        if (!(std::isalpha(c) || std::isspace(c))) return false;
    }
    return true;
}

bool StudentValidator::validName(const std::string& s) {
    return !s.empty() && s.size() <= 15 && isAlphaSpace(s);
}

bool StudentValidator::validStudentID(const std::string& s) {
    if (s.size() != 10 || !isDigits(s)) return false;
    int year = std::stoi(s.substr(0,4));
    return (year >= 1900 && year <= 2100);
}

bool StudentValidator::validDepartment(const std::string& s) {
    return s.size() <= 20 && isAlphaSpace(s);
}

bool StudentValidator::validBirthYear(int y) {
    return (y >= 1900 && y <= 2100);
}

bool StudentValidator::validTel(const std::string& s) {
    // 비어있을 수 있음, 비어있지 않으면 숫자만, 길이 제한
    if (s.size() > 12) return false;
    if (s.empty()) return true;
    return isDigits(s);
}
