#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "students.txt"
#define NAME_LEN  50

typedef struct {
    int    id;
    char   firstName[NAME_LEN];
    char   lastName[NAME_LEN];
    char   major[NAME_LEN];
    double gpa;
} Student;

typedef struct {
    Student *data;  
    int count;       
    int capacity;    
    int nextId;     
} StudentList;

void readLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\n")] = '\0';  
}

int readInt(const char *prompt) {
    char buf[64];
    char *end;
    while (1) {
        readLine(prompt, buf, sizeof buf);
        long v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0') return (int)v;
        printf("Xeta: yalniz tam eded yazin.\n");
    }
}

double readDouble(const char *prompt) {
    char buf[64];
    char *end;
    while (1) {
        readLine(prompt, buf, sizeof buf);
        double v = strtod(buf, &end);
        if (end != buf && *end == '\0' && v >= 0.0 && v <= 4.0) return v;
        printf("Xeta: 0 ile 4 arasi eded yazin.\n");
    }
}

void readText(const char *prompt, char *buf, int size) {
    while (1) {
        readLine(prompt, buf, size);
        if (buf[0] != '\0' && strchr(buf, '|') == NULL) return;
        printf("Xeta: bos ola bilmez ve '|' simvolu olmaz.\n");
    }
}


int addToList(StudentList *list, Student s) {
    if (list->count == list->capacity) {
        int newCap = (list->capacity == 0) ? 4 : list->capacity * 2;
        Student *tmp = realloc(list->data, newCap * sizeof(Student));
        if (tmp == NULL) {
            printf("Xeta: yaddas catismir.\n");
            return 0;
        }
        list->data = tmp;
        list->capacity = newCap;
    }
    list->data[list->count] = s;
    list->count++;
    if (s.id >= list->nextId) list->nextId = s.id + 1;
    return 1;
}

void loadFromFile(StudentList *list) {
    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) return;   // fayl yoxdur = ilk acilis, problem deyil

    char line[256];
    while (fgets(line, sizeof line, f) != NULL) {
        Student s;
        int n = sscanf(line, "%d|%49[^|]|%49[^|]|%49[^|]|%lf",
                       &s.id, s.firstName, s.lastName, s.major, &s.gpa);
        if (n == 5) addToList(list, s);   // pozulmus setirleri atiriq
    }
    fclose(f);
}

int appendToFile(const Student *s) {
    FILE *f = fopen(FILE_NAME, "a");
    if (f == NULL) {
        printf("Xeta: fayl acila bilmedi.\n");
        return 0;
    }
    fprintf(f, "%d|%s|%s|%s|%.2f\n",
            s->id, s->firstName, s->lastName, s->major, s->gpa);
    fclose(f);
    return 1;
}

int saveAllToFile(const StudentList *list) {
    FILE *f = fopen(FILE_NAME, "w");
    if (f == NULL) {
        printf("Xeta: fayl acila bilmedi.\n");
        return 0;
    }
    for (int i = 0; i < list->count; i++) {
        const Student *s = &list->data[i];
        fprintf(f, "%d|%s|%s|%s|%.2f\n",
                s->id, s->firstName, s->lastName, s->major, s->gpa);
    }
    fclose(f);
    return 1;
}

void addStudent(StudentList *list) {
    Student s;
    s.id = list->nextId;
    readText("Ad: ", s.firstName, NAME_LEN);
    readText("Soyad: ", s.lastName, NAME_LEN);
    readText("Ixtisas: ", s.major, NAME_LEN);
    s.gpa = readDouble("GPA (0-4): ");

    if (addToList(list, s) && appendToFile(&s)) {
        printf("Telebe elave olundu. ID = %d\n", s.id);
    }
}

void displayStudents(const StudentList *list) {
    if (list->count == 0) {
        printf("Sistemde telebe tapilmadi.\n");
        return;
    }
    printf("\n%-5s %-15s %-15s %-20s %-5s\n", "ID", "Ad", "Soyad", "Ixtisas", "GPA");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < list->count; i++) {
        const Student *s = &list->data[i];
        printf("%-5d %-15s %-15s %-20s %.2f\n",
               s->id, s->firstName, s->lastName, s->major, s->gpa);
    }
}

int findIndexById(const StudentList *list, int id) {
    for (int i = 0; i < list->count; i++) {
        if (list->data[i].id == id) return i;
    }
    return -1;
}

void deleteStudent(StudentList *list) {
    int id = readInt("Silinecek telebenin ID-si: ");
    int idx = findIndexById(list, id);
    if (idx == -1) {
        printf("Bele ID tapilmadi.\n");
        return;
    }
    for (int i = idx; i < list->count - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->count--;
    if (saveAllToFile(list)) printf("Telebe silindi.\n");
}

static void updateText(const char *prompt, char *field) {
    char buf[NAME_LEN];
    readLine(prompt, buf, NAME_LEN);
    if (buf[0] != '\0' && strchr(buf, '|') == NULL) {
        strcpy(field, buf);
    }
}

void updateStudent(StudentList *list) {
    int id = readInt("Yenilenecek telebenin ID-si: ");
    int idx = findIndexById(list, id);
    if (idx == -1) {
        printf("Bele ID tapilmadi.\n");
        return;
    }
    Student *s = &list->data[idx];
    printf("Cari: %s %s, %s, GPA %.2f\n", s->firstName, s->lastName, s->major, s->gpa);
    printf("(Deyismek istemirsense bos burax, Enter bas)\n");

    updateText("Yeni ad: ", s->firstName);
    updateText("Yeni soyad: ", s->lastName);
    updateText("Yeni ixtisas: ", s->major);

    char buf[64];
    char *end;
    readLine("Yeni GPA: ", buf, sizeof buf);
    if (buf[0] != '\0') {
        double v = strtod(buf, &end);
        if (end != buf && *end == '\0' && v >= 0.0 && v <= 4.0) s->gpa = v;
        else printf("GPA yanlisdir, kohne deyer qaldi.\n");
    }

    if (saveAllToFile(list)) printf("Melumat yenilendi.\n");
}

void printMenu(void) {
    printf("\n=== Telebe Idareetme Sistemi ===\n");
    printf("1. Telebe elave et\n");
    printf("2. Telebeni sil\n");
    printf("3. Telebelerin siyahisi\n");
    printf("4. Telebeni yenile\n");
    printf("5. Cix\n");
}

int main(void) {
    StudentList list = { NULL, 0, 0, 1 };
    loadFromFile(&list);

    int choice;
    do {
        printMenu();
        choice = readInt("Secim: ");
        switch (choice) {
            case 1: addStudent(&list);      break;
            case 2: deleteStudent(&list);   break;
            case 3: displayStudents(&list); break;
            case 4: updateStudent(&list);   break;
            case 5: printf("Sagol!\n");     break;
            default: printf("Yanlis secim, 1-5 arasi yazin.\n");
        }
    } while (choice != 5);

    free(list.data);   // dinamik yaddasi azad et
    list.data = NULL;
    return 0;
}