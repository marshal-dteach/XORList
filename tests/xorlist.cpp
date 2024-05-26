#include <XORList.hpp>
#include <gtest/gtest.h>
#include <list>

namespace scc
{

    TEST(XORListTest, PushFront)
    {
        XORList<int> list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);

        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 3);
        EXPECT_EQ(list.back(), 1);
    }

    TEST(XORListTest, PushBack)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);
    }

    TEST(XORListTest, PopFront)
    {
        XORList<int> list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);
        list.pop_front();

        EXPECT_EQ(list.size(), 2);
        EXPECT_EQ(list.front(), 2);
    }

    TEST(XORListTest, PopBack)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.pop_back();

        EXPECT_EQ(list.size(), 2);
        EXPECT_EQ(list.back(), 2);
    }

    TEST(XORListTest, Insert)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(3);
        list.insert(1, 2);

        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);

        list.pop_front();
        EXPECT_EQ(list.front(), 2);
    }

    TEST(XORListTest, Erase)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.erase(1);

        EXPECT_EQ(list.size(), 2);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);
    }

    TEST(XORListTest, Reverse)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.reverse();

        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 3);
        EXPECT_EQ(list.back(), 1);
    }

    TEST(XORListIteratorTest, IteratorIncrement)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        auto it = list.begin();
        EXPECT_EQ(*it, 1);

        ++it;
        EXPECT_EQ(*it, 2);

        it++;
        EXPECT_EQ(*it, 3);

        ++it;
        EXPECT_EQ(it, list.end());
    }

    TEST(XORListIteratorTest, IteratorDecrement)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        auto it = list.end();
        --it;
        EXPECT_EQ(*it, 3);

        it--;
        EXPECT_EQ(*it, 2);

        --it;
        EXPECT_EQ(*it, 1);

        EXPECT_EQ(it, list.begin());
    }

    TEST(XORListIteratorTest, IteratorArithmetic)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        list.push_back(4);
        list.push_back(5);

        auto it = list.begin();
        it = it + 2;
        EXPECT_EQ(*it, 3);

        it = it - 1;
        EXPECT_EQ(*it, 2);

        auto it2 = list.begin();
        EXPECT_EQ(it - it2, 1);

        it2 = it2 + 3;
        EXPECT_EQ(it2 - list.begin(), 3);
    }

    TEST(XORListIteratorTest, IteratorComparison)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        auto it1 = list.begin();
        auto it2 = list.begin();
        auto it3 = list.begin();
        ++it3;

        EXPECT_TRUE(it1 == it2);
        EXPECT_FALSE(it1 == it3);
        EXPECT_TRUE(it1 != it3);

        ++it1;
        EXPECT_TRUE(it1 == it3);
    }

    TEST(XORListIteratorTest, IteratorTraversal)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        auto it = list.begin();
        EXPECT_EQ(*it, 1);
        ++it;
        EXPECT_EQ(*it, 2);
        ++it;
        EXPECT_EQ(*it, 3);
        ++it;
        EXPECT_EQ(it, list.end());

        auto rit = list.rbegin();
        EXPECT_EQ(*rit, 3);
        ++rit;
        EXPECT_EQ(*rit, 2);
        ++rit;
        EXPECT_EQ(*rit, 1);
        ++rit;
        EXPECT_EQ(rit, list.rend());
    }

    TEST(XORListIteratorTest, ReverseIteratorIncrementDecrement)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        auto rit = list.rbegin();
        EXPECT_EQ(*rit, 3);

        ++rit;
        EXPECT_EQ(*rit, 2);

        ++rit;
        EXPECT_EQ(*rit, 1);

        ++rit;
        EXPECT_EQ(rit, list.rend());

        --rit;
        EXPECT_EQ(*rit, 1);

        --rit;
        EXPECT_EQ(*rit, 2);

        --rit;
        EXPECT_EQ(*rit, 3);
    }

    // test sort
    TEST(XORListTest, Sort)
    {
        XORList<int> list;
        list.push_back(3);
        list.push_back(2);
        list.push_back(1);

        list.sort();
        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);
    }

    // Robustness Tests

    TEST(XORListTest, HandlesEmptyListPopFront)
    {
        {
            XORList<int, CanThrow::NoThrow> list;
            EXPECT_NO_THROW(list.pop_front());
        }
        {
            XORList<int, CanThrow::Throw> list;
            EXPECT_THROW(list.pop_front(), std::runtime_error);
        }
    }

    TEST(XORListTest, HandlesEmptyListPopBack)
    {
        {
            XORList<int, CanThrow::NoThrow> list;
            EXPECT_NO_THROW(list.pop_back());
        }
        {
            XORList<int, CanThrow::Throw> list;
            EXPECT_THROW(list.pop_back(), std::runtime_error);
        }
    }

    TEST(XORListTest, HandlesOutOfBoundsInsert)
    {
        {
            XORList<int, CanThrow::NoThrow> list;
            EXPECT_NO_THROW(list.insert(1, 1));
        }
        {
            XORList<int, CanThrow::Throw> list;
            EXPECT_THROW(list.insert(-1, 1), std::out_of_range);
        }
    }

    TEST(XORListTest, HandlesOutOfBoundsErase)
    {
        {
            XORList<int, CanThrow::NoThrow> list;
            EXPECT_NO_THROW(list.erase(1));
        }
        {
            XORList<int, CanThrow::Throw> list;
            EXPECT_THROW(list.erase(-1), std::out_of_range);
        }
    }

    TEST(XORListTest, AssignmentOperator)
    {
        XORList<int> list;
        XORList<int> otherList;
        otherList.push_back(1);
        otherList.push_back(2);
        otherList.push_back(3);

        list = otherList;

        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);

        // Modify the original list and ensure it does not affect the assigned list
        otherList.pop_front();
        EXPECT_EQ(otherList.size(), 2);
        EXPECT_EQ(list.size(), 3);
    }

    TEST(XORListTest, Assign)
    {
        XORList<int> list;
        list.assign(5, 42);

        EXPECT_EQ(list.size(), 5);
        for (auto it = list.begin(); it != list.end(); ++it)
        {
            EXPECT_EQ(*it, 42);
        }
    }

    TEST(XORListTest, Resize)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        list.resize(5, 42);
        EXPECT_EQ(list.size(), 5);
        EXPECT_EQ(list.back(), 42);

        list.resize(2);
        EXPECT_EQ(list.size(), 2);
        EXPECT_EQ(list.back(), 2);
    }

    TEST(XORListTest, Unique)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(2);
        list.push_back(3);
        list.push_back(3);
        list.push_back(3);

        list.unique();
        EXPECT_EQ(list.size(), 3);

        auto it = list.begin();
        EXPECT_EQ(*it, 1);
        ++it;
        EXPECT_EQ(*it, 2);
        ++it;
        EXPECT_EQ(*it, 3);
    }

    TEST(XORListTest, IteratorTraversal)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        auto it = list.begin();
        EXPECT_EQ(*it, 1);
        ++it;
        EXPECT_EQ(*it, 2);
        ++it;
        EXPECT_EQ(*it, 3);
        ++it;
        EXPECT_EQ(it, list.end());

        auto rit = list.rbegin();
        EXPECT_EQ(*rit, 3);
        ++rit;
        EXPECT_EQ(*rit, 2);
        ++rit;
        EXPECT_EQ(*rit, 1);
        ++rit;
        EXPECT_EQ(rit, list.rend());
    }

    TEST(XORListTest, HandlesNullElements)
    {
        XORList<int *> list;
        int *null_ptr = nullptr;
        list.push_back(null_ptr);

        EXPECT_EQ(list.size(), 1);
        EXPECT_EQ(list.front(), nullptr);
        EXPECT_EQ(list.back(), nullptr);

        list.pop_back();
        EXPECT_EQ(list.size(), 0);
    }

    TEST(XORListTest, HandlesMultipleDataTypes)
    {
        XORList<std::string> stringList;
        stringList.push_back("test1");
        stringList.push_back("test2");
        stringList.push_back("test3");

        EXPECT_EQ(stringList.size(), 3);
        EXPECT_EQ(stringList.front(), "test1");
        EXPECT_EQ(stringList.back(), "test3");

        XORList<double> doubleList;
        doubleList.push_back(1.1);
        doubleList.push_back(2.2);
        doubleList.push_back(3.3);

        EXPECT_EQ(doubleList.size(), 3);
        EXPECT_EQ(doubleList.front(), 1.1);
        EXPECT_EQ(doubleList.back(), 3.3);
    }

    TEST(XORListTest, HandlesSelfAssignment)
    {
        XORList<int> list;
        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        list = list;

        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);
    }

    // Performance Tests for XORList

    TEST(XORListPerformanceTest, LargeNumberOfInsertions)
    {
        XORList<int> list;
        const int num_elements = 1000000;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.push_back(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        EXPECT_EQ(list.size(), num_elements);
        std::cout << "XORList - Time taken for " << num_elements << " insertions: " << elapsed.count() << " seconds\n";
    }

    TEST(XORListPerformanceTest, LargeNumberOfDeletions)
    {
        XORList<int> list;
        const int num_elements = 1000000;

        for (int i = 0; i < num_elements; ++i)
        {
            list.push_back(i);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.pop_back();
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        EXPECT_EQ(list.size(), 0);
        std::cout << "XORList - Time taken for " << num_elements << " deletions: " << elapsed.count() << " seconds\n";
    }

    TEST(XORListPerformanceTest, InsertErase)
    {
        XORList<int> list;
        const int num_elements = 100000;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.insert(0, i); // Inserting at the beginning
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_insert = end - start;

        EXPECT_EQ(list.size(), num_elements);
        std::cout << "XORList - Time taken for " << num_elements << " insertions at begin: " << elapsed_insert.count() << " seconds\n";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.erase(0); // Erasing from the beginning
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_erase = end - start;

        EXPECT_EQ(list.size(), 0);
        std::cout << "XORList - Time taken for " << num_elements << " erasures from begin: " << elapsed_erase.count() << " seconds\n";
    }

    // Performance Tests for std::list

    TEST(StdListPerformanceTest, LargeNumberOfInsertions)
    {
        std::list<int> list;
        const int num_elements = 1000000;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.push_back(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        EXPECT_EQ(list.size(), num_elements);
        std::cout << "std::list - Time taken for " << num_elements << " insertions: " << elapsed.count() << " seconds\n";
    }

    TEST(StdListPerformanceTest, LargeNumberOfDeletions)
    {
        std::list<int> list;
        const int num_elements = 1000000;

        for (int i = 0; i < num_elements; ++i)
        {
            list.push_back(i);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.pop_back();
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        EXPECT_EQ(list.size(), 0);
        std::cout << "std::list - Time taken for " << num_elements << " deletions: " << elapsed.count() << " seconds\n";
    }

    TEST(StdListPerformanceTest, InsertErase)
    {
        std::list<int> list;
        const int num_elements = 100000;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.insert(list.begin(), i); // Inserting at the beginning
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_insert = end - start;

        EXPECT_EQ(list.size(), num_elements);
        std::cout << "std::list - Time taken for " << num_elements << " insertions at begin: " << elapsed_insert.count() << " seconds\n";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_elements; ++i)
        {
            list.erase(list.begin()); // Erasing from the beginning
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_erase = end - start;

        EXPECT_EQ(list.size(), 0);
        std::cout << "std::list - Time taken for " << num_elements << " erasures from begin: " << elapsed_erase.count() << " seconds\n";
    }
} // namespace scc
