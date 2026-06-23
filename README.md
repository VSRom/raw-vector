# raw-vector

![CI](https://github.com/VSRom/raw-vector/actions/workflows/ci.yml/badge.svg)

## Особенности

- Размер контейнера равен размеру одного указателя
- Сырая память: данные, size и capacity упакованы в один `char*` блок
- Placement new — объекты создаются прямо в выделенной памяти
- Автоматическое расширение (capacity удваивается при нехватке места)

---

## Реализовано

### Конструкторы / деструктор
- `raw_vector()`
- `raw_vector(size_t)`
- `raw_vector(std::initializer_list<T>)`
- копирующий / перемещающий конструктор
- деструктор

### Операции
- `push_back`, `pop_back`
- `insert`, `erase`
- `resize`, `reserve`, `assign`
- `clear`, `empty`

### Доступ к элементам
- `front`, `back`
- `at`
- `operator[]`
- `begin`, `end`
- `get_size`, `get_space`, `get_data`

---

## В разработке

- `emplace_back`
- `shrink_to_fit`
- `swap`
- `max_size`
- `cbegin`, `cend`
- операторы сравнения
