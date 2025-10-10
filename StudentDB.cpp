#include "StudentDB.hpp"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <map>
#include <string>
#include <random>
#include <iomanip> 


StudentDB::StudentDB(const std::string& path)
: path_(path), repository_(path) {}

bool StudentDB::isEmpty() const {
    return data_.empty();
}

std::string StudentDB::toLower(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// ---------- DUP CHECK ----------
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

// ---------- LOAD/SAVE ----------
bool StudentDB::load() {
    std::vector<Student> raw;
    if (!repository_.load(raw)) return false;

    data_.clear();
    for (const auto& s : raw) {
        // Original behavior: load only if validation passes and student ID is unique
        if (!StudentValidator::validName(s.name))             continue;
        if (!StudentValidator::validStudentID(s.studentID))   continue;
        if (!StudentValidator::validBirthYear(s.birthYear))   continue;
        if (!StudentValidator::validDepartment(s.department)) continue;
        if (!StudentValidator::validTel(s.tel))               continue;
        if (existsID(s.studentID))                            continue; // If duplicates exist in file, keep only the first one
        data_.push_back(s);
    }
    return true;
}

bool StudentDB::save() const {
    return repository_.save(data_);
}

// ---------- INSERTION ----------
bool StudentDB::insert(const Student& s) {
    std::vector<std::string> err;
    return insert(s, err);
}

bool StudentDB::insert(const Student& s, std::vector<std::string>& err) {
    // Pre-check for duplicate student ID/telephone (keep original behavior/message)
    if (existsID(s.studentID)) { err.push_back("DUP_ID"); return false; }
    if (existsTel(s.tel))      { err.push_back("DUP_TEL"); return false; }

    // Field validation (preserve original key strings)
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
//  ---------- FRESHMAN INSERTION ----------
bool StudentDB::insertFreshman(Student& s, std::vector<std::string>& err) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 999999); // create 6 digits in the range (0 ~ 999999)

    const std::string admissionYear = "2025";
    std::string newID;
    
    do {
        std::stringstream ss;
        ss << admissionYear << std::setw(6) << std::setfill('0') << distrib(gen);
        newID = ss.str();
    } while (existsID(newID));

    s.studentID = newID;

    return insert(s, err);
}
// ---------- SEARCH ----------
std::vector<Student> StudentDB::searchByName(const std::string& key) const {
    std::vector<Student> out;
    auto k = toLower(key);
    for (const auto& s : data_) {
        if (toLower(s.name).find(k) != std::string::npos)
            out.push_back(s);
    }
    return out;
}

std::vector<Student> StudentDB::searchByID(const std::string& id10) const {
    std::vector<Student> out;
    for (const auto& s : data_) {
        if (s.studentID == id10) out.push_back(s);
    }
    return out;
}

std::vector<Student> StudentDB::searchByAdmissionYear(int y) const {
    std::vector<Student> out;
    for (const auto& s : data_) {
        int ay = std::stoi(s.studentID.substr(0,4)); // checked in load/insert part
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

// ---------- SORTING ----------
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

// ---------- UPDATE ----------
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
// Internal Tree Node
struct __StatsNode {
    std::map<std::string, __StatsNode> children; // child groups
    int count = 0;                               // total count of this node (group)
};

// Output label (for display formatting)
static std::string __labelOf(StatKey key, const std::string& v) {
    switch (key) {
        case StatKey::AdmissionYear: return v + "Admission";   // ex) "2025 Admission"
        case StatKey::BirthYear:     return v + "Born";   // ex) "2005 Born"
        case StatKey::Department:    return v;            // Department Name
        default:                     return v;
    }
}

// Extract the key value from a student record
static std::string __valueOf(const Student& s, StatKey key) {
    switch (key) {
        case StatKey::AdmissionYear:
            if (s.studentID.size() >= 4) {
                try { return s.studentID.substr(0,4); } catch (...) { return "????"; }
            }
            return "????";
        case StatKey::BirthYear:
            return std::to_string(s.birthYear);
        case StatKey::Department:
            return s.department;
        default:
            return "";
    }
}

// Recursive printing of the hierarchical tree
static void __printTree(const __StatsNode& node,
                        const std::vector<StatKey>& order,
                        size_t depth,
                        std::ostream& out,
                        const std::string& prefix)
{
    if (depth >= order.size() || node.children.empty()) {
        return;
    }

    StatKey currentKey = order[depth];
    auto it = node.children.begin();
    while (it != node.children.end()) {
        const auto& key = it->first;
        const auto& childNode = it->second;
        
        bool isLast = (++it == node.children.end());

        out << prefix << "+-- " << __labelOf(currentKey, key) << " (" << childNode.count << ")\n";

        std::string nextPrefix = prefix + (isLast ? "    " : "|   ");
        // Recursively call the function for child nodes
        __printTree(childNode, order, depth + 1, out, nextPrefix);
    }
}

// Public API: print hierarchical summary statistics
void StudentDB::groupSummary(const std::vector<StatKey>& order, std::ostream& out) const {
    if (order.empty()) {
        out << "Total (" << data_.size() << ")\n\n";
        return;
    }

    __StatsNode root;
    root.count = static_cast<int>(data_.size());

    for (const auto& s : data_) {
        __StatsNode* cur = &root;
        for (StatKey k : order) {
            std::string key = __valueOf(s, k);
            cur = &cur->children[key];
            cur->count += 1;
        }
    }

    out << "Total (" << root.count << ")\n";
    
    __printTree(root, order, 0, out, ""); // initial prefix is empty
    
    out << '\n';
}
