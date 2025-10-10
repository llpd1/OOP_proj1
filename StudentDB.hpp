#ifndef STUDENT_DB_HPP
#define STUDENT_DB_HPP

#include "Student.hpp"
#include "StudentValidator.hpp"
#include "FileStudentRepository.hpp"
#include <vector>
#include <string>
#include <map>
#include <ostream> 

enum class SortKey { Name, StudentID, BirthYear, Department };
enum class StatKey { AdmissionYear, BirthYear, Department };

class StudentDB {
public:
    explicit StudentDB(const std::string& path);

    // Load/Save
    bool load();
    bool save() const;

    // check if empty
    bool isEmpty() const;

    // Insertion(duplicate StudentId/Tel check)
    bool insert(const Student& s); // wrapper for convinience
    bool insert(const Student& s, std::vector<std::string>& err);
    
    // Function for Freshman Insertion
    bool insertFreshman(Student& s, std::vector<std::string>& err);

    // Search
    std::vector<Student> searchByName(const std::string& key) const;      
    std::vector<Student> searchByID(const std::string& id10) const;        
    std::vector<Student> searchByAdmissionYear(int year4) const;          
    std::vector<Student> searchByBirthYear(int year4) const;                
    std::vector<Student> searchByDepartmentKeyword(const std::string& kw) const; 

    // Sorting Option
    std::vector<Student>& sortByKey();
    void setSortKey(SortKey k);
    SortKey sortKey() const;

    // Update
    bool updateName(const std::string& studentID, const std::string& newName, std::string& err);
    bool updateDepartment(const std::string& studentID, const std::string& newDept, std::string& err);
    bool updateTel(const std::string& studentID, const std::string& newTel, std::string& err);

    void groupSummary(const std::vector<StatKey>& order, std::ostream& out) const;

private:
    static std::string toLower(std::string s);  // convert to small letter

    bool existsID(const std::string& ID) const;
    bool existsTel(const std::string& Tel) const;

    // Internal state
    std::string                 path_;
    std::vector<Student>        data_;
    SortKey                     sortKey_;

    // Separated components
    StudentValidator            validator_;           // Stateless: safe to hold as an instance
    FileStudentRepository       repository_;          // Responsible for file I/O
};

#endif // STUDENT_DB_HPP
