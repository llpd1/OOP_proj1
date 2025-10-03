#ifndef STUDENT_DB_HPP
#define STUDENT_DB_HPP

#include "Student.hpp"
#include <vector>
#include <string>

enum class SortKey { Name, StudentID, BirthYear, Department };

class StudentDB {
public:
    explicit StudentDB(const std::string& path);

    // 로드/세이브
    bool load();
    bool save() const;

    // 데이터가 비었는지 확인
    bool isEmpty() const;

    // 삽입(중복 학번 체크)
    bool insert(const Student& s); // 검증/중복학번
    bool insert(const Student& s, std::vector<std::string>& err); // 정렬된 복사 반환

    // 검색
    std::vector<Student> searchByName(const std::string& key) const;      
    std::vector<Student> searchById(const std::string& id10) const;        
    std::vector<Student> searchByAdmissionYear(int year4) const;          
    std::vector<Student> searchByBirthYear(int year4) const;                
    std::vector<Student> searchByDepartmentKeyword(const std::string& kw) const; 

    // 정렬 옵션
    std::vector<Student>& sortByKey();
    void setSortKey(SortKey k);
    SortKey sortKey() const;

    // 추가기능: 수정
    bool updateName(const std::string& studentID, const std::string& newName, std::string& err);
    bool updateDepartment(const std::string& studentID, const std::string& newDept, std::string& err);
    bool updateTel(const std::string& studentID, const std::string& newTel, std::string& err);

private:
    static bool isDigits(const std::string& s);         // 숫자만
    static bool isAlphaSpace(const std::string& s);     // 영문/공백만
    static std::string toLower(std::string s);          // 소문자 변환
    static bool validName(const std::string& s);        // 이름 검증
    static bool validStudentID(const std::string& s);   // 학번 검증
    static bool validDepartment(const std::string& s);  // 단과대 검증
    static bool validBirthYear(int y);                  // 출생연도 검증
    static bool validTel(const std::string& s);         // 전화번호 검증

    bool existsID(const std::string& ID) const;
    
    std::string          path_;
    std::vector<Student> data_;
    SortKey              sortKey_;
};

#endif // STUDENT_DB_HPP
