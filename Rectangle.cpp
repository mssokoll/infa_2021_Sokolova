#include <iostream>
#include <string>
#include <sstream>
using namespace std;


struct Point {
	unsigned long long const x, y;
	Point (unsigned long long x, unsigned long long y): x(x), y(y){}

	Point minx(Point const& rha) const {
		return Point(rha.x < x ? rha.x : x, y);}

	Point miny(Point const& rha) const {
		return Point(x, rha.y < y ? rha.y : y);}

	Point maxx(Point const & rha) const {
	return Point(rha.x > x ? rha.x : x, y);}

	Point maxy(Point const & rha) const {
	return Point(x, rha.y > y ? rha.y : y);}

	void print() const {
		cout << '(' << x << "," << y << ')';
	}
};

struct Rectangle
{
	Point a;
	Rectangle():Rectangle(Point(0,0)){}
	Rectangle(Point const& c):a(c){}

	Rectangle operator+(Rectangle const& rha) const
	{
		return Rectangle(a.maxx(rha.a).maxy(rha.a));
	}

	Rectangle operator*(Rectangle const& rha) const
	{
		return Rectangle(a.minx(rha.a).miny(rha.a));
	}

	void print() const
	{
		a.print();
	}
};

Rectangle strinrect(string str) //преобразует строку в прямоугольник
{
	stringstream con; //преобразователь
	int h = str.find(',');
	string hl = str.substr(1, h - 1); //копирование элемента строки длиной h-1 c позиции 1
	string hr = str.substr(h+1, str.size()-2-h); //копирование с h+1-го элемента до правой скобки
	con.clear();
	int x;
	con << hl;
	con >> x;
	con.clear();
	int y;
	con << hr;
	con >> y;
	return Rectangle(Point(x, y)); //Вводим строку, получаем на выходе прямоугольник с координатами х и у
}

string kkkk(Rectangle d) //переводит прямоугольник в строку
{
	stringstream con;
	string h;
	string x;
	string y;
	con.clear();
	con << d.a.x;
	con >> x;
	con.clear();
	con << d.a.y;
	con >> y;
	h = "(" + x + "," + y + ")"; //Наоборот на выходе из двух координат получается строка
	return h;
}



int main()
{
	string ss;
	getline(cin, ss); //По очереди считываются строки
	while (ss.find(" ") != string::npos) // удаляем пробелы
	{
		ss.erase(ss.find(" "), 1);
	}
	while (ss.find("*") != string::npos) // выполняем приоритетные операции и склеиваем строки обратно
	{
		long long int s = ss.find("*");
		long long int i = s;
		long long int j = s;
		while (ss[i] != '(') i--; //Поиск начала координаты х
		while (ss[j] != ')') j++; //Поиск конца координаты у
		Rectangle d1 = strinrect(ss.substr(i, s - i)); //Возвращает координату х (уже в виде координаты)
		Rectangle d2 = strinrect(ss.substr(s+1, j-s));
		Rectangle d = d1 * d2;
		string ds = kkkk(d); //Перевод прямоугольника в строку
		ss = ss.substr(0, i) + ds + ss.substr(j + 1, ss.size() - j - 1); //Склеивание строки из того, что получилось с помощью приоритетных операций, и всего остального
	}
	while (ss.find('+') != string::npos) // аналогично *, только теперь заменяем на +
	{
		long long int s = ss.find('+');
		long long int i = s;
		long long int j = s;
		while (ss[i] != '(') i--;
		while (ss[j] != ')') j++;
		Rectangle d1 = strinrect(ss.substr(i, s - i));
		Rectangle d2 = strinrect(ss.substr(s + 1, j - s));
		Rectangle d = d1+d2;
		string ds = kkkk(d);
		string hh = ss.substr(0, i) + ds + ss.substr(j + 1, ss.size() - j - 1);
    	ss = hh;
	}
	cout << ss;
}

