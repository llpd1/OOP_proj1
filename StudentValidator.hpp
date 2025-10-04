#ifndef STUDENT_VALIDATOR_HPP
#define STUDENT_VALIDATOR_HPP

#include <string>

class StudentValidator {
public:
    // 이름 유효성: 비어있지 않고 길이 <= 15, 영문/공백만 허용
    static bool validName(const std::string& s);

    // 학번 유효성: 정확히 10자리 숫자, 앞 4자리는 1900~2100
    static bool validStudentID(const std::string& s);

    // 단과대 유효성: 길이 <= 20, 영문/공백만 허용
    static bool validDepartment(const std::string& s);

    // 출생연도 유효성: 1900~2100
    static bool validBirthYear(int y);

    // 전화번호 유효성: 비어있을 수 있으며 숫자만 가능, 길이 <= 12
    static bool validTel(const std::string& s);

private:
    static bool isDigits(const std::string& s);
    static bool isAlphaSpace(const std::string& s);
};

#endif // STUDENT_VALIDATOR_HPP
