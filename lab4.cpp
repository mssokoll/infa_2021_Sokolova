#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <random>
#include <ctime>
#include <algorithm>
#include <cstdlib>
using namespace std;

template <typename T>
struct Comparator
{
    virtual bool operator()(T const&, T const&) const = 0;
};

struct IntComparator final : Comparator<int>
{
    bool operator()(int const& lha, int const& rha) const override
    {
        return lha < rha;
    }
};

// быстрая сортировка Хоара
template <typename T>
void hoaraQuickSort(int first, int last, Comparator<T>& comp, T *buf) // принимаем на вход первый и последний элемент, компоратор
{

    auto i = first, j = last-1; // инициализируем переменные цикла, auto используется, так как неизвестен тип, он выводится из выражения инициализации в объявлении.
    T x = buf[(first+last)/2]; //значение среднего элемента
    do {

        while (comp(buf[i], x)) // пока i-ый элемент меньше среднего ...
            ++i; // ... увеличиваем i
        while (comp(x, buf[j])) // аналогично с j-ым элементом
            --j;

        if (i <= j) {
            if (comp(buf[j], buf[i])) // если i-ый и j-ый элементы неупорядочены ...
                swap(buf[i], buf[j]); // ... меняем их местами
            ++i;
            --j;
        }
    } while (i <= j); // разделили две половины массива ...

    if (i < last - 1)
        hoaraQuickSort(i, last, comp, buf); // ... теперь сортируем правую половину ...
    if (first < j)
        hoaraQuickSort(first, j + 1, comp, buf); // ... и левую
}



// очередь с приоритетом
template <typename T>
class PriorityQueue
{
private:
    vector<T> _tree; // массив значений
    Comparator<T>& _comp; // компаратор

public:
    PriorityQueue(Comparator<T>& comp) : _comp(comp) // конструктор по умолчанию, создающий пустую очередь с приоритетом
    {
        _tree.push_back(T);
    }

    void push(T val) // метод push для добавления нового значения в очередь
    {
        _tree.push_back(val);
        sift_up(_tree.size() - 1); //элемент добавляется на то место, где он больше всех предшествующих ему
    }

    T peek() // метод peek для чтения верхнего элемента очереди, но не удаления элемента, возвращает следующий символ в потоке
    {
        if (_tree.size() > 1)
            return _tree[1];
    }

    void poll() // метод poll для удаления верхнего элемента очереди
    {
        if (_tree.size() > 1) {
            _tree[1] = _tree.back();
            _tree.pop_back(); //используется для удаления последнего элемента вектора сзади и уменьшения размера вектора на 1
            sift_down(1);
        }
    }

    void sift_up(int v) // проталкивание наверх, код из википедии
    {
        if (v == 1)
            return;

        if (_comp(_tree[v / 2], _tree[v])) {
            swap(_tree[v], _tree[v / 2]);
            sift_up(v / 2);
        }
    }

    void sift_down(int v) // проталкивание вниз, из википедии
    {
        if (v * 2 >= _tree.size())
            return;

        int max_idx;
        if (v * 2 + 1 == _tree.size())
            max_idx = v * 2;
        else if (!_comp(_tree[v * 2], _tree[v * 2 + 1]))
            max_idx = v * 2;
        else
            max_idx = v * 2 + 1;

        if (_comp(_tree[v], _tree[max_idx])) {
            swap(_tree[v], _tree[max_idx]);
            sift_down(max_idx);
        }
    }

    bool is_empty() // метод is_empty, который возвращает true, если очередь пуста, иначе - false
    {
        return _tree.size() == 1;
    }

    void free() // метод free, который освобождает всю выделенную ранее память
    {
        _tree.clear();
    }
};

int main()
{
// тесты сортировки Хоара


    int n = 10000;
    int *buf = new int[n];

    for (int i = 0; i < n; i++){
        buf[i] = rand();
    }

    IntComparator comparator = IntComparator();

    hoaraQuickSort(0, n, comparator, buf);

    bool sorted = true;

    for (int i = 1; i < n; i++)
        if (buf[i-1] > buf[i])
            sorted = false;


    if (sorted) cout << "yes";
    else cout << "no";

    delete [] buf;


// тесты очереди с приоритетом

    vector<int> arr;

    for (int i = 0; i < n; i++){
        arr[i] = rand();
    }

    PriorityQueue<int> pq(comparator);

    for (int i = 0; i < n; i++)
        pq.push(i);

    for (int i = 0; i < n; i++) {
        i = pq.peek();
        pq.poll();
    }

    pq.free();

    bool wsorted = true;

    for (int i = 1; i < n; i++)
        if (arr[i - 1] > arr[i])
            wsorted = false;

    if (wsorted) cout << "yes";
    else cout << "no";

}
