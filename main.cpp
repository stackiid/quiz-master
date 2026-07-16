#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cstdlib>

// Windows-specific: Sleep() and cls. Compiles fine on any Windows C++ compiler.
#ifdef _WIN32
    #include <windows.h>
    void waitSec(int s)   { Sleep(s * 1000); }
    void clearScreen()    { system("cls"); }
#else
    #include <unistd.h>
    void waitSec(int s)   { sleep(s); }
    void clearScreen()    { system("clear"); }
#endif

using namespace std;

// ================================================================
//  User  -  collects and stores the student's registration info.
//  The certificate pulls from here at the end.
// ================================================================
class User {
public:
    string name, fatherName, education, level, gender;
    string cnic, address, hobby, email;
    int age, yearOrGrade, semester;

    User() : age(0), yearOrGrade(0), semester(0) {}

    void fillDetails() {
        cout << " ================================================\n";
        cout << "          STUDENT REGISTRATION FORM\n";
        cout << "     (details will appear on your certificate)\n";
        cout << " ================================================\n\n";

        cout << "  1. Full Name        : "; getline(cin >> ws, name);
        cout << "  2. Father's Name    : "; getline(cin >> ws, fatherName);
        cout << "  3. Age              : "; cin >> age;
        cout << "  4. Gender (M/F)     : "; cin >> gender;
        cout << "  5. CNIC / Form-B    : "; cin >> cnic;
        cout << "  6. Email Address    : "; cin >> email;
        cout << "  7. Home Address     : "; getline(cin >> ws, address);
        cout << "  8. Favorite Hobby   : "; getline(cin >> ws, hobby);

        // level determines what year/grade/semester fields to ask
        bool validLevel = false;
        while (!validLevel) {
            cout << "  9. Level (School / College / Uni): ";
            cin >> level;
            transform(level.begin(), level.end(), level.begin(), ::tolower);

            if (level == "school") {
                cout << "     Grade (1-10): "; cin >> yearOrGrade;
                validLevel = (yearOrGrade >= 1 && yearOrGrade <= 10);
                if (!validLevel) cout << "     Invalid grade for School.\n";
            }
            else if (level == "college") {
                cout << "     Year (11 or 12): "; cin >> yearOrGrade;
                validLevel = (yearOrGrade == 11 || yearOrGrade == 12);
                if (!validLevel) cout << "     Invalid year for College.\n";
            }
            else if (level == "uni") {
                cout << "     Year (1-4)     : "; cin >> yearOrGrade;
                cout << "     Semester (1-8) : "; cin >> semester;
                validLevel = (yearOrGrade >= 1 && yearOrGrade <= 4 &&
                              semester >= 1    && semester <= 8);
                if (!validLevel) cout << "     Invalid year or semester.\n";
            }
            else {
                cout << "     Enter School, College, or Uni.\n";
            }
        }

        cout << " 10. Status (Undergraduate / Graduated): "; cin >> education;
        clearScreen();
    }
};

// ================================================================
//  Question  -  one quiz entry. Holds the prompt, four options,
//  the correct answer, and whatever the user actually answered.
// ================================================================
struct Question {
    string category;
    string prompt;
    string options[4];   // index 0=A, 1=B, 2=C, 3=D
    char   correct;
    char   userAnswer  = ' ';
    bool   skipped     = false;
    bool   skippedTwice = false;
};

// ================================================================
//  QuizEngine  -  manages the question bank, runs the quiz loop,
//  and handles the two-round skip system.
// ================================================================
class QuizEngine {
private:
    vector<Question> questions;
    int score;

public:
    QuizEngine() : score(0) {}

    void addQuestion(string cat, string prompt,
                     string a, string b, string c, string d, char correct) {
        Question q;
        q.category   = cat;
        q.prompt     = prompt;
        q.options[0] = a;
        q.options[1] = b;
        q.options[2] = c;
        q.options[3] = d;
        q.correct    = toupper(correct);
        questions.push_back(q);
    }

    int totalQuestions() {
        return (int)questions.size();
    }

    // Displays one question and loops until a valid answer is given
    void askQuestion(int idx, bool isRetry) {
        Question &q = questions[idx];
        bool answered = false;

        while (!answered) {
            clearScreen();
            cout << "\n Category : [" << q.category << "]";
            cout << "   |   Question " << idx + 1 << " of " << totalQuestions() << "\n";
            cout << " ------------------------------------------------\n";
            cout << " " << q.prompt << "\n\n";
            cout << "   A)  " << q.options[0] << "\n";
            cout << "   B)  " << q.options[1] << "\n";
            cout << "   C)  " << q.options[2] << "\n";
            cout << "   D)  " << q.options[3] << "\n";
            cout << " ------------------------------------------------\n";

            if (!isRetry)
                cout << " [Enter A / B / C / D   or   S to skip for later]\n";
            else
                cout << " [WARNING: Skipping again gives 0 for this question]\n";

            cout << " Your Answer: ";
            char ans; cin >> ans;
            ans = toupper(ans);

            if (ans == 'S') {
                if (isRetry) q.skippedTwice = true;
                else         q.skipped      = true;
                answered = true;
            }
            else if (ans >= 'A' && ans <= 'D') {
                q.userAnswer = ans;
                if (ans == q.correct) score++;
                answered = true;
            }
            else {
                cout << " Invalid. Enter A, B, C, D, or S.\n";
                waitSec(1);
            }
        }
    }

    // Round 1 = all questions, Round 2 = skipped ones re-attempted
    void startQuiz() {
        vector<int> skippedList;

        for (int i = 0; i < totalQuestions(); i++) {
            askQuestion(i, false);
            if (questions[i].skipped) skippedList.push_back(i);
        }

        if (!skippedList.empty()) {
            clearScreen();
            cout << "\n ===== ROUND 2 — SKIPPED QUESTIONS =====\n";
            cout << " You skipped " << (int)skippedList.size() << " question(s). Attempting now...\n";
            waitSec(2);
            for (int idx : skippedList) {
                askQuestion(idx, true);
            }
        }
    }

    // Compute a letter grade from a percentage
    string getGrade(float percent) {
        if      (percent >= 90) return "A+";
        else if (percent >= 80) return "A";
        else if (percent >= 70) return "B";
        else if (percent >= 60) return "C";
        else if (percent >= 50) return "D  (Pass)";
        else                    return "F  (Fail)";
    }

    void showResults(User &u) {
        clearScreen();
        cout << " Calculating results...\n";
        waitSec(2);

        cout << "\n ================ RESULT SHEET ================\n";
        cout << left
             << setw(13) << " Category"
             << setw(9)  << "Your Ans"
             << setw(9)  << "Correct"
             << "Status\n";
        cout << " -----------------------------------------------\n";

        for (int i = 0; i < totalQuestions(); i++) {
            Question &q = questions[i];
            string status;
            char displayAns;

            if (q.skippedTwice) {
                status     = "SKIPPED";
                displayAns = '-';
            }
            else if (q.userAnswer == q.correct) {
                status     = "Correct";
                displayAns = q.userAnswer;
            }
            else {
                status     = "Wrong";
                displayAns = (q.userAnswer == ' ') ? '-' : q.userAnswer;
            }

            cout << " " << left
                 << setw(13) << q.category.substr(0, 11)
                 << setw(9)  << displayAns
                 << setw(9)  << q.correct
                 << status   << "\n";
        }

        float percent = ((float)score / totalQuestions()) * 100.0f;
        string grade  = getGrade(percent);

        cout << " -----------------------------------------------\n";
        cout << " Score   : " << score << " / " << totalQuestions() << "\n";
        cout << " Average : " << fixed << setprecision(1) << percent << "%\n";
        cout << " Grade   : " << grade << "\n";
        cout << " ===============================================\n\n";

        cout << " Press Enter to generate your certificate...";
        cin.ignore(); cin.get();

        generateCertificate(u, grade);
    }

    void generateCertificate(User &u, string grade) {
        clearScreen();

        // random 4-digit verification code (seeded in main)
        string code = "QM-" + to_string(1000 + rand() % 9000);

        cout << "\n";
        cout << " *****************************************************\n";
        cout << "           QUIZ COMPLETION CERTIFICATE             \n";
        cout << " *****************************************************\n";
        cout << "                                                    \n";
        cout << "   Name      : " << u.name                          << "\n";
        cout << "   Father    : " << u.fatherName                    << "\n";
        cout << "   Age / Gender : " << u.age << " / " << u.gender   << "\n";
        cout << "   CNIC      : " << u.cnic                          << "\n";
        cout << "   Level     : " << u.level;
        if (u.level == "uni")
            cout << "  (Year " << u.yearOrGrade << ", Sem " << u.semester << ")";
        else
            cout << "  (Year/Grade " << u.yearOrGrade << ")";
        cout << "\n";
        cout << "   Status    : " << u.education                     << "\n";
        cout << "                                                    \n";
        cout << "   This confirms the above candidate has completed  \n";
        cout << "   the " << totalQuestions() << "-Question Knowledge Quiz.";
        cout << "                    \n";
        cout << "                                                    \n";
        cout << "   Score  : " << score << " / " << totalQuestions();
        cout << "   |   Grade : " << grade                            << "\n";
        cout << "                                                    \n";
        cout << "   Verify : " << code                               << "\n";
        cout << "                                                    \n";
        cout << " *****************************************************\n\n";
    }
};

// ================================================================
//                              main
// ================================================================
int main() {
    srand((unsigned int)time(0));
    clearScreen();

    cout << "\n ####################################################\n";
    cout << " #          QUIZ MASTER  -  30 Questions            #\n";
    cout << " #       Test your knowledge across 7 topics        #\n";
    cout << " ####################################################\n\n";
    waitSec(2);

    cout << " Ready to begin? Type  yes  to continue: ";
    string confirm; cin >> confirm;

    if (confirm != "yes") {
        cout << "\n Come back when you're ready. See you next time!\n\n";
        return 0;
    }

    clearScreen();
    User student;
    student.fillDetails();

    QuizEngine quiz;

    // ---- General Knowledge (5) ----
    quiz.addQuestion("GK",
        "Which is the largest ocean?",
        "Atlantic", "Indian", "Pacific", "Arctic", 'C');

    quiz.addQuestion("GK",
        "What is the official currency of Japan?",
        "Yuan", "Yen", "Won", "Dollar", 'B');

    quiz.addQuestion("GK",
        "Which is the tallest mountain on Earth?",
        "K2", "Mount Everest", "Lhotse", "Makalu", 'B');

    quiz.addQuestion("GK",
        "What is the fastest land animal?",
        "Lion", "Cheetah", "Leopard", "Falcon", 'B');

    quiz.addQuestion("GK",
        "Which is the smallest country by area?",
        "Monaco", "Malta", "Vatican City", "San Marino", 'C');

    // ---- AI / Machine Learning (5) ----
    quiz.addQuestion("AI/ML",
        "What does the abbreviation AI stand for?",
        "Auto Intel", "Artificial Intelligence", "Array Info", "Active Intel", 'B');

    quiz.addQuestion("AI/ML",
        "Who is widely considered the Father of AI?",
        "Alan Turing", "Elon Musk", "John McCarthy", "Bill Gates", 'C');

    quiz.addQuestion("AI/ML",
        "Machine Learning is a subset of which field?",
        "Networking", "Artificial Intelligence", "Cloud Computing", "Hardware", 'B');

    quiz.addQuestion("AI/ML",
        "NLP stands for?",
        "Natural Language Processing", "Node Link Protocol", "New Loop", "None of these", 'A');

    quiz.addQuestion("AI/ML",
        "Which language is most widely used for AI development?",
        "HTML", "CSS", "Python", "SQL", 'C');

    // ---- Islamic Studies (5) ----
    quiz.addQuestion("Islam",
        "Who was the first Prophet of Allah?",
        "Adam (A.S)", "Nuh (A.S)", "Isa (A.S)", "Musa (A.S)", 'A');

    quiz.addQuestion("Islam",
        "How many Surahs are in the Holy Quran?",
        "110", "112", "114", "116", 'C');

    quiz.addQuestion("Islam",
        "What is the meaning of the word 'Islam'?",
        "Power", "Peace and Submission", "Strength", "Life", 'B');

    quiz.addQuestion("Islam",
        "Who was the first Caliph of Islam?",
        "Hazrat Abu Bakr (R.A)", "Hazrat Umar (R.A)", "Hazrat Usman (R.A)", "Hazrat Ali (R.A)", 'A');

    quiz.addQuestion("Islam",
        "Zakat is which pillar of Islam?",
        "1st", "2nd", "3rd", "4th", 'C');

    // ---- Pakistan Studies (5) ----
    quiz.addQuestion("Pak Study",
        "Who is the founder of Pakistan?",
        "Allama Iqbal", "Quaid-e-Azam Jinnah", "Liaquat Ali Khan", "Sir Syed Ahmed", 'B');

    quiz.addQuestion("Pak Study",
        "In which year did Pakistan gain independence?",
        "1940", "1947", "1948", "1950", 'B');

    quiz.addQuestion("Pak Study",
        "What is the national flower of Pakistan?",
        "Rose", "Jasmine", "Lily", "Tulip", 'B');

    quiz.addQuestion("Pak Study",
        "Which is the highest peak located in Pakistan?",
        "K2", "Nanga Parbat", "Rakaposhi", "Tirich Mir", 'A');

    quiz.addQuestion("Pak Study",
        "Which is the largest province of Pakistan by area?",
        "Sindh", "Punjab", "Balochistan", "Khyber Pakhtunkhwa", 'C');

    // ---- Science (6) ----
    quiz.addQuestion("Science",
        "What is the chemical formula of water?",
        "CO2", "H2O2", "H2O", "NaCl", 'C');

    quiz.addQuestion("Science",
        "How many laws of motion did Newton formulate?",
        "2", "3", "4", "5", 'B');

    quiz.addQuestion("Science",
        "Which organelle is called the powerhouse of the cell?",
        "Nucleus", "Ribosome", "Mitochondria", "Cell Wall", 'C');

    quiz.addQuestion("Science",
        "What is the chemical symbol for Gold?",
        "Ag", "Au", "Fe", "Cu", 'B');

    quiz.addQuestion("Science",
        "What is the approximate speed of light?",
        "150,000 km/s", "300,000 km/s", "100,000 km/s", "500,000 km/s", 'B');

    quiz.addQuestion("Science",
        "What is the pH value of pure water?",
        "5", "6", "7", "8", 'C');

    // ---- English (2) ----
    quiz.addQuestion("English",
        "Which of the following is a synonym of 'Fast'?",
        "Slow", "Quick", "Lazy", "Hard", 'B');

    quiz.addQuestion("English",
        "Which of the following letters is a vowel?",
        "B", "C", "E", "F", 'C');

    // ---- Mathematics (2) ----
    quiz.addQuestion("Math",
        "What is the square root of 64?",
        "6", "7", "8", "9", 'C');

    quiz.addQuestion("Math",
        "A 90-degree angle is called a?",
        "Acute angle", "Obtuse angle", "Right angle", "Reflex angle", 'C');

    quiz.startQuiz();
    quiz.showResults(student);

    return 0;
}
