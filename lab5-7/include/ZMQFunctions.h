#pragma once

#include <string>    
#include <cstring>   
#include <iostream> 
#include <zmq.hpp>

const int MAIN_PORT = 4040;


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


void connect(zmq::socket_t &socket, int port) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.connect(address);
}

void disconnect(zmq::socket_t &socket, int port) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.disconnect(address);
}

int bind(zmq::socket_t &socket, int id) {
    int port = MAIN_PORT + id;
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    while(1){
        try{
            socket.bind(address);
            break;
        }
        catch(...){
            port++;
        }
    }
    return port;
}

void unbind(zmq::socket_t &socket, int port) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(port);
    socket.unbind(address);
}
