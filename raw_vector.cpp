#include <utility>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
//======================================================================================================================
template <typename T>
class raw_vector {

	char* block = nullptr;

public:
//======================================================================================================================
	void swap() {

	}
//======================================================================================================================
	int max_size() {
		return (*get_space());
	}
//======================================================================================================================
	void shrink_to_fit() {	// Уменьшение свободного пространства до уровня размера
		if (block) {
			if (*get_size() < *get_space()) {
				void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * (*get_size()) );
				char* temp_block = static_cast<char*>(raw);

				*(size_t*)(temp_block) = *get_size();
				*(size_t*)(temp_block + sizeof(size_t)) = *get_size();
				T* data = get_data();
				T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);

				for (size_t i = 0; i < *get_size(); i++, address++)
					new (address) T(std::move(data[i]));

				for (size_t i = 0; i < *get_size(); i++)
					data[i].~T();

				operator delete(block);
				block = temp_block;
			}
			else return;
		}
	}
//======================================================================================================================
	template<class... Args>
	void emplace_back(Args&&... args) {
		if (!block) {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * 1);
			block = static_cast<char*>(raw);

			*(size_t*)(block) = 0;
			*(size_t*)(block + sizeof(size_t)) = 1;
			T* address = reinterpret_cast<T*>(block + sizeof(size_t) * 2);
			new (address) T(std::forward<Args>(args)...);
			*(size_t*)(block) = (*get_size() + 1);
			return;
		}
		if (*get_size() >= *get_space()) {
			// Создали новый блок вектора с увеличенным размером(для добавлемого элемента(1)
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * (*get_space() * 2));
			char* temp_block = static_cast<char*>(raw);

			// Получили указатель на данные
			T* data = get_data();

			// Получили адрес поля space в новом блоке и записали туда удвоенную ёмкость из старого блока
			*(size_t*)(temp_block + sizeof(size_t)) = (*get_space()) * 2;

			// Получили адрес поля size в новом блоке и записали туда размер из старого блока +1 новый элемент
			*(size_t*)(temp_block) = (*get_size() + 1);

			// Получили адрес для дальнейшего копирования элементов из старого блока в новый
			T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);

			// Копируем элементы(data) из старого блока в новый опираясь на количество элементов старого блока
			// и добавили новое значение в конец(если дошли до конца элементов
			for (size_t i = 0; i <= *get_size(); i++, address++) {
				if (i == *get_size())
					new (address) T(std::forward<Args>(args)...);
				else
					new (address) T(std::move(data[i]));
			}
			// Вызвали деструктор каждого элемента для старого вектора
			for (size_t i = 0; i < *get_size(); i++)
				data[i].~T();
			// Удаляем выделенную память под старый блок
			operator delete[](block);
			// Переназначили указатель вектора на новый блок
			block = temp_block;
		}
		else {
			T* address = reinterpret_cast<T*>(block + sizeof(size_t) * 2) + (*get_size());
			new (address) T(std::forward<Args>(args)...);
			*(size_t*)(block) = (*get_size() + 1);
		}
	}
//======================================================================================================================
	void resize(size_t new_size, T& val) {

		if (!block) {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * new_size + 1);
			block = static_cast<char*>(raw);

			*(size_t*)(block) = new_size;
			*(size_t*)(block + sizeof(size_t)) = new_size + sizeof(T);
			T* address = reinterpret_cast<T*>(block + sizeof(size_t) * 2);

			for (size_t i = 0; i < new_size; i++, address++)
				new (address) T(val);

			return;
		}
		else {
			if (new_size >= *get_space()) {
				void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * new_size + sizeof(T));
				char* temp_block = static_cast<char*>(raw);

				*(size_t*)(temp_block) = new_size;
				*(size_t*)(temp_block + sizeof(size_t)) = new_size + sizeof(T);
				T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);
				T* data = get_data();

				for (size_t i = 0; i < *get_size(); i++, address++)
					new (address) T(std::move(data[i]));

				for (size_t i = 0; i < new_size - (*get_size()); i++, address++)
					new (address) T(val);

				for (size_t i = 0; i < *get_size(); i++)
					data[i].~T();

				operator delete(block);
				block = temp_block;
			}
			else {
				T* ptr = begin() + *get_size();

				for (size_t i = 0; i < new_size - (*get_size()); i++, ptr++)
					new (ptr) T(val);

				*(size_t*)(block) = new_size;
			}
		}
	}
//======================================================================================================================
	void resize(size_t new_size) {

		if (!block) {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * new_size * 2);
			block = static_cast<char*>(raw);

			*(size_t*)(block) = new_size;
			*(size_t*)(block + sizeof(size_t)) = new_size * 2;
			return;
		}
		else {
			if (new_size >= *get_space()) {
				void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * new_size * 2);
				char* temp_block = static_cast<char*>(raw);

				*(size_t*)(temp_block) = new_size;
				*(size_t*)(temp_block + sizeof(size_t)) = new_size * 2;
				T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);
				T* data = get_data();

				for (size_t i = 0; i < *get_size(); i++, address++)
					new (address) T(std::move(data[i]));

				for (size_t i = 0; i < new_size - (*get_size()); i++, address++)
					new (address) T();

				for (size_t i = 0; i < *get_size(); i++)
					data[i].~T();

				operator delete(block);
				block = temp_block;
			}
			else if (new_size < *get_size()) {
				// Просто обрезать блок 

				T* ptr_del = begin() + new_size;

				for (size_t i = 0; i < *get_size() - new_size; i++)
					ptr_del[i].~T();

				*(size_t*)(block) = new_size;
			}
			else {
				T* ptr = begin() + *get_size();

				for (size_t i = 0; i < new_size - (*get_size()); i++, ptr++)
					new (ptr) T();

				*(size_t*)(block) = new_size;
			}
		}
	}
//======================================================================================================================
	void insert(const T* iter, const T& val) {
		if (!block) {
			if (iter) throw std::out_of_range("Cannot insert element: iterator is out of bounds for an empty vector.");
			
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * 2);
			block = static_cast<char*>(raw);

			*(size_t*)(block) = 1;
			*(size_t*)(block + sizeof(size_t)) = 2;

			T* address = reinterpret_cast<T*>(block + sizeof(size_t) * 2);
			new (address) T(val);
			return;
		}
		else {
			auto know = iter - begin();

			if (*get_size() + 1 >= *get_space())
				reserve(*get_size() + 2);

			T* address = begin() + know;
			T* last = end();
			T* prev = last - 1;

			new (last) T(*prev);
			last--;
			while (last != address) {
				*last = *(last - 1);
				last--;
			}
			*address = val;
			*(size_t*)(block) = (*get_size()) + 1;
		}
	}
//======================================================================================================================
	void erase(const T* iter) {
		if (!block) return;
		if (iter == end()) return;

			T* address = const_cast<T*>(iter);
			T* next = address + 1;

			while (next != end()) {
				*address = *next;
				address++;
				next++;
			}
			address->~T();
			*(size_t*)(block) = (*get_size()) - 1;
		}
//======================================================================================================================
	void reserve(size_t new_space) {
		if (!block) {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * new_space);
			block = static_cast<char*>(raw);

			*(size_t*)(block) = 0;
			*(size_t*)(block + sizeof(size_t)) = new_space;
			T* address = reinterpret_cast<T*>(block + sizeof(size_t) * 2);
			return;
		}
		if (new_space > *get_space()) {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * new_space);
			char* temp_block = static_cast<char*>(raw);

			*(size_t*)(temp_block) = *get_size();
			*(size_t*)(temp_block + sizeof(size_t)) = new_space;

			T* data = get_data();
			T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);

			for (size_t i = 0; i < *get_size(); i++, address++)
				new (address) T(std::move(data[i]));

			for (size_t i = 0; i < *get_size(); i++)
				data[i].~T();

			operator delete(block);
			block = temp_block;
		}
		return;
	}
//======================================================================================================================
	void push_back(const T& val) {
		if (!block) {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * 1);
			block = static_cast<char*>(raw);

			*(size_t*)(block) = 0;
			*(size_t*)(block + sizeof(size_t)) = 1;

			T* address = reinterpret_cast<T*>(block + sizeof(size_t) * 2);
			new (address) (T)(val);
			*(size_t*)(block) = (*get_size() + 1);
			return;
		}
		if (*get_size() >= *get_space()) {
			// Создали новый блок вектора с увеличенным размером(для добавлемого элемента(1)
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * (*get_space() * 2));
			char* temp_block = static_cast<char*>(raw);

			// Получили указатель на данные
			T* data = get_data();

			// Получили адрес поля space в новом блоке и записали туда удвоенную ёмкость из старого блока
			*(size_t*)(temp_block + sizeof(size_t)) = (*get_space()) * 2;

			// Получили адрес поля size в новом блоке и записали туда размер из старого блока +1 новый элемент
			*(size_t*)(temp_block) = (*get_size() + 1);

			// Получили адрес для дальнейшего копирования элементов из старого блока в новый
			T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);

			// Копируем элементы(data) из старого блока в новый опираясь на количество элементов старого блока
			// и добавили новое значение в конец(если дошли до конца элементов
			for (size_t i = 0; i <= *get_size(); i++, address++) {
				if (i == *get_size())
					new (address) T(val);
				else
					new (address) T(data[i]);
			}
			// Вызвали деструктор каждого элемента для старого вектора
			for (size_t i = 0; i < *get_size(); i++)
				data[i].~T();
			// Удаляем выделенную память под старый блок
			operator delete(block);
			// Переназначили указатель вектора на новый блок
			block = temp_block;
		}
		else {
			T* address = reinterpret_cast<T*>(block + sizeof(size_t) * 2) + (*get_size());
			new (address) T(val);
			*(size_t*)(block) = (*get_size() + 1);
		}
	}
//======================================================================================================================
	void assign(size_t amount, const T& val) {
		if (!block) {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * amount);
			block = static_cast<char*>(raw);

			*(size_t*)(block) = amount;
			*(size_t*)(block + sizeof(size_t)) = amount;

			T* address = reinterpret_cast<T*>(block + sizeof(size_t) * 2);

			for (size_t i = 0; i < amount; i++, address++)
				new (address) T(val);

			return;
		}
		else {

			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * amount);
			char* temp_block = static_cast<char*>(raw);

			*(size_t*)(temp_block) = amount;
			*(size_t*)(temp_block + sizeof(size_t)) = amount;	// Записали в блоки сайз и спайс == amount

			T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);

			T* data = get_data();
			for (size_t i = 0; i < amount; i++, address++)
				new (address) T(val);

			for (size_t i = 0; i < *get_size(); i++)
				data[i].~T();

			operator delete(block);
			block = temp_block;
		}
	}
//======================================================================================================================
	void assign(const T* first, const T* last) {
		auto differ = last - first;
		void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * differ);
		char* temp_block = static_cast<char*>(raw);

		*(size_t*)(temp_block) = differ;
		*(size_t*)(temp_block + sizeof(size_t)) = differ;
		T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);

		while (first != last) {
			new (address) T(*first);
			address++;
			first++;
		}

		if (block) {
			T* data = get_data();

			for (size_t i = 0; i < *get_size(); i++)
				data[i].~T();

			operator delete(block);
			block = temp_block;
		}
		else
			block = temp_block;
	}
//======================================================================================================================
	void pop_back() {
		if (block) {
			T* raw = &this->back();
			raw->~T();
			*(size_t*)(block) = (*get_size() - 1);
		}
		else
			return;
	}
//======================================================================================================================
	bool empty() const {
		if (*this->get_size() == 0)
			return true;
		else
			return false;
	}
//======================================================================================================================
	void clear() {
		if (block) {
			T* data = get_data();
			for (size_t i = 0; i < *get_size(); i++)
				data[i].~T();
			*(size_t*)(block) = 0;	// Сброс размера вектора в ноль
		}
	}
//======================================================================================================================
	T& at(size_t index) {
		if (index >= (*this->get_size()))
			throw std::out_of_range("Error, out of range!");

		T* ptr = this->get_data();
		return ptr[index];
	}
//======================================================================================================================
	T& front() {
		T* ptr = this->get_data();
		return *ptr;
	}
//===============================================================================================================================
	T& back() {
		T* raw = this->get_data() + (*this->get_size() - 1);
		return *raw;
	}
//======================================================================================================================
	raw_vector(size_t capac = 0) {
		if (capac == 0) block = nullptr;
		else {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * capac);
			block = static_cast<char*>(raw;
			*get_size() = 0;
			*get_space() = capac;
		}
	}
	raw_vector(std::initializer_list<T> list) {
		this->assign(list.begin(), list.end());
	}
//======================================================================================================================
	size_t* get_size() const {
		if (block)
			return (size_t*)block;
		return nullptr;
	}

	size_t* get_space() const {
		if (block)
			return (size_t*)(block + sizeof(size_t));
		return nullptr;
	}

	T* get_data() const {
		if (block)
			return (T*)(block + 2 * sizeof(size_t));
		return nullptr;
	}

	T* begin() const {
		return (T*)(block + 2 * sizeof(size_t));
	}

	T* end() const {
		return (T*)(this->begin() + *this->get_size());
	}
//======================================================================================================================
	~raw_vector() {
		if (block) {
			T* data = get_data();
			size_t count = *get_size();
			for (size_t i = 0; i < count; i++)
				data[i].~T();
			operator delete(block);
		}
	}
	// ПЕРЕГРУЗКИ
//======================================================================================================================
	friend std::ostream& operator<<(std::ostream& os, raw_vector<T>& other) {
		T* data_this = other.get_data();
		for (size_t i = 0; i < *(other.get_size()); i++)
			os << data_this[i] << '\t';
		return os;
	}
//======================================================================================================================
			// Копирующее присваивание
	raw_vector& operator=(const raw_vector& other) {
		if (this == &other) return *this;

		if (other.block) {
			void* raw = operator new(sizeof(size_t) + sizeof(size_t) + sizeof(T) * *(other.get_space()));
			char* temp_block = static_cast<char*>(raw);
			T* data = other.get_data();

			*reinterpret_cast<size_t*>(temp_block) = *(other.get_size());
			*(size_t*)(temp_block + sizeof(size_t)) = *(other.get_space());

			T* address = reinterpret_cast<T*>(temp_block + sizeof(size_t) * 2);
			
			for (size_t i = 0; i < *(other.get_size()); i++, address++)
				new (address) (T)(data[i]);

			if (block) {
				T* data_this = this->get_data();
				for (size_t i = 0; i < *(this->get_size()); i++)
					data_this[i].~T();
			}

			operator delete(this->block);

			this->block = temp_block;
			return *this;
		}
		else {
			if (block) {
				T* data_this = this->get_data();
				for (size_t i = 0; i < *(this->get_size()); i++)
					data_this[i].~T();

				operator delete(this->block);
			}

			block = nullptr;
			return *this;
		}
	}
//======================================================================================================================
			// Перемещающее присваивание
	raw_vector& operator=(raw_vector&& other) {
		if (this == &other) return *this;

		if (block) {
			T* data_this = this->get_data();
			for (size_t i = 0; i < *(this->get_size()); i++)
				data_this[i].~T();

			operator delete(block);
		}

		block = other.block;
		other.block = nullptr;
		return *this;
	}
//======================================================================================================================
	T& operator[](const size_t index) {
		T* ptr_r = this->get_data();
		return ptr_r[index];
	}

	const T& operator[](const size_t index) const {
		const T* ptr_r = this->get_data();
		return ptr_r[index];
	}
};
//======================================================================================================================
int main() {
	{
		raw_vector<int> hello = { 748, 21, 15 ,23, 14 };
		raw_vector<int> copy;
		raw_vector<std::string> v;
		std::string s = "abc";

		v.emplace_back(s);
		v.emplace_back(std::move(s));

		//	copy = hello;
		//	hello = hello;

		//	for (const auto& x : hello)
		//		std::cout << x << '\n';
		//	int a = 420;
		//	hello.resize(25);
		//	//std::cout << copy << '\n';
		//	std::cout << hello << '\n';
	}
	std::cout << sizeof(raw_vector<int>) << " == " << sizeof(int*) << "\n\n\n";



	return 0;
}
//======================================================================================================================
//		Разбор
//raw_vector& operator=(const raw_vector& other) {
//	if (this == &other) return *this;
//
//	// Константы выравнивания и смещения
//	constexpr size_t metadata_size = sizeof(size_t) * 2;
//	constexpr size_t alignment = std::max(alignof(size_t), alignof(T));
//
//	// Формула выравнивания смещения вверх
//	constexpr size_t data_offset = (metadata_size + alignof(T) - 1) & ~(alignof(T) - 1);
//
//	if (other.block) {
//		size_t other_space = *(other.get_space());
//		size_t other_size = *(other.get_size());
//
//		// Всего байт: смещение до данных + размер массива элементов
//		size_t total_bytes = data_offset + (sizeof(T) * other_space);
//
//		// Выделяем память с учетом строгого выравнивания (C++17 std::align_val_t)
//		void* raw = operator new(total_bytes, std::align_val_t(alignment));
//		char* temp_block = static_cast<char*>(raw);
//
//		// Записываем метаданные в начало блока
//		*reinterpret_cast<size_t*>(temp_block) = other_size;
//		*reinterpret_cast<size_t*>(temp_block + sizeof(size_t)) = other_space;
//
//		// Вычисляем абсолютно правильный и выровненный адрес для элементов
//		T* dst_data = reinterpret_cast<T*>(temp_block + data_offset);
//		T* src_data = other.get_data();
//
//		size_t constructed_count = 0;
//		try {
//			// Копируем элементы. Конструктор копирования может бросить исключение!
//			for (size_t i = 0; i < other_size; ++i) {
//				new (dst_data + i) T(src_data[i]);
//				constructed_count++;
//			}
//		}
//		catch (...) {
//			// Если упали на середине — уничтожаем то, что успели создать, и чистим память
//			for (size_t i = 0; i < constructed_count; ++i) {
//				dst_data[i].~T();
//			}
//			operator delete(raw, std::align_val_t(alignment));
//			throw; // Пробрасываем исключение дальше
//		}
//
//		// Очищаем старый блок текущего объекта (если он был)
//		if (this->block) {
//			T* this_data = this->get_data();
//			size_t this_size = *(this->get_size());
//			for (size_t i = 0; i < this_size; ++i) {
//				this_data[i].~T();
//			}
//			operator delete(this->block, std::align_val_t(alignment));
//		}
//
//		this->block = temp_block;
//	}
//	else {
//		// Логика для ветки else (когда other.block == nullptr)
//		if (this->block) {
//			T* this_data = this->get_data();
//			size_t this_size = *(this->get_size());
//			for (size_t i = 0; i < this_size; ++i) {
//				this_data[i].~T();
//			}
//			operator delete(this->block, std::align_val_t(alignment));
//		}
//		this->block = nullptr;
//	}
//
//	return *this;
//}
