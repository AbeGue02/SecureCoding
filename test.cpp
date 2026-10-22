// Uncomment the next line to use precompiled headers
#include "pch.h"
// uncomment the next line if you do not use precompiled headers
//#include "gtest/gtest.h"
//
// the global test environment setup and tear down
// you should not need to change anything here
class Environment : public ::testing::Environment
{
public:
    ~Environment() override {}

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        //  initialize random seed
        srand(time(nullptr));
    }

    // Override this to define how to tear down the environment.
    void TearDown() override {}
};

// create our test class to house shared data between tests
// you should not need to change anything here
class CollectionTest : public ::testing::Test
{
protected:
    // create a smart point to hold our collection
    std::unique_ptr<std::vector<int>> collection;

    void SetUp() override
    { // create a new collection to be used in the test
        collection.reset(new std::vector<int>);
    }

    void TearDown() override
    { //  erase all elements in the collection, if any remain
        collection->clear();
        // free the pointer
        collection.reset(nullptr);
    }

    // helper function to add random values from 0 to 99 count times to the collection
    void add_entries(int count)
    {
        assert(count > 0);
        for (auto i = 0; i < count; ++i)
            collection->push_back(rand() % 100);
    }
};

// When should you use the EXPECT_xxx or ASSERT_xxx macros?
// Use ASSERT when failure should terminate processing, such as the reason for the test case.
// Use EXPECT when failure should notify, but processing should continue

// Test that a collection is empty when created.
// Prior to calling this (and all other TEST_F defined methods),
//  CollectionTest::StartUp is called.
// Following this method (and all other TEST_F defined methods),
//  CollectionTest::TearDown is called
TEST_F(CollectionTest, CollectionSmartPointerIsNotNull)
{
    // is the collection created
    ASSERT_TRUE(collection);

    // if empty, the size must be 0
    ASSERT_NE(collection.get(), nullptr);
}

// Test that a collection is empty when created.
TEST_F(CollectionTest, IsEmptyOnCreate)
{
    // is the collection empty?
    ASSERT_TRUE(collection->empty());

    // if empty, the size must be 0
    ASSERT_EQ(collection->size(), 0);
}

/* Comment this test out to prevent the test from running
 * Uncomment this test to see a failure in the test explorer */
//TEST_F(CollectionTest, AlwaysFail)
//{
//    FAIL();
//}

// Create a test to verify adding a single value to an empty collection
TEST_F(CollectionTest, CanAddToEmptyVector)
{
    // Verify collection is empty before adding
    ASSERT_TRUE(collection->empty());
    ASSERT_EQ(collection->size(), 0);

    add_entries(1);

    // Verify collection is no longer empty and has 1 element
    ASSERT_FALSE(collection->empty());
    ASSERT_EQ(collection->size(), 1);
}

// Create a test to verify adding five values to collection
TEST_F(CollectionTest, CanAddFiveValuesToVector)
{
	ASSERT_TRUE(collection->empty()); // Verify collection is empty before adding

    add_entries(5);

	ASSERT_EQ(collection->size(), 5); // Verify collection has 5 elements
}

// Create a test to verify that max size is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, MaxSizeGreaterThanOrEqualToSize)
{
	EXPECT_GE(collection->max_size(), collection->size()); // Verify max size is greater than or equal to size for 0 entries

    add_entries(1);
	EXPECT_GE(collection->max_size(), collection->size()); // Verify max size is greater than or equal to size for 1 entry

    collection->clear(); 
    add_entries(5);
    EXPECT_GE(collection->max_size(), collection->size()); // Verify max size is greater than or equal to size for 5 entries

    collection->clear();
    add_entries(10);
    EXPECT_GE(collection->max_size(), collection->size()); // Verify max size is greater than or equal to size for 10 entries
}

// Create a test to verify that capacity is greater than or equal to size for 0, 1, 5, 10 entries
TEST_F(CollectionTest, CapacityGreaterThanOrEqualToSize)
{
    EXPECT_GE(collection->capacity(), collection->size()); // Verify capacity is greater than or equal to size for 0 entries

    add_entries(1);
    EXPECT_GE(collection->capacity(), collection->size()); // Verify capacity is greater than or equal to size for 1 entry

    collection->clear();
    add_entries(5);
	EXPECT_GE(collection->capacity(), collection->size()); // Verify capacity is greater than or equal to size for 5 entries

    collection->clear();
    add_entries(10);
	EXPECT_GE(collection->capacity(), collection->size()); // Verify capacity is greater than or equal to size for 10 entries
}

// Create a test to verify resizing increases the collection
TEST_F(CollectionTest, ResizeIncreasesCollection)
{
    add_entries(3);
    collection->resize(7);

	ASSERT_EQ(collection->size(), 7); // Verify collection size is now 7
}

// Create a test to verify resizing decreases the collection
TEST_F(CollectionTest, ResizeDecreasesCollection)
{
    add_entries(10);
    collection->resize(5);

	ASSERT_EQ(collection->size(), 5); // Verify collection size is now 5
}

// Create a test to verify resizing decreases the collection to zero
TEST_F(CollectionTest, ResizeToZero)
{
    add_entries(10);
    collection->resize(0);

	ASSERT_TRUE(collection->empty()); // Verify collection is empty
	ASSERT_EQ(collection->size(), 0); // Verify collection size is now 0
}

// Create a test to verify clear erases the collection
TEST_F(CollectionTest, ClearErasesCollection)
{
    add_entries(5);
	ASSERT_FALSE(collection->empty()); // Verify collection is not empty before clearing

    collection->clear();

	ASSERT_TRUE(collection->empty()); // Verify collection is empty after clearing
	ASSERT_EQ(collection->size(), 0); // Verify collection size is now 0
}

// Create a test to verify erase(begin,end) erases the collection
TEST_F(CollectionTest, EraseRangeErasesCollection)
{
    add_entries(5);

    collection->erase(collection->begin(), collection->end());

	ASSERT_TRUE(collection->empty()); // Verify collection is empty after erasing the range
	ASSERT_EQ(collection->size(), 0); // Verify collection size is now 0
}

// Create a test to verify reserve increases the capacity but not the size of the collection
TEST_F(CollectionTest, ReserveIncreasesCapacityNotSize)
{
    add_entries(3);
    size_t originalSize = collection->size();

    collection->reserve(10);

	ASSERT_EQ(collection->size(), originalSize); // Verify collection size remains the same after reserving
	ASSERT_GE(collection->capacity(), 10); // Verify collection capacity is now at least 10
}

// Create a test to verify the std::out_of_range exception is thrown when calling at() with an index out of bounds
// NOTE: This is a negative test
TEST_F(CollectionTest, AtThrowsOutOfRangeException)
{
    add_entries(3);

	EXPECT_THROW(collection->at(10), std::out_of_range); // index 10 is out of bounds for a collection of size 3
}

// Create 2 unit tests of your own to test something on the collection - do 1 positive & 1 negative

// Create a test to verify that elements can be accessed by index
TEST_F(CollectionTest, CanAccessElementByIndex)
{
    add_entries(3);

	ASSERT_NO_THROW(collection->at(0)); // Verify that accessing a valid index does not throw an exception
	ASSERT_NO_THROW(collection->at(2)); // Verify that accessing the last valid index does not throw an exception
}

// Create a test to verify that elements can be accessed by index
TEST_F(CollectionTest, CannotAccessNegativeIndex)
{
    add_entries(5);

	EXPECT_THROW(collection->at(-1), std::out_of_range); // Verify that accessing a negative index throws an out_of_range exception
}
