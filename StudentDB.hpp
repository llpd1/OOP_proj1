#ifndef STUDENT_DB_HPP
#define STUDENT_DB_HPP

#include "Student.hpp"
#include "StudentValidator.hpp"
#include "FileStudentRepository.hpp"
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

    // 삽입(중복 학번/전화 체크)
    bool insert(const Student& s); // 편의 wrapper
    bool insert(const Student& s, std::vector<std::string>& err);

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

    // 수정
    bool updateName(const std::string& studentID, const std::string& newName, std::string& err);
    bool updateDepartment(const std::string& studentID, const std::string& newDept, std::string& err);
    bool updateTel(const std::string& studentID, const std::string& newTel, std::string& err);

private:
    static std::string toLower(std::string s);  // 소문자 변환

    bool existsID(const std::string& ID) const;
    bool existsTel(const std::string& Tel) const;

    // 내부 상태
    std::string                 path_;
    std::vector<Student>        data_;
    SortKey                     sortKey_{SortKey::StudentID};

    // 분리된 컴포넌트
    StudentValidator            validator_;           // 무상태: 인스턴스 보유해도 무방
    FileStudentRepository       repository_;          // 파일 I/O 담당
};

#endif // STUDENT_DB_HPP
