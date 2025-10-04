#include "StudentDB.hpp"
#include <algorithm>
#include <sstream>
#include <cctype>

StudentDB::StudentDB(const std::string& path)
: path_(path), repository_(path) {}

bool StudentDB::isEmpty() const {
    return data_.empty();
}

std::string StudentDB::toLower(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// ---------- 중복 체크 ----------
bool StudentDB::existsID(const std::string& id) const {
    for (const Student& s : data_) {
        if (s.studentID == id) return true;
    }
    return false;
}

bool StudentDB::existsTel(const std::string& tel) const {
    for (const Student& s : data_) {
        if (s.tel == tel) return true;
    }
    return false;
}

// ---------- 로드/세이브 ----------
bool StudentDB::load() {
    std::vector<Student> raw;
    if (!repository_.load(raw)) return false;

    data_.clear();
    for (const auto& s : raw) {
        // 원래 동작: 유효성 검증 통과 + 학번 중복 없을 때만 적재
        if (!StudentValidator::validName(s.name))             continue;
        if (!StudentValidator::validStudentID(s.studentID))   continue;
        if (!StudentValidator::validBirthYear(s.birthYear))   continue;
        if (!StudentValidator::validDepartment(s.department)) continue;
        if (!StudentValidator::validTel(s.tel))               continue;
        if (existsID(s.studentID))                            continue; // 파일 내 중복 있으면 첫 항목만 채택
        data_.push_back(s);
    }
    return true;
}

bool StudentDB::save() const {
    return repository_.save(data_);
}

// ---------- 삽입 ----------
bool StudentDB::insert(const Student& s) {
    std::vector<std::string> err;
    return insert(s, err);
}

bool StudentDB::insert(const Student& s, std::vector<std::string>& err) {
    // 학번/전화 중복 우선 차단 (원래 동작/메시지 유지)
    if (existsID(s.studentID)) { err.push_back("DUP_ID"); return false; }
    if (existsTel(s.tel))      { err.push_back("DUP_TEL"); return false; }

    // 필드 검증 (원래 키 문자열 유지)
    if (!StudentValidator::validName(s.name))               { err.push_back("Name"); }
    if (!StudentValidator::validStudentID(s.studentID))     { err.push_back("Student ID"); }
    if (!StudentValidator::validBirthYear(s.birthYear))     { err.push_back("Birth Year"); }
    if (!StudentValidator::validDepartment(s.department))   { err.push_back("Department"); }
    if (!StudentValidator::validTel(s.tel))                 { err.push_back("Tel"); }
    if (s.name.empty() || s.studentID.empty())              { err.push_back("Empty required"); }

    if (err.empty()) {
        data_.push_back(s);
        err.clear();
        return true;
    } else {
        return false;
    }
}

// ---------- 검색 ----------
std::vector<Student> StudentDB::searchByName(const std::string& key) const {
    std::vector<Student> out;
    auto k = toLower(key);
    for (const auto& s : data_) {
        if (toLower(s.name).find(k) != std::string::npos)
            out.push_back(s);
    }
    return out;
}

std::vector<Student> StudentDB::searchById(const std::string& id10) const {
    std::vector<Student> out;
    for (const auto& s : data_) {
        if (s.studentID == id10) out.push_back(s);
    }
    return out;
}

std::vector<Student> StudentDB::searchByAdmissionYear(int y) const {
    std::vector<Student> out;
    for (const auto& s : data_) {
        int ay = std::stoi(s.studentID.substr(0,4)); // load/insert에서 검증되므로 예외 위험 낮음
        if (ay == y) out.push_back(s);
    }
    return out;
}

std::vector<Student> StudentDB::searchByBirthYear(int y) const {
    std::vector<Student> out;
    for (const auto& s : data_) {
        if (s.birthYear == y) out.push_back(s);
    }
    return out;
}

std::vector<Student> StudentDB::searchByDepartmentKeyword(const std::string& kw) const {
    std::vector<Student> out;
    auto k = toLower(kw);
    for (const auto& s : data_) {
        if (toLower(s.department).find(k) != std::string::npos)
            out.push_back(s);
    }
    return out;
}

// ---------- 정렬 ----------
void StudentDB::setSortKey(SortKey k) { sortKey_ = k; }
SortKey StudentDB::sortKey() const { return sortKey_; }

std::vector<Student>& StudentDB::sortByKey() {
    switch (sortKey_) {
        case SortKey::Name:
            std::sort(data_.begin(), data_.end(), [](const Student& a, const Student& b){
                return (a.name == b.name) ? a.studentID < b.studentID : a.name < b.name;
            });
            break;

        case SortKey::StudentID:
            std::sort(data_.begin(), data_.end(), [](const Student& a, const Student& b){
                return a.studentID < b.studentID;
            });
            break;

        case SortKey::BirthYear:
            std::sort(data_.begin(), data_.end(), [](const Student& a, const Student& b){
                return (a.birthYear == b.birthYear) ? a.studentID < b.studentID : a.birthYear < b.birthYear;
            });
            break;

        case SortKey::Department:
            std::sort(data_.begin(), data_.end(), [](const Student& a, const Student& b){
                return (a.department == b.department) ? a.studentID < b.studentID : a.department < b.department;
            });
            break;
        
        default:
            break;
    }
    return data_;
}

// ---------- 수정 ----------
bool StudentDB::updateName(const std::string& studentID, const std::string& newName, std::string& err) {
    if (!StudentValidator::validName(newName)) { err = "Name"; return false; }
    for (auto& s : data_) {
        if (s.studentID == studentID) {
            s.name = newName;
            err.clear(); return true;
        }
    }
    err = "Not found"; return false;
}

bool StudentDB::updateDepartment(const std::string& studentID, const std::string& newDept, std::string& err) {
    if (!StudentValidator::validDepartment(newDept)) { err = "Department"; return false; }
    for (auto& s : data_) {
        if (s.studentID == studentID) {
            s.department = newDept;
            err.clear(); return true;
        }
    }
    err = "Not found"; return false;
}

bool StudentDB::updateTel(const std::string& studentID, const std::string& newTel, std::string& err) {
    if (!StudentValidator::validTel(newTel)) { err = "Telephone number"; return false; }
    if (existsTel(newTel)) { err = "\nTelephone number already exists.\n\n"; return false; }
    for (auto& s : data_) {
        if (s.studentID == studentID) {
            s.tel = newTel;
            err.clear(); return true;
        }
    }
    err = "Not found"; return false;
}
