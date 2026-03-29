#include <iostream>
#include <cstring>
using namespace std;

class QuizAttempt {
protected:
    char studentID[6];

public:
    QuizAttempt(const char *id) {
        strcpy(studentID, id);
    }

    virtual double score() = 0;
    virtual void print() = 0;

    bool operator>=(QuizAttempt &other) {
        return score() >= other.score();
    }

    virtual ~QuizAttempt() {}
};

class MCQuizAttempt : public QuizAttempt {
private:
    char correctAnswers[10];
    char studentAnswers[10];

public:
    MCQuizAttempt(const char *id,
                  const char *correct,
                  const char *answers)
        : QuizAttempt(id) {
        strcpy(correctAnswers, correct);
        strcpy(studentAnswers, answers);
    }

    double score() override {
        double total = 0;

        for (int i = 0; i < 10; i++) {
            if (correctAnswers[i] == studentAnswers[i])
                total += 1;
            else
                total -= 0.25;
        }

        return total;
    }

    void print() override {
        for (int i = 0; i < 10; i++) {
            cout << "Correct: " << correctAnswers[i]
                 << " Answer: " << studentAnswers[i]
                 << " Points: "
                 << (correctAnswers[i] == studentAnswers[i] ? 1 : -0.25)
                 << endl;
        }

        cout << "Total score: " << score() << endl;
    }

    ~MCQuizAttempt() {}
};

class TFQuizAttempt : public QuizAttempt {
private:
    bool correct[10];
    bool answers[10];

public:
    TFQuizAttempt(const char *id, bool *corr, bool *ans)
        : QuizAttempt(id) {
        for (int i = 0; i < 10; i++) {
            correct[i] = corr[i];
            answers[i] = ans[i];
        }
    }

    double score() override {
        double total = 0;

        for (int i = 0; i < 10; i++) {
            if (correct[i] == answers[i])
                total += 1;
            else
                total -= 0.5;
        }

        return total;
    }

    void print() override {
        for (int i = 0; i < 10; i++) {
            cout << "Correct: " << (correct[i] ? "True" : "False")
                 << " Answer: " << (answers[i] ? "True" : "False")
                 << " Points: "
                 << (correct[i] == answers[i] ? 1 : -0.5)
                 << endl;
        }

        cout << "Total score: " << score() << endl;
    }

    ~TFQuizAttempt() {}
};

double averagePointsOfPassedStudents(QuizAttempt **attempts, int n) {
    double sum = 0;
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (attempts[i]->score() >= 5.0) {
            sum += attempts[i]->score();
            count++;
        }
    }

    if (count == 0) return 0;
    return sum / count;
}

QuizAttempt *readMCQuizAttempt() {
    char ID[7];
    string correctStr, answersStr;

    cin >> ID >> correctStr >> answersStr;
    return new MCQuizAttempt(ID,
                             correctStr.c_str(),
                             answersStr.c_str());
}

QuizAttempt *readTFQuizAttempt() {
    char ID[7];
    cin >> ID;

    int tempCorrect[10];
    int tempAnswers[10];

    bool correct[10];
    bool answers[10];

    for (int i = 0; i < 10; i++)
        cin >> tempCorrect[i];

    for (int i = 0; i < 10; i++)
        cin >> tempAnswers[i];

    for (int i = 0; i < 10; i++) {
        correct[i] = tempCorrect[i] != 0;
        answers[i] = tempAnswers[i] != 0;
    }

    return new TFQuizAttempt(ID, correct, answers);
}

int main() {
    int testCase;
    cin >> testCase;

    if (testCase == 1) {
        cout << "Test MCQuizAttempt" << endl;
        QuizAttempt *attempt = readMCQuizAttempt();

        cout << "Score: " << attempt->score() << endl;
        attempt->print();

    } else if (testCase == 2) {
        cout << "Test TFQuizAttempt" << endl;
        QuizAttempt *attempt = readTFQuizAttempt();

        cout << "Score: " << attempt->score() << endl;
        attempt->print();

    } else if (testCase == 3) {
        cout << "Test operator >=" << endl;

        int n;
        cin >> n;

        QuizAttempt **attempts = new QuizAttempt *[n];

        for (int i = 0; i < n; i++) {
            int type;
            cin >> type;

            if (type == 1)
                attempts[i] = readMCQuizAttempt();
            else
                attempts[i] = readTFQuizAttempt();
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    if (*attempts[i] >= *attempts[j])
                        cout << i << " >= " << j << endl;
                    else
                        cout << i << " < " << j << endl;
                }
            }
        }

    } else {
        int n;
        cin >> n;

        QuizAttempt **attempts = new QuizAttempt *[n];

        for (int i = 0; i < n; i++) {
            int type;
            cin >> type;

            if (type == 1)
                attempts[i] = readMCQuizAttempt();
            else
                attempts[i] = readTFQuizAttempt();

            attempts[i]->print();
            cout << endl;
        }

        for (int i = 0; i < n; i++) {
            attempts[i]->print();
            cout << endl;
        }

        cout << "Average score of passed students is: "
             << averagePointsOfPassedStudents(attempts, n)
             << endl;

        for (int i = 0; i < n; i++)
            delete attempts[i];

        delete[] attempts;
    }

    return 0;
}