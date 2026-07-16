# Quiz Master

> A 30-question terminal quiz game with OOP design, student registration, a two-round skip system, and a certificate generator - built in C++ as a Semester 2 project.

---

## 📋 Overview

This project was built in Semester 2, after being introduced to Object-Oriented Programming. The goal was to apply OOP concepts - classes, objects, encapsulation - to something that felt like a complete application rather than just a textbook exercise.

The program walks a student through registration, runs a 30-question quiz across 7 categories, handles skipped questions in a second round, and ends by printing a result sheet and a personalized certificate.

---

## ✅ Features

- **Student registration** - collects personal details (name, CNIC, education level) used on the certificate
- **30 questions across 7 categories** - General Knowledge, AI/ML, Islamic Studies, Pakistan Studies, Science, English, and Mathematics
- **Two-round skip system** - skip a question in round 1 to attempt it later; skipping again in round 2 marks it as 0
- **Result sheet** - shows every question, the user's answer, the correct answer, and the status (Correct / Wrong / Skipped)
- **Grade calculation** - A+ to F based on percentage
- **Certificate generation** - prints a formatted completion certificate with the student's details and a random verification code
- **Cross-platform compile** - `#ifdef _WIN32` block keeps it compatible with both Windows and Linux

---

## 🧠 Concepts Demonstrated

- **Classes** - `User` class for student data, `QuizEngine` class for quiz logic
- **Structs** - `Question` struct holding per-question data
- **Encapsulation** - quiz state (score, question bank) is private inside `QuizEngine`
- **Vectors** - dynamic question bank and skip-list
- **STL algorithms** - `transform()` for case-insensitive input
- **`iomanip`** - formatted output with `setw` and `setprecision`
- **Conditional compilation** - `#ifdef _WIN32` for platform portability
- **Input validation loops** - level selection re-prompts on bad input
- **`srand` / `rand`** - seeded with `time(0)` for the certificate code

---

## 📁 Project Structure

```
quiz-master/
├── main.cpp        ← All classes and logic in a single file
└── README.md
```

The `main.cpp` is organized in this order:

1. Platform-specific helpers (`clearScreen`, `waitSec`)
2. `User` class - registration and data storage
3. `Question` struct - per-question data
4. `QuizEngine` class - quiz flow, scoring, results, certificate
5. `main()` - startup, question bank population, quiz execution

---

## 🚀 Getting Started

**Requirements:** A C++ compiler with C++11 or later (g++, MinGW, MSVC, or Code::Blocks)

**Compile and run:**

```bash
g++ main.cpp -o quizmaster
./quiz-master
```

Or open in Code::Blocks and hit Run (the `.cbp` project file is not included - just add `main.cpp` to a new project).

---

## 📝 Notes

- The question bank is hard-coded in `main()` - 30 questions, 7 categories. Adding new questions only requires one additional `addQuestion()` call.
- The certificate's verify code uses `rand()` seeded with `time(0)` - it's a visual feature, not actual verification.
- Semester 2 scope: no file I/O, no database, no persistent scores. Those would be later-semester additions.
- The `wait(10)` delay from the original was intentional for presentation purposes - reduced to 2 seconds here since it's better for normal usage.

---

## 📄 License

MIT - see the [LICENSE](./LICENSE) file in the root of the repository.
