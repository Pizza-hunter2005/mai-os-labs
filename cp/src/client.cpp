#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <signal.h>
#include <zmq.hpp>
#include <thread>
#include <chrono>


int port_iter;
std::string command;
pthread_mutex_t mutex;

zmq::context_t context(2);
zmq::socket_t player_socket(context, ZMQ_REP);


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

void* check_invite(void *param) {
    std::string invite_tmp;
    pthread_mutex_lock(&mutex);
    std::string invite_msg = receive_message(player_socket);
    std::stringstream invite_ss(invite_msg);
    std::getline(invite_ss, invite_tmp, ':');
    if (invite_tmp == "invite") {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::getline(invite_ss, invite_tmp, ':');
        std::cout << "Игрок с ником " + invite_tmp + " приглашает вас в игру!" << std::endl;
        std::cout << "Вы согласны? (y/n)" << std::endl;
        std::cin >> command;
        std::cout << "Ваш ответ: " + command + "\n";
        if (command[0] == 'y') {
            std::cout << "Вы приняли запрос!" << std::endl;
            send_message(player_socket, "accept");
            pthread_mutex_unlock(&mutex);
            pthread_exit(0);
        }
        else {
            std::cout << "Вы отклонили запрос!" << std::endl;
            pthread_mutex_unlock(&mutex);
            send_message(player_socket, "reject");
        }
    }
	pthread_exit(0);
}


typedef struct {
} check_params;


int main(int argc, char** argv) {
    zmq::context_t context(2);
    zmq::socket_t main_socket(context, ZMQ_REQ);
    int pid = getpid();
    main_socket.connect(get_port_name(5555));
    pthread_mutex_init(&mutex, NULL);

    check_params check_param;
    pthread_t tid;
    std::string received_message;
    std::string tmp;
    int iteration = 1;

    while(true) {
        if (iteration == 1) {
            iteration += 1;

            std::string login;
            std::cout << "Введите ваш логин: ";
            std::cin >> login;

            std::string login_msg = "login:" + std::to_string(pid) + ":" + login;
            send_message(main_socket, login_msg);
            received_message = receive_message(main_socket);
            std::stringstream ss(received_message);
            
            std::getline(ss, tmp, ':');
            if (tmp == "Ok") {
                std::getline(ss, tmp, ':');
                port_iter = std::stoi(tmp);
                player_socket.connect(get_port_name(5555 + port_iter));
                std::cout << "Вы успешно авторизовались!" << std::endl;
                std::cout << "Вы хотите пригласить друга? (y/n)" << std::endl;
                std::cin >> tmp;
                if (tmp[0] == 'n') {
                    std::cout << "Ждем приглашения от друга..." << std::endl;
                    pthread_create(&tid, NULL, check_invite, &check_param);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    break;
                }
                else {
                    std::cout << "Чтобы пригласить друга напишите invite (friend_login)" << std::endl;
                }
            }
            else if (tmp == "Error") {
                std::getline(ss, tmp, ':');
                if (tmp == "NameAlreadyExist") {
                    std::cout << "ERROR: Это имя уже занято! Попробуйте другое" << std::endl;
                    iteration -= 1;
                }
            }
        }
        else {
            std::cin >> command;
            if (command == "invite") {
                std::string invite_login;
                std::cin >> invite_login;
                std::cout << "Вы пригласили игрока с ником " + invite_login << std::endl;
                std::cout << "Ждем ответ..." << std::endl;
                std::string invite_cmd = "invite:" + std::to_string(port_iter) + ":" + invite_login; 
                send_message(main_socket, invite_cmd);
                received_message = receive_message(main_socket);
                std::stringstream ss(received_message);
                std::getline(ss, tmp, ':');
                if (tmp == "accept") {
                    std::cout << "Запрос принят" << std::endl;
                    break;
                }
                else if (tmp == "reject") {
                    std::cout << "Запрос отклонен" << std::endl;
                }
                else if (tmp == "Error") {
                    std::getline(ss, tmp, ':');
                    if (tmp == "SelfInvite") {
                        std::cout << "ERROR: Вы отправили запрос на игру самому себе. Попробуйте снова" << std::endl;
                    }
                    else if (tmp == "LoginNotExist") {
                        std::cout << "ERROR: Игрока с таким ником не существует. Попробуйте снова" << std::endl;
                    }
                    else if (tmp == "AlreadyInviting") {
                        std::cout << "ERROR: Другой игрок уже хочет вас пригласить. Дадим ему это сделать" << std::endl;
                        pthread_create(&tid, NULL, check_invite, &check_param);
                        break;
                    }
                }
            }
            else {
                std::cout << "Вы ввели несуществующую команду. Попробуйте снова" << std::endl;
            }
        }
    }

    pthread_mutex_lock(&mutex);
    received_message = receive_message(player_socket);
    std::string answer;
    if (received_message == "ping") {
        std::cout << "Вы готовы к игре? (y/n)" << std::endl;
        std::cin >> answer;
        if (answer[0] == 'y') {
            send_message(player_socket, "pong");
            std::cout << "Дождитесь другого игрока" << std::endl;
        }
        else {
            send_message(player_socket, "no_pong");
            std::cout << "До свидания" << std::endl;
            return 0;
        }
    }
    else {
        std::cout << "Мне прилетел не пинг" << std::endl;
    }
    if (port_iter == 1) {
        std::cout << "Начинаем игру" << std::endl;
    }
    else {
        std::cout << "Начинаем игру. Подождите, пока другой пользователь расставит корабли" << std::endl;
    }

    std::cout << "Расстановка кораблей\nФормат: x, y и ориентация H - горизонтальная или V - вертикальная через пробелы:" << std::endl;
    while(1) {
        std::string inside_msg;
        std::string send_msg;
        inside_msg = receive_message(player_socket);
        std::stringstream strs(inside_msg);
        strs >> tmp;

        if (tmp == "Разместите") {
            std::cout << inside_msg << std::endl;
            char orientation;
            int x, y;
		    std::cin >> y >> x >> orientation;
            send_msg = "coords:" + std::to_string(x) + ":" + std::to_string(y) + ":" + orientation;
            send_message(player_socket, send_msg);
        }
        else if (tmp == "board") {
            std::cout << inside_msg.substr(5, inside_msg.size()) << std::endl;
            send_message(player_socket, "ok");
        }
        else if (tmp == "Error") {
            std::cout << inside_msg << std::endl;
            send_message(player_socket, "ok");
        }

        else if (tmp == "your_turn") {
            send_message(player_socket, "ok");
            std::cout << "Ваш ход:" << std::endl;
            inside_msg = receive_message(player_socket);
            if (inside_msg == "shoot") {
                int x, y;
                std::cout << "Сделайте выстрел (введите координаты x, y через пробел):"  << std::endl;
                std::cin >> y >> x;
                send_msg = "coords:" + std::to_string(x) + ":" + std::to_string(y);
                send_message(player_socket, send_msg);
                inside_msg = receive_message(player_socket);
                if (inside_msg == "shooted") {
                    std::cout << "Есть пробитие!" << std::endl;
                    send_message(player_socket, "ok");
                }
                else if (inside_msg == "miss") {
                    std::cout << "Промах!" << std::endl;
                    send_message(player_socket, "ok");
                }
            }
        }
        else if (tmp == "not_your_turn") {
            std::cout << "Ход соперника: " << std::endl;
            send_message(player_socket, "ok");
            inside_msg = receive_message(player_socket);
            if (inside_msg == "shooted") {
                std::cout << "Вас подстрелили!" << std::endl;
                send_message(player_socket, "ok");
            }
            else if (inside_msg == "miss") {
                std::cout << "Противник промахнулся" << std::endl;
                send_message(player_socket, "ok");
            }
        }
        else if (tmp == "win") {
            std::cout << "Вы выиграли!" << std::endl;
            send_message(player_socket, "ok");
            return 0;
        }
        else if (tmp == "lose") {
            std::cout << "Вы проиграли!" << std::endl;
            send_message(player_socket, "ok");
            return 0;
        }
    }
}
 