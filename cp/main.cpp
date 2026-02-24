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
#include "src/GamePlayer.cpp"


int main() {
    zmq::socket_t first_player_socket(context, ZMQ_REQ);
    zmq::socket_t second_player_socket(context, ZMQ_REQ);
    main_socket.bind("tcp://*:5555");
    first_player_socket.bind("tcp://*:5556");
    second_player_socket.bind("tcp://*:5557");
    std::cout << "Сервер начал работу" << std::endl;
    std::map<int, std::string> auth_map;
    int port_iter = 1;
    while (true) {
        std::string received_message = receive_message(main_socket);
        std::cout << "Запрос: '" + received_message + "'" << std::endl;
        std::stringstream ss(received_message);
        std::string tmp;
        std::getline(ss, tmp, ':');
        if (tmp == "login") {
            if (port_iter > 2) {
                send_message(main_socket, "Error:TwoPlayersAlreadyExist");
            }
            else {
                std::getline(ss, tmp, ':');
                std::getline(ss, tmp, ':');
				if (auth_map[1] == tmp) {
					std::cout << "Игрок ввел занятое имя" << std::endl;
					send_message(main_socket, "Error:NameAlreadyExist");
				}
				else {
					std::cout << "Логин игрока номер " + std::to_string(port_iter) + ": " + tmp << std::endl;
					std::string login = tmp;
					auth_map[port_iter] = login;
					
					send_message(main_socket, "Ok:" + std::to_string(port_iter));
					port_iter += 1;
				}
            }
        }
        // получили от клиента запрос на игру другому клиенту
        else if (tmp == "invite") {
			std::cout << "Обработка приглашения" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

            std::string invite_login;
            std::getline(ss, tmp, ':');
            int sender_id = std::stoi(tmp);
            std::getline(ss, invite_login, ':');
            
            // если клиент 1 отправил запрос клиенту 2
			if (invite_login == auth_map[sender_id]) {
				std::cout << "Игрок пригласил сам себя" << std::endl;
				send_message(main_socket, "Error:SelfInvite");
			}
            else if (invite_login == auth_map[2]) {
				std::cout << "Игрок " + auth_map[1] + " пригласил в игру " + auth_map[2] << std::endl;
                send_message(second_player_socket, "invite:" + auth_map[1]);
				std::string invite_message = receive_message(second_player_socket);

				second_player_socket.set(zmq::sockopt::rcvtimeo, -1);
                if (invite_message == "accept") {
					std::cout << "Игрок " + auth_map[2] + " принял запрос " << std::endl;
                    send_message(main_socket, invite_message);
                    break;
                }
                else if (invite_message == "reject") {
					std::cout << "Игрок " + auth_map[2] + " отклонил запрос " << std::endl;
                    send_message(main_socket, invite_message);
                }
                else {
                    std::cout << "Что-то пошло не так во время обработки запроса на игру" << std::endl;
                }

            }
            // если клиент 2 отправил запрос клиенту 1
            else if (invite_login == auth_map[1]){
				std::cout << "Игрок " + auth_map[2] + " пригласил в игру " + auth_map[1] << std::endl;
                send_message(first_player_socket, "invite:" + auth_map[2]);
				std::string invite_message = receive_message(first_player_socket);

                if (invite_message == "accept") {
					std::cout << "Игрок " + auth_map[1] + " принял запрос " << std::endl;
                    send_message(main_socket, invite_message);
                    break;
                }
                else if (invite_message == "reject") {
					std::cout << "Игрок " + auth_map[1] + " отклонил запрос " << std::endl;
                    send_message(main_socket, invite_message);
                }
                else {
                    std::cout << "Что-то пошло не так во время обработки запроса на игру" << std::endl;
                }
            }
            else {
				std::cout << "Логина " + invite_login + " нет в базе" << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				send_message(main_socket, "Error:LoginNotExist");
            }
            std::getline(ss, tmp, ':');
        }
    }
	
    std::cout << "Подтверждение готовности игроков" << std::endl;
	send_message(first_player_socket, "ping");
	send_message(second_player_socket, "ping");

	std::string received_message1 = receive_message(first_player_socket);
	std::string received_message2 = receive_message(second_player_socket);

	if (received_message1 == "pong") {
		std::cout << "Игрок " + auth_map[1] + " готов" << std::endl;
	}
	else {
		std::cout << "Игрок " + auth_map[1] + " откзался от игры" << std::endl;
		return 0;
	}

	if (received_message2 == "pong") {
		std::cout << "Игрок " + auth_map[2] + " готов" << std::endl;
	}
	else {
		std::cout << "Игрок " + auth_map[2] + " откзался от игры" << std::endl;
		return 0;
	}


    std::cout << "Начинаю игру!" << std::endl;
    Game game;
    game.play(first_player_socket, second_player_socket);
}