#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include <cassert>
#include <zmq.hpp>
#include <chrono>
#include <thread>
#include <exception>
#include <map>
#include <cstdlib>
#include <ctime>

const int BOARD_SIZE = 10;
zmq::context_t context(3);
zmq::socket_t main_socket(context, ZMQ_REP);

void send_message(zmq::socket_t &socket, const std::string &msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message, zmq::send_flags::none);
}

std::string receive_message(zmq::socket_t &socket) {
    zmq::message_t message;
    auto chars_read = socket.recv(message, zmq::recv_flags::none);
    if (!chars_read) {
        throw std::runtime_error("Failed to receive message");
    }
    return std::string(static_cast<char*>(message.data()), message.size());
}

std::string get_port_name(const int port) {
    return "tcp://127.0.0.1:" + std::to_string(port);
}

class Player {
public:
    std::vector<std::vector<char>> board;
	int num;

    Player() {
        board = std::vector<std::vector<char>>(BOARD_SIZE, std::vector<char>(BOARD_SIZE, ' '));
    }

    void shipsPlacement(zmq::socket_t &player_socket) {
		std::string msg;
		std::string tmp;

		int ships_count = 5;
		// 
		for (int i = 1; i <= 1; ++i) {
			ships_count -= 1;
			for (int j = 0; j < ships_count; j++) {
				char orientation;
				int x, y;
				msg = "Разместите корабль " + std::to_string(i) + " (1x" + std::to_string(i) + "): ";
				send_message(player_socket, msg);
				std::string received_message = receive_message(player_socket);
				std::cout << "Получил запрос: " + received_message << std::endl;

				std::stringstream strs(received_message);
				std::getline(strs, tmp, ':');
				if (tmp == "coords") {
					std::getline(strs, tmp, ':');
					x = std::stoi(tmp);
					std::getline(strs, tmp, ':');
					y = std::stoi(tmp);
					std::getline(strs, tmp, ':');
					orientation = tmp[0];

				
					if (!((orientation == 'V') or (orientation == 'H'))) {
						send_message(player_socket, "Error : Неверный ввод!");
						received_message = receive_message(player_socket);
						--j;
						continue;
					}
					if (isValidPlacement(x, y, i, orientation)) {
						placeShip(x, y, i, orientation);
					} else {
						send_message(player_socket, "Error : Неверное местоположение! Попробуйте еще раз.");
						received_message = receive_message(player_socket);
						--j;
						continue;
					}

					std::string boardState = "board";
					boardState += getBoard();
					// Получение нового состояния доски
					send_message(player_socket, boardState);
					received_message = receive_message(player_socket);
				}
			}
		}
    }

    bool isValidPlacement(int x, int y, int size, char orientation) const {
		if ((x > 9) or (x < 0) or (y > 9) or (y < 0)) {
			return false;
		}
        if (orientation == 'V') {
            if (x + size - 1 >= BOARD_SIZE) {
                return false;
            }

            for (int i = x; i < x + size; ++i) {
                if (board[i][y] != ' ') {
                    return false;
                }
            }
        } 
		else if (orientation == 'H') {
            if (y + size - 1 >= BOARD_SIZE) {
                return false;
            }

            for (int j = y; j < y + size; ++j) {
                if (board[x][j] != ' ') {
                    return false;
                }
            }
        }
		for (int i = 0; i < size; i++) {
			if (orientation == 'H') {
				if (!isEmptyAround(x + i, y)) {
					return false;
				}
			}
			else {
				if (!isEmptyAround(x, y + i)) {
					return false;
				}
			}
		}

        return true;
    }

	bool isEmptyAround(int x, int y) const {
		for (int i = x - 1; i <= x + 1; ++i) {
			for (int j = y - 1; j <= y + 1; ++j) {
				if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && board[i][j] != ' ') {
					return false;
				}
			}
		}
		return true;
	}

    void placeShip(int x, int y, int size, char orientation) {
        if (orientation == 'V') {
            for (int i = x; i < x + size; ++i) {
                board[i][y] = 'O';
            }
        } else if (orientation == 'H') {
            for (int j = y; j < y + size; ++j) {
                board[x][j] = 'O';
            }
        }
    }

    std::string getBoard() const {
		std::string result;
		std::string space(1, ' ');
        result = "  0 1 2 3 4 5 6 7 8 9\n";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            result += std::to_string(i) + " ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
				std::string brd(1, board[i][j]);
                result += brd + space;
            }
            result += '\n';
        }
        result += '\n';
		return result;
    }

    std::string getClearBoard() const {
		std::string result;
		std::string space(1, ' ');
        result = "  0 1 2 3 4 5 6 7 8 9\n";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            result += std::to_string(i) + " ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
				std::string brd(1, board[i][j]);
				if (brd == "O") {
                	result += space + space;	
				}
				else {
					result += brd + space;
				}
            }
            result += '\n';
        }
        result += '\n';
		return result;
    }
};

class Game {
public:
    Player player1;
	Player player2;

    void play(zmq::socket_t &player1_socket, zmq::socket_t &player2_socket) {
        std::cout << "Игра началась\n" << std::endl;

		std::string msg1;
		std::string msg2;
		player1.num = 1;
		player2.num = 2;
        player1.shipsPlacement(player1_socket);
        player2.shipsPlacement(player2_socket);

		int turn = 0;
        while (!gameOver()) {
			if (turn % 2 == 0) {
				msg1 = "your_turn";
				msg2 = "not_your_turn";
				send_message(player1_socket, msg1);
				receive_message(player1_socket);
				send_message(player2_socket, msg2);
				receive_message(player2_socket);
				std::cout << "Ход игрока 1:" << std::endl;
				if (playerTurn(player1, player2, player1_socket, player2_socket)) {
					if (gameOver())  {
						std::cout << "Победил игрок 1" << std::endl;
						send_message(player1_socket, "win");
						receive_message(player1_socket);
						send_message(player2_socket, "lose");
						receive_message(player2_socket);
						break;
					}
					continue;
				}
				else {
					turn += 1;
				}
			}
			else {
				std::cout << "Ход игрока 2:" << std::endl;
				msg1 = "your_turn";
				msg2 = "not_your_turn";
				send_message(player2_socket, msg1);
				receive_message(player2_socket);
				send_message(player1_socket, msg2);
				receive_message(player1_socket);
				if (playerTurn(player2, player1, player2_socket, player1_socket)) {
					if (gameOver()) {
						std::cout << "Победил игрок 2" << std::endl;
						send_message(player2_socket, "win");
						receive_message(player2_socket);
						send_message(player1_socket, "lose");
						receive_message(player1_socket);
						break;
					}
					continue;
				}
				else {
					turn += 1;
				}
			}
        }
        std::cout << "Конец игры!" << std::endl;
    }

    bool gameOver() const {
        return allShipsShoted(player1) || allShipsShoted(player2);
    }

    bool allShipsShoted(const Player& player) const {
        for (const auto& row : player.board) {
            for (char cell : row) {
                if (cell == 'O') {
                    return false;
                }
            }
        }
        return true;
    }


    bool playerTurn(Player& attacker, Player& defender, zmq::socket_t &attacker_socket, zmq::socket_t &defender_socket) {
		bool shoot = false;
		std::string received_message;
		std::string tmp;
        int x, y;
		send_message(attacker_socket, "shoot");
		received_message = receive_message(attacker_socket);
		std::stringstream strs(received_message);
		std::getline(strs, tmp, ':');
		std::getline(strs, tmp, ':');
		x = std::stoi(tmp);
		std::getline(strs, tmp, ':');
		y = std::stoi(tmp);

        if (isValidMove(x, y, defender)) {
            if (defender.board[x][y] == 'O') {
				send_message(attacker_socket, "shooted");
				received_message = receive_message(attacker_socket);
				send_message(defender_socket, "shooted");
				received_message = receive_message(defender_socket);

                std::cout << "Попадание!" << std::endl;
                defender.board[x][y] = 'X';
				shoot = true;
				return shoot;
            } else {
				send_message(attacker_socket, "miss");
				received_message = receive_message(attacker_socket);
				send_message(defender_socket, "miss");
				received_message = receive_message(defender_socket);

                std::cout << "Промах" << std::endl;
                defender.board[x][y] = '*';
				std::string board = defender.getBoard();
				std::string clearBoard = defender.getClearBoard();
				
				send_message(attacker_socket, "board" + clearBoard);
				received_message = receive_message(attacker_socket);
				send_message(defender_socket, "board" + board);
				received_message = receive_message(defender_socket);
				return shoot;
            }

        } else {
            std::cout << "Неверные ввод, попробуйте еще раз." << std::endl;
            return playerTurn(attacker, defender, attacker_socket, defender_socket);
        }
    }

    bool isValidMove(int x, int y, const Player& defender) const {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE &&
               (defender.board[x][y] == ' ' || defender.board[x][y] == 'O');
    }
};
