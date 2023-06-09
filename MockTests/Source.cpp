#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include "Game/Location2D.h"
#include "Game/World.h"
#include "Game/Player.h"

class MockWorld : public IWorld {
public:
    MOCK_METHOD(Location2D, GetExtent, (), (const, override));
    MOCK_METHOD(std::string&, GetContentRef, (), (override));
};

// Define a test case
TEST(TestPlayerWorldInteraction, TestPlayerWorldInteraction) 
{   
    using namespace testing;

    std::unique_ptr<MockWorld> world = std::make_unique<MockWorld>();
    std::unique_ptr<Player> player = std::make_unique<Player>(Location2D{ 1, 2 }, world.get());

    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 3, 3 }));
    std::string default_value = "default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));

    // Set expectations on mock methods
    EXPECT_CALL(*world, GetExtent());
    EXPECT_CALL(*world, GetContentRef());

    // Invoke the method being tested
    player->UpdateWorldLocation({ 0, 0 });
}

// Run the tests
int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}