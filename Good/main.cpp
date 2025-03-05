#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

// ��������ڵ�ṹ
struct Node
{
    string name;  // �ҵ�����
    string brand; // Ʒ��
    float price;  // ����
    int quantity; // ����
    Node *next;   // ָ����һ���ڵ��ָ��

    // ���캯��
    Node(string n, string b, float p, int q) : name(n), brand(b), price(p), quantity(q), next(nullptr) {}
};

// �����û��ṹ
struct User
{
    string username;
    string password;
};

// ȫ�ֱ���
Node *head = nullptr;
vector<User> users;
string currentUser;

// ��������
void menu();
void loadUsers();
void registerUser();
bool loginUser();
void createList();
void loadFromFile();
void saveToFile();
void insertNode(string name, string brand, float price, int quantity);
void updateOrDeleteNode(string name, string brand, int quantity);
void searchNode(string name, string brand);
void updateNode(string name, string brand, float newPrice, int newQuantity);
void printList();
void calculateTotalValue();
void countByBrand(string brand);
void logAction(const string &action);
void validateInput(float &value, const string &prompt);
void validateInput(int &value, const string &prompt);

// ������
int main()
{
    loadUsers(); // �����û�����

    // �û���¼��ע��
    int choice;
    cout << "===== �ҵ������ϵͳ =====" << endl;
    cout << "1. ��¼\n2. ע��\n��ѡ�����: ";
    cin >> choice;

    if (choice == 1)
    {
        if (!loginUser())
        {
            cout << "��¼ʧ�ܣ������˳���" << endl;
            return 0;
        }
    }
    else if (choice == 2)
    {
        registerUser();
        if (!loginUser())
        {
            cout << "��¼ʧ�ܣ������˳���" << endl;
            return 0;
        }
    }
    else
    {
        cout << "��Чѡ������˳���" << endl;
        return 0;
    }

    // �Ƿ����ʹ��
    char initChoice;
    cout << "�Ƿ��ǳ���ʹ�ã�(y/n): ";
    cin >> initChoice;
    if (initChoice == 'y' || initChoice == 'Y')
    {
        createList(); // ����ʹ�ã��������
    }
    else
    {
        loadFromFile(); // �ǳ���ʹ�ã����ļ���������
    }

    // ����˵�
    menu();
    return 0;
}

// �˵�����
void menu()
{
    int option;
    do
    {
        cout << "\n===== �ҵ������ϵͳ =====" << endl;
        cout << "1. ��ӡ�����Ϣ" << endl;
        cout << "2. ��ѯ�ҵ���Ϣ" << endl;
        cout << "3. ����" << endl;
        cout << "4. ���" << endl;
        cout << "5. �޸ļҵ���Ϣ" << endl;
        cout << "6. ͳ�ƿ���ܼ�ֵ" << endl;
        cout << "7. ��Ʒ��ͳ�Ƽҵ�����" << endl;
        cout << "8. ���沢�˳�" << endl;
        cout << "��ѡ�����: ";
        cin >> option;

        string name, brand;
        float price;
        int quantity;

        switch (option)
        {
        case 1:
            printList();
            break;
        case 2:
            cout << "������Ҫ��ѯ�ļҵ�����: ";
            cin >> name;
            cout << "������Ʒ��: ";
            cin >> brand;
            searchNode(name, brand);
            break;
        case 3:
            cout << "������ҵ�����: ";
            cin >> name;
            cout << "������Ʒ��: ";
            cin >> brand;
            validateInput(price, "�����뵥��: ");
            validateInput(quantity, "����������: ");
            insertNode(name, brand, price, quantity);
            logAction("����: " + name + " (" + brand + ")");
            break;
        case 4:
            cout << "������Ҫ����ļҵ�����: ";
            cin >> name;
            cout << "������Ʒ��: ";
            cin >> brand;
            validateInput(quantity, "�������������: ");
            updateOrDeleteNode(name, brand, quantity);
            logAction("���: " + name + " (" + brand + "), ����: " + to_string(quantity));
            break;
        case 5:
            cout << "������Ҫ�޸ĵļҵ�����: ";
            cin >> name;
            cout << "������Ʒ��: ";
            cin >> brand;
            validateInput(price, "�������µĵ���: ");
            validateInput(quantity, "�������µ�����: ");
            updateNode(name, brand, price, quantity);
            logAction("�޸�: " + name + " (" + brand + ")");
            break;
        case 6:
            calculateTotalValue();
            break;
        case 7:
            cout << "������Ʒ������: ";
            cin >> brand;
            countByBrand(brand);
            break;
        case 8:
            saveToFile();
            cout << "�����ѱ��棬�����˳���" << endl;
            break;
        default:
            cout << "��Чѡ�������ѡ��" << endl;
        }
    } while (option != 8);
}

// �����û�����
void loadUsers()
{
    ifstream file("D:\\DeskTop\\KCSJ\\Good\\user.txt");
    if (!file)
        return;

    User user;
    while (file >> user.username >> user.password)
    {
        users.push_back(user);
    }
    file.close();
}

// ע�����û�
void registerUser()
{
    User newUser;
    cout << "�������û���: ";
    cin >> newUser.username;
    cout << "����������: ";
    cin >> newUser.password;

    users.push_back(newUser);
    ofstream file("D:\\DeskTop\\KCSJ\\Good\\user.txt", ios::app);
    file << newUser.username << " " << newUser.password << endl;
    file.close();
    cout << "ע��ɹ���" << endl;
}

// �û���¼
bool loginUser()
{
    string username, password;
    cout << "�������û���: ";
    cin >> username;
    cout << "����������: ";
    cin >> password;

    for (const auto &user : users)
    {
        if (user.username == username && user.password == password)
        {
            currentUser = username;
            cout << "��¼�ɹ���" << endl;
            return true;
        }
    }
    return false;
}

// ��¼������־
void logAction(const string &action)
{
    ofstream file("D:\\DeskTop\\KCSJ\\Good\\log.txt", ios::app);
    if (!file)
        return;

    time_t now = time(0);
    file << "�û�: " << currentUser << ", ����: " << action << ", ʱ��: " << ctime(&now);
    file.close();
}

// ������֤����������
void validateInput(float &value, const string &prompt)
{
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (value >= 0)
            break;
        cout << "������Ч�����������룡" << endl;
    }
}

// ������֤��������
void validateInput(int &value, const string &prompt)
{
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (value >= 0)
            break;
        cout << "������Ч�����������룡" << endl;
    }
}

// ������ʼ���
void createList()
{
    cout << "����ʹ�ã������ó�ʼ��档" << endl;
    int n;
    cout << "������ҵ���������: ";
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        string name, brand;
        float price;
        int quantity;

        cout << "������� " << i + 1 << " �ּҵ������: ";
        cin >> name;
        cout << "������Ʒ��: ";
        cin >> brand;
        validateInput(price, "�����뵥��: ");
        validateInput(quantity, "����������: ");

        insertNode(name, brand, price, quantity);
    }
}

// ���ļ���������
void loadFromFile()
{
    ifstream file("D:\\DeskTop\\KCSJ\\Good\\new.txt");
    if (!file)
    {
        cout << "�ļ������ڣ����ȴ�����棡" << endl;
        return;
    }

    string name, brand;
    float price;
    int quantity;

    while (file >> name >> brand >> price >> quantity)
    {
        insertNode(name, brand, price, quantity);
    }
    file.close();
    cout << "�����Ѵ��ļ����أ�" << endl;
}

// �������ݵ��ļ�
void saveToFile()
{
    ofstream file("D:\\DeskTop\\KCSJ\\Good\\new.txt");
    Node *current = head;

    while (current != nullptr)
    {
        file << current->name << " " << current->brand << " " << current->price << " " << current->quantity << endl;
        current = current->next;
    }
    file.close();
    cout << "�����ѱ��浽�ļ���" << endl;
}

// �����½ڵ㣨������
void insertNode(string name, string brand, float price, int quantity)
{
    Node *current = head;

    // ����Ƿ��Ѵ�����ͬ�����ƺ�Ʒ��
    while (current != nullptr)
    {
        if (current->name == name && current->brand == brand)
        {
            current->quantity += quantity; // ��������
            cout << "�Ѵ�����ͬ�ҵ磬�����Ѹ��£�" << endl;
            return;
        }
        current = current->next;
    }

    // �����½ڵ�
    Node *newNode = new Node(name, brand, price, quantity);

    if (head == nullptr || head->price > price)
    {
        newNode->next = head;
        head = newNode;
    }
    else
    {
        current = head;
        while (current->next != nullptr && current->next->price < price)
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    cout << "�����ɹ���" << endl;
}

// ���»�ɾ���ڵ㣨�����
void updateOrDeleteNode(string name, string brand, int quantity)
{
    Node *current = head;
    Node *prev = nullptr;

    while (current != nullptr && (current->name != name || current->brand != brand))
    {
        prev = current;
        current = current->next;
    }

    if (current == nullptr)
    {
        cout << "δ�ҵ��üҵ磡" << endl;
        return;
    }

    if (current->quantity > quantity)
    {
        current->quantity -= quantity;
        cout << "����ɹ���ʣ������: " << current->quantity << endl;
    }
    else
    {
        if (prev == nullptr)
        {
            head = current->next;
        }
        else
        {
            prev->next = current->next;
        }
        delete current;
        cout << "����ɹ����üҵ��Ѵӿ�����Ƴ���" << endl;
    }
}

// ��ѯ�ҵ���Ϣ
void searchNode(string name, string brand)
{
    Node *current = head;

    while (current != nullptr && (current->name != name || current->brand != brand))
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        cout << "δ�ҵ��üҵ磡" << endl;
    }
    else
    {
        cout << "�ҵ�����: " << current->name << ", Ʒ��: " << current->brand
             << ", ����: " << current->price << ", ����: " << current->quantity << endl;
    }
}

// �޸ļҵ���Ϣ
void updateNode(string name, string brand, float newPrice, int newQuantity)
{
    Node *current = head;

    while (current != nullptr && (current->name != name || current->brand != brand))
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        cout << "δ�ҵ��üҵ磡" << endl;
    }
    else
    {
        current->price = newPrice;
        current->quantity = newQuantity;
        cout << "�ҵ���Ϣ�Ѹ��£�" << endl;
    }
}

// ��ӡ����
void printList()
{
    Node *current = head;

    if (current == nullptr)
    {
        cout << "���Ϊ�գ�" << endl;
        return;
    }

    cout << "\n===== �����Ϣ =====" << endl;
    while (current != nullptr)
    {
        cout << "�ҵ�����: " << current->name << ", Ʒ��: " << current->brand
             << ", ����: " << current->price << ", ����: " << current->quantity << endl;
        current = current->next;
    }
}

// ͳ�ƿ���ܼ�ֵ
void calculateTotalValue()
{
    Node *current = head;
    float totalValue = 0;

    while (current != nullptr)
    {
        totalValue += current->price * current->quantity;
        current = current->next;
    }

    cout << "����ܼ�ֵ: " << totalValue << " Ԫ" << endl;
}

// ��Ʒ��ͳ�Ƽҵ�����
void countByBrand(string brand)
{
    Node *current = head;
    int totalQuantity = 0;

    while (current != nullptr)
    {
        if (current->brand == brand)
        {
            totalQuantity += current->quantity;
        }
        current = current->next;
    }

    cout << "Ʒ�� " << brand << " �ļҵ�������: " << totalQuantity << endl;
}