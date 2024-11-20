#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Öðrenci sýnýfý
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

    // Öðrenci bilgilerini dosyaya kaydetme fonksiyonu
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

// Öðrencileri dosyaya kaydetme
void saveStudentsToFile(const vector<Student>& students, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Dosya açýlamadý: " << filename << endl;
        return;
    }
    for (const Student& student : students) {
        student.save(file);
    }
}

// Dosyadan öðrenci verilerini yükleme
void loadStudentsFromFile(vector<Student>& students, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Dosya açýlamadý veya mevcut deðil: " << filename << endl;
        return;
    }

    Student student;
    int odevValue;
    bool affectsGrades;

    // Dosyadan öðrenci bilgilerini satýr satýr okuma
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

// Öðrenci ekleme
void addStudent(vector<Student>& students) {
    Student student;
    cout << "Öðrenci Numarasý: ";
    cin >> student.id;
    cout << "Öðrenci Adý: ";
    cin >> student.firstName;
    cout << "Öðrenci Soyadý: ";
    cin >> student.lastName;

    // Ödev durumu sorusu
    char hasHomeworkInput;
    cout << "Ödev verdiniz mi? (E/e için Evet, diðerleri için Hayýr): ";
    cin >> hasHomeworkInput;
    student.hasHomework = (hasHomeworkInput == 'E' || hasHomeworkInput == 'e');

    // Ödevin sýnav notlarýna etkisi soruluyor
    if (student.hasHomework) {
        char affectsGradesInput;
        cout << "Ödev sýnav notlarýna etkili mi? (E/e için Evet, diðerleri için Hayýr): ";
        cin >> affectsGradesInput;
        student.homeworkAffectsGrades = (affectsGradesInput == 'E' || affectsGradesInput == 'e');

        if (student.homeworkAffectsGrades) {
            // Ödevin etkisi varsa, ne kadar etkileyceði soruluyor
            cout << "Ödevin sýnav notlarýna etkisi (% olarak): ";
            cin >> student.odevWeight;
        }
        else {
            student.odevWeight = 0; // Ödev etkili deðilse aðýrlýðý sýfýrlanýr
        }
    }
    else {
        student.odevWeight = 0; // Ödev yoksa, aðýrlýk sýfýr
    }

    // Vize ve final notu aðýrlýklarý
    cout << "Vize Aðýrlýðý (%) girin: ";
    cin >> student.vizeWeight;
    cout << "Final Aðýrlýðý (%) girin: ";
    cin >> student.finalWeight;

    // Vize ve final aðýrlýklarýnýn toplamýnýn %100 olup olmadýðýný kontrol et
    if (student.vizeWeight + student.finalWeight + student.odevWeight != 100) {
        cout << "Hata: Aðýrlýklar toplamý %100 olmalý! Bu öðrenci eklenmedi.\n";
        return;
    }

    students.push_back(student);
    cout << "Öðrenci eklendi!" << endl;
}

// Öðrenci arama ve gösterme
void findStudent(const vector<Student>& students) {
    string id;
    cout << "Aranacak öðrenci numarasý: ";
    cin >> id;

    for (const Student& student : students) {
        if (student.id == id) {
            double average = student.calculateAverage();
            string letterGrade = student.getLetterGrade(average);
            cout << "ID: " << student.id << ", Adý: " << student.firstName << " " << student.lastName << endl;
            cout << "Vize Notu: " << student.vize << " (%" << student.vizeWeight << "), Final Notu: " << student.final << " (%" << student.finalWeight << ")";
            if (student.hasHomework) {
                cout << ", Ödev Notu: " << student.odev << " (%" << student.odevWeight << ")";
            }
            cout << endl;
            cout << "Ortalama: " << fixed << setprecision(2) << average << ", Harf Notu: " << letterGrade << endl;
            return;
        }
    }
    cout << "Öðrenci bulunamadý." << endl;
}

// Tüm öðrencileri listeleme
void listAllStudents(const vector<Student>& students) {
    if (students.empty()) {
        cout << "Kayýtlý öðrenci bulunmamaktadýr." << endl;
        return;
    }
    cout << "Tüm Öðrenciler:" << endl;
    for (const Student& student : students) {
        double average = student.calculateAverage();
        string letterGrade = student.getLetterGrade(average);
        cout << "ID: " << student.id << ", Adý: " << student.firstName << " " << student.lastName << endl;
        cout << "Vize Notu: " << student.vize << " (%" << student.vizeWeight << "), Final Notu: " << student.final << " (%" << student.finalWeight << ")";
        if (student.hasHomework) {
            cout << ", Ödev Notu: " << student.odev << " (%" << student.odevWeight << ")";
        }
        cout << endl;
        cout << "Ortalama: " << fixed << setprecision(2) << average << ", Harf Notu: " << letterGrade << endl;
    }
}

// Öðrenci silme
void deleteStudent(vector<Student>& students) {
    string id;
    cout << "Silinecek öðrenci numarasý: ";
    cin >> id;

    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->id == id) {
            students.erase(it);
            cout << "Öðrenci silindi." << endl;
            return;
        }
    }
    cout << "Öðrenci bulunamadý." << endl;
}

// Ana menü
void menu() {
    vector<Student> students;
    loadStudentsFromFile(students, "students.txt");

    while (true) {
        cout << "\n--- Öðrenci Yönetim Sistemi ---\n";
        cout << "1. Öðrenci Ekle\n";
        cout << "2. Öðrenci Bul\n";
        cout << "3. Tüm Öðrencileri Listele\n";
        cout << "4. Öðrenci Sil\n";
        cout << "5. Çýk ve Kaydet\n";
        cout << "6. Çýkýþ\n";
        cout << "Seçiminiz: ";

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
            cout << "Veriler kaydedildi. Çýkýlýyor..." << endl;
            return;
        case 6:
            cout << "Çýkýlýyor..." << endl;
            return;
        default:
            cout << "Geçersiz seçim. Tekrar deneyin." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    menu();
    return 0;
}
