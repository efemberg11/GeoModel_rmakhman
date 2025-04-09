// GeoModel includes
#include "GeoModelDBManager/GMDBManager.h"

#include <gtest/gtest.h>
#include <filesystem>

class DatabaseTest : public ::testing::Test {
protected:
    std::string dbFile = "test_database.db";
    std::unique_ptr<GMDBManager> dbManager{};

    void SetUp() override {
        // Remove any existing test database
        if (std::filesystem::exists(dbFile)) {
            std::filesystem::remove(dbFile);
        }
        
        // open the DB connection
        dbManager = std::make_unique<GMDBManager>(dbFile);
        
        // check the DB connection
        if (dbManager->checkIsDBOpen())
        {
            std::cout << "OK! Database is open!" << std::endl;
        }
        else
        {
            std::cout << "Database ERROR!! Exiting..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void TearDown() override {
        if (std::filesystem::exists(dbFile)) {
            std::filesystem::remove(dbFile);
        }
    }
};

TEST_F(DatabaseTest, CreateDatabaseTest) {
    // EXPECT_TRUE(dbManager->createDatabase());
    EXPECT_TRUE(std::filesystem::exists(dbFile));
}

TEST_F(DatabaseTest, PopulateDatabaseTest) {
    // ASSERT_TRUE(dbManager->createDatabase());
    EXPECT_TRUE(dbManager->initDB());
}

// TEST_F(DatabaseTest, InsertDataTest) {
//     ASSERT_TRUE(dbManager->createDatabase());
//     ASSERT_TRUE(dbManager->initDB());

//     EXPECT_TRUE(dbManager->insertData("key1", "value1"));
// }

// TEST_F(DatabaseTest, ReadDataTest) {
//     ASSERT_TRUE(dbManager->createDatabase());
//     ASSERT_TRUE(dbManager->populateDatabase());

//     dbManager->insertData("key1", "value1");
//     EXPECT_EQ(dbManager->readData("key1"), "value1");
// }

