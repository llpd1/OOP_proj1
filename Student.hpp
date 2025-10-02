#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>

struct Student {
    std::string name;       // 최대 15자(영문) - 검증 TODO
    std::string studentID;  // 정확히 10자리, 앞 4자리 = 입학년도
    int         birthYear{}; 
    std::string department; // 공백 포함 가능
    std::string tel;        // 최대 12자리
//***

    // 입학년도(학번 앞 4자리) 파생
    int admissionYear() const {
        if (studentID.size() >= 4) return stoi(studentID.substr(0,4));
        return 0;
    }
};

#endif // STUDENT_HPP


