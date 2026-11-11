#include <iostream>
#include <string>
using namespace std;

// Дані одного елемента списку.
struct PlanetData {
    string name;
    string type;
    int diameter;
};

// Вузол містить дані та адресу наступного вузла.
struct Node {
    PlanetData data;
    Node* next;
};

// Список зберігає початок, кінець і кількість елементів.
struct List {
    Node* head;
    Node* tail;
    int size;
};

int allocated = 0, freed = 0;

// Початкове налаштування порожнього списку.
void initList(List& list) {
    list.head = nullptr;
    list.tail = nullptr;
    list.size = 0;
}

// Створення нового вузла та заповнення його даних.
Node* createNode(int value) {
    Node* p = new Node;
    p->data.name = "Planet" + to_string(value);
    p->data.type = (value % 2 == 0) ? "gas" : "rocky";
    p->data.diameter = value;
    p->next = nullptr;
    allocated++;
    return p;
}

// Додавання нового елемента в кінець списку.
void addBack(List& list, int value) {
    Node* p = createNode(value);

    if (list.head == nullptr)
        list.head = p;
    else
        list.tail->next = p;

    list.tail = p;
    list.size++;
}

// Додавання нового елемента на початок списку.
void addFront(List& list, int value) {
    Node* p = createNode(value);
    p->next = list.head;
    list.head = p;

    if (list.tail == nullptr)
        list.tail = p;

    list.size++;
}

// Формування списку з масиву початкових значень.
void buildList(List& list, const int a[], int n) {
    for (int i = 0; i < n; i++)
        addBack(list, a[i]);
}

// Виведення всіх значень списку.
void displayList(const List& list) {
    Node* p = list.head;

    if (!p) {
        cout << "empty";
        return;
    }

    while (p) {
        cout << p->data.diameter;

        if (p->next)
            cout << " -> ";

        p = p->next;
    }
}

// Пошук першого збігу та підрахунок переглянутих вузлів.
Node* searchFirst(List& list, int key, int& position, int& checked) {
    Node* p = list.head;
    position = -1;
    checked = 0;
    int i = 0;

    while (p) {
        checked++;

        if (p->data.diameter == key) {
            position = i;
            return p;
        }

        p = p->next;
        i++;
    }

    return nullptr;
}

// Пошук усіх збігів ключа та збереження їхніх позицій.
int searchAll(List& list, int key, int positions[], int maxCount, int& checked) {
    Node* p = list.head;
    int i = 0, found = 0;
    checked = 0;

    while (p) {
        checked++;

        if (p->data.diameter == key && found < maxCount)
            positions[found++] = i;

        p = p->next;
        i++;
    }

    return found;
}

// Вставлення елемента за заданою позицією.
bool insertAt(List& list, int value, int pos) {
    if (pos < 0 || pos > list.size)
        return false;

    if (pos == 0) {
        addFront(list, value);
        return true;
    }

    if (pos == list.size) {
        addBack(list, value);
        return true;
    }

    Node* prev = list.head;

    for (int i = 0; i < pos - 1; i++)
        prev = prev->next;

    Node* p = createNode(value);
    p->next = prev->next;
    prev->next = p;
    list.size++;

    return true;
}

// Вставлення нового елемента перед першим вузлом із заданим ключем.
bool insertBefore(List& list, int key, int value) {
    Node* p = list.head;
    Node* prev = nullptr;

    while (p && p->data.diameter != key) {
        prev = p;
        p = p->next;
    }

    if (!p)
        return false;

    Node* fresh = createNode(value);
    fresh->next = p;

    if (!prev)
        list.head = fresh;
    else
        prev->next = fresh;

    list.size++;
    return true;
}

// Вставлення нового елемента після першого вузла із заданим ключем.
bool insertAfter(List& list, int key, int value) {
    Node* p = list.head;

    while (p && p->data.diameter != key)
        p = p->next;

    if (!p)
        return false;

    Node* fresh = createNode(value);
    fresh->next = p->next;
    p->next = fresh;

    if (list.tail == p)
        list.tail = fresh;

    list.size++;
    return true;
}

// Видалення елемента за позицією.
bool eraseAt(List& list, int pos) {
    if (pos < 0 || pos >= list.size || !list.head)
        return false;

    Node* victim;

    if (pos == 0) {
        victim = list.head;
        list.head = victim->next;

        if (list.tail == victim)
            list.tail = list.head;
    }
    else {
        Node* prev = list.head;

        for (int i = 0; i < pos - 1; i++)
            prev = prev->next;

        victim = prev->next;
        prev->next = victim->next;

        if (list.tail == victim)
            list.tail = prev;
    }

    delete victim;
    freed++;
    list.size--;

    if (list.size == 0) {
        list.head = nullptr;
        list.tail = nullptr;
    }

    return true;
}

// Видалення першого елемента із заданим ключем.
bool eraseFirst(List& list, int key) {
    Node* p = list.head;
    Node* prev = nullptr;

    while (p && p->data.diameter != key) {
        prev = p;
        p = p->next;
    }

    if (!p)
        return false;

    if (!prev)
        list.head = p->next;
    else
        prev->next = p->next;

    if (list.tail == p)
        list.tail = prev;

    delete p;
    freed++;
    list.size--;

    if (!list.head)
        list.tail = nullptr;

    return true;
}

// Видалення всіх елементів із заданим ключем.
int eraseAll(List& list, int key) {
    int removed = 0;
    Node* p = list.head;
    Node* prev = nullptr;

    while (p) {
        if (p->data.diameter == key) {
            Node* victim = p;
            p = p->next;

            if (!prev)
                list.head = p;
            else
                prev->next = p;

            if (list.tail == victim)
                list.tail = prev;

            delete victim;
            freed++;
            list.size--;
            removed++;
        }
        else {
            prev = p;
            p = p->next;
        }
    }

    if (!list.head)
        list.tail = nullptr;

    return removed;
}

// Перевірка списку на наявність циклу методом двох вказівників.
bool hasCycle(const List& list) {
    Node* slow = list.head;
    Node* fast = list.head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return true;
    }

    return false;
}

// Перевірка кількості вузлів, кінцевого вузла та коректності посилань.
bool checkList(const List& list, bool show = true) {
    if (hasCycle(list)) {
        if (show)
            cout << "error: cycle";

        return false;
    }

    int count = 0;
    Node* p = list.head;
    Node* last = nullptr;

    while (p) {
        count++;
        last = p;
        p = p->next;
    }

    bool ok = (count == list.size &&
        last == list.tail &&
        ((!list.head && !list.tail && list.size == 0) ||
            (list.head && list.tail && list.tail->next == nullptr)));

    if (show)
        cout << (ok ? "structure OK" : "structure error");

    return ok;
}

// Звільнення пам'яті всіх вузлів і очищення списку.
void clearList(List& list) {
    Node* p = list.head;

    while (p) {
        Node* next = p->next;
        delete p;
        freed++;
        p = next;
    }

    initList(list);
}

// Порівняння списку з очікуваними значеннями.
bool equals(const List& list, const int expected[], int n) {
    if (list.size != n)
        return false;

    Node* p = list.head;
    int i = 0;

    while (p && i < n) {
        if (p->data.diameter != expected[i])
            return false;

        p = p->next;
        i++;
    }

    return p == nullptr && i == n && checkList(list, false);
}

int testsPassed = 0, testsTotal = 0;

// Виконання одного тесту.
void test(const string& name, List& list, const int expected[], int n) {
    testsTotal++;
    bool ok = equals(list, expected, n);

    cout << "Test " << testsTotal << " - " << name << ": "
        << (ok ? "PASSED" : "FAILED") << " | ";

    displayList(list);
    cout << "\n";

    if (ok)
        testsPassed++;

    clearList(list);
}

// Виведення стану списку та перевірка його структури.
void printStatus(const string& title, List& list) {
    cout << title << ": ";
    displayList(list);
    cout << " | size=" << list.size << " | ";
    checkList(list);
    cout << "\n";
}

// Запуск 20 автоматичних тестів для різних випадків.
void runTests() {
    List l;
    int checked, pos, found, positions[20];
    int a0[1] = { 0 };

    initList(l);
    searchFirst(l, 5, pos, checked);
    test("search in empty list", l, a0, 0);

    initList(l);
    insertAt(l, 5, 0);
    int a1[] = { 5 };
    test("insert into empty list", l, a1, 1);

    initList(l);
    addBack(l, 5);
    eraseAt(l, 0);
    test("delete the only element", l, a0, 0);

    int a[] = { 1, 3 };
    initList(l);
    buildList(l, a, 2);
    insertAt(l, 2, 1);
    int e4[] = { 1, 2, 3 };
    test("insert in the middle", l, e4, 3);

    initList(l);
    buildList(l, a, 2);
    insertAt(l, 0, 0);
    int e5[] = { 0, 1, 3 };
    test("insert at the beginning", l, e5, 3);

    initList(l);
    buildList(l, a, 2);
    insertAt(l, 4, 2);
    int e6[] = { 1, 3, 4 };
    test("insert at the end", l, e6, 3);

    int b[] = { 1, 2, 3 };
    initList(l);
    buildList(l, b, 3);
    eraseAt(l, 0);
    int e7[] = { 2, 3 };
    test("delete the first element", l, e7, 2);

    initList(l);
    buildList(l, b, 3);
    eraseAt(l, 1);
    int e8[] = { 1, 3 };
    test("delete a middle element", l, e8, 2);

    initList(l);
    buildList(l, b, 3);
    eraseAt(l, 2);
    int e9[] = { 1, 2 };
    test("delete the last element", l, e9, 2);

    int c[] = { 1, 2, 2, 3 };
    initList(l);
    buildList(l, c, 4);
    eraseFirst(l, 2);
    int e10[] = { 1, 2, 3 };
    test("delete the first matching key", l, e10, 3);

    int d[] = { 2, 2, 1, 2 };
    initList(l);
    buildList(l, d, 4);
    eraseAll(l, 2);
    int e11[] = { 1 };
    test("delete all matching keys", l, e11, 1);

    initList(l);
    buildList(l, b, 3);
    insertAt(l, 9, 5);
    test("invalid insertion position", l, b, 3);

    initList(l);
    buildList(l, b, 3);
    eraseAt(l, -1);
    test("invalid deletion position", l, b, 3);

    initList(l);
    buildList(l, b, 3);
    insertAfter(l, 3, 9);
    int e14[] = { 1, 2, 3, 9 };
    test("insert after the last element", l, e14, 4);

    initList(l);
    buildList(l, b, 3);
    insertBefore(l, 1, 0);
    int e15[] = { 0, 1, 2, 3 };
    test("insert before the first element", l, e15, 4);

    int f[] = { 4, 4, 4, 2 };
    initList(l);
    buildList(l, f, 4);
    found = searchAll(l, 4, positions, 20, checked);

    bool t16 = (found == 3 && positions[0] == 0 &&
        positions[1] == 1 && positions[2] == 2);

    cout << "Test 16 - consecutive duplicate keys: "
        << (t16 ? "PASSED" : "FAILED") << "\n";

    testsTotal++;

    if (t16)
        testsPassed++;

    clearList(l);

    initList(l);
    buildList(l, b, 3);
    bool t17 = insertBefore(l, 99, 7) == false;

    cout << "Test 17 - missing key for insertion: "
        << (t17 && equals(l, b, 3) ? "PASSED" : "FAILED") << "\n";

    testsTotal++;

    if (t17 && equals(l, b, 3))
        testsPassed++;

    clearList(l);

    initList(l);
    buildList(l, b, 3);
    bool t18 = eraseFirst(l, 99) == false;

    cout << "Test 18 - missing key for deletion: "
        << (t18 && equals(l, b, 3) ? "PASSED" : "FAILED") << "\n";

    testsTotal++;

    if (t18 && equals(l, b, 3))
        testsPassed++;

    clearList(l);

    initList(l);
    buildList(l, b, 3);
    clearList(l);

    int g[] = { 8, 6 };
    buildList(l, g, 2);
    int e19[] = { 8, 6 };
    test("refill after clearing", l, e19, 2);

    initList(l);
    int mix[] = { 1, 2, 3 };
    buildList(l, mix, 3);

    insertAt(l, 4, 1);
    eraseFirst(l, 3);
    insertAfter(l, 2, 8);
    eraseAt(l, 0);
    insertBefore(l, 8, 6);
    insertAt(l, 10, l.size);
    eraseAll(l, 2);

    int e20[] = { 4, 6, 8, 10 };
    test("series of mixed operations", l, e20, 4);
}

// Виконання операцій відповідно до варіанта 18.
void runVariant() {
    List l;
    initList(l);

    int start[] = { 17, 4, 13, 4, 6 };
    buildList(l, start, 5);

    cout << "\n=== VARIANT 18 ===\n";
    cout << "Initial list: ";
    displayList(l);
    cout << "\n";

    int pos, checked, positions[20];
    Node* p = searchFirst(l, 13, pos, checked);

    cout << "Search for 13: " << (p ? "found" : "not found")
        << ", position=" << pos << ", checked=" << checked << "\n";

    int n = searchAll(l, 13, positions, 20, checked);

    cout << "All matches for 13: ";

    for (int i = 0; i < n; i++)
        cout << positions[i] << " ";

    cout << "| checked=" << checked << "\n";

    insertAt(l, 8, 5);
    printStatus("After inserting 8 at position 5", l);

    eraseAll(l, 4);
    printStatus("After deleting all 4s", l);

    clearList(l);

    cout << "After clearing: ";
    displayList(l);
    cout << " | ";
    checkList(l);
    cout << "\n";
}

// Виконання сценарію з 20 командами та перевіркою після кожної.
void runCommandScenario() {
    cout << "\n=== 20-COMMAND LOG ===\n";

    List l;
    initList(l);

    int initial[] = { 17, 4, 13, 4, 6 };
    buildList(l, initial, 5);

    for (int i = 1; i <= 20; i++) {
        if (i == 1) {
            int p, c;
            searchFirst(l, 13, p, c);
            cout << i << ". search 13, position " << p;
        }
        else if (i == 2) {
            int ps[20], c;
            int n = searchAll(l, 4, ps, 20, c);
            cout << i << ". search all 4s, matches " << n;
        }
        else if (i == 3) {
            insertAt(l, 8, 5);
            cout << i << ". insert 8 at position 5";
        }
        else if (i == 4) {
            insertBefore(l, 17, 20);
            cout << i << ". insert 20 before 17";
        }
        else if (i == 5) {
            insertAfter(l, 13, 21);
            cout << i << ". insert 21 after 13";
        }
        else if (i == 6) {
            eraseAt(l, 2);
            cout << i << ". delete position 2";
        }
        else if (i == 7) {
            eraseFirst(l, 4);
            cout << i << ". delete first 4";
        }
        else if (i == 8) {
            insertAt(l, 30, l.size);
            cout << i << ". add 30 at the end";
        }
        else if (i == 9) {
            insertAt(l, 10, 0);
            cout << i << ". add 10 at the beginning";
        }
        else if (i == 10) {
            eraseAll(l, 4);
            cout << i << ". delete all 4s";
        }
        else if (i == 11) {
            insertAfter(l, 13, 31);
            cout << i << ". insert 31 after 13";
        }
        else if (i == 12) {
            eraseAt(l, l.size - 1);
            cout << i << ". delete the last element";
        }
        else if (i == 13) {
            insertBefore(l, 6, 15);
            cout << i << ". insert 15 before 6";
        }
        else if (i == 14) {
            eraseFirst(l, 999);
            cout << i << ". try to delete 999";
        }
        else if (i == 15) {
            insertAt(l, 40, 1);
            cout << i << ". insert 40 at position 1";
        }
        else if (i == 16) {
            eraseAt(l, 0);
            cout << i << ". delete the first element";
        }
        else if (i == 17) {
            insertAt(l, 50, l.size);
            cout << i << ". insert 50 at the end";
        }
        else if (i == 18) {
            eraseFirst(l, 21);
            cout << i << ". delete 21";
        }
        else if (i == 19) {
            insertAt(l, 60, 0);
            cout << i << ". insert 60 at the beginning";
        }
        else {
            eraseAll(l, 31);
            cout << i << ". delete all 31s";
        }

        cout << " | ";
        displayList(l);
        cout << " | ";
        checkList(l);
        cout << "\n";
    }

    clearList(l);
}

// Головна функція програми.
int main() {
    cout << "PRACTICAL WORK 12. VARIANT 18\n";

    runTests();

    cout << "\nTest summary: " << testsPassed
        << "/" << testsTotal << " passed\n";

    runVariant();
    runCommandScenario();

    cout << "\nAllocated nodes: " << allocated << "\n";
    cout << "Freed nodes: " << freed << "\n";
    cout << "Active nodes: " << allocated - freed << "\n";

    cout << ((allocated == freed)
        ? "Memory balance OK"
        : "MEMORY BALANCE ERROR") << "\n";

    return 0;
}