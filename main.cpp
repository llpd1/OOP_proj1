#include "StudentDB.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <set>

static void printMainMenu() {
    std::cout << "1. Insertion\n"
                 "2. Insertion (freshman)\n"
                 "3. Search\n"
                 "4. Sorting Option\n"
                 "5. Update\n"
                 "6. Statistics\n"
                 "7. Exit\n> ";
}

static void printSearchMenu() {
    std::cout << "\n- Search Option -\n"
                 "1. Search by Name\n"
                 "2. Search by Student ID (10 digits)\n"
                 "3. Search by Admission Year (4 digits)\n"
                 "4. Search by Birth Year (4 digits)\n"
                 "5. Search by Department Name\n"
                 "6. List All\n> ";
}

static void printSortMenu() {
    std::cout << "\n- Sorting Option -\n"
                 "1. Sort by Name\n"
                 "2. Sort by Student ID (10 digits)\n"
                 "3. Sort by Birth Year (4digits)\n"
                 "4. Sort by Department Name\n> ";
}

static void printUpdateMenu() {
    std::cout << "\n- Update Option -\n"
                "1. Update Name\n"
                "2. Update Department Name\n"
                "3. Update Telephone Number\n> ";
}

static void printStudents(const std::vector<Student>& list) {
    const int W_NAME   = 12;
    const int W_ID     = 10;
    const int W_DEPT   = 20;
    const int W_B_YEAR = 10;
    const int W_TEL    = 12;

    std::cout << '\n'
              << std::left
              << std::setw(W_NAME)   << "Name" << " | "
              << std::setw(W_ID)     << "Student ID" << " | "
              << std::setw(W_DEPT)   << "Dept" << " | "
              << std::setw(W_B_YEAR) << "Birth Year" << " | "
              << std::setw(W_TEL)    << "Tel"
              <<'\n';

    for (int i = 0; i < W_NAME + 1; i++) std::cout << '-';
    std::cout << "+-";
    for (int i = 0; i < W_ID + 1; i++) std::cout << '-';
    std::cout << "+-";
    for (int i = 0; i < W_DEPT + 1; i++) std::cout << '-';
    std::cout << "+-";
    for (int i = 0; i < W_B_YEAR + 1; i++) std::cout << '-';
    std::cout << "+-";
    for (int i = 0; i < W_TEL + 1; i++) std::cout << '-';
    std::cout << '\n';

    for (const auto& s : list) {
        std::cout << std::left
                  << std::setw(W_NAME)   << s.name << " | "
                  << std::setw(W_ID)     << s.studentID << " | "
                  << std::setw(W_DEPT)   << s.department << " | "
                  << std::setw(W_B_YEAR) << s.birthYear << " | "
                  << std::setw(W_TEL)    << s.tel
                  <<'\n';
    }
    std::cout << '\n';
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: a.exe file1.txt\n\n";
        return 1;
    }
    StudentDB db(argv[1]);
    db.load();

    while (true) {
        printMainMenu();
        int sel{};
        if (!(std::cin >> sel)) {
            std::cin.clear();
            std::cout << "\nInvalid input. Please insert a number between 1 to 7.\n\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (sel == 1) {
            Student s;
            std::cout << "\nName ?\n";        getline(std::cin, s.name);
            std::cout << "Student ID (10 digits)?\n"; getline(std::cin, s.studentID);
            std::cout << "Birth Year (4 digits) ?\n";
            if (!(std::cin >> s.birthYear)) {
                std::cout << "Error : Birth Year must be 4 digits number\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Department ?\n";  getline(std::cin, s.department);
            std::cout << "Tel ?\n";         getline(std::cin, s.tel);

            std::vector<std::string> err;
            if (!db.insert(s, err)) {
                if (err.size() && err[0] == "DUP_ID") {
                    std::cout << "\nError : Already inserted\n\n";
                } else if (err.size() && err[0] == "DUP_TEL") {
                    std::cout << "\nError : Already inserted telephone number\n\n";
                } else {
                    int size = static_cast<int>(err.size());
                    std::cout << "\nInvalid input: ";
                    for (int i = 0; i < size; i++) {
                        std::cout << err[i];
                        if (i != size - 1) std::cout << ", ";
                    }
                    std::cout << "\n\n";
                }
            } else {
                std::cout << "Inserted.\n\n";
                db.save();
            }
        }
        else if (sel == 2) {
            Student s;
            std::cout << "\nName ?\n";        getline(std::cin, s.name);
            // 학번은 사용자에게 입력받지 않음
            std::cout << "Birth Year (4 digits) ?\n";
            if (!(std::cin >> s.birthYear)) {
                std::cout << "Error : Birth Year must be 4 digits number\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Department ?\n";  getline(std::cin, s.department);
            std::cout << "Tel ?\n";         getline(std::cin, s.tel);

            std::vector<std::string> err;
            // StudentDB에 새로 추가한 insertFreshman 함수 호출
            if (!db.insertFreshman(s, err)) {
                if (err.size() && err[0] == "DUP_TEL") {
                    std::cout << "\nError : Already inserted telephone number\n\n";
                } else {
                    int size = static_cast<int>(err.size());
                    std::cout << "\nInvalid input: ";
                    for (int i = 0; i < size; i++) {
                        std::cout << err[i];
                        if (i != size - 1) std::cout << ", ";
                    }
                    std::cout << "\n\n";
                }
            } else {
                // 성공 시, 생성된 학번을 사용자에게 알려줌
                std::cout << "\nInserted with new Student ID: " << s.studentID << "\n\n";
                db.save();
            }
        }
        else if (sel == 3) {
            if (db.isEmpty()) {
                std::cout << "\nDatabase is empty. Please insert students first.\n\n";
                continue;
            }
            printSearchMenu();
            int opt{};
            if (!(std::cin >> opt)) {
                std::cin.clear();
                std::cout << "\nInvalid input. Please insert a number between 1 to 6.\n\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (opt == 1) {
                std::string key; std::cout << "\nName keyword ?\n"; getline(std::cin, key);
                auto res = db.searchByName(key);
                printStudents(res);
            } else if (opt == 2) {
                std::string id; std::cout << "\nStudent ID ?\n"; getline(std::cin, id);
                auto res = db.searchById(id);
                printStudents(res);
            } else if (opt == 3) {
                int y{}; std::cout << "\nAdmission year ?\n";
                if (!(std::cin >> y)) {
                    std::cout << "Invalid year\n\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto res = db.searchByAdmissionYear(y);
                printStudents(res);
            } else if (opt == 4) {
                int y{}; std::cout << "\nBirth year ?\n";
                if (!(std::cin >> y)) {
                    std::cout << "Invalid year\n\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto res = db.searchByBirthYear(y);
                printStudents(res);
            } else if (opt == 5) {
                std::string kw; std::cout << "\nDepartment name keyword ?\n";
                getline(std::cin, kw);
                auto res = db.searchByDepartmentKeyword(kw);
                printStudents(res);
            } else if (opt == 6) {
                auto res = db.sortByKey();
                printStudents(res);
            } else {
                std::cout << "\nInvalid input. Please insert a number between 1 to 6.\n\n";
                continue;
            }
        }
        else if (sel == 4) {
            if (db.isEmpty()) {
                std::cout << "\nDatabase is empty. Please insert students first.\n\n";
                continue;
            }
            printSortMenu();
            int k{};
            if (!(std::cin >> k)) {
                std::cin.clear();
                std::cout << "\nInvalid input. Please insert a number between 1 to 4.\n\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if      (k == 1) db.setSortKey(SortKey::Name);
            else if (k == 2) db.setSortKey(SortKey::StudentID);
            else if (k == 3) db.setSortKey(SortKey::BirthYear);
            else if (k == 4) db.setSortKey(SortKey::Department);
            else {
                std::cout << "\nInvalid input. Please insert a number between 1 to 4.\n\n";
                continue;
            }
            db.sortByKey();
            db.save();
            std::cout << "\nSorting updated.\n\n";
        }
        else if (sel == 5) {
            if (db.isEmpty()) {
                std::cout << "\nDatabase is empty. Please insert students first.\n\n";
                continue;
            }
            printUpdateMenu();
            int u{};
            if (!(std::cin >> u)) {
                std::cin.clear();
                std::cout << "\nInvalid input. Please insert a number between 1 to 3.\n\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (u == 1) {
                std::string id, newName, err;
                std::cout << "\nStudent ID to update ?\n"; getline(std::cin, id);
                std::cout << "New Name ?\n";             getline(std::cin, newName);
                if (db.updateName(id, newName, err)) {
                    std::cout << "Updated.\n\n"; db.save();
                } else {
                    if (err == "Not found") std::cout << "No student found.\n\n";
                    else std::cout << "Invalid input: " << err << "\n\n";
                }
            } else if (u == 2) {
                std::string id, newDept, err;
                std::cout << "\nStudent ID to update ?\n"; getline(std::cin, id);
                std::cout << "New Department name ?\n";  getline(std::cin, newDept);
                if (db.updateDepartment(id, newDept, err)) {
                    std::cout << "Updated.\n\n"; db.save();
                } else {
                    if (err == "Not found") std::cout << "No student found.\n\n";
                    else std::cout << "Invalid input: " << err << "\n\n";
                }
            } else if (u == 3) {
                std::string id, newTel, err;
                std::cout << "\nStudent ID to update ?\n"; getline(std::cin, id);
                std::cout << "New Telephone number ?\n"; getline(std::cin, newTel);
                if (db.updateTel(id, newTel, err)) {
                    std::cout << "Updated.\n\n"; db.save();
                } else {
                    if (err == "Not found") std::cout << "No student found.\n\n";
                    else if (err == "\nTelephone number already exists.\n\n") std::cout << err;
                    else std::cout << "Invalid input: " << err << "\n\n";
                }
            } else {
                std::cout << "\nInvalid input. Please insert a number between 1 to 3.\n\n";
                continue;
            }
        }
        else if (sel == 6) { // ADD: Statistics
            if (db.isEmpty()) {
                std::cout << "\nDatabase is empty. Please insert students first.\n\n";
            } else {
                std::cout << "\n[Statistics]\n"
                             "Choose grouping order (1~3 items, space-separated):\n"
                             "  1) Admission Year\n"
                             "  2) Birth Year\n"
                             "  3) Department\n"
                             "Examples:\n"
                             "  1            -> Admission Year\n"
                             "  1 2          -> Admission Year -> Birth Year\n"
                             "  3 1 2        -> Department -> Admission Year -> Birth Year\n"
                             "> ";

                std::string line;
                std::getline(std::cin, line);

                std::istringstream iss(line);
                int x;
                std::vector<StatKey> order;
                while (iss >> x) {
                    if      (x == 1) order.push_back(StatKey::AdmissionYear);
                    else if (x == 2) order.push_back(StatKey::BirthYear);
                    else if (x == 3) order.push_back(StatKey::Department);
                }

                if (order.empty() || order.size() > 3 || std::set<StatKey>(order.begin(), order.end()).size() != order.size()) {
                    std::cout << "\nInvalid input. Please follow this format : 2 1 3\n\n";
                } else {
                    db.groupSummary(order, std::cout);
                }
            }
        }
        else if (sel == 7) {
            db.save();
            break;
        }
        else {
            std::cout << "\nInvalid input. Please insert a number between 1 to 7.\n\n";
            continue;
        }
    }
    return 0;
}
