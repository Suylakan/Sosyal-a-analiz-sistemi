#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 50

/* --------------------
   Veri Yapilari
   -------------------- */
typedef struct Friend {
    int id;
    struct Friend* next;
} Friend;

typedef struct User {
    int id;
    char name[MAX_NAME_LENGTH];
    Friend* friends;
    struct User* next;
} User;

/* --------------------
   Red-Black Tree Veri Yapisi
   -------------------- */
typedef enum { RED, BLACK } Color;
typedef struct RBTNode {
    int data;
    Color color;
    struct RBTNode *left, *right, *parent;
} RBTNode;

/* --------------------
   Fonksiyon Prototipleri
   -------------------- */

// Kullanici ve Arkadas Islemleri
User* addUser(User** userList, int userID, const char* userName);
void addFriend(User** userList, int user1, int user2);
void readDataFromFile(const char* filename, User** userList);
bool isFriend(int userID, Friend* friends);
void findCommonFriends(User* userList, int userA, int userB);

// Duzenli dosya yazimi fonksiyonu
void writeDataToFile(const char* filename, User* userList);

// Analiz Fonksiyonlari
int getMaxUserId(User* userList);
void printRelationshipTreeHelper(User* user, User* userList, bool* visited, int m, int level);
void printRelationshipTree(User* user, User* userList);
bool dfsFriendSearch(User* userList, int currentID, int targetID, bool* visited, int m);
bool findFriendPath(User* userList, int startID, int targetID);
void printCommunityMembers(User* userList, int currentID, bool* visited, int m);
void detectCommunities(User* userList);
int calcInfluenceArea(User* user, User* userList, bool* visited, int m);
int influenceArea(User* user, User* userList);

// Red-Black Tree Fonksiyonlari (Tek blok halinde)
RBTNode* createRBTNode(int data);
void leftRotate(RBTNode** root, RBTNode* x);
void rightRotate(RBTNode** root, RBTNode* y);
void rbInsertFixup(RBTNode** root, RBTNode* z);
void rbInsert(RBTNode** root, int data);
void inorderRBT(RBTNode* root);

// Yardimci Fonksiyonlar
void clearInputBuffer(void);
void printMenu(void);
void interactiveMode(User** userList, const char* filePath);

/* --------------------
   Fonksiyonlar: Kullanici ve Arkadas Islemleri
   -------------------- */
User* addUser(User** userList, int userID, const char* userName) {
    User* temp = *userList;
    while (temp) {
        if (temp->id == userID)
            return temp;
        temp = temp->next;
    }
    User* newUser = (User*)malloc(sizeof(User));
    newUser->id = userID;
    strncpy(newUser->name, userName, MAX_NAME_LENGTH);
    newUser->friends = NULL;
    newUser->next = *userList;
    *userList = newUser;
    return newUser;
}

void addFriend(User** userList, int user1, int user2) {
    User* u1 = addUser(userList, user1, "Unknown");
    User* u2 = addUser(userList, user2, "Unknown");
    Friend* newFriend1 = (Friend*)malloc(sizeof(Friend));
    newFriend1->id = user2;
    newFriend1->next = u1->friends;
    u1->friends = newFriend1;
    Friend* newFriend2 = (Friend*)malloc(sizeof(Friend));
    newFriend2->id = user1;
    newFriend2->next = u2->friends;
    u2->friends = newFriend2;
}

void readDataFromFile(const char* filename, User** userList) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        // Dosya bulunamazsa devam ediyoruz.
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#')
            continue;
        char type[10];
        if (sscanf(line, "%s", type) != 1)
            continue;
        if (strcmp(type, "USER") == 0) {
            int id;
            char name[MAX_NAME_LENGTH];
            if (sscanf(line, "%*s %d %s", &id, name) == 2)
                addUser(userList, id, name);
        } else if (strcmp(type, "FRIEND") == 0) {
            int id1, id2;
            if (sscanf(line, "%*s %d %d", &id1, &id2) == 2)
                addFriend(userList, id1, id2);
        }
    }
    fclose(file);
}

bool isFriend(int userID, Friend* friends) {
    while (friends) {
        if (friends->id == userID)
            return true;
        friends = friends->next;
    }
    return false;
}

void findCommonFriends(User* userList, int userA, int userB) {
    User *uA = NULL, *uB = NULL;
    User* temp = userList;
    while (temp) {
        if (temp->id == userA)
            uA = temp;
        if (temp->id == userB)
            uB = temp;
        temp = temp->next;
    }
    if (!uA || !uB) {
        printf("Kullanicilardan biri bulunamadi.\n");
        return;
    }
    printf("%s ve %s'nin ortak arkadaslari: ", uA->name, uB->name);
    int commonFound = 0;
    Friend* fA = uA->friends;
    while (fA) {
        if (isFriend(fA->id, uB->friends)) {
            User* tmpUser = userList;
            while (tmpUser) {
                if (tmpUser->id == fA->id) {
                    printf("%s (%d) ", tmpUser->name, tmpUser->id);
                    commonFound = 1;
                    break;
                }
                tmpUser = tmpUser->next;
            }
        }
        fA = fA->next;
    }
    if (!commonFound)
        printf("Ortak arkadas bulunamadi.");
    printf("\n");
}

/* --------------------
   Fonksiyon: Duzenli Dosya Yazimi
   -------------------- */
void writeDataToFile(const char* filename, User* userList) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Dosya acilamadi");
        exit(EXIT_FAILURE);
    }
    // KULLANICI BILGILERI Bolumu
    fprintf(file, "========== KULLANICI BILGILERI ==========\n");
    fprintf(file, "%-10s %-5s %-20s\n", "Type", "ID", "Name");
    fprintf(file, "-----------------------------------------\n");

    User* temp = userList;
    while (temp) {
        fprintf(file, "%-10s %-5d %-20s\n", "USER", temp->id, temp->name);
        temp = temp->next;
    }
    // ARKADASLIK ILISKILERI Bolumu
    fprintf(file, "\n========== ARKADASLIK ILISKILERI ==========\n");
    fprintf(file, "%-10s %-5s %-5s\n", "Type", "ID1", "ID2");
    fprintf(file, "---------------------------------------------\n");

    temp = userList;
    while (temp) {
        Friend* f = temp->friends;
        while (f) {
            if (temp->id < f->id)  // Duplicate kayitlari onlemek icin
                fprintf(file, "%-10s %-5d %-5d\n", "FRIEND", temp->id, f->id);
            f = f->next;
        }
        temp = temp->next;
    }
    fclose(file);
}

/* --------------------
   Fonksiyonlar: Analiz Islemleri
   -------------------- */
int getMaxUserId(User* userList) {
    int max = 0;
    while (userList) {
        if (userList->id > max)
            max = userList->id;
        userList = userList->next;
    }
    return max;
}

void printRelationshipTreeHelper(User* user, User* userList, bool* visited, int m, int level) {
    if (!user)
        return;
    if (user->id < m && visited[user->id])
        return;
    if (user->id < m)
        visited[user->id] = true;
    for (int i = 0; i < level; i++)
        printf("  ");
    printf("%s (%d)\n", user->name, user->id);
    Friend* fr = user->friends;
    while (fr) {
        User* friendUser = NULL;
        User* tmp = userList;
        while (tmp) {
            if (tmp->id == fr->id) {
                friendUser = tmp;
                break;
            }
            tmp = tmp->next;
        }
        printRelationshipTreeHelper(friendUser, userList, visited, m, level + 1);
        fr = fr->next;
    }
}

void printRelationshipTree(User* user, User* userList) {
    int m = getMaxUserId(userList) + 1;
    bool* visited = (bool*)calloc(m, sizeof(bool));
    printRelationshipTreeHelper(user, userList, visited, m, 0);
    free(visited);
}

bool dfsFriendSearch(User* userList, int currentID, int targetID, bool* visited, int m) {
    User* current = NULL;
    User* tmp = userList;
    while (tmp) {
        if (tmp->id == currentID) {
            current = tmp;
            break;
        }
        tmp = tmp->next;
    }
    if (!current)
        return false;
    if (current->id == targetID)
        return true;
    if (current->id < m) {
        if (visited[current->id])
            return false;
        visited[current->id] = true;
    }
    Friend* fr = current->friends;
    while (fr) {
        if (dfsFriendSearch(userList, fr->id, targetID, visited, m))
            return true;
        fr = fr->next;
    }
    return false;
}

bool findFriendPath(User* userList, int startID, int targetID) {
    int m = getMaxUserId(userList) + 1;
    bool* visited = (bool*)calloc(m, sizeof(bool));
    bool found = dfsFriendSearch(userList, startID, targetID, visited, m);
    free(visited);
    return found;
}

void printCommunityMembers(User* userList, int currentID, bool* visited, int m) {
    User* current = NULL;
    User* tmp = userList;
    while (tmp) {
        if (tmp->id == currentID) {
            current = tmp;
            break;
        }
        tmp = tmp->next;
    }
    if (!current)
        return;
    if (current->id < m && visited[current->id])
        return;
    if (current->id < m)
        visited[current->id] = true;
    printf("%s(%d) ", current->name, current->id);
    Friend* fr = current->friends;
    while (fr) {
        printCommunityMembers(userList, fr->id, visited, m);
        fr = fr->next;
    }
}

void detectCommunities(User* userList) {
    int m = getMaxUserId(userList) + 1;
    bool* visited = (bool*)calloc(m, sizeof(bool));
    int community = 0;
    User* temp = userList;
    while (temp) {
        if (temp->id < m && !visited[temp->id]) {
            community++;
            printf("Topluluk %d: ", community);
            printCommunityMembers(userList, temp->id, visited, m);
            printf("\n");
        }
        temp = temp->next;
    }
    free(visited);
}

int calcInfluenceArea(User* user, User* userList, bool* visited, int m) {
    if (!user)
        return 0;
    if (user->id < m) {
        if (visited[user->id])
            return 0;
        visited[user->id] = true;
    }
    int count = 1; // kendisi dahil
    Friend* fr = user->friends;
    while (fr) {
        User* friendUser = NULL;
        User* tmp = userList;
        while (tmp) {
            if (tmp->id == fr->id) {
                friendUser = tmp;
                break;
            }
            tmp = tmp->next;
        }
        count += calcInfluenceArea(friendUser, userList, visited, m);
        fr = fr->next;
    }
    return count;
}

int influenceArea(User* user, User* userList) {
    int m = getMaxUserId(userList) + 1;
    bool* visited = (bool*)calloc(m, sizeof(bool));
    int area = calcInfluenceArea(user, userList, visited, m) - 1;
    free(visited);
    return area;
}

/* --------------------
   Fonksiyonlar: Red-Black Tree Islemleri
   -------------------- */
RBTNode* createRBTNode(int data) {
    RBTNode* newNode = (RBTNode*)malloc(sizeof(RBTNode));
    newNode->data = data;
    newNode->color = RED; // Yeni dugumler kirmizi
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

void leftRotate(RBTNode** root, RBTNode* x) {
    RBTNode* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rightRotate(RBTNode** root, RBTNode* y) {
    RBTNode* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void rbInsertFixup(RBTNode** root, RBTNode* z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBTNode* y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            RBTNode* y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void rbInsert(RBTNode** root, int data) {
    RBTNode* z = createRBTNode(data);
    RBTNode* y = NULL;
    RBTNode* x = *root;
    while (x != NULL) {
        y = x;
        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == NULL)
        *root = z;
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;
    rbInsertFixup(root, z);
}

void inorderRBT(RBTNode* root) {
    if (root != NULL) {
        inorderRBT(root->left);
        printf("%d (%s) ", root->data, root->color == RED ? "R" : "B");
        inorderRBT(root->right);
    }
}

/* --------------------
   Yardimci Fonksiyonlar
   -------------------- */
void clearInputBuffer(void) {
    while(getchar() != '\n');
}

void printMenu(void) {
    printf("\n====================================\n");
    printf("             ANA MENU\n");
    printf("====================================\n");
    printf("1 - Yeni Kullanici Ekle\n");
    printf("2 - Arkadaslik Iliskisini Tanimla\n");
    printf("3 - Ortak Arkadaslari Bul\n");
    printf("4 - Iliski Agacini Goster\n");
    printf("5 - DFS ile Arkadas Arama\n");
    printf("6 - Topluluk Tespiti\n");
    printf("7 - Etki Alanini Hesapla\n");
    printf("8 - Red-Black Tree Test\n");
    printf("9 - Dosyayi Duzenli Sekilde Yazdir\n");
    printf("10 - Cikis\n");
    printf("Seciminiz: ");
}

void interactiveMode(User** userList, const char* filePath) {
    int choice, id1, id2, friendCount, i;
    char name[MAX_NAME_LENGTH];
    int treeCount, treeValue;
    RBTNode* rbRoot = NULL;

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Gecersiz giris! Tekrar deneyin.\n");
            continue;
        }
        switch (choice) {
            case 1:
                printf("Kullanici ID girin: ");
                scanf("%d", &id1);
                printf("Kullanici ismini girin: ");
                scanf("%s", name);
                addUser(userList, id1, name);
                printf("Kullanici basariyla eklendi!\n");
                writeDataToFile(filePath, *userList);
                break;
            case 2:
                printf("Kac tane arkadas eklemek istiyorsunuz? ");
                scanf("%d", &friendCount);
                printf("Iliski icin ana kullanici ID'si girin: ");
                scanf("%d", &id1);
                for (i = 0; i < friendCount; i++) {
                    printf("Arkadas ID girin: ");
                    scanf("%d", &id2);
                    addFriend(userList, id1, id2);
                    printf("Arkadaslik eklendi!\n");
                }
                writeDataToFile(filePath, *userList);
                break;
            case 3:
                printf("Ortak arkadaslarini bulmak icin iki kullanici ID'si girin: ");
                scanf("%d %d", &id1, &id2);
                findCommonFriends(*userList, id1, id2);
                break;
            case 4:
                if (*userList) {
                    printf("\n--- Iliski Agaci ---\n");
                    printRelationshipTree(*userList, *userList);
                } else {
                    printf("Henuz kullanici yok.\n");
                }
                break;
            case 5:
                printf("DFS ile kontrol icin iki kullanici ID'si girin: ");
                scanf("%d %d", &id1, &id2);
                if (findFriendPath(*userList, id1, id2))
                    printf("DFS: %d numarali kullanici %d numarali kullaniciya ulasabiliyor.\n", id1, id2);
                else
                    printf("DFS: %d numarali kullanici %d numarali kullaniciya ulasilamiyor.\n", id1, id2);
                break;
            case 6:
                printf("\n--- Topluluk Tespiti ---\n");
                detectCommunities(*userList);
                break;
            case 7:
                printf("Etki alanini hesaplanacak kullanici ID'si girin: ");
                scanf("%d", &id1);
                {
                    User* u = NULL;
                    User* temp = *userList;
                    while (temp) {
                        if (temp->id == id1) {
                            u = temp;
                            break;
                        }
                        temp = temp->next;
                    }
                    if (u) {
                        int area = influenceArea(u, *userList);
                        printf("%s'nin etki alani: %d kisi.\n", u->name, area);
                    } else {
                        printf("Kullanici bulunamadi.\n");
                    }
                }
                break;
            case 8:
                printf("Red-Black Tree icin kac tane deger gireceksiniz? ");
                scanf("%d", &treeCount);
                rbRoot = NULL;
                printf("Lutfen degerleri girin (tam sayi):\n");
                for (i = 0; i < treeCount; i++) {
                    scanf("%d", &treeValue);
                    rbInsert(&rbRoot, treeValue);
                }
                printf("Red-Black Tree (Inorder):\n");
                inorderRBT(rbRoot);
                printf("\n");
                break;
            case 9:
                writeDataToFile(filePath, *userList);
                printf("Veri dosyasi duzenli sekilde guncellendi.\n");
                break;
            case 10:
                printf("Cikis yapiliyor...\n");
                return;
            default:
                printf("Gecersiz secim! Tekrar deneyin.\n");
        }
    }
}



/* --------------------
   Ana Fonksiyon
   -------------------- */
int main() {
    User* userList = NULL;
    const char* filePath = "C:/dosya_konumu/veriseti.txt";  // Kendi sistem konumunuza göre düzenleyin.

    // Dosyadaki veriyi okuyarak mevcut kullanicilari ve iliskileri yukluyoruz.
    readDataFromFile(filePath, &userList);

    // Ornek testler:
    if (userList) {
        printf("\n--- Iliski Agaci ---\n");
        printRelationshipTree(userList, userList);
    }
    if (findFriendPath(userList, 1, 3))
        printf("DFS: 1 numarali kullanici 3 numarali kullaniciya ulasabiliyor.\n");
    else
        printf("DFS: 1 numarali kullanici 3 numarali kullaniciya ulasilamiyor.\n");
    printf("\n--- Topluluk Tespiti ---\n");
    detectCommunities(userList);
    if (userList) {
        int area = influenceArea(userList, userList);
        printf("%s'nin etki alani: %d kisi\n", userList->name, area);
    }
    RBTNode* rbRoot = NULL;
    rbInsert(&rbRoot, 20);
    rbInsert(&rbRoot, 15);
    rbInsert(&rbRoot, 25);
    rbInsert(&rbRoot, 10);
    rbInsert(&rbRoot, 18);
    rbInsert(&rbRoot, 22);
    rbInsert(&rbRoot, 30);
    printf("\n--- Red-Black Tree (Inorder) ---\n");
    inorderRBT(rbRoot);
    printf("\n");

    // Etkileşimli mod:
    interactiveMode(&userList, filePath);

    return 0;
}