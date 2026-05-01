#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class Student {
public:
    int rollNo;
    char name[50];
    char course[50];

    void input() {
        cout << "\nEnter Roll Number: ";
        cin >> rollNo;
        cin.ignore(); // Clear buffer
        cout << "Enter Name: ";
        cin.getline(name, 50);
        cout << "Enter Course: ";
        cin.getline(course, 50);
    }

    void display() {
        cout << left << setw(10) << rollNo << setw(20) << name << setw(20) << course << endl;
    }
};

// Function prototypes
void addStudent();
void displayAll();
void updateStudent();
void deleteStudent();

const string FILE_NAME = "students.dat";

int main() {
    int choice;

    while (true) {
        cout << "\n--- STUDENT MANAGEMENT SYSTEM ---";
        cout << "\n1. Add Student Record";
        cout << "\n2. Display All Records";
        cout << "\n3. Update Student Record";
        cout << "\n4. Delete Student Record";
        cout << "\n5. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: updateStudent(); break;
            case 4: deleteStudent(); break;
            case 5: exit(0);
            default: cout << "\nInvalid Choice!";
        }
    }
    return 0;
}

void addStudent() {
    Student s;
    ofstream outFile(FILE_NAME, ios::binary | ios::app);
    s.input();
    outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
    outFile.close();
    cout << "\nRecord added successfully!";
}

void displayAll() {
    Student s;
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened or is empty.";
        return;
    }
    cout << "\n" << left << setw(10) << "Roll No" << setw(20) << "Name" << setw(20) << "Course" << endl;
    cout << "---------------------------------------------------" << endl;
    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        s.display();
    }
    inFile.close();
}

void updateStudent() {
    int r;
    bool found = false;
    cout << "\nEnter Roll Number to update: ";
    cin >> r;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    Student s;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo == r) {
            cout << "\nEnter New Details:";
            s.input();
            int pos = -1 * static_cast<int>(sizeof(Student));
            file.seekp(pos, ios::cur); // Move pointer back to overwrite
            file.write(reinterpret_cast<char*>(&s), sizeof(Student));
            found = true;
            break;
        }
    }
    file.close();
    if (found) cout << "\nRecord updated!";
    else cout << "\nRecord not found!";
}

void deleteStudent() {
    int r;
    cout << "\nEnter Roll Number to delete: ";
    cin >> r;

    ifstream inFile(FILE_NAME, ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    Student s;
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        if (s.rollNo != r) {
            outFile.write(reinterpret_cast<char*>(&s), sizeof(Student));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();
    remove(FILE_NAME.c_str());
    rename("temp.dat", FILE_NAME.c_str());

    if (found) cout << "\nRecord deleted!";
    else cout << "\nRecord not found!";
}
