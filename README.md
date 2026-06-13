raw-vector

Особенности:

    Минимальный размер: sizeof(my_vector<T>) == sizeof(int*) — весь вектор хранится в одном указателе
    Сырая память: данные, размер и ёмкость упакованы в единый блок char*
    Placement new: объекты создаются напрямую в выделенной памяти
    Автоматическое расширение: ёмкость удваивается при нехватке места

Реализовано:

    push_back(), pop_back(), get_size(), get_space(), get_data(), empty(), at(), front(), back(), clear(), reserve(), assign(iter* a, iter* b), assign(amount, &val), begin(), end(), erase(), insert(), resize(size), resize(size, val), emplace_back(), erase(), shrink_to_fir(),
	Итераторы begin(), end()
	operator[] /const, <<, =(&), =(&&)
	
В разработке:

    swap(), max_size()
	Операторы сравнения
