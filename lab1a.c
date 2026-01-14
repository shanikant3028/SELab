#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define SUBJECTS 5

struct Student {
    char id[20];
    char name[30];
    int minor[SUBJECTS];
    int major[SUBJECTS];
    int totalSub[SUBJECTS];   // minor + major
    int totalMarks;
    float percentage;
    char grade[3];
    float cgpa;
};

int isValidID(char id[]) {
    for (int i = 0; id[i]; i++) {
        if (!isalnum(id[i]))
            return 0;
    }
    return 1;
}

int isDuplicateID(struct Student s[], int count, char id[]) {
    for (int i = 0; i < count; i++) {
        if (strcmp(s[i].id, id) == 0)
            return 1;
    }
    return 0;
}

int isValidName(char name[]) {
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]))
            return 0;
    }
    return 1;
}

int isValidMinor(int m) {
    return (m >= 0 && m <= 40);
}

int isValidMajor(int m) {
    return (m >= 0 && m <= 60);
}

void calculateResult(struct Student *s) {
    s->totalMarks = 0;
    int passAll = 1;

    for (int i = 0; i < SUBJECTS; i++) {
        s->totalSub[i] = s->minor[i] + s->major[i];
        s->totalMarks += s->totalSub[i];

        if (s->totalSub[i] < 50)
            passAll = 0;
    }

    s->percentage = s->totalMarks / (float)SUBJECTS;

    if (!passAll)
        strcpy(s->grade, "F");
    else if (s->percentage >= 90) strcpy(s->grade, "O");
    else if (s->percentage >= 85) strcpy(s->grade, "A+");
    else if (s->percentage >= 75) strcpy(s->grade, "A");
    else if (s->percentage >= 65) strcpy(s->grade, "B+");
    else if (s->percentage >= 60) strcpy(s->grade, "B");
    else if (s->percentage >= 55) strcpy(s->grade, "C");
    else if (s->percentage >= 50) strcpy(s->grade, "D");
    else strcpy(s->grade, "F");

    s->cgpa = s->percentage / 10;
}

void displayReport(struct Student s[], int n) {
    printf("\nID\tName\tTotal\tPercent\tGrade\tCGPA\n");
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%s\t%s\t%d\t%.2f\t%s\t%.2f\n",
               s[i].id, s[i].name, s[i].totalMarks,
               s[i].percentage, s[i].grade, s[i].cgpa);
    }
}

void statistics(struct Student s[], int n) {
    float sum = 0, max = s[0].percentage, min = s[0].percentage;
    int gradeCount[8] = {0}; // O, A+, A, B+, B, C, D, F

    for (int i = 0; i < n; i++) {
        sum += s[i].percentage;
        if (s[i].percentage > max) max = s[i].percentage;
        if (s[i].percentage < min) min = s[i].percentage;

        if (strcmp(s[i].grade, "O") == 0) gradeCount[0]++;
        else if (strcmp(s[i].grade, "A+") == 0) gradeCount[1]++;
        else if (strcmp(s[i].grade, "A") == 0) gradeCount[2]++;
        else if (strcmp(s[i].grade, "B+") == 0) gradeCount[3]++;
        else if (strcmp(s[i].grade, "B") == 0) gradeCount[4]++;
        else if (strcmp(s[i].grade, "C") == 0) gradeCount[5]++;
        else if (strcmp(s[i].grade, "D") == 0) gradeCount[6]++;
        else gradeCount[7]++;
    }

    printf("\nClass Average Percentage: %.2f", sum / n);
    printf("\nHighest Percentage: %.2f", max);
    printf("\nLowest Percentage: %.2f\n", min);

    printf("\nGrade Distribution:\n");
    printf("O : %d\nA+: %d\nA : %d\nB+: %d\nB : %d\nC : %d\nD : %d\nF : %d\n",
           gradeCount[0], gradeCount[1], gradeCount[2], gradeCount[3],
           gradeCount[4], gradeCount[5], gradeCount[6], gradeCount[7]);
}
int main() {
    struct Student s[MAX];
    int n;

    FILE *fp = fopen("student.txt", "r");
    if (fp == NULL) {
        perror("File opening failed");
        return 1;
    }

    fscanf(fp, "%d", &n);

    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s %s", s[i].id, s[i].name);

        if (!isValidID(s[i].id) || isDuplicateID(s, i, s[i].id)) {
            printf("Invalid or duplicate Student ID\n");
            return 1;
        }

        if (!isValidName(s[i].name)) {
            printf("Invalid Name\n");
            return 1;
        }

        for (int j = 0; j < SUBJECTS; j++) {
            fscanf(fp, "%d %d", &s[i].minor[j], &s[i].major[j]);

            if (!isValidMinor(s[i].minor[j]) || !isValidMajor(s[i].major[j])) {
                printf("Invalid Minor/Major marks\n");
                return 1;
            }
        }
        calculateResult(&s[i]);
    }

    fclose(fp);

    displayReport(s, n);
    statistics(s, n);

    return 0;
}

