# Quiz Master

![Language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![Interface](https://img.shields.io/badge/interface-console-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)

A console-based, 30-question multiple-choice quiz written in C++ as a Semester 2 project. The program registers a student, runs an untimed quiz across seven subject areas with a two-round skip system, prints a result sheet with a letter grade, and finishes by generating a text-based completion certificate.

## Table of Contents

- [Features](#features)
- [Tech Stack](#tech-stack)
- [How It Works](#how-it-works)
- [Question Bank](#question-bank)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Scoring and Grades](#scoring-and-grades)
- [Known Limitations](#known-limitations)
- [License](#license)

## Features

- Student registration form with 10 fields (name, father's name, age, gender, CNIC / Form-B, email, address, hobby, level, status)
- Level-aware validation: School (grade 1-10), College (year 11 or 12), or Uni (year 1-4 and semester 1-8)
- 30 multiple-choice questions with four options each, grouped into seven categories
- Screen is cleared between questions, and each question shows its category and position (for example, "Question 3 of 30")
- Input validation for answers: only `A`, `B`, `C`, `D`, or `S` (case-insensitive) is accepted
- Two-round skip system: questions skipped with `S` are asked again after the first pass, and skipping a second time scores zero for that question
- Result sheet listing the category, the user's answer, the correct answer, and a status for every question
- Score, percentage, and letter grade calculation
- Quiz completion certificate that includes the student's details, score, grade, and a randomly generated verification code

## Tech Stack

| Category | Details |
| --- | --- |
| Language | C++ |
| Standard library | `<iostream>`, `<string>`, `<vector>`, `<iomanip>`, `<algorithm>`, `<ctime>`, `<cstdlib>` |
| Platform headers | `<windows.h>` on Windows, `<unistd.h>` on other systems |
| Compiler | Any C++11 (or later) compiler such as g++ |

The program has no external dependencies.

## How It Works

All logic lives in `main.cpp` and is organized into three parts.

| Component | Type | Responsibility |
| --- | --- | --- |
| `User` | class | Collects and stores the registration details through `fillDetails()` and validates the level, grade, year, and semester combination |
| `Question` | struct | Holds the category, prompt, four options, the correct answer, the user's answer, and skip flags |
| `QuizEngine` | class | Stores the question bank, runs the quiz, handles the skip rounds, calculates the grade, prints the result sheet, and generates the certificate |

Two small helper functions, `waitSec()` and `clearScreen()`, are defined with a `_WIN32` preprocessor check. On Windows they use `Sleep()` and `cls`; on other systems they use `sleep()` and `clear`.

### Quiz Flow

1. The program shows a banner and asks the user to type `yes` to continue. Any other input exits the program.
2. The student registration form is displayed and the details are stored in a `User` object.
3. The 30 questions are added to the `QuizEngine`.
4. **Round 1:** every question is asked in order. The user answers with `A`-`D` or skips with `S`.
5. **Round 2:** if any questions were skipped, they are asked again with a warning that skipping again gives 0 for that question.
6. The result sheet is displayed with the score, percentage, and grade.
7. After pressing Enter, the completion certificate is printed.

## Question Bank

Questions are defined directly in `main()` through `addQuestion(category, prompt, optionA, optionB, optionC, optionD, correctLetter)`.

| Category label | Subject | Questions |
| --- | --- | --- |
| `GK` | General Knowledge | 5 |
| `AI/ML` | Artificial Intelligence and Machine Learning | 5 |
| `Islam` | Islamic Studies | 5 |
| `Pak Study` | Pakistan Studies | 5 |
| `Science` | Science | 6 |
| `English` | English | 2 |
| `Math` | Mathematics | 2 |
| | **Total** | **30** |

To add or change a question, edit the corresponding `quiz.addQuestion(...)` call in `main()` and recompile. The banner text ("30 Questions", "7 topics") is hardcoded, so update it if the question count or category count changes.

## Project Structure

```text
quiz-master/
|-- main.cpp     # Complete application: User, Question, QuizEngine, and main()
|-- LICENSE      # MIT License
`-- README.md
```

## Prerequisites

- A C++ compiler such as g++ (for example, MinGW on Windows)
- A terminal that supports the `cls` command (Windows) or the `clear` command (Linux and macOS)

## Getting Started

Clone the repository and move into it:

```bash
git clone https://github.com/stackiid/quiz-master.git
cd quiz-master
```

Compile the program:

```bash
g++ -Wall main.cpp -o quiz-master
```

Run it:

```bash
# Windows PowerShell
.\quiz-master.exe

# Windows Command Prompt
quiz-master

# Linux and macOS
./quiz-master
```

## Usage

1. Type `yes` at the start screen (lowercase, exactly as shown) and press Enter.
2. Fill in the registration form. For the level prompt, enter `School`, `College`, or `Uni` (case-insensitive), then provide the requested grade, year, or semester.
3. Answer each question by typing `A`, `B`, `C`, or `D` and pressing Enter. Type `S` to skip a question and return to it later.
4. After the skipped questions are re-attempted, review the result sheet and press Enter to generate the certificate.

Example question screen:

```text
 Category : [GK]   |   Question 1 of 30
 ------------------------------------------------
 Which is the largest ocean?

   A)  Atlantic
   B)  Indian
   C)  Pacific
   D)  Arctic
 ------------------------------------------------
 [Enter A / B / C / D   or   S to skip for later]
 Your Answer:
```

Example result sheet layout:

```text
 ================ RESULT SHEET ================
 Category    Your Ans Correct  Status
 -----------------------------------------------
 GK          C        C        Correct
 GK          A        B        Wrong
 ...
 -----------------------------------------------
 Score   : <score> / 30
 Average : <percentage>%
 Grade   : <grade>
 ===============================================
```

The certificate lists the student's name, father's name, age and gender, CNIC, level (with year, grade, or semester), and status, followed by the score, grade, and a verification code in the format `QM-XXXX`.

## Scoring and Grades

Each correct answer adds one point. Skipped questions that are skipped again in round 2 earn no points and are marked `SKIPPED` on the result sheet. The percentage is `score / 30 * 100`.

| Percentage | Grade |
| --- | --- |
| 90 and above | A+ |
| 80 to below 90 | A |
| 70 to below 80 | B |
| 60 to below 70 | C |
| 50 to below 60 | D (Pass) |
| Below 50 | F (Fail) |

## Known Limitations

These points describe the current behavior of the code:

- Results and registration data are not saved; everything exists only for the duration of the run.
- The verification code on the certificate is a random 4-digit number generated at the end of the quiz. It is not stored or checked against anything.
- The address, email, and hobby fields are collected in the registration form but are not printed on the certificate.
- The age, grade, year, and semester prompts do not handle non-numeric input.
- Answers are read one character at a time, so typing more than one character at an answer prompt is not treated as a single invalid answer.
- Questions are always asked in the same fixed order, and the answer options are not shuffled.

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.

Copyright (c) 2026 Ubaid Ahmad
