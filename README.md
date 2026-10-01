# Quiz Master

![Language](https://img.shields.io/badge/language-C%2B%2B11-blue)
![License](https://img.shields.io/badge/license-MIT-green)

Quiz Master is a single-file, console-based quiz application written in C++. It registers a student, runs a fixed 30-question multiple-choice quiz across 7 categories, offers a second round for skipped questions, prints a result sheet with a letter grade, and finishes with a text-based completion certificate.

The project was built as a Semester 2 assignment to apply object-oriented programming concepts (classes, structs, encapsulation) in a complete interactive program.

## Features

- Student registration form with 10 prompts, including a level-dependent validation step for School, College, or University
- 30 hard-coded multiple-choice questions (A to D) across 7 categories
- Two-round skip system: skip a question with `S` in round 1 and attempt it again in round 2
- Input validation for quiz answers and for the education level, grade, year, and semester ranges
- Result sheet listing the category, the user's answer, the correct answer, and a status for every question
- Score, percentage, and letter grade calculation
- Completion certificate showing the student's details, score, grade, and a randomly generated `QM-XXXX` code
- Platform-aware screen clearing and delays for Windows and non-Windows systems

## Question Bank

| Category (as shown in the program) | Questions |
| --- | --- |
| GK | 5 |
| AI/ML | 5 |
| Islam | 5 |
| Pak Study | 5 |
| Science | 6 |
| English | 2 |
| Math | 2 |
| **Total** | **30** |

Questions are defined directly in `main()` with calls to `QuizEngine::addQuestion()`. The order is fixed and is not shuffled.

## Tech Stack

- Language: C++ (C++11 or later, due to features such as `to_string`, range-based `for`, and default member initializers)
- Standard library headers: `iostream`, `string`, `vector`, `iomanip`, `algorithm`, `ctime`, `cstdlib`
- Platform headers: `windows.h` on Windows, `unistd.h` on other systems
- IDE project file: Code::Blocks (GCC compiler, Debug and Release targets)

No third-party libraries, databases, or network services are used.

## Project Structure

Files tracked in the repository (the `.gitignore` uses an allow-list):

```text
.
|-- main.cpp      # All classes, helpers, the question bank, and main()
|-- README.md
|-- LICENSE
`-- .gitignore
```

The original project folder also contains local IDE and build artifacts that are intentionally ignored by Git: `Semester-02_Quiz-Master.cbp`, `Semester-02_Quiz-Master.layout`, `main.o`, and `main.exe`.

### Code organization in `main.cpp`

1. Platform helpers: `waitSec()` and `clearScreen()`, selected with `#ifdef _WIN32`
2. `User` class: collects and stores registration details
3. `Question` struct: category, prompt, four options, correct answer, the user's answer, and skip flags
4. `QuizEngine` class: question storage, quiz flow, scoring, grading, result sheet, and certificate
5. `main()`: seeds the random generator, shows the welcome screen, registers the student, loads the questions, and runs the quiz

## Prerequisites

- A C++ compiler with C++11 support (for example `g++` or MinGW)
- A terminal

## Getting Started

From the directory containing `main.cpp`:

```bash
g++ -std=c++11 main.cpp -o quizmaster
```

Run on Linux or macOS:

```bash
./quizmaster
```

Run on Windows:

```bash
quizmaster.exe
```

### Using Code::Blocks

If the local `Semester-02_Quiz-Master.cbp` file is present, open it in Code::Blocks and build and run the Debug or Release target. Both targets use the GCC compiler. Because this file is excluded by `.gitignore`, a fresh clone will not contain it; in that case create a new console project and add `main.cpp`.

## Usage

1. At the welcome screen, type `yes` (lowercase) to continue. Any other input exits the program.
2. Complete the registration form:

   | # | Field | Notes |
   | --- | --- | --- |
   | 1 | Full Name | Free text |
   | 2 | Father's Name | Free text |
   | 3 | Age | Integer |
   | 4 | Gender (M/F) | Single word, not validated |
   | 5 | CNIC / Form-B | Single word, no spaces |
   | 6 | Email Address | Single word, not validated |
   | 7 | Home Address | Free text |
   | 8 | Favorite Hobby | Free text |
   | 9 | Level | `School`, `College`, or `Uni` (case-insensitive) |
   | 10 | Status | `Undergraduate` or `Graduated` suggested by the prompt, stored as a single word |

   The level determines the follow-up validation:

   | Level | Required input | Accepted range |
   | --- | --- | --- |
   | School | Grade | 1 to 10 |
   | College | Year | 11 or 12 |
   | Uni | Year and Semester | Year 1 to 4, Semester 1 to 8 |

3. Answer each question with `A`, `B`, `C`, or `D`, or enter `S` to skip it for later. Input is case-insensitive. Invalid input re-displays the question after a short delay.
4. If any questions were skipped, round 2 presents them again with a warning. Skipping a second time awards no points and the question is shown as `SKIPPED` in the results.
5. After the quiz, the result sheet is displayed. Press Enter to generate the certificate.

### Grading

The percentage is `score / total questions * 100`. Skipped and wrong answers both count as zero.

| Percentage | Grade |
| --- | --- |
| 90 and above | A+ |
| 80 to below 90 | A |
| 70 to below 80 | B |
| 60 to below 70 | C |
| 50 to below 60 | D (Pass) |
| Below 50 | F (Fail) |

### Example output

Result sheet (excerpt):

```text
 ================ RESULT SHEET ================
 Category    Your Ans Correct  Status
 -----------------------------------------------
 GK           B        C        Wrong
 GK           B        B        Correct
 ...
 -----------------------------------------------
 Score   : 15 / 30
 Average : 50.0%
 Grade   : D  (Pass)
 ===============================================
```

Certificate:

```text
 *****************************************************
           QUIZ COMPLETION CERTIFICATE
 *****************************************************

   Name      : Ali Khan
   Father    : Khan Sr
   Age / Gender : 20 / M
   CNIC      : 12345-1234567-1
   Level     : uni  (Year 2, Sem 4)
   Status    : Graduated

   This confirms the above candidate has completed
   the 30-Question Knowledge Quiz.

   Score  : 15 / 30   |   Grade : D  (Pass)

   Verify : QM-6376

 *****************************************************
```

The certificate displays name, father's name, age, gender, CNIC, level, and status. Email address, home address, and hobby are collected during registration but are not printed on the certificate.

## Implementation Notes

- `User` stores the registration data and runs the registration form in `fillDetails()`.
- `QuizEngine` keeps the question list and score private and exposes `addQuestion()`, `startQuiz()`, and `showResults()`.
- `startQuiz()` asks every question once, collects the indexes of skipped questions, and re-asks only those in round 2.
- The verification code on the certificate is `"QM-"` followed by a pseudo-random number from 1000 to 9999 (`rand()` seeded with `time(0)`). It is generated for display only and is not checked or stored anywhere.
- Screen clearing uses `system("cls")` on Windows and `system("clear")` elsewhere.

## Limitations

- The question bank is hard-coded and cannot be changed without editing and recompiling the source.
- Results and certificates are printed to the console only. Nothing is saved to disk.
- Age, grade, year, and semester are read as integers without checking for stream errors, so non-numeric input in those fields is not handled.
- Gender, CNIC, email, and status are not validated.

## License

This project is licensed under the MIT License. See [LICENSE](./LICENSE) for details.
