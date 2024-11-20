#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// ��renci s�n�f�
class Student {
public:
    string id;
    string firstName;
    string lastName;
    int vize;
    int final;
    int odev;
    double vizeWeight;
    double finalWeight;
    double odevWeight;
    bool hasHomework;
    bool homeworkAffectsGrades;

    // ��renci bilgilerini dosyaya kaydetme fonksiyonu
    void save(ofstream& file) const {
        file << id << " " << firstName << " " << lastName << " " << vize << " " << final << " ";
        if (hasHomework) {
            file << odev << " ";
        }
        else {
            file << "-1 ";
        }
        file << vizeWeight << " " << finalWeight << " " << (hasHomework ? odevWeight : 0) << " " << homeworkAffectsGrades << endl;
    }

    // Ortalama hesaplama fonksiyonu
    double calculateAverage() const {
        double weightedVize = vize * (vizeWeight / 100);
        double weightedFinal = final * (finalWeight / 100);
        double weightedOdev = (hasHomework && homeworkAffectsGrades) ? odev * (odevWeight / 100) : 0;
        return weightedVize + weightedFinal + weightedOdev;
    }

    // Harf notu atanma fonksiyonu
    string getLetterGrade(double average) const {
        if (average >= 90) return "AA";
        if (average >= 85) return "BA";
        if (average >= 80) return "BB";
        if (average >= 75) return "CB";
        if (average >= 65) return "CC";
        if (average >= 58) return "DC";
        if (average >= 50) return "DD";
        return "FF";
    }
};

// ��rencileri dosyaya kaydetme
void saveStudentsToFile(const vector<Student>& students, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Dosya a��lamad�: " << filename << endl;
        return;
    }
    for (const Student& student : students) {
        student.save(file);
    }
}

// Dosyadan ��renci verilerini y�kleme
void loadStudentsFromFile(vector<Student>& students, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Dosya a��lamad� veya mevcut de�il: " << filename << endl;
        return;
    }

    Student student;
    int odevValue;
    bool affectsGrades;

    // Dosyadan ��renci bilgilerini sat�r sat�r okuma
    while (file >> student.id >> student.firstName >> student.lastName >> student.vize >> student.final >> odevValue
        >> student.vizeWeight >> student.finalWeight >> student.odevWeight >> affectsGrades) {
        student.hasHomework = odevValue != -1;
        student.homeworkAffectsGrades = affectsGrades;
        if (student.hasHomework) {
            student.odev = odevValue;
        }
        students.push_back(student);
    }
}

// ��renci ekleme
void addStudent(vector<Student>& students) {
    Student student;
    cout << "��renci Numaras�: ";
    cin >> student.id;
    cout << "��renci Ad�: ";
    cin >> student.firstName;
    cout << "��renci Soyad�: ";
    cin >> student.lastName;

    // �dev durumu sorusu
    char hasHomeworkInput;
    cout << "�dev verdiniz mi? (E/e i�in Evet, di�erleri i�in Hay�r): ";
    cin >> hasHomeworkInput;
    student.hasHomework = (hasHomeworkInput == 'E' || hasHomeworkInput == 'e');

    // �devin s�nav notlar�na etkisi soruluyor
    if (student.hasHomework) {
        char affectsGradesInput;
        cout << "�dev s�nav notlar�na etkili mi? (E/e i�in Evet, di�erleri i�in Hay�r): ";
        cin >> affectsGradesInput;
        student.homeworkAffectsGrades = (affectsGradesInput == 'E' || affectsGradesInput == 'e');

        if (student.homeworkAffectsGrades) {
            // �devin etkisi varsa, ne kadar etkileyce�i soruluyor
            cout << "�devin s�nav notlar�na etkisi (% olarak): ";
            cin >> student.odevWeight;
        }
        else {
            student.odevWeight = 0; // �dev etkili de�ilse a��rl��� s�f�rlan�r
        }
    }
    else {
        student.odevWeight = 0; // �dev yoksa, a��rl�k s�f�r
    }

    // Vize ve final notu a��rl�klar�
    cout << "Vize A��rl��� (%) girin: ";
    cin >> student.vizeWeight;
    cout << "Final A��rl��� (%) girin: ";
    cin >> student.finalWeight;

    // Vize ve final a��rl�klar�n�n toplam�n�n %100 olup olmad���n� kontrol et
    if (student.vizeWeight + student.finalWeight + student.odevWeight != 100) {
        cout << "Hata: A��rl�klar toplam� %100 olmal�! Bu ��renci eklenmedi.\n";
        return;
    }

    students.push_back(student);
    cout << "��renci eklendi!" << endl;
}

// ��renci arama ve g�sterme
void findStudent(const vector<Student>& students) {
    string id;
    cout << "Aranacak ��renci numaras�: ";
    cin >> id;

    for (const Student& student : students) {
        if (student.id == id) {
            double average = student.calculateAverage();
            string letterGrade = student.getLetterGrade(average);
            cout << "ID: " << student.id << ", Ad�: " << student.firstName << " " << student.lastName << endl;
            cout << "Vize Notu: " << student.vize << " (%" << student.vizeWeight << "), Final Notu: " << student.final << " (%" << student.finalWeight << ")";
            if (student.hasHomework) {
                cout << ", �dev Notu: " << student.odev << " (%" << student.odevWeight << ")";
            }
            cout << endl;
            cout << "Ortalama: " << fixed << setprecision(2) << average << ", Harf Notu: " << letterGrade << endl;
            return;
        }
    }
    cout << "��renci bulunamad�." << endl;
}

// T�m ��rencileri listeleme
void listAllStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "Kay�tl� ��renci bulunmamaktad�r." << endl;
        return;
    }
    cout << "T�m ��renciler:" << endl;
    for (const Student& student : students) {
        double average = student.calculateAverage();
        string letterGrade = student.getLetterGrade(average);
        cout << "ID: " << student.id << ", Ad�: " << student.firstName << " " << student.lastName << endl;
        cout << "Vize Notu: " << student.vize << " (%" << student.vizeWeight << "), Final Notu: " << student.final << " (%" << student.finalWeight << ")";
        if (student.hasHomework) {
            cout << ", �dev Notu: " << student.odev << " (%" << student.odevWeight << ")";
        }
        cout << endl;
        cout << "Ortalama: " << fixed << setprecision(2) << average << ", Harf Notu: " << letterGrade << endl;
    }
}

// ��renci silme
void deleteStudent(vector<Student>& students) {
    string id;
    cout << "Silinecek ��renci numaras�: ";
    cin >> id;

    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->id == id) {
            students.erase(it);
            cout << "��renci silindi." << endl;
            return;
        }
    }
    cout << "��renci bulunamad�." << endl;
}

// Ana men�
void menu() {
    vector<Student> students;
    loadStudentsFromFile(students, "students.txt");

    while (true) {
        cout << "\n--- ��renci Y�netim Sistemi ---\n";
        cout << "1. ��renci Ekle\n";
        cout << "2. ��renci Bul\n";
        cout << "3. T�m ��rencileri Listele\n";
        cout << "4. ��renci Sil\n";
        cout << "5. ��k ve Kaydet\n";
        cout << "6. ��k��\n";
        cout << "Se�iminiz: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            addStudent(students);
            break;
        case 2:
            findStudent(students);
            break;
        case 3:
            listAllStudents(students);
            break;
        case 4:
            deleteStudent(students);
            break;
        case 5:
            saveStudentsToFile(students, "students.txt");
            cout << "Veriler kaydedildi. ��k�l�yor..." << endl;
            return;
        case 6:
            cout << "��k�l�yor..." << endl;
            return;
        default:
            cout << "Ge�ersiz se�im. Tekrar deneyin." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    menu();
    return 0;
}
