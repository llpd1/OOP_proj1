#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>

struct Student {
    std::string name;       // Up to 15 chars (letters/spaces), validated externally
    std::string studentID;  // Exactly 10 digits; first 4 digits = admission year
    int         birthYear{};// Four-digit year 
    std::string department; // May contain spaces
    std::string tel;        // Up to 12 digits; empty string is allowed

    // Admission year is the first 4 digits of StudentID
    int admissionYear() const {
        if (studentID.size() >= 4) return std::stoi(studentID.substr(0,4));
        return 0;
    }
};

#endif // STUDENT_HPP
