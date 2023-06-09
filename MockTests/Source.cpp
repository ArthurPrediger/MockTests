#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>
#include "Game/Location2D.h"
#include "Game/GameLoop.h"
#include "Game/World.h"
#include "Game/GameStatus.h"
#include "Game/Player.h"
#include "Game/ComplementsManager.h"

class MockWorld : public IWorld {
public:
    MOCK_METHOD(Location2D, GetExtent, (), (const, override));
    MOCK_METHOD(std::string&, GetContentRef, (), (override));
    MOCK_METHOD(void, Draw, (), (const, override));
};

class MockGameStatus : public IGameStatus {
public:
    MOCK_METHOD(void, Draw, (), (const, override));
    MOCK_METHOD(void, AddToScore, (int value), (override));
    MOCK_METHOD(void, AddToScoreLost, (int value), (override));
    MOCK_METHOD(void, PlayerLifesMinusOne, (), (override));
    MOCK_METHOD(bool, IsGameOver, (), (override));
};

class MockPlayer : public IPlayer {
public:
    MOCK_METHOD(void, UpdateWorldLocation, (Location2D displacement), (override));
    MOCK_METHOD(Location2D, GetLocation, (), (const, override));
    MOCK_METHOD(int, GetNumber, (), (const, override));
    MOCK_METHOD(void, SetNumber, (int number), (override));
};

class MockComplementsManager : public IComplementsManager {
public:
    MOCK_METHOD(void, UpdateComplementsLifetime, (float dt), (override));
};

TEST(TestPlayerWorldInteraction, PlayerWorldInteraction) 
{   
    using namespace testing;

    // Classes instantiation
    std::unique_ptr<MockWorld> world = std::make_unique<MockWorld>();
    std::unique_ptr<Player> player = std::make_unique<Player>(Location2D{ 1, 1 }, world.get());

    // Setting default values to called methods
    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 3, 3 }));
    std::string default_value = "default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));

    // Set expectations on mock methods
    EXPECT_CALL(*world, GetExtent());
    EXPECT_CALL(*world, GetContentRef());

    // Invoke the method being tested
    player->UpdateWorldLocation({ 1, 0 });

    // Assertion
    ASSERT_TRUE(player->GetLocation() == Location2D( 1, 1 ));
}

TEST(TestGameLoop, GameLoopMemberCalls)
{
    using namespace testing;

    // Classes instantiation
    std::shared_ptr<MockWorld> world = std::make_shared<MockWorld>();
    std::shared_ptr<MockGameStatus> game_status = std::make_shared<MockGameStatus>();
    std::shared_ptr<MockPlayer> player = std::make_shared<MockPlayer>();
    std::shared_ptr<MockComplementsManager> comps_manager = std::make_shared<MockComplementsManager>();

    std::unique_ptr<GameLoop> GL = std::make_unique<GameLoop>(world, game_status, player, comps_manager);

    // Setting default values to called methods
    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 3, 3 }));
    std::string default_value = "default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));
    
    int player_lifes = 3;
    ON_CALL(*game_status, IsGameOver).WillByDefault(
        [player_lifes]() mutable 
        {
            return player_lifes-- < 0;
        });

    ON_CALL(*player, GetLocation).WillByDefault(Return(Location2D(1, 1)));
    ON_CALL(*player, GetNumber).WillByDefault(Return(1));

    // Set expectations on mock methods
    EXPECT_CALL(*game_status, IsGameOver()).Times(5);
    EXPECT_CALL(*player, UpdateWorldLocation(Location2D{0, 0})).Times(5);
    EXPECT_CALL(*world, Draw()).Times(4);

    // Invoke the method being tested
    GL->Run();
}

// Run the tests
int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
