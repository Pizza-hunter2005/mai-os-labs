#include <gtest/gtest.h>
#include "../src/GamePlayer.cpp"

TEST(PlayerTest, PlaceShipsCorrectly) {
    Player player;
    EXPECT_TRUE(player.isValidPlacement(0, 0, 4, 'H')); 
    player.placeShip(0, 0, 4, 'H');
    EXPECT_FALSE(player.isValidPlacement(0, 0, 4, 'H')); 
    EXPECT_TRUE(player.isValidPlacement(5, 5, 3, 'V')); 
}

TEST(PlayerTest, IsEmptyAround) {
    Player player;
    player.placeShip(0, 0, 4, 'H');
    EXPECT_FALSE(player.isEmptyAround(0, 0));
    EXPECT_TRUE(player.isEmptyAround(5, 5));
}


TEST(PlayerTest, GetBoardState) {
    Player player;
    player.placeShip(1, 1, 3, 'H');
    std::string board = player.getBoard();
    EXPECT_NE(board.find('O'), std::string::npos); 
}

TEST(GameTest, GameOver) {
    Game game;
    game.player1.placeShip(0, 0, 4, 'H');
    game.player2.placeShip(1, 1, 3, 'V');
    EXPECT_FALSE(game.gameOver());

    for (int i = 0; i < 4; ++i) {
        game.player1.board[0][i] = 'X';
    }
    EXPECT_TRUE(game.gameOver());
}


TEST(ClientServerTest, LoginTest) {
    zmq::context_t context(1);
    zmq::socket_t server_socket(context, zmq::socket_type::rep);
    zmq::socket_t client_socket(context, zmq::socket_type::req);

    server_socket.bind("tcp://*:5555");
    client_socket.connect("tcp://localhost:5555");

    std::string login_msg = "login:12345:Player1";
    send_message(client_socket, login_msg);

    std::string received = receive_message(server_socket);
    EXPECT_EQ(received, login_msg);

    send_message(server_socket, "Ok:1");
    std::string response = receive_message(client_socket);
    EXPECT_EQ(response, "Ok:1");
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}