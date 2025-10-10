#include "FileStudentRepository.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

FileStudentRepository::FileStudentRepository(std::string path)
: path_(std::move(path)) {}

bool FileStudentRepository::load(std::vector<Student>& out) const {
    std::ifstream fin(path_);
    if (!fin.is_open()) {
        // Create a new empty file if it doesn't exist, and print a notification message.
        std::ofstream create(path_);
        std::cout << "File '" << path_ << "' does not exist. File created.\n\n";
        return create.is_open();
    }

    out.clear();
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue; //skip empty lines

        std::istringstream iss(line);
        std::string name, studentID, birthYear, dept, tel;

        if (!std::getline(iss, name, '\t'))       continue;
        if (!std::getline(iss, studentID, '\t'))  continue;
        if (!std::getline(iss, birthYear, '\t'))  continue;
        if (!std::getline(iss, dept, '\t'))       continue;
        if (!std::getline(iss, tel, '\t'))        tel.clear(); //tel is optional

        int by = 0;
        try { by = std::stoi(birthYear); }
        catch (...) { continue; } // Skip records with invalid number format

        //add a new Student record to the vector
        out.push_back(Student{ name, studentID, by, dept, tel });
    }

    return true;
}

bool FileStudentRepository::save(const std::vector<Student>& in) const {
    std::ofstream fout(path_, std::ios::trunc);
    // If the file cannot be opened for writing, print an error and return false
    if (!fout.is_open()) {
        std::cerr << "Error: cannot write file " << path_ << "\n";
        return false;
    }

    // Write each Student record as a tab-separated line
    for (const auto& s : in) {
        fout << s.name       << '\t'
             << s.studentID  << '\t'
             << s.birthYear  << '\t'
             << s.department << '\t'
             << s.tel        << '\n';
    }
    return true;
}
