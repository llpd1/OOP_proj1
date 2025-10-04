#include "FileStudentRepository.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

FileStudentRepository::FileStudentRepository(std::string path)
: path_(std::move(path)) {}

bool FileStudentRepository::load(std::vector<Student>& out) const {
    std::ifstream fin(path_);
    if (!fin.is_open()) {
        // 파일 없으면 새로 생성(빈 파일 준비) + 안내 메시지 (원래 출력 유지)
        std::ofstream create(path_);
        std::cout << "File '" << path_ << "' does not exist. File created.\n\n";
        return create.is_open();
    }

    out.clear();
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string name, studentID, birthYear, dept, tel;

        if (!std::getline(iss, name, '\t'))       continue;
        if (!std::getline(iss, studentID, '\t'))  continue;
        if (!std::getline(iss, birthYear, '\t'))  continue;
        if (!std::getline(iss, dept, '\t'))       continue;
        if (!std::getline(iss, tel, '\t'))        tel.clear(); // tel 누락 허용

        int by = 0;
        try { by = std::stoi(birthYear); }
        catch (...) { continue; } // 원래처럼 파싱 실패 레코드는 건너뜀

        out.push_back(Student{ name, studentID, by, dept, tel });
    }

    return true;
}

bool FileStudentRepository::save(const std::vector<Student>& in) const {
    std::ofstream fout(path_, std::ios::trunc);
    if (!fout.is_open()) {
        std::cerr << "Error: cannot write file " << path_ << "\n";
        return false;
    }
    for (const auto& s : in) {
        fout << s.name       << '\t'
             << s.studentID  << '\t'
             << s.birthYear  << '\t'
             << s.department << '\t'
             << s.tel        << '\n';
    }
    return true;
}
