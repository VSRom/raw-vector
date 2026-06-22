#include <gtest/gtest.h>
#include <string>
#include "raw_vector.h" // Подключаем ваш разделенный заголовочный файл

// Тест 1: Проверка вашей главной фичи — размер класса равен одному указателю
TEST(RawVectorTest, InternalLayoutSize) {
    EXPECT_EQ(sizeof(raw_vector<int>), sizeof(char*));
    EXPECT_EQ(sizeof(raw_vector<std::string>), sizeof(char*));
}

// Тест 2: Базовые операции вставки, чтения и проверки на пустоту
TEST(RawVectorTest, PushBackAndAccess) {
    raw_vector<int> vec;
    EXPECT_TRUE(vec.empty());

    vec.push_back(748);
    vec.push_back(21);

    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.at(0), 748);
    EXPECT_EQ(vec.front(), 748);
    EXPECT_EQ(vec.back(), 21);
    EXPECT_EQ(vec[1], 21);
}

// Тест 3: Проверка Emplace Back и Move-семантики для строк (из вашего main)
TEST(RawVectorTest, EmplaceBackAndMove) {
    raw_vector<std::string> v;
    std::string s = "abc";
    
    v.emplace_back(s);            // Копирование
    v.emplace_back(std::move(s)); // Перемещение

    EXPECT_EQ(v.at(0), "abc");
    EXPECT_EQ(v.at(1), "abc");
    EXPECT_TRUE(s.empty());       // Строка должна остаться пустой после std::move
}

// Тест 4: Проверка деструкторов при вызове pop_back и полной очистке
class DestructorCounter {
public:
    static int count;
    ~DestructorCounter() { count++; }
};
int DestructorCounter::count = 0;

TEST(RawVectorTest, PopBackDestructorCall) {
    DestructorCounter::count = 0;
    {
        raw_vector<DestructorCounter> vec;
        vec.push_back(DestructorCounter());
        vec.push_back(DestructorCounter());
        
        vec.pop_back(); 
        // 1 деструктор временного объекта при push_back + 1 деструктор при pop_back
        EXPECT_GE(DestructorCounter::count, 2); 
    }
}
