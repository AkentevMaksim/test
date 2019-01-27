
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
// базовый класс элемента связного списка
struct ClassBase {
	ClassBase *prev; // указатели
	ClassBase *next;

	//  конструктор
	ClassBase() : prev(this), next(this) {}
	ClassBase(ClassBase *prev_, ClassBase *next_) : prev(prev_), next(next_) { prev->next = next->prev = this; }

	// деструктор
	virtual ~ClassBase() { prev->next = next; next->prev = prev; }
};


// элемент связного списка,  содержащий данные.
template <typename ValueType>
struct Node : public ClassBase {
	ValueType value;
	Node(ClassBase *prev_, ClassBase *next_, ValueType value)
		: ClassBase(prev_, next_), value(value) {}
};


template <typename ValueType>
struct Iterator {
public:
	explicit Iterator(ClassBase *node_) : node(node_) {}
	Iterator(const Iterator &other) : node(other.node) {}

	ValueType &operator*() { return static_cast<Node<ValueType>*>(node)->value; }

	Iterator &operator=(const Iterator &other) { if (&other != this)	node = other.node;	return *this; }

	Iterator &operator++() { node = node->next; return *this; }

	Iterator &operator--() { node = node->prev; 	return *this; }

	bool operator==(const Iterator &other) { return node == other.node; }

	bool operator!=(const Iterator &other) { return node != other.node; }

private: ClassBase *node;

};


template <typename ValueType>
class List {
public:
	List() : base() {};
	~List() { Clear(); }
	bool Empty() { return ((base.next == &base) && (base.prev == &base)); }
	void Clear() { while (!Empty() delete base.next; }

	void PushBack(const ValueType &value) { new Node<ValueType>(base.prev, &base, value); }
	void PopBack() { delete base.prev; }
	void PrintAll() {
		for (ClassBase *node = base.next; node != &base; node = node->next)
			std::cout << static_cast<Node<ValueType>*>(node)->value;
	}
	template <class Predicate>
	void PrintIf(Predicate condition)
	{
		for (ClassBase *node = base.next; node != &base; node = node->next)
			if (condition(static_cast<Node<ValueType>*>(node)->value))
				std::cout << static_cast<Node<ValueType>*>(node)->value;
	}


	Iterator<ValueType> Begin() { return Iterator<ValueType>(base.next); }
	Iterator<ValueType> End() { return Iterator<ValueType>(&base); }

	template <class Predicate>
	Iterator<ValueType> Search(Iterator<ValueType> begin,
		Iterator<ValueType> end,
		Predicate condition) {
		for (; begin != end; ++begin)
			if (condition(*begin)) break;
		return begin;
	}
private:  ClassBase base;
};

// класс список
struct Spisok {
	int number;
	std::string stroka;
	float veshestvennoe;
	bool zadannoe;

	Spisok(int number_, std::string stroka_, float veshestvennoe_, bool zadannoe_)
		: number(number_), stroka(stroka_), veshestvennoe(veshestvennoe_), zadannoe(zadannoe_) {}
	friend std::ostream& operator<<(std::ostream &stream, const Spisok &Spisok) {
		return stream << "Числовое: " << Spisok.number << ", Строковое: " << Spisok.stroka << ", Вещественное: " << Spisok.veshestvennoe
			<< ", Заданное: " << (Spisok.zadannoe ? "zadannoe_first" : "zadannoe_second") << std::endl;
	}

	struct IsNumber
	{
		IsNumber(int parameter_) : parameter(parameter_) {}
		bool operator()(const Spisok &Spisok) { return Spisok.number == parameter; }
		int parameter;
	};


	struct Iszadannoe
	{

		Iszadannoe(bool parameter_) : parameter(parameter_) {}
		bool operator()(const Spisok &Spisok) { return Spisok.zadannoe == parameter; }
		bool parameter;
	};
	-
		struct Isveshestvennoe
	{
		Isveshestvennoe(int parameter_) : parameter(parameter_) {}
		bool operator()(const Spisok &Spisok) { return Spisok.veshestvennoe == parameter; }
		int parameter;
	};

	struct Isstroka
	{
		Isstroka(const std::string &parameter_) : parameter(parameter_) {}
		bool operator()(const Spisok &Spisok) { return Spisok.stroka == parameter; }
		std::string parameter;
	};
};



int main(int argc, char *argv[]) {
	setlocale(LC_CTYPE, "Russian");
	srand(time(NULL));
	List<Spisok> a; // список 
	for (int i = 0; i < 1; ++i)
		a.PushBack(Spisok(i + 1, "an", 2.1, rand() % 2));
	for (int i = 1; i < 4; ++i)
		a.PushBack(Spisok(i + 1, "an1", 2.2, rand() % 2));
	for (int i = 4; i < 7; ++i)
		a.PushBack(Spisok(i + 1, "an2", 2.5, rand() % 2));
	// вывод его на экран
	a.PrintAll();
	std::cout << "вывели все на экран" << "\n";



	std::cout << "по числовому" << "\n";
	Iterator<Spisok> b = a.Search(a.Begin(), a.End(), Spisok::IsNumber(5));
	if (b != a.End())
		std::cout << *b;
	std::cout << "\n";


	std::cout << "по строке " << "\n";
	Iterator<Spisok> c = a.Search(a.Begin(), a.End(), Spisok::Isstroka("an"));
	if (c != a.End())
		std::cout << *c;
	std::cout << "\n";



	system("PAUSE");
	return 0;
};