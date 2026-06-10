/*
Library Management System (C++ console)
Features:
 - Admin Login (Add/Remove books, view issued books)
 - Student Login (Issue/Return books)
 - Search books by title or author
 - Fine calculation for delayed return (default loan period 14 days, fine_per_day = 5)
Persistence:
 - books.csv, students.csv, issues.csv (simple CSV format)
Compile:
 g++ -std=c++17 library.cpp -o library
 Run:
 ./library
*/

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const string BOOK_FILE = "books.csv";
const string STUDENT_FILE = "students.csv";
const string ISSUE_FILE = "issues.csv";
const string ADMIN_PASSWORD = "admin123";   // change as needed
const int DEFAULT_DUE_DAYS = 14;
const int FINE_PER_DAY = 5;

struct Book {
    int id;
    string title;
    string author;
    string category;
    int totalCopies;
    int availableCopies;

    string to_csv() const {
        // escape semicolons by replacing them (simple approach)
        auto esc = [](const string &s){
            string out = s;
            // replace any ';' with ',' to avoid CSV break
            for (char &c : out) if (c == ';') c = ',';
            return out;
        };
        return to_string(id) + ";" + esc(title) + ";" + esc(author) + ";" + esc(category) + ";" +
               to_string(totalCopies) + ";" + to_string(availableCopies);
    }

    static Book from_csv(const string &line) {
        Book b;
        vector<string> parts;
        string cur;
        for (char c : line) {
            if (c == ';') { parts.push_back(cur); cur.clear(); }
            else cur.push_back(c);
        }
        parts.push_back(cur);
        if (parts.size() >= 6) {
            b.id = stoi(parts[0]);
            b.title = parts[1];
            b.author = parts[2];
            b.category = parts[3];
            b.totalCopies = stoi(parts[4]);
            b.availableCopies = stoi(parts[5]);
        }
        return b;
    }
};

struct Student {
    int id;
    string name;
    int totalFines; // total outstanding fines in integer currency units

    string to_csv() const {
        auto esc = [](const string &s){
            string out = s;
            for (char &c : out) if (c == ';') c = ',';
            return out;
        };
        return to_string(id) + ";" + esc(name) + ";" + to_string(totalFines);
    }

    static Student from_csv(const string &line) {
        Student s;
        vector<string> parts;
        string cur;
        for (char c : line) {
            if (c == ';') { parts.push_back(cur); cur.clear(); }
            else cur.push_back(c);
        }
        parts.push_back(cur);
        if (parts.size() >= 3) {
            s.id = stoi(parts[0]);
            s.name = parts[1];
            s.totalFines = stoi(parts[2]);
        }
        return s;
    }
};

struct IssueRecord {
    int bookId;
    int studentId;
    time_t issueTime; // epoch seconds
    int dueDays;      // loan term in days

    string to_csv() const {
        return to_string(bookId) + ";" + to_string(studentId) + ";" + to_string((long long)issueTime) + ";" + to_string(dueDays);
    }

    static IssueRecord from_csv(const string &line) {
        IssueRecord r{};
        vector<string> parts;
        string cur;
        for (char c : line) {
            if (c == ';') { parts.push_back(cur); cur.clear(); }
            else cur.push_back(c);
        }
        parts.push_back(cur);
        if (parts.size() >= 4) {
            r.bookId = stoi(parts[0]);
            r.studentId = stoi(parts[1]);
            r.issueTime = (time_t)stoll(parts[2]);
            r.dueDays = stoi(parts[3]);
        }
        return r;
    }
};


vector<Book> books;
vector<Student> students;
vector<IssueRecord> issues;

// --- utility ---
void press_enter_to_continue() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int next_book_id() {
    int mx = 0;
    for (auto &b : books) mx = max(mx, b.id);
    return mx + 1;
}
int next_student_id() {
    int mx = 0;
    for (auto &s : students) mx = max(mx, s.id);
    return mx + 1;
}

Book* find_book_by_id(int id) {
    for (auto &b : books) if (b.id == id) return &b;
    return nullptr;
}

Student* find_student_by_id(int id) {
    for (auto &s : students) if (s.id == id) return &s;
    return nullptr;
}

vector<IssueRecord*> find_issues_by_student(int sid) {
    vector<IssueRecord*> res;
    for (auto &r : issues) if (r.studentId == sid) res.push_back(&r);
    return res;
}

IssueRecord* find_issue(int bookId, int studentId) {
    for (auto &r : issues) if (r.bookId == bookId && r.studentId == studentId) return &r;
    return nullptr;
}

// --- file I/O ---
void load_books() {
    books.clear();
    ifstream fin(BOOK_FILE);
    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        books.push_back(Book::from_csv(line));
    }
}
void save_books() {
    ofstream fout(BOOK_FILE);
    for (auto &b : books) fout << b.to_csv() << "\n";
}

void load_students() {
    students.clear();
    ifstream fin(STUDENT_FILE);
    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        students.push_back(Student::from_csv(line));
    }
}
void save_students() {
    ofstream fout(STUDENT_FILE);
    for (auto &s : students) fout << s.to_csv() << "\n";
}

void load_issues() {
    issues.clear();
    ifstream fin(ISSUE_FILE);
    string line;
    while (getline(fin, line)) {
        if (line.size() == 0) continue;
        issues.push_back(IssueRecord::from_csv(line));
    }
}
void save_issues() {
    ofstream fout(ISSUE_FILE);
    for (auto &r : issues) fout << r.to_csv() << "\n";
}

void load_all() {
    load_books();
    load_students();
    load_issues();
}
void save_all() {
    save_books();
    save_students();
    save_issues();
}

// --- book operations ---
void add_book() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    Book b;
    b.id = next_book_id();
    cout << "Enter Title: ";
    getline(cin, b.title);
    cout << "Enter Author: ";
    getline(cin, b.author);
    cout << "Enter Category: ";
    getline(cin, b.category);
    cout << "Enter number of copies: ";
    cin >> b.totalCopies;
    if (b.totalCopies < 0) b.totalCopies = 1;
    b.availableCopies = b.totalCopies;
    books.push_back(b);
    save_books();
    cout << "Added Book with ID: " << b.id << "\n";
}

void remove_book() {
    int id;
    cout << "Enter Book ID to remove: ";
    cin >> id;
    auto it = remove_if(books.begin(), books.end(), [id](const Book &b){ return b.id == id; });
    if (it == books.end()) {
        cout << "No book removed (book not found).\n";
    } else {
        books.erase(it, books.end());
        // Also remove any outstanding issues for that book
        issues.erase(remove_if(issues.begin(), issues.end(), [id](const IssueRecord &r){ return r.bookId == id; }), issues.end());
        save_all();
        cout << "Book removed (and related issue records if any).\n";
    }
}

void list_books(bool onlyAvailable=false) {
    cout << left << setw(6) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(12) << "Category" << setw(8) << "Avail" << setw(6) << "Total" << "\n";
    cout << string(90, '-') << "\n";
    for (auto &b : books) {
        if (onlyAvailable && b.availableCopies <= 0) continue;
        cout << setw(6) << b.id << setw(30) << (b.title.size()>28?b.title.substr(0,27)+"...":b.title) << setw(20) << (b.author.size()>18?b.author.substr(0,17)+"...":b.author) 
             << setw(12) << (b.category.size()>10?b.category.substr(0,9)+"...":b.category)
             << setw(8) << b.availableCopies << setw(6) << b.totalCopies << "\n";
    }
}

void search_books() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Search by (1) Title (2) Author : ";
    int opt; cin >> opt; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string term;
    if (opt == 1) {
        cout << "Enter title keyword: ";
        getline(cin, term);
    } else {
        cout << "Enter author keyword: ";
        getline(cin, term);
    }
    transform(term.begin(), term.end(), term.begin(), ::tolower);
    cout << "\nSearch results:\n";
    cout << left << setw(6) << "ID" << setw(30) << "Title" << setw(20) << "Author" << setw(8) << "Avail" << "\n";
    cout << string(70, '-') << "\n";
    for (auto &b : books) {
        string field = (opt==1?b.title:b.author);
        string low = field;
        transform(low.begin(), low.end(), low.begin(), ::tolower);
        if (low.find(term) != string::npos) {
            cout << setw(6) << b.id << setw(30) << (b.title.size()>28?b.title.substr(0,27)+"...":b.title) << setw(20) << (b.author.size()>18?b.author.substr(0,17)+"...":b.author)
                 << setw(8) << b.availableCopies << "\n";
        }
    }
}

// --- student operations ---
void register_student() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    Student s;
    s.id = next_student_id();
    cout << "Enter student name: ";
    getline(cin, s.name);
    s.totalFines = 0;
    students.push_back(s);
    save_students();
    cout << "Student registered. ID: " << s.id << "\n";
}

void view_student(int sid) {
    Student* s = find_student_by_id(sid);
    if (!s) {
        cout << "Student not found.\n";
        return;
    }
    cout << "Student ID: " << s->id << "\nName: " << s->name << "\nOutstanding fines: " << s->totalFines << "\n";
    auto recs = find_issues_by_student(sid);
    if (recs.empty()) cout << "No books currently issued.\n";
    else {
        cout << "Currently issued books:\n";
        for (auto rptr : recs) {
            Book* b = find_book_by_id(rptr->bookId);
            time_t now = time(nullptr);
            double daysPassed = difftime(now, rptr->issueTime) / (60*60*24);
            int daysInt = (int)floor(daysPassed);
            cout << " BookID: " << rptr->bookId << " | Title: " << (b?b->title:"<not found>") 
                 << " | Issued: " << daysInt << " day(s) ago | Due in: " << (rptr->dueDays - daysInt) << " day(s)\n";
        }
    }
}

// --- issue/return ---
void issue_book(int studentId) {
    Student* s = find_student_by_id(studentId);
    if (!s) {
        cout << "Student not found. Register first.\n";
        return;
    }
    list_books(true);
    cout << "Enter Book ID to issue: ";
    int bid; cin >> bid;
    Book* b = find_book_by_id(bid);
    if (!b) {
        cout << "Book not found.\n";
        return;
    }
    if (b->availableCopies <= 0) {
        cout << "No copies available to issue.\n";
        return;
    }
    if (find_issue(bid, studentId)) {
        cout << "This student already has this book issued.\n";
        return;
    }
    IssueRecord r;
    r.bookId = bid;
    r.studentId = studentId;
    r.issueTime = time(nullptr);
    r.dueDays = DEFAULT_DUE_DAYS;
    issues.push_back(r);
    b->availableCopies -= 1;
    save_all();
    cout << "Book issued successfully. Due in " << DEFAULT_DUE_DAYS << " days.\n";
}

void return_book(int studentId) {
    Student* s = find_student_by_id(studentId);
    if (!s) {
        cout << "Student not found.\n";
        return;
    }
    auto recs = find_issues_by_student(studentId);
    if (recs.empty()) {
        cout << "No issued books to return.\n";
        return;
    }
    cout << "Books issued to you:\n";
    for (auto rptr : recs) {
        Book* b = find_book_by_id(rptr->bookId);
        cout << " Book ID: " << rptr->bookId << " | Title: " << (b?b->title:"<not found>") << "\n";
    }
    cout << "Enter Book ID to return: ";
    int bid; cin >> bid;
    IssueRecord* r = find_issue(bid, studentId);
    if (!r) {
        cout << "No such issued book for this student.\n";
        return;
    }
    time_t now = time(nullptr);
    double daysPassed = difftime(now, r->issueTime) / (60*60*24);
    int daysInt = (int)floor(daysPassed);
    int lateDays = max(0, daysInt - r->dueDays);
    int fine = lateDays * FINE_PER_DAY;
    if (fine > 0) {
        cout << "Book is returned " << lateDays << " day(s) late. Fine: " << fine << "\n";
        s->totalFines += fine;
    } else {
        cout << "Book returned on time. No fine.\n";
    }
    // update book availability
    Book* bptr = find_book_by_id(bid);
    if (bptr) bptr->availableCopies += 1;
    // remove issue record
    issues.erase(remove_if(issues.begin(), issues.end(), [&](const IssueRecord &ir){ return ir.bookId==bid && ir.studentId==studentId; }), issues.end());
    save_all();
}

void view_all_issues() {
    if (issues.empty()) {
        cout << "No issued books currently.\n";
        return;
    }
    cout << left << setw(8) << "BookID" << setw(10) << "StudentID" << setw(22) << "Title" << setw(18) << "StudentName" << setw(12) << "Issued(days ago)" << setw(8) << "Due" << "\n";
    cout << string(90, '-') << "\n";
    time_t now = time(nullptr);
    for (auto &r : issues) {
        Book* b = find_book_by_id(r.bookId);
        Student* s = find_student_by_id(r.studentId);
        double daysPassed = difftime(now, r.issueTime) / (60*60*24);
        int daysInt = (int)floor(daysPassed);
        cout << setw(8) << r.bookId << setw(10) << r.studentId << setw(22) << (b? (b->title.size()>20?b->title.substr(0,19)+"...":b->title) : "<not found>")
             << setw(18) << (s? (s->name.size()>16?s->name.substr(0,15)+"...":s->name) : "<not found>")
             << setw(12) << daysInt << setw(8) << r.dueDays << "\n";
    }
}

// Admin menu
void admin_menu() {
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Book\n2. Remove Book\n3. List All Books\n4. List Available Books\n5. View Issued Books\n6. Search Books\n7. Register Student\n8. List Students\n9. Logout\nChoose: ";
        int c; cin >> c;
        switch (c) {
            case 1: add_book(); break;
            case 2: remove_book(); break;
            case 3: list_books(false); break;
            case 4: list_books(true); break;
            case 5: view_all_issues(); break;
            case 6: search_books(); break;
            case 7: register_student(); break;
            case 8: {
                cout << left << setw(8) << "ID" << setw(30) << "Name" << setw(10) << "Fines" << "\n";
                cout << string(50, '-') << "\n";
                for (auto &s : students) cout << setw(8) << s.id << setw(30) << s.name << setw(10) << s.totalFines << "\n";
                break;
            }
            case 9:
                cout << "Logging out admin.\n"; return;
            default: cout << "Invalid.\n"; break;
        }
        press_enter_to_continue();
    }
}

// Student menu
void student_menu(int sid) {
    while (true) {
        cout << "\n--- Student Menu ---\n";
        cout << "1. View my info\n2. Search books\n3. Issue book\n4. Return book\n5. Pay fines (clear fines)\n6. Logout\nChoose: ";
        int c; cin >> c;
        switch (c) {
            case 1: view_student(sid); break;
            case 2: search_books(); break;
            case 3: issue_book(sid); break;
            case 4: return_book(sid); break;
            case 5: {
                Student* s = find_student_by_id(sid);
                if (!s) { cout << "Student not found.\n"; break; }
                cout << "Outstanding fines: " << s->totalFines << ". Pay now? (y/n): ";
                char ch; cin >> ch;
                if (ch=='y' || ch=='Y') { s->totalFines = 0; save_students(); cout << "Fines cleared.\n"; }
                else cout << "Payment cancelled.\n";
                break;
            }
            case 6: cout << "Logging out student.\n"; return;
            default: cout << "Invalid.\n"; break;
        }
        press_enter_to_continue();
    }
}

int main_menu() {
    load_all();
    while (true) {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Admin Login\n2. Student Login\n3. Register Student\n4. Exit\nChoose: ";
        int c; cin >> c;
        if (c == 1) {
            cout << "Enter admin password: ";
            string pass; cin >> pass;
            if (pass == ADMIN_PASSWORD) {
                cout << "Admin authenticated.\n";
                admin_menu();
            } else {
                cout << "Wrong password.\n";
            }
        } else if (c == 2) {
            cout << "Enter Student ID: ";
            int sid; cin >> sid;
            Student* s = find_student_by_id(sid);
            if (!s) {
                cout << "No student found with this ID. Register? (y/n): ";
                char ch; cin >> ch;
                if (ch=='y' || ch=='Y') {
                    register_student();
                }
            } else {
                cout << "Welcome, " << s->name << "!\n";
                student_menu(sid);
            }
        } else if (c == 3) {
            register_student();
        } else if (c == 4) {
            cout << "Exiting. Saving data...\n";
            save_all();
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}
