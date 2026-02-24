#include <bits/stdc++.h>  
#include "CalculationNode.h"
#include "ZMQFunctions.h" 
#include "BalancedTree.h"

void menu(){
    std::cout << "Enter your command:\n";
    std::cout << "1. create <id>\n";
    std::cout << "2. exec <id> <n> <n1 n2... n>\n";
    std::cout << "3. ping <id>\n";
    std::cout << "4. kill <id>\n";
    std::cout << "5. exit\n";
}


int main() {
    std::string command;
    CalculationNode node(-1, -1, -1);
    std::string answer;
    BalancedTree tree;

    menu();

    while ((std::cout << "> ") && (std::cin >> command)) {

        if (command == "create") {
            int child; 
            std::cin >> child;
            
            if (tree.Exist(child)) {
                std::cout << "Error: " +  std::to_string(child) + ":  Already exists\n";
            }
            else {
                while (true) {
                    int idParent = tree.FindID();
                    if (idParent == node.id) {
                        answer = node.create(child);
                        tree.AddInTree(child, idParent);
                        break;
                    }
                    else {
                        std::string message = "create " +  std::to_string(child);
                        answer = node.sendstring(message, idParent);
                        if (answer == "Error: Parent not found") {
                            tree.AvailabilityCheck(idParent);
                        }
                        else {
                            tree.AddInTree(child, idParent);
                            break;
                        }
                    }
                }
                std::cout << answer << std::endl;
            }
        }

        else if (command == "exec") {
            int child;
            if (!tree.Exist(child)) {
                std::cout << "Error: Node does not exist!\n";
            }

            std::string str;
            std::cin >> child;
            std::getline(std::cin, str);

            if (!tree.Exist(child)) {
                std::cout << "Error: Parent is not existed\n";
            }
            else {
                std::string message = "exec " + str;
                answer = node.sendstring(message, child);
                std::cout << answer <<  std::endl;
            }
        } 

        else if (command == "ping") {
            int child; std::cin >> child;

            if (!tree.Exist(child)) {
                std::cout << "Ok: 0\n";
            }
            else if (node.left_id == child || node.right_id == child) {
                answer = node.ping(child);
                std::cout << answer <<  std::endl;
            }
            else {
                std::string message = "ping " +  std::to_string(child);
                answer = node.sendstring(message, child);
                std::cout << answer <<  std::endl;
            } 
        }

        else if (command == "kill") {
            int child; 
            std::cin >> child;

            std::string message = "kill";
            if (!tree.Exist(child)) {
                std::cout << "Error: Parent is not existed\n";
            }
            else {
                answer = node.sendstring(message, child);
                if (answer != "Error: Parent not found") {
                    tree.RemoveFromRoot(child);
                    if (child == node.left_id){
                        node.left_id = -2;
                        unbind(node.left, node.left_port);
                        answer = "Ok";
                    }
                    else if (child == node.right_id) {
                        node.right_id = -2;
                        unbind(node.right, node.right_port);
                        answer = "Ok";
                    }
                    else {
                        message = "clear " +  std::to_string(child);
                        answer = node.sendstring(message,  stoi(answer));
                    }
                    std::cout << answer << std::endl;
                }
            }
        }
        else if (command == "exit") {
            node.kill();
            return 0;
        }
        else {
            std::cout << "Wrong command!\n\n";
            menu();
        }
    }
    
    
}