#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;


class list
{
public:
	string name;
	int age;
	float nit;
	list(string n, int a,float t)
	{
		name = n;
		age = a;
		nit = t;
	}
	list(string n)
	{
		name = n;
		age = 18;
		nit = 1.1;
	}
	list()
	{
		name = "ave";
		age = 18;
		nit = 3.9;
	}
	void move()
	{
		cout << name << " is moving" << endl;
	}
};
int main()
{
	list dendy("dendy", 1, 1.0);
	cout << "Name: " << dendy.name << "\tAge: " << dendy.age << "\tnit: "<< dendy.nit << endl;
	list rebe("rebe");
	cout << "Name: " << rebe.name << "\tAge: " << rebe.age << "\tnit: " <<rebe.nit << endl;
	list ave = list();
	cout << "Name: " << ave.name << "\tAge: " << ave.age << "\tnit: " <<ave.nit << endl;
	system("pause");
	return 0;
}