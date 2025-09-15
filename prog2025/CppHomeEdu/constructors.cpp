#include <iostream>

class Student
{
public:
    std::string name;
    int age;
    double gpa;

    Student(std::string name, int age, double gpa)
    {
        this->name = name;
        this->age = age;
        this->gpa = gpa;
    }
};

int main()
{

    Student student1("Spongebob", 25, 3.2);
    Student student2("Patrik", 40, 1.5);
    Student student3("Sendy", 21, 4.0);

    std::cout << student3.name << '\n';
    std::cout << student3.age << '\n';
    std::cout << student3.gpa << '\n';
    return 0;
}