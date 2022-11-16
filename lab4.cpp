#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <random>
#include <ctime>
#include <algorithm>
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
template <class RandomAccessIterator> // шаблонный класс итератора произвольного доступа
void hoaraQuickSort(RandomAccessIterator first, RandomAccessIterator last, // принимаем на вход итераторы начала и конца контейнера ...
    Comparator<typename iterator_traits<RandomAccessIterator>::value_type>& comp) // ... и класс Comparator IT Вспомогательная структура-шаблон, используется для указания всех критических определений типов, которые должен иметь итератор.
{
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type; // переобозначаем тип значения для краткости, RAI позволяет получить доступ к произвольному элементы диапазона по индексу

    auto i = first, j = last - 1; // инициализируем переменные цикла, auto используется, так как неизвестен тип, он выводится из выражения инициализации в объявлении.
    value_type x = *(first + (last - first) / 2); // значение среднего элемента
    do {
        while (comp(*i, x)) // пока i-ый элемент меньше среднего ...
            ++i; // ... увеличиваем i
        while (comp(x, *j)) // аналогично с j-ым элементом
            --j;

        if (i <= j) {
            if (comp(*j, *i)) // если i-ый и j-ый элементы неупорядочены ...
                swap(*i, *j); // ... меняем их местами
            ++i;
            --j;
        }
    } while (i <= j); // разделили две половины массива ...

    if (i < last - 1)
        hoaraQuickSort(i, last, comp); // ... теперь сортируем правую половину ...
    if (first < j)
        hoaraQuickSort(first, j + 1, comp); // ... и левую
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
        _tree.push_back(static_cast<T>(0)); //sc осуществляет преобразование типов данных
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

    void sift_up(int v) // проталкивание наверх
    {
        if (v == 1)
            return;

        if (_comp(_tree[v / 2], _tree[v])) {
            swap(_tree[v], _tree[v / 2]);
            sift_up(v / 2);
        }
    }

    void sift_down(int v) // проталкивание вниз
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
    mt19937 gen(static_cast<unsigned int>(time(nullptr))); //генератор случайных чисел
    uniform_int_distribution<int> dist_n(10000, 20000);
    uniform_int_distribution<int> dist(-100000, 100000);

    size_t n = dist_n(gen);
    vector<int> arr;
    generate_n(back_inserter(arr), n, [&dist, &gen]() { return dist(gen); });

    IntComparator comparator;
    hoaraQuickSort(arr.begin(), arr.end(), comparator);

    bool sorted = true;
    for (auto i = arr.begin() + 1; i != arr.end(); ++i)
        if (*(i - 1) > *i)
            sorted = false;

    cout << (sorted ? "Array is sorted\n" : "Array isn't sorted\n");

// тесты очереди с приоритетом
    size_t n2 = dist_n(gen);
    vector<int> arr2;
    generate_n(back_inserter(arr2), n2, [&dist, &gen]() { return dist(gen); });

    PriorityQueue<int> pq(comparator);

    for (auto i : arr2)
        pq.push(i);

    for (auto& i : arr2) {
        i = pq.peek();
        pq.poll();
    }

    pq.free();

    sorted = true;
    for (auto i = arr.begin() + 1; i != arr.end(); ++i)
        if (*(i - 1) > *i)
            sorted = false;

    cout << (sorted ? "Array 2 is sorted\n" : "Array 2 isn't sorted\n");
}
