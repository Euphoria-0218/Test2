#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

// 定义链表节点结构
struct Node
{
    string name;  // 家电名称
    string brand; // 品牌
    float price;  // 单价
    int quantity; // 数量
    Node *next;   // 指向下一个节点的指针

    // 构造函数
    Node(string n, string b, float p, int q) : name(n), brand(b), price(p), quantity(q), next(nullptr) {}
};

// 定义用户结构
struct User
{
    string username;
    string password;
};

// 全局变量
Node *head = nullptr;
vector<User> users;
string currentUser;

// 函数声明
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

// 主函数
int main()
{
    loadUsers(); // 加载用户数据

    // 用户登录或注册
    int choice;
    cout << "===== 家电库存管理系统 =====" << endl;
    cout << "1. 登录\n2. 注册\n请选择操作: ";
    cin >> choice;

    if (choice == 1)
    {
        if (!loginUser())
        {
            cout << "登录失败，程序退出！" << endl;
            return 0;
        }
    }
    else if (choice == 2)
    {
        registerUser();
        if (!loginUser())
        {
            cout << "登录失败，程序退出！" << endl;
            return 0;
        }
    }
    else
    {
        cout << "无效选项，程序退出！" << endl;
        return 0;
    }

    // 是否初次使用
    char initChoice;
    cout << "是否是初次使用？(y/n): ";
    cin >> initChoice;
    if (initChoice == 'y' || initChoice == 'Y')
    {
        createList(); // 初次使用，创建库存
    }
    else
    {
        loadFromFile(); // 非初次使用，从文件加载数据
    }

    // 进入菜单
    menu();
    return 0;
}

// 菜单函数
void menu()
{
    int option;
    do
    {
        cout << "\n===== 家电库存管理系统 =====" << endl;
        cout << "1. 打印库存信息" << endl;
        cout << "2. 查询家电信息" << endl;
        cout << "3. 进货" << endl;
        cout << "4. 提货" << endl;
        cout << "5. 修改家电信息" << endl;
        cout << "6. 统计库存总价值" << endl;
        cout << "7. 按品牌统计家电数量" << endl;
        cout << "8. 保存并退出" << endl;
        cout << "请选择操作: ";
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
            cout << "请输入要查询的家电名称: ";
            cin >> name;
            cout << "请输入品牌: ";
            cin >> brand;
            searchNode(name, brand);
            break;
        case 3:
            cout << "请输入家电名称: ";
            cin >> name;
            cout << "请输入品牌: ";
            cin >> brand;
            validateInput(price, "请输入单价: ");
            validateInput(quantity, "请输入数量: ");
            insertNode(name, brand, price, quantity);
            logAction("进货: " + name + " (" + brand + ")");
            break;
        case 4:
            cout << "请输入要提货的家电名称: ";
            cin >> name;
            cout << "请输入品牌: ";
            cin >> brand;
            validateInput(quantity, "请输入提货数量: ");
            updateOrDeleteNode(name, brand, quantity);
            logAction("提货: " + name + " (" + brand + "), 数量: " + to_string(quantity));
            break;
        case 5:
            cout << "请输入要修改的家电名称: ";
            cin >> name;
            cout << "请输入品牌: ";
            cin >> brand;
            validateInput(price, "请输入新的单价: ");
            validateInput(quantity, "请输入新的数量: ");
            updateNode(name, brand, price, quantity);
            logAction("修改: " + name + " (" + brand + ")");
            break;
        case 6:
            calculateTotalValue();
            break;
        case 7:
            cout << "请输入品牌名称: ";
            cin >> brand;
            countByBrand(brand);
            break;
        case 8:
            saveToFile();
            cout << "数据已保存，程序退出！" << endl;
            break;
        default:
            cout << "无效选项，请重新选择！" << endl;
        }
    } while (option != 8);
}

// 加载用户数据
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

// 注册新用户
void registerUser()
{
    User newUser;
    cout << "请输入用户名: ";
    cin >> newUser.username;
    cout << "请输入密码: ";
    cin >> newUser.password;

    users.push_back(newUser);
    ofstream file("D:\\DeskTop\\KCSJ\\Good\\user.txt", ios::app);
    file << newUser.username << " " << newUser.password << endl;
    file.close();
    cout << "注册成功！" << endl;
}

// 用户登录
bool loginUser()
{
    string username, password;
    cout << "请输入用户名: ";
    cin >> username;
    cout << "请输入密码: ";
    cin >> password;

    for (const auto &user : users)
    {
        if (user.username == username && user.password == password)
        {
            currentUser = username;
            cout << "登录成功！" << endl;
            return true;
        }
    }
    return false;
}

// 记录操作日志
void logAction(const string &action)
{
    ofstream file("D:\\DeskTop\\KCSJ\\Good\\log.txt", ios::app);
    if (!file)
        return;

    time_t now = time(0);
    file << "用户: " << currentUser << ", 操作: " << action << ", 时间: " << ctime(&now);
    file.close();
}

// 数据验证（浮点数）
void validateInput(float &value, const string &prompt)
{
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (value >= 0)
            break;
        cout << "输入无效，请重新输入！" << endl;
    }
}

// 数据验证（整数）
void validateInput(int &value, const string &prompt)
{
    while (true)
    {
        cout << prompt;
        cin >> value;
        if (value >= 0)
            break;
        cout << "输入无效，请重新输入！" << endl;
    }
}

// 创建初始库存
void createList()
{
    cout << "初次使用，请设置初始库存。" << endl;
    int n;
    cout << "请输入家电种类数量: ";
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        string name, brand;
        float price;
        int quantity;

        cout << "请输入第 " << i + 1 << " 种家电的名称: ";
        cin >> name;
        cout << "请输入品牌: ";
        cin >> brand;
        validateInput(price, "请输入单价: ");
        validateInput(quantity, "请输入数量: ");

        insertNode(name, brand, price, quantity);
    }
}

// 从文件加载数据
void loadFromFile()
{
    ifstream file("D:\\DeskTop\\KCSJ\\Good\\new.txt");
    if (!file)
    {
        cout << "文件不存在，请先创建库存！" << endl;
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
    cout << "数据已从文件加载！" << endl;
}

// 保存数据到文件
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
    cout << "数据已保存到文件！" << endl;
}

// 插入新节点（进货）
void insertNode(string name, string brand, float price, int quantity)
{
    Node *current = head;

    // 检查是否已存在相同的名称和品牌
    while (current != nullptr)
    {
        if (current->name == name && current->brand == brand)
        {
            current->quantity += quantity; // 更新数量
            cout << "已存在相同家电，数量已更新！" << endl;
            return;
        }
        current = current->next;
    }

    // 插入新节点
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
    cout << "进货成功！" << endl;
}

// 更新或删除节点（提货）
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
        cout << "未找到该家电！" << endl;
        return;
    }

    if (current->quantity > quantity)
    {
        current->quantity -= quantity;
        cout << "提货成功！剩余数量: " << current->quantity << endl;
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
        cout << "提货成功！该家电已从库存中移除。" << endl;
    }
}

// 查询家电信息
void searchNode(string name, string brand)
{
    Node *current = head;

    while (current != nullptr && (current->name != name || current->brand != brand))
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        cout << "未找到该家电！" << endl;
    }
    else
    {
        cout << "家电名称: " << current->name << ", 品牌: " << current->brand
             << ", 单价: " << current->price << ", 数量: " << current->quantity << endl;
    }
}

// 修改家电信息
void updateNode(string name, string brand, float newPrice, int newQuantity)
{
    Node *current = head;

    while (current != nullptr && (current->name != name || current->brand != brand))
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        cout << "未找到该家电！" << endl;
    }
    else
    {
        current->price = newPrice;
        current->quantity = newQuantity;
        cout << "家电信息已更新！" << endl;
    }
}

// 打印链表
void printList()
{
    Node *current = head;

    if (current == nullptr)
    {
        cout << "库存为空！" << endl;
        return;
    }

    cout << "\n===== 库存信息 =====" << endl;
    while (current != nullptr)
    {
        cout << "家电名称: " << current->name << ", 品牌: " << current->brand
             << ", 单价: " << current->price << ", 数量: " << current->quantity << endl;
        current = current->next;
    }
}

// 统计库存总价值
void calculateTotalValue()
{
    Node *current = head;
    float totalValue = 0;

    while (current != nullptr)
    {
        totalValue += current->price * current->quantity;
        current = current->next;
    }

    cout << "库存总价值: " << totalValue << " 元" << endl;
}

// 按品牌统计家电数量
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

    cout << "品牌 " << brand << " 的家电总数量: " << totalQuantity << endl;
}