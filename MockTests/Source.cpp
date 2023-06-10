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
    std::shared_ptr<NiceMock<MockPlayer>> player = std::make_shared<NiceMock<MockPlayer>>();
    std::shared_ptr<MockComplementsManager> comps_manager = std::make_shared<MockComplementsManager>();

    std::unique_ptr<GameLoop> GL = std::make_unique<GameLoop>(world, game_status, player, comps_manager);

    // Setting default values to called methods
    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 3, 3 }));
    std::string default_value = "default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));
    ON_CALL(*world, Draw).WillByDefault([]() {});
    
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
    EXPECT_CALL(*game_status, Draw()).Times(4);
    EXPECT_CALL(*world, Draw()).Times(4);
    EXPECT_CALL(*player, UpdateWorldLocation(Location2D{ 0, 0 })).Times(5);
    EXPECT_CALL(*comps_manager, UpdateComplementsLifetime).Times(4);

    // Invoke the method being tested
    GL->Run();   
}    

TEST(TestGameLoop, GameLoopDrawFinalStatus)
{
    using namespace testing;

    // Classes instantiation
    std::shared_ptr<MockWorld> world = std::make_shared<MockWorld>();
    std::shared_ptr<MockGameStatus> game_status = std::make_shared<MockGameStatus>();
    std::shared_ptr<NiceMock<MockPlayer>> player = std::make_shared<NiceMock<MockPlayer>>();
    std::shared_ptr<MockComplementsManager> comps_manager = std::make_shared<MockComplementsManager>();

    std::unique_ptr<GameLoop> GL = std::make_unique<GameLoop>(world, game_status, player, comps_manager);

    // Setting default values to called methods
    int player_lifes = -1;
    ON_CALL(*game_status, IsGameOver).WillByDefault(
        [player_lifes]() mutable
        {
            return player_lifes-- < 0;
        });

    // Set expectations on mock methods
    EXPECT_CALL(*game_status, IsGameOver()).Times(1);
    EXPECT_CALL(*game_status, Draw()).Times(1); 
    EXPECT_CALL(*player, UpdateWorldLocation(Location2D{ 0, 0 })).Times(1);

    // Invoke the method being tested
    GL->Start();
}

TEST(TestComplementsManager, PlayerGotScoreComplementRight)
{
    using namespace testing;

    // Classes instantiation
    std::shared_ptr<NiceMock<MockWorld>> world = std::make_shared<NiceMock<MockWorld>>();
    std::shared_ptr<MockGameStatus> game_status = std::make_shared<MockGameStatus>();
    std::shared_ptr<NiceMock<MockPlayer>> player = std::make_shared<NiceMock<MockPlayer>>();

    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 5, 3 }));

    std::unique_ptr<ComplementsManager> comps_manager = std::make_unique<ComplementsManager>(world.get(), game_status.get(), player.get());
    comps_manager->complements.push_back(ComplementsManager::Complement{ .loc_ = { 1 , 0 }, .number_ = 9, .time_since_last_update_ = 0.5f });

    // Setting default values to called methods
    std::string default_value = "default value default value default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));

    ON_CALL(*player, GetLocation).WillByDefault(Return(Location2D(1, 1)));
    ON_CALL(*player, GetNumber).WillByDefault(Return(1));

    // Set expectations on mock methods
    EXPECT_CALL(*world, GetExtent());
    EXPECT_CALL(*world, GetContentRef());
    EXPECT_CALL(*game_status, AddToScore(9));
    EXPECT_CALL(*player, GetLocation());
    EXPECT_CALL(*player, GetNumber());

    // Invoke the method being tested
    comps_manager->UpdateComplementsLifetime(0.1f);

    // Assertion
    ASSERT_TRUE(comps_manager->complements.empty());
}

TEST(TestComplementsManager, PlayerGotScoreComplementWrong)
{
    using namespace testing;

    // Classes instantiation
    std::shared_ptr<NiceMock<MockWorld>> world = std::make_shared<NiceMock<MockWorld>>();
    std::shared_ptr<MockGameStatus> game_status = std::make_shared<MockGameStatus>();
    std::shared_ptr<NiceMock<MockPlayer>> player = std::make_shared<NiceMock<MockPlayer>>();

    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 5, 3 }));

    std::unique_ptr<ComplementsManager> comps_manager = std::make_unique<ComplementsManager>(world.get(), game_status.get(), player.get());
    comps_manager->complements.push_back(ComplementsManager::Complement{ .loc_ = { 1 , 0 }, .number_ = 8, .time_since_last_update_ = 0.5f });

    // Setting default values to called methods
    std::string default_value = "default value default value default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));

    ON_CALL(*player, GetLocation).WillByDefault(Return(Location2D(1, 1)));
    ON_CALL(*player, GetNumber).WillByDefault(Return(1));

    // Set expectations on mock methods
    EXPECT_CALL(*world, GetExtent());
    EXPECT_CALL(*world, GetContentRef());
    EXPECT_CALL(*game_status, PlayerLifesMinusOne());
    EXPECT_CALL(*player, GetLocation());
    EXPECT_CALL(*player, GetNumber());

    // Invoke the method being tested
    comps_manager->UpdateComplementsLifetime(0.1f);

    // Assertion
    ASSERT_TRUE(comps_manager->complements.empty());
}

TEST(TestComplementsManager, PlayerMissedScore)
{
    using namespace testing;

    // Classes instantiation
    std::shared_ptr<NiceMock<MockWorld>> world = std::make_shared<NiceMock<MockWorld>>();
    std::shared_ptr<MockGameStatus> game_status = std::make_shared<MockGameStatus>();
    std::shared_ptr<NiceMock<MockPlayer>> player = std::make_shared<NiceMock<MockPlayer>>();

    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 5, 3 }));

    std::unique_ptr<ComplementsManager> comps_manager = std::make_unique<ComplementsManager>(world.get(), game_status.get(), player.get());
    comps_manager->complements.push_back(ComplementsManager::Complement{ .loc_ = { 1 , 3 }, .number_ = 9, .time_since_last_update_ = 0.5f });

    // Setting default values to called methods
    std::string default_value = "default value default value default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));

    ON_CALL(*player, GetLocation).WillByDefault(Return(Location2D(1, 1)));
    ON_CALL(*player, GetNumber).WillByDefault(Return(1));

    // Set expectations on mock methods
    EXPECT_CALL(*world, GetExtent());
    EXPECT_CALL(*world, GetContentRef());
    EXPECT_CALL(*game_status, AddToScoreLost(9));
    EXPECT_CALL(*player, GetLocation());

    // Invoke the method being tested
    comps_manager->UpdateComplementsLifetime(0.1f);

    // Assertion
    ASSERT_TRUE(comps_manager->complements.empty());
}

TEST(TestComplementsManager, ScoreUpdateNoCatchNorMiss)
{
    using namespace testing;

    // Classes instantiation
    std::shared_ptr<NiceMock<MockWorld>> world = std::make_shared<NiceMock<MockWorld>>();
    std::shared_ptr<MockGameStatus> game_status = std::make_shared<MockGameStatus>();
    std::shared_ptr<NiceMock<MockPlayer>> player = std::make_shared<NiceMock<MockPlayer>>();

    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 5, 3 }));

    std::unique_ptr<ComplementsManager> comps_manager = std::make_unique<ComplementsManager>(world.get(), game_status.get(), player.get());
    comps_manager->complements.push_back(ComplementsManager::Complement{ .loc_ = { 2 , 0 }, .number_ = 9, .time_since_last_update_ = 0.5f });

    // Setting default values to called methods
    std::string default_value = "default value default value default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));

    ON_CALL(*player, GetLocation).WillByDefault(Return(Location2D(1, 1)));
    ON_CALL(*player, GetNumber).WillByDefault(Return(1));

    // Set expectations on mock methods
    EXPECT_CALL(*world, GetExtent());
    EXPECT_CALL(*world, GetContentRef());
    EXPECT_CALL(*player, GetLocation());

    // Invoke the method being tested
    comps_manager->UpdateComplementsLifetime(0.1f);

    // Assertion
    ASSERT_FALSE(comps_manager->complements.empty());
}

TEST(TestComplementsManager, ScoreNoNeedToUpdate)
{
    using namespace testing;

    // Classes instantiation
    std::shared_ptr<NiceMock<MockWorld>> world = std::make_shared<NiceMock<MockWorld>>();
    std::shared_ptr<MockGameStatus> game_status = std::make_shared<MockGameStatus>();
    std::shared_ptr<NiceMock<MockPlayer>> player = std::make_shared<NiceMock<MockPlayer>>();

    ON_CALL(*world, GetExtent).WillByDefault(Return(Location2D{ 5, 3 }));

    std::unique_ptr<ComplementsManager> comps_manager = std::make_unique<ComplementsManager>(world.get(), game_status.get(), player.get());
    comps_manager->complements.push_back(ComplementsManager::Complement{ .loc_ = { 1 , 0 }, .number_ = 9, .time_since_last_update_ = 0.1f });

    // Setting default values to called methods
    std::string default_value = "default value default value default value";
    ON_CALL(*world, GetContentRef).WillByDefault(ReturnRef(default_value));

    ON_CALL(*player, GetLocation).WillByDefault(Return(Location2D(1, 1)));
    ON_CALL(*player, GetNumber).WillByDefault(Return(1));

    // Set expectations on mock methods
    EXPECT_CALL(*world, GetExtent()).Times(0);
    EXPECT_CALL(*world, GetContentRef()).Times(0);
    EXPECT_CALL(*player, GetLocation()).Times(0);

    // Invoke the method being tested
    comps_manager->UpdateComplementsLifetime(0.1f);

    // Assertion
    ASSERT_FALSE(comps_manager->complements.empty());
}

// Run the tests
int main(int argc, char** argv) {
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
