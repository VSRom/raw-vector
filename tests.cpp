#include <gtest/gtest.h>
#include <string>
#include "raw_vector.h"

// =====================================================================

TEST(RawVectorTest, EmptyVector)
{
    raw_vector<int> v;

    EXPECT_TRUE(v.empty());
}

// =====================================================================

TEST(RawVectorTest, PushBackIntegers)
{
    raw_vector<int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

// =====================================================================

TEST(RawVectorTest, FrontBack)
{
    raw_vector<int> v;

    v.push_back(100);
    v.push_back(200);

    EXPECT_EQ(v.front(), 100);
    EXPECT_EQ(v.back(), 200);
}

// =====================================================================

TEST(RawVectorTest, ResizeGrow)
{
    raw_vector<int> v;

    v.resize(5);

    EXPECT_EQ(*v.get_size(), 5);
}

// =====================================================================

TEST(RawVectorTest, ResizeShrink)
{
    raw_vector<int> v;

    v.resize(10);
    v.resize(3);

    EXPECT_EQ(*v.get_size(), 3);
}

// =====================================================================

TEST(RawVectorTest, PopBack)
{
    raw_vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.pop_back();

    EXPECT_EQ(*v.get_size(), 2);
    EXPECT_EQ(v.back(), 2);
}

// =====================================================================

TEST(RawVectorTest, Clear)
{
    raw_vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.clear();

    EXPECT_TRUE(v.empty());
}

// =====================================================================

TEST(RawVectorTest, InsertMiddle)
{
    raw_vector<int> v;

    v.push_back(1);
    v.push_back(3);

    v.insert(v.begin() + 1, 2);

    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

// =====================================================================

TEST(RawVectorTest, EraseMiddle)
{
    raw_vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    v.erase(v.begin() + 1);

    EXPECT_EQ(*v.get_size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
}

// =====================================================================

TEST(RawVectorTest, CopyConstructor)
{
    raw_vector<int> a;

    a.push_back(10);
    a.push_back(20);

    raw_vector<int> b(a);

    EXPECT_EQ(b[0], 10);
    EXPECT_EQ(b[1], 20);
}

// =====================================================================

TEST(RawVectorTest, CopyAssignment)
{
    raw_vector<int> a;

    a.push_back(10);
    a.push_back(20);

    raw_vector<int> b;

    b = a;

    EXPECT_EQ(b[0], 10);
    EXPECT_EQ(b[1], 20);
}

// =====================================================================

TEST(RawVectorTest, MoveConstructor)
{
    raw_vector<int> a;

    a.push_back(5);
    a.push_back(6);

    raw_vector<int> b(std::move(a));

    EXPECT_EQ(b[0], 5);
    EXPECT_EQ(b[1], 6);
}

// =====================================================================

TEST(RawVectorTest, MoveAssignment)
{
    raw_vector<int> a;

    a.push_back(7);
    a.push_back(8);

    raw_vector<int> b;

    b = std::move(a);

    EXPECT_EQ(b[0], 7);
    EXPECT_EQ(b[1], 8);
}

// =====================================================================

TEST(RawVectorTest, Strings)
{
    raw_vector<std::string> v;

    v.push_back("hello");
    v.push_back("world");

    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
}