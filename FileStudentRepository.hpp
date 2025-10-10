#ifndef FILE_STUDENT_REPOSITORY_HPP
#define FILE_STUDENT_REPOSITORY_HPP

#include "Student.hpp"
#include <string>
#include <vector>

class FileStudentRepository {
public:
    explicit FileStudentRepository(std::string path);

    // Loads the list of students from the file.
    // - If the file does not exist, create a new empty file and display a notice.
    // - Each record is parsed as a tab-separated line.
    // - If birthYear cannot be parsed as an integer, that record is skipped.
    bool load(std::vector<Student>& out) const;

    // Saves all student records to the file (tab-separated, each ending with '\n').
    bool save(const std::vector<Student>& in) const;

    const std::string& path() const { return path_; }

private:
    std::string path_;// Path to the text file storing student records
};

#endif // FILE_STUDENT_REPOSITORY_HPP
