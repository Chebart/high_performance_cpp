# Лабораторная работа №1 

## Задание

Мой вариант - 506020 % 3 = 1 (Хэш таблица с цепочками)

## Ход работы:

### Собственная реализация хэш-таблицы

ПО аналогии с std::unordered_map была реализован собственная хэш-таблица(./src/own_implementation/)

### Тестирование

Тестирование проводилось как для std::unordered_map, так и для собственной реализации на операциях insert, find, erase, используя библиотеку https://github.com/google/benchmark и std::chrono.

Распределение данных по хэш-таблицам моделировались с помощью 3х функций:

1) Константная функция
```cpp
struct BadHash {
    std::size_t operator()(int) const {
        return 42; 
    }
};
```

2) Функция, задающася двумя переменными A и p, где p - случайное большое простое, A - случайное число из [1; p-1]
```cpp
// ((key * A) % p) % m
struct MediumHash {
    static constexpr std::size_t p = 1'048'583;
    static constexpr std::size_t A = 786433;

    std::size_t operator()(int key) const {
        return (static_cast<std::size_t>(key) * A) % p;
    }
};
```

3) Хэш-функция из стандартной библиотеки
```cpp
struct GoodHash {
    std::size_t operator()(int key) const {
        return std::hash<int>{}(key);
    }
};
```

Далее сравним распределение задержек операций

- erase

![insert](./plots/erase_7BadHash.png)

![insert](./plots/erase_10MediumHash.png)

![insert](./plots/erase_8GoodHash.png)

- insert

![insert](./plots/insert_7BadHash.png)

![insert](./plots/insert_10MediumHash.png)

![insert](./plots/insert_8GoodHash.png)

- find

![insert](./plots/find_7BadHash.png)

![insert](./plots/find_10MediumHash.png)

![insert](./plots/find_8GoodHash.png)


Видно что для всех операций количество коллизий сильно влияет на время итерации. Графики сгенерированы на основе полученных задержек с помощью chrono

### Сэмплирование

Сэмплирование было произведено через утилиту perf

```bash
perf record -F 99 -g -- "${BENCH}" --benchmark_filter="${op}"
```

результат был сохранён в perf.data

Затем через утилиту https://github.com/brendangregg/FlameGraph был получен интерактивный svg файл с flamegraph'ом 

```bash
perf script | "${FLAMEGRAPH_DIR}/stackcollapse-perf.pl" | \
          "${FLAMEGRAPH_DIR}/flamegraph.pl" > "${PLOTS_DIR}/flame_${op,,}.svg"
```

Флеймграф для каждой операции собственной хэш-таблицы представлены ниже

- erase
![erase](./plots/flame_erase.svg)

- insert
![insert](./plots/flame_insert.svg)

- find
![find](./plots/flame_find.svg)