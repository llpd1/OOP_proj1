#ifndef FILE_STUDENT_REPOSITORY_HPP
#define FILE_STUDENT_REPOSITORY_HPP

#include "Student.hpp"
#include <string>
#include <vector>

class FileStudentRepository {
public:
    explicit FileStudentRepository(std::string path);

    // 파일에서 학생 목록을 읽어들인다.
    // - 파일이 없으면 생성하고 안내 문구 출력 (원래 동작 유지)
    // - 각 라인을 탭(\t)으로 파싱한다.
    // - birthYear 파싱 실패 시 해당 레코드는 건너뜀(원래 동작과 동일하게 "무시")
    bool load(std::vector<Student>& out) const;

    // 전체 학생 목록을 파일에 저장 (탭 구분, \n)
    bool save(const std::vector<Student>& in) const;

    const std::string& path() const { return path_; }

private:
    std::string path_;
};

#endif // FILE_STUDENT_REPOSITORY_HPP
