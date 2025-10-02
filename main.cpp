#include "StudentDB.hpp"
#include <iostream>
#include <iomanip>
#include <limits>

static void printMainMenu() { //***** */
    std::cout << "1. Insertion\n"
            "2. Search\n"
            "3. Sorting Option\n"
            "4. Update\n"
            "5. Exit\n> ";
}

static void printSearchMenu() {
    std::cout << "- Search Option -\n"
                 "1. Search by Name\n"
                 "2. Search by Student ID (10 digits)\n"
                 "3. Search by Admission Year (4 digits)\n"
                 "4. Search by Birth Year (4 digits)\n"
                 "5. Search by Department Name\n"
                 "6. List All\n> ";
}

static void printSortMenu() {
    std::cout << "- Sorting Option -\n"
                 "1. Sort by Name\n"
                 "2. Sort by Student ID (10 digits)\n"
                 "3. Sort by Birth Year (4digits)\n"
                 "4. Sort by Department Name\n> ";
}

static void printUpdateMenu() {
    std::cout << "- Update Option -\n"
                "1. Update Name\n"
                "2. Update Department name\n"
                "3. Update Telephone number\n>"; // 이름, 학과, 전화번호는 바뀔 수 있다고 생각
}

static void printStudents(const std::vector<Student>& list) {
    const int W_NAME   = 12;
    const int W_ID     = 10;
    const int W_DEPT   = 20;
    const int W_B_YEAR = 10;
    const int W_TEL    = 12;

    std::cout << std::left
              << std::setw(W_NAME)   << "Name" << ' '
              << std::setw(W_ID)     << "StudentID" << ' '
              << std::setw(W_DEPT)   << "Dept" << ' '
              << std::setw(W_B_YEAR) << "Birth Year" << ' '
              << std::setw(W_TEL)    << "Tel" << ' '
              <<'\n';

    for(const auto& s : list) {
        std::cout << std::left
                  << std::setw(W_NAME) << s.name << ' '
                  << std::setw(W_ID) << s.studentID << ' '
                  << std::setw(W_DEPT) << s.department << ' '
                  << std::right
                  << std::setw(W_B_YEAR) << s.birthYear << ' '
                  << std::left
                  << std::setw(W_TEL) << s.tel << ' '
                  <<'\n';
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: a.exe file1.txt\n";
        return 1;
    }
    StudentDB db(argv[1]);
    
    if (!db.load()) {
        std::cerr << "Failed to load file. File created.\n";
    }

    while (true) {
        printMainMenu();
        int sel{}; if (!(std::cin >> sel)) break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 입력 버퍼 지우기

        if (sel == 1) { // valid 생성 후 if문 valid == true 넘어가고 valid == false면 처음부터 재입력, mainmenu X
            // Insertion
            Student s;
            std::cout << "Name ?\n";        getline(std::cin, s.name);
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

            std::string err;
            if (!db.insert(s, err)) {
                if (err == "DUP") {
                    std::cout << "Error : Already inserted\n";
                } else {
                    std::cout << "Invalid input: " << err << '\n';
                }
            } else {
                std::cout << "Inserted.\n";
                db.save();
            }
        }
        else if (sel == 2) {
            printSearchMenu();
            int opt{}; if (!(std::cin >> opt)) break;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (opt == 1) {
                std::string key; std::cout << "Name keyword? "; getline(std::cin, key);
                auto res = db.searchByName(key);
                printStudents(res);
            } else if (opt == 2) {
                std::string id; std::cout << "Student ID? "; getline(std::cin, id);
                auto res = db.searchById(id);
                printStudents(res);
            } else if (opt == 3) {
                int y{}; 
                std::cout << "Admission year? ";
                if (!(std::cin >> y)) {
                    std::cout << "Invalid year\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue; // 메인 메뉴로 복귀
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto res = db.searchByAdmissionYear(y);
                printStudents(res);
            } else if (opt == 4) {
                int y{}; std::cout << "Birth year? ";
                if (!(std::cin >> y)) {
                    std::cout << "Invalid year\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue; // 메인 메뉴로 복귀
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                auto res = db.searchByBirthYear(y);
                printStudents(res);
            } else if (opt == 5) {
                std::string kw; std::cout << "Department name keyword? ";
                getline(std::cin, kw);
                auto res = db.searchByDepartmentKeyword(kw);
                printStudents(res);
            } else if (opt == 6) {
                auto res = db.listAll();
                printStudents(res);
            }
        }
        else if (sel == 3) {
            printSortMenu();
            int k{}; if (!(std::cin >> k)) break;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if      (k == 1) db.setSortKey(SortKey::Name);
            else if (k == 2) db.setSortKey(SortKey::StudentId);
            else if (k == 3) db.setSortKey(SortKey::BirthYear);
            else if (k == 4) db.setSortKey(SortKey::Department);
            std::cout << "Sorting updated.\n";
        }
        else if (sel == 4) {
            printUpdateMenu();
            int u{}; if (!(std::cin >> u)) break;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (u == 1) {
                std::string id, newName, err;
                std::cout << "Student ID to update? "; getline(std::cin, id);
                std::cout << "New Name? ";             getline(std::cin, newName);
                if (db.updateName(id, newName, err)) {
                    std::cout << "Updated.\n"; db.save();
                } else {
                    if (err == "Not found") std::cout << "No student found.\n";
                    else std::cout << "Invalid input: " << err << '\n';
                }
            } else if (u == 2) {
                std::string id, newDept, err;
                std::cout << "Student ID to update? "; getline(std::cin, id);
                std::cout << "New Department name? ";  getline(std::cin, newDept);
                if (db.updateDepartment(id, newDept, err)) {
                    std::cout << "Updated.\n"; db.save();
                } else {
                    if (err == "Not found") std::cout << "No student found.\n";
                    else std::cout << "Invalid input: " << err << '\n';
                }
            } else if (u == 3) {
                std::string id, newTel, err;
                std::cout << "Student ID to update? "; getline(std::cin, id);
                std::cout << "New Telephone number? "; getline(std::cin, newTel);
                if (db.updateTel(id, newTel, err)) {
                    std::cout << "Updated.\n"; db.save();
                } else {
                    if (err == "Not found") std::cout << "No student found.\n";
                    else std::cout << "Invalid input: " << err << '\n';
                }
            }
        }
        else if (sel == 5) {
            db.save();
            break;
        }
    }
    return 0;
}
