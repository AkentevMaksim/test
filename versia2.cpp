

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

// базовый класс элемента связного списка
struct ClassBase {
	ClassBase *prev; // указатель на предыдущий элемент связного списка
	ClassBase *next; // указатель на следующий элемент связного списка



	// при создании объекта класса конструктор по-умолчанию
	// назначает следующий и предыдущий элемент указывать на себя 
	ClassBase() : prev(this), next(this) {}


	// при создании с таким констуктором, элемент добавляет сам себя
	// в связный список, представляя себя предыдущему и следующему элементу
	ClassBase(ClassBase *prev_, ClassBase *next_)
		: prev(prev_), next(next_) { // назначаются предыдущий и следующий элементы
		// у предыдущего элемента следующим элементом назначается данный элемент
		// и у следующего элемента предыдущим элементом назначается данный элемент
		prev->next = next->prev = this;
	}
	// деструктор. при удалении элемента связного списка, 
	// элемент убирает сам себя из списка
	virtual ~ClassBase() {
		prev->next = next;
		next->prev = prev;
	}
};




// элемент связного списка, уже содержащий данные.
template <typename ValueType>
struct Node : public ClassBase {
	ValueType value; // то значение, которое хранит класс
	// конструктор подставляет значения и вызывает конструтор предка
	Node(ClassBase *prev_, ClassBase *next_, ValueType value)
		: ClassBase(prev_, next_), value(value) {}
};




// итератор. класс, который позволит получать доступ к данным,
// сохранив инкапсуляцию
template <typename ValueType>
struct Iterator {
public:
	// создание итератора при помощи элемента связного списка
	explicit Iterator(ClassBase *node_) : node(node_) {}
	// создание копированием с другого итератора
	Iterator(const Iterator &other) : node(other.node) {}
	// получение значения элемента связного списка
	ValueType &operator*() {
		return static_cast<Node<ValueType>*>(node)->value;
	}
	// присваивание итератору другого итератора
	Iterator &operator=(const Iterator &other) {
		if (&other != this)
			node = other.node;
		return *this;
	}
	// переход к следующему элементу (постфиксный)
	Iterator &operator++() {
		node = node->next;
		return *this;
	}
	// переход к предыдущему элементу (постфиксный)
	Iterator &operator--() {
		node = node->prev;
		return *this;
	}
	// проверка итераторов на равенство
	bool operator==(const Iterator &other) {
		return node == other.node;
	}
	// ... на неравенство
	bool operator!=(const Iterator &other) {
		return node != other.node;
	}
private:
	// собственно, элемент связного спискa, хранит итератор
	ClassBase *node;
};





// собственно, шаблон двусвязного списка
// смотрите, как мало собственно кода, если убрать комментарии
template <typename ValueType>
class List {
public:
	// конструктору по-умолчанию делать в принципе нечего, он должен быть объявлен
	List() : base() {};
	// деструктор удаляет список функцией Clear
	~List() {
		Clear();
	}
	// функция проверяет, пустой ли список
	bool Empty() {
		// список пустой, если базовый элемент указывает сам на себя
		return ((base.next == &base) && (base.prev == &base));
	}
	void Clear() {
		// пока список не пуст
		while (!Empty())
			// удаляется первый элемент
			// работу по удалению из списка сделает деструктор класса элемента
			delete base.next;
	}
	// добавление элемента в конец списка
	void PushBack(const ValueType &value) {
		// просто создается новый элемент списка,
		// всё остальное сделает конструктор класса ClassBase
		new Node<ValueType>(base.prev, &base, value);
	}
	// удаление последнего элемента
	void PopBack() {
		// удаляется элемент связного списка 
		// (в данном случае последний, но в принципе, сработает с любым)
		// работу по фактическому удалению элемента из списка выполнит деструктор
		// класса ClassBase
		delete base.prev;
	}
	// плохой стиль, лучше делать через итераторы, но эта концепция, наверно,
	// слишком сложна
	void PrintAll() {
		// перебор всех элементов в одном цикле
		for (ClassBase *node = base.next; node != &base; node = node->next)
			// для получения значения элемент списка приводится к типу Node*
			std::cout << static_cast<Node<ValueType>*>(node)->value;
	}
	// печать данных по условию
	template <class Predicate> // класс или функция, принимающие параметром
							   // ValueType и возвращающая true или false.
	void PrintIf(Predicate condition) {
		// цикл по всем элементам массива
		for (ClassBase *node = base.next; node != &base; node = node->next)
			// если условие выполняется
			if (condition(static_cast<Node<ValueType>*>(node)->value))
				// вывести значение
				std::cout << static_cast<Node<ValueType>*>(node)->value;
	}

	// итератор на начало связного списка
	Iterator<ValueType> Begin() {
		// началом связного списка является элемент, на который указывает
		// поле next переменной base
		return Iterator<ValueType>(base.next);
	}
	// итератор на элемент связного списка _после_ последнего
	Iterator<ValueType> End() {
		// элементом за связным списком является само поле base
		return Iterator<ValueType>(&base);
	}
	// поиск элемента в списке
	// принимает параметрами первый элемент и последний элемент, в
	// которых надо искать список
	// и функтор, принимающий значение элемента и возвращающая true или
	// false
	// возвращает итератор найденного элемента или последний параметр
	// если элемента не найдено
	template <class Predicate>
	Iterator<ValueType> Search(Iterator<ValueType> begin,
		Iterator<ValueType> end,
		Predicate condition) {
		for (; begin != end; ++begin)
			if (condition(*begin)) break;
		return begin;
	}
private:
	// базовый эелемент.
	// его поле next указывает на первый элемент списка
	// поле prev указываеты на последний элемент списка
	// если список пуст, next == prev == &base
	ClassBase base;
};

// класс автобуса
struct Spisok {
	int number;
	std::string stroka;
	float veshestvennoe;
	bool zadannoe;
	//... тут наверно нужны другие поля
	Spisok(int number_, std::string stroka_, float veshestvennoe_, bool zadannoe_)
		: number(number_), stroka(stroka_), veshestvennoe(veshestvennoe_), zadannoe(zadannoe_) {}  // переопределенный оператор для вывода на экран
	friend std::ostream& operator<<(std::ostream &stream, const Spisok &Spisok) {
		return stream << "Числовое: " << Spisok.number
			<< ", Строковое: " << Spisok.stroka
			<< ", Вещественное: " << Spisok.veshestvennoe
			<< ", Заданное: " << (Spisok.zadannoe ? "zadannoe_first" : "zadannoe_second")
			<< std::endl;
	}
	// класс-функтор для поиска по номеру
	struct IsNumber {
		// запоминаем параметр в классе
		IsNumber(int parameter_) : parameter(parameter_) {}
		// оператор-функтор для сравнения параметра
		bool operator()(const Spisok &Spisok) {
			return Spisok.number == parameter;
		}
		int parameter; // параметр
	};
	// класс-функтор для поиска по заданному
	struct Iszadannoe {
		// запоминаем параметр в классе
		Iszadannoe(bool parameter_) : parameter(parameter_) {}
		// оператор-функтор для сравнения параметра
		bool operator()(const Spisok &Spisok) {
			return Spisok.zadannoe == parameter;
		}
		bool parameter; // параметр
	};
	// класс-функтор для поиска по вещественному числу 
	struct Isveshestvennoe {
		// запоминаем параметр в классе
		Isveshestvennoe(int parameter_) : parameter(parameter_) {}
		// оператор-функтор для сравнения параметра
		bool operator()(const Spisok &Spisok) {
			return Spisok.veshestvennoe == parameter;
		}
		int parameter; // параметр
	};
	// класс-функтор для поиска по строке
	struct Isstroka {
		// запоминаем параметр в классе
		Isstroka(const std::string &parameter_) : parameter(parameter_) {}
		// оператор-функтор для сравнения параметра
		bool operator()(const Spisok &Spisok) {
			return Spisok.stroka == parameter;
		}
		std::string parameter; // параметр
	};
};









int main(int argc, char *argv[]) {
	setlocale(LC_CTYPE, "Russian");
	srand(time(NULL));
	List<Spisok> a; // список 
	for (int i = 0; i < 1; ++i)
		a.PushBack(Spisok(i + 1, "an",  2.1, rand() % 2));
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