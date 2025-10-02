#include "StudentDB.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>

// 추가 기능: 문자열을 소문자로 변환 (대소문자 무시 검색용)
std::string StudentDB::toLower(std::string s) {
    for (auto& c : s)
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return s;
}

// 문자열이 비어있지않고 전부 숫자인지 판정
bool StudentDB::isDigits(const std::string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) {              // unsigned char로 받는 게 안전
        if (!isdigit(c)) return false;  // 하나라도 숫자 아니면 탈락
    }
    return true;
}

// 문자열이 비어있지 않고 영문자+공백으로만 이루어졌는지 판정 (이름 규칙)
bool StudentDB::isAlphaSpace(const std::string& s) {
    if (s.empty()) return false; // 비어 있으면 false
    for (unsigned char c : s) {
        if (!(isalpha(c) || isspace(c))) return false; // 하나라도 영문자/공백 아니면 실패
    }
    return true; // 전부 영문자나 공백이면 true
}

// 이름 유효성: 비어있지 않고 길이 <= 15, 영문/공백만 허용
bool StudentDB::validName(const std::string& s) {
    return !s.empty() && s.size() <= 15 && isAlphaSpace(s);
}

// 학번 유효성: 정확히 10자리 숫자, 앞 4자리는 1900~2100(합리 범위)로 제한
bool StudentDB::validStudentID(const std::string& s) {
    if (s.size() != 10 || !isDigits(s)) return false;
    int year = stoi(s.substr(0,4));
    return (year >= 1900 && year <= 2100);
}

// 출생연도 유효성: 1900~2100 범위의 정수 (4자리를 실수 방지 차원에서 범위로 보장)
bool StudentDB::validBirthYear(int y) {
    return (y >= 1900 && y <= 2100);
}

// 전화번호 유효성: 비어 있을 수 있으며 숫자만 가능, 길이 <= 12
bool StudentDB::validTel(const std::string& s) {
    return s.size() <= 12 && (s.empty() || isDigits(s));
}

// 메모리 내 학번 중복 여부 확인 (선형 탐색)
bool StudentDB::existsID(const std::string& id) const {
    // data_에 있는 모든 Student를 순회하면서 학번이 같은지 확인
    for (const Student& s : data_) {
        if (s.studentID == id) {   // 하나라도 같으면 즉시 true
            return true;
        }
    }
    return false; // 끝까지 못 찾으면 false
}

// ========================== 본체 구현부 ==========================

StudentDB::StudentDB(const std::string& path) {
    path_ = path;
}

// 파일에서 데이터 로딩
bool StudentDB::load() {
    std::ifstream fin(path_);
    if (!fin.is_open()) {
        // 파일 없으면 새로 생성(빈 파일 준비)
        std::ofstream create(path_);
        return create.is_open();
    }

    data_.clear();            // 기존 메모리 데이터 초기화
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        // 공백이 들어갈 수 있는 필드를 위해 '탭'을 구분자로 사용
        std::istringstream iss(line);
        std::string name, studentID, birthYear, dept, tel;

        if (!getline(iss, name, '\t'))       continue;
        if (!getline(iss, studentID,  '\t')) continue;
        if (!getline(iss, birthYear, '\t'))  continue;
        if (!getline(iss, dept, '\t'))       continue;
        if (!getline(iss, tel,  '\t'))       tel.clear(); // tel 필드가 없으면 공란 허용 <- 이것도 허용하는게 맞나? ㅋㅋ

        // birthYear 문자열 → 정수 변환 (숫자 아님/오버플로우 등은 스킵)
        int by = 0;
        try { by = std::stoi(birthYear); } catch (...) { continue; }

        // 유효성 검증에 통과한 레코드만 적재
        Student s{ name, studentID, by, dept, tel };
        if (!validName(s.name))            continue;
        if (!validStudentID(s.studentID))  continue;
        if (!validBirthYear(s.birthYear))  continue;
        if (!validTel(s.tel))              continue;
        if (existsID(s.studentID))         continue; // 파일 내 중복이 있으면 첫 항목만 채택

        data_.push_back(std::move(s));
    }
    return true;
}

// 메모리의 전체 데이터를 파일로 저장
bool StudentDB::save() const {
    std::ofstream fout(path_, std::ios::trunc);
    if (!fout.is_open()) {
        std::cerr << "Error: cannot write file " << path_ << "\n";
        return false;
    }

    for (const auto& s : data_) {
        fout << s.name       << '\t'
             << s.studentID  << '\t'
             << s.birthYear  << '\t'
             << s.department << '\t'
             << s.tel        << '\n';
    }
    return true;    
}

// ========================== 삽입 ==========================
// 내부에서 에러 메시지 버전 호출
bool StudentDB::insert(const Student& s) {
    std::string err;
    bool ok = insert(s, err);
    if (!ok && err == "DUP") {
        std::cout << "Error : Already inserted\n";
    }
    return ok;
}

// 에러 사유 반환형 insert
bool StudentDB::insert(const Student& s, std::string& err) {
    if (!validName(s.name))               { err = "Invalid name"; return false; }
    if (!validStudentID(s.studentID))     { err = "Invalid studentId"; return false; }
    if (!validBirthYear(s.birthYear))     { err = "Invalid birthYear"; return false; }
    if (!validTel(s.tel))                 { err = "Invalid tel"; return false; }
    if (s.name.empty() || s.studentID.empty()) { err = "Empty required"; return false; }

    // 학번 중복 금지
    if (existsID(s.studentID)) { err = "DUP"; return false; }

    data_.push_back(s);
    err.clear();
    return true;
}

// ========================== 검색 API들 ==========================
std::vector<Student> StudentDB::searchByName(const std::string& key) const {
    std::vector<Student> out;
    auto k = toLower(key);
    for (const auto& s : data_)
        if (toLower(s.name).find(k) != std::string::npos)
            out.push_back(s);
    return out; 
}

std::vector<Student> StudentDB::searchById(const std::string& id10) const {
    std::vector<Student> out;
    for (const auto& s : data_)
        if (s.studentID == id10)
            out.push_back(s);
    return out;
}

std::vector<Student> StudentDB::searchByAdmissionYear(int y) const {
    std::vector<Student> out;
    for (const auto& s : data_) {
        int ay = stoi(s.studentID.substr(0,4)); // load/insert 단계에서 검증되어 예외 위험 사실상 없음
        if (ay == y) out.push_back(s);
    }
    return out; 
}

std::vector<Student> StudentDB::searchByBirthYear(int y) const {
    std::vector<Student> out;
    for (const auto& s : data_)
        if (s.birthYear == y)
            out.push_back(s);
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

// ========================== 정렬 옵션/목록 ==========================
void StudentDB::setSortKey(SortKey k) { sortKey_ = k; }
SortKey StudentDB::sortKey() const { return sortKey_; }

std::vector<Student> StudentDB::listAll() const {
    std::vector<Student> v = data_; // 복사본 정렬

    switch (sortKey_) {
        case SortKey::Name:
            sort(v.begin(), v.end(), [](const Student& a, const Student& b){
                return (a.name == b.name) ? a.studentID < b.studentID : a.name < b.name;
            });
            break;

        case SortKey::StudentId:
            sort(v.begin(), v.end(), [](const Student& a, const Student& b){
                return a.studentID < b.studentID;
            });
            break;

        case SortKey::BirthYear:
            sort(v.begin(), v.end(), [](const Student& a, const Student& b){
                return (a.birthYear == b.birthYear) ? a.studentID < b.studentID : a.birthYear < b.birthYear;
            });
            break;

        case SortKey::Department:
            sort(v.begin(), v.end(), [](const Student& a, const Student& b){
                return (a.department == b.department) ? a.studentID < b.studentID : a.department < b.department;
            });
            break;
    }
    return v;
}

// ========================== 수정 ==========================
bool StudentDB::updateName(const std::string& studentId, const std::string& newName, std::string& err) {
    if (!validName(newName)) { err = "Invalid name"; return false; }
    for (auto& s : data_) {
        if (s.studentID == studentId) {
            s.name = newName;
            err.clear(); return true;
        }
    }
    err = "Not found"; return false;
}

bool StudentDB::updateDepartment(const std::string& studentId, const std::string& newDept, std::string& err) {
    // Dept는 공백 허용 → 길이 제한만(필요시) 둘 수 있음. 여기서는 그대로 허용.
    for (auto& s : data_) {
        if (s.studentID == studentId) {
            s.department = newDept;
            err.clear(); return true;
        }
    }
    err = "Not found"; return false;
}

bool StudentDB::updateTel(const std::string& studentId, const std::string& newTel, std::string& err) {
    for (auto& s : data_) {
        if (s.studentID == studentId) {
            s.tel = newTel;
            err.clear(); return true;
        }
    }
    err = "Not found"; return false;
}