#include <iostream>
#include <memory>
//===============================================================================================================================
template <typename T>
class vsr_vector {

	char* block;

public:
	void vsr_push_back(const T& val) {
		if (!block) {
			block = new char[sizeof(size_t) + sizeof(size_t) + sizeof(T) * 1];
			*(size_t*)(block) = 0;
			*(size_t*)(block + sizeof(size_t)) = 1;
			T* address = (T*)(block + sizeof(size_t) * 2);
			new (address) (T)(val);
			*(size_t*)(block) = (*get_size() + 1);
			return;
		}
		if (*get_size() >= *get_space()) {
			// Создали новый блок вектора с увеличенным размером(для добавлемого элемента(1)
			char* temp_block = new char[sizeof(size_t) + sizeof(size_t) + sizeof(T) * (*get_space() * 2)];

			// Получили указатель на данные
			T* data = get_data();

			// Получили адрес поля space в новом блоке и записали туда удвоенную ёмкость из старого блока
			*(size_t*)(temp_block + sizeof(size_t)) = (*get_space()) * 2;

			// Получили адрес поля size в новом блоке и записали туда размер из старого блока +1 новый элемент
			*(size_t*)(temp_block) = (*get_size() + 1);

			// Получили адрес для дальнейшего копирования элементов из старого блока в новый
			T* address = (T*)(temp_block + sizeof(size_t) * 2);

			// Копируем элементы(data) из старого блока в новый опираясь на количество элементов старого блока
			// и добавили новое значение в конец(если дошли до конца элементов
			for (size_t i = 0; i <= *get_size(); i++, address++) {
				if (i == *get_size())
					new (address) T(val);
				else
					new (address) (T)(data[i]);
			}
			// Вызвали деструктор каждого элемента для старого вектора
			for (size_t i = 0; i < *get_size(); i++)
				data[i].~T();
			// Удаляем выделенную память под старый блок
			delete[] block;
			// Переназначили указатель вектора на новый блок
			block = temp_block;
			// Увеличили size для уже текущего блока(учитывая что добавили новый элемент)
		}
		else {
			T* address = (T*)(block + sizeof(size_t) * 2);
			new (address) (T)(val);
			*(size_t*)(block) = (*get_size() + 1);
		}
	}

	vsr_vector(size_t capac = 0) {
		if (capac == 0) block = nullptr;
		else {
			block = new char[sizeof(size_t) + sizeof(size_t) + sizeof(T) * capac];
			*get_size() = 0;
			*get_space() = capac;
		}
	}

	size_t* get_size() {
		if (block)
			return (size_t*)block;
		return nullptr;
	}

	size_t* get_space() {
		if (block)
			return (size_t*)(block + sizeof(size_t));
		return nullptr;
	}

	T* get_data() {
		if (block)
			return (T*)(block + 2 * sizeof(size_t));
		return nullptr;
	}

	~vsr_vector() {
		if (block) {
			T* data = get_data();	// Указатель на массив данных
			size_t count = *get_size(); // Количество элементов
			for (size_t i = 0; i < count; i++)
				data[i].~T();
			delete[] block;
		}
	}
};
//===============================================================================================================================
int main() {
	{
		vsr_vector<int> hello;

		hello.vsr_push_back(218);

	}
	std::cout << sizeof(vsr_vector<int>) << " == " << sizeof(int*) << "\n\n\n";



	return 0;
}
//===============================================================================================================================