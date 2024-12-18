#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define SIZE 100

struct Services {
	string TNN = "";
	string name = "";
	string master = "";
	int cost = 0;
	void clear() {
		TNN = "";
		name = "";
		master = "";
		cost = 0;
	}
};

class HashTable {
private:
	int count = 0;
	Services array[SIZE];
	int array_i[SIZE]{ 0 };
	bool array_del[SIZE]{ 0 };

	int getIndex(string data) {
		string key = data.substr(1, 2);
		double a = stoi(key) * 0.31415926535;
		return (int)(SIZE * (a - (long)a));
	}

	// Функция построения массива префиксов
	vector<int> build_prefix(const string& pattern) {
		int m = pattern.size();
		vector<int> prefix(m, 0);
		int k = 0;
		for (int q = 1; q < m; ++q) {
			while (k > 0 && pattern[k] != pattern[q])
				k = prefix[k - 1];
			if (pattern[k] == pattern[q])
				++k;
			prefix[q] = k;
		}
		return prefix;
	}

	int KMP(const string& pattern, const string& text) {
		int n = text.size(), m = pattern.size();
		if (m == 0) return -1;
		vector<int> prefix = build_prefix(pattern);
		int q = 0;
		for (int i = 0; i < n; ++i) {
			while (q > 0 && pattern[q] != text[i])
				q = prefix[q - 1];
			if (pattern[q] == text[i])
				++q;
			if (q == m) {
				return i - m + 1;
			}
		}
		return -1;
	}
public:

	int getCount() {
		return count;
	}

	void add(Services data) { //Добавление услуги
		int index = getIndex(data.TNN), i = 1;
		while (array_i[index]) {
			index = (index + i + i * i) % SIZE;
			i++;
			if (i == 5) {
				throw exception("Hashtable is full");
			}
		}
		array[index] = data;
		array_i[index] = i;
		array_del[index] = 0;
		count++;
	}

	void remove(string TNN) { //Удаление элемента
		int index = getIndex(TNN);
		bool found = 0;
		for (int i = 0; i < 5; i++) {
			if (array[index].TNN == TNN) {
				found = 1;
				break;
			}
			index = (index + i + i * i) % SIZE;
		}
		if (!found) {
			throw exception("Элемент не найден\n");
		}
		array_i[index] = 0;
		array[index].clear();
		array_del[index] = 1;
		count--;
	}

	Services* getArray() { //Вывод массива
		Services* arr = new Services[count];
		int i = 0;
		for (int j = 0; j < SIZE; j++) {
			if (array_i[j]) {
				arr[i] = array[j];
				i++;
			}
		}
		return arr;
	}

	void clear() { //Удаление всех данных
		for (int i = 0; i < SIZE; i++) {
			array[i].clear();
			array_i[i] = 0;
			array_del[i] = 0;
		}
		count = 0;
	}

	Services& find(string TNN) { //Поиск по коду услуги
		int index = getIndex(TNN);
		for (int i = 0; i < 5; i++) {
			if (!array_del[index] && array[i].TNN == TNN) {
				return array[index];
			}
			index = (index + i + i * i) % SIZE;
		}
		throw exception("Элемент не найден");
	}

	Services& findByName(string subname) { //Поиск по фрагменту
		for (int index = 0; index < SIZE; index++) {
			if (array[index].name != "" && KMP(subname, array[index].name) != -1) {
				return array[index];
			}
		}
		throw exception("Элемент не найден");
	}
};

template <typename T>
T getInput(string message) { //Проверка вода
	T input;
	while (true) {
		cout << message;
		cin >> input;
		if (cin.fail()) {
			cout << "Неверный ввод\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return input;;
		}
	}
}

void addService(HashTable& oresh) {
	Services serv;
	cout << "Введите код услуги\n";
	cout << "Формат кода - <Символ><1-6><1-6>\n";
	string key;
	while (true) {
		getline(cin, serv.TNN);
		key = serv.TNN.substr(1, 2);
		try {
			int a = stoi(key);
			if (a / 10 > 6 || a % 10 > 6 || a / 10 < 1 || a % 10 < 1)
				cout << "Неверный формат кода\n";
			else
				break;
		}
		catch (exception e) {
			continue;
		}
	}
	cin.ignore();
	cout << "Введите название услуги\n";
	getline(cin, serv.name);
	cout << "Введите ФИО мастера\n";
	getline(cin, serv.master);
	serv.cost = getInput<int>("Введите стоимость в рублях\n");
	try {
		oresh.add(serv);
	}
	catch (exception e) {
		cout << "Невозможно вставить элемент в таблицу\n";
	}
	cout << "\n";
}

void delService(HashTable& oresh) {
	while (true) {
		string code;
		getline(cin, code);
		try {
			oresh.remove(code);
			cout << "Элемент удалён\n";
			break;
		}
		catch (exception e) {
			cout << "Элемент не найден\n";
		}
	}
	cout << "\n";
}

void show(HashTable& oresh) {
	Services* arr = oresh.getArray();
	int size = oresh.getCount();
	for (int i = 0; i < size; i++) {
		cout << "Код услуги: " << arr[i].TNN << "\n";
		cout << "Название услуги: " << arr[i].name << "\n";
		cout << "Мастер: " << arr[i].master << "\n";
		cout << "Стоимость в рублях: " << arr[i].cost << "\n\n";
	}
	if (size == 0)
		cout << "Нет услуг\n";
	delete[] arr;
}

void search(HashTable& oresh) {
	cout << "Введите код услуги\n";
	string key;
	getline(cin, key);
	try {
		Services serv = oresh.find(key);
		cout << "Название услуги: " << serv.name << "\n";
		cout << "Мастер: " << serv.master << "\n";
		cout << "Стоимость в рублях: " << serv.cost << "\n\n";
	}
	catch (exception e) {
		cout << "Не найдено\n";
	}
}

void peaceSearch(HashTable& oresh) {
	cout << "Введите фрагмент названия услуги\n";
	string tfu;
	getline(cin, tfu);
	try {
		Services serv = oresh.findByName(tfu);
		cout << "Код услуги: " << serv.TNN << "\n";
		cout << "Название услуги: " << serv.name << "\n";
		cout << "Мастер: " << serv.master << "\n";
		cout << "Стоимость в рублях: " << serv.cost << "\n\n";
	}
	catch (exception e) {
		cout << "Не найдено\n";
	}
}
	
int main() {
	setlocale(LC_ALL, "RU");
	HashTable oreshnik;
	int mode;
	while (true) {
		cout << ">=========| МЕНЮ |=========<\n";
		cout << ">|1. Добавить услугу|======<\n";
		cout << ">|8. Удалить услугу|=======<\n";
		cout << ">|2. Посмотреть услуги|====<\n";
		cout << ">|9. Стереть данные|=======<\n";
		cout << ">|3. Поиск по коду услуги|=<\n";
		cout << ">|4. Поиск по фрагменту|===<\n";
		cout << ">========|0. Выход|========<\n";
		cout << endl;
		mode = getInput<int>(">|Введите цифру для выбора|<\n");
		switch (mode) {
		case 1:
			addService(oreshnik);
			break;
		case 8:
			delService(oreshnik);
			break;
		case 2:
			show(oreshnik);
			break;
		case 9:
			oreshnik.clear();
			break;
		case 3:
			search(oreshnik);
			break;
		case 4:
			peaceSearch(oreshnik);
			break;
		case 0:
			return 0;
		}
	}
}