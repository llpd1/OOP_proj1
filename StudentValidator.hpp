#ifndef STUDENT_VALIDATOR_HPP
#define STUDENT_VALIDATOR_HPP

#include <string>

class StudentValidator {
public:
    // Name validation: not empty, length <= 15, letters and spaces only
    static bool validName(const std::string& s);

    // Student ID validation: exactly 10 digits, first 4 digits between 1900 and 2100
    static bool validStudentID(const std::string& s);

    // Department validation: length <= 20, letters and spaces only
    static bool validDepartment(const std::string& s);

    // Birth year validation: must be between 1900 and 2100
    static bool validBirthYear(int y);

    // Telephone validation: may be empty, digits only, length <= 12
    static bool validTel(const std::string& s);

private:
    static bool isDigits(const std::string& s);
    static bool isAlphaSpace(const std::string& s);
};

#endif // STUDENT_VALIDATOR_HPP
