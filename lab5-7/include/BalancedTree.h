#pragma once

#include <set>       
#include <algorithm> 
#include <cstddef>


class BalancedTree {
        class BalancedTreeNode {
            public:
                int id;
                BalancedTreeNode* left;
                BalancedTreeNode* right;
                int height;
                bool available;
                BalancedTreeNode(int id) {
                    this->id = id;
                    available = true;
                    left = nullptr;
                    right = nullptr;
                }
                void CheckAvailability(int id) {
                    if (this->id == id){
                        available = false;
                    }
                    else {
                        if (left != nullptr) {
                            left->CheckAvailability(id);
                        }
                        if (right != nullptr) {
                            right->CheckAvailability(id);
                        }
                    }
                }
                void Remove(int id, std::set<int> &ids) {
                    if (left != nullptr && left->id == id) {
                        left->RecursionRemove(ids);
                        ids.erase(left->id);
                        delete left;
                        left = nullptr;
                    }
                    else if (right != nullptr && right->id == id) {
                        right->RecursionRemove(ids);
                        ids.erase(right->id);
                        delete right;
                        right = nullptr;
                    }
                    else {
                        if (left != nullptr) {
                            left->Remove(id, ids);
                        }
                        if (right != nullptr) {
                            right->Remove(id, ids);
                        }
                    }
                }
                void RecursionRemove(std::set<int> &ids) {
                    if (left != nullptr) {
                        left->RecursionRemove(ids);
                        ids.erase(left->id);
                        delete left;
                        left = nullptr;
                    }
                    if (right != nullptr) {
                        right->RecursionRemove(ids);
                        ids.erase(right->id);
                        delete right;
                        right = nullptr;
                    }
                }
                void AddInNode(int id, int parent_id, std::set<int> &ids) {
                    if (this->id == parent_id) {
                        if (left == nullptr){
                            left = new BalancedTreeNode(id);
                        }
                        else {
                            right = new BalancedTreeNode(id);
                        }
                        ids.insert(id);
                        
                    }
                    else {
                        if (left != nullptr) {
                            left->AddInNode(id, parent_id, ids);
                        }
                        if (right != nullptr) {
                            right->AddInNode(id, parent_id, ids);
                        }
                    }
                }
                int MinimalHeight() {
                    if (left == nullptr || right == nullptr) {
                        return 0;
                    }
                    int left_height = -1;
                    int right_height = -1;
                    if (left != nullptr && left->available == true) {
                        left_height = left->MinimalHeight();
                    }
                    if (right != nullptr && right->available == true) {
                        right_height = right->MinimalHeight();
                    }
                    if (right_height == -1 && left_height == -1) {
                        available = false;
                        return -1;
                    }
                    else if (right_height  == -1) {
                        return left_height + 1;
                    }
                    else if (left_height == -1) {
                        return right_height + 1;
                    }
                    else {
                        return std::min(left_height, right_height) + 1;
                    }
                }
                int IDMinimalHeight(int height, int current_height) {
                    if (height < current_height) {
                        return -2;
                    }
                    else if (height > current_height) {
                        int current_id = -2;
                        if (left != nullptr && left->available == true) {
                            current_id = left->IDMinimalHeight(height, (current_height + 1));
                        }
                        if (right != nullptr && right->available == true && current_id == -2){
                            current_id = right->IDMinimalHeight(height, (current_height + 1));
                        }
                        return current_id;
                    }
                    else {
                        if (left == nullptr || right == nullptr){
                            return id;
                        }
                        return -2;
                    }
                }
                ~BalancedTreeNode() {}
        };
    private:
            BalancedTreeNode* root;
    public:
        std::set<int> ids;
        BalancedTree() {
            root = new BalancedTreeNode(-1);
        } 
        bool Exist(int id) {
            if (ids.find(id) != ids.end()) {
                return true;
            }
            return false;
        }
        void AvailabilityCheck(int id) {
            root->CheckAvailability(id);
        }
        int FindID() {
            int h = root->MinimalHeight();
            return root->IDMinimalHeight(h, 0);
        }
        void AddInTree(int id, int parent) {
            root->AddInNode(id, parent, ids);
        }
        void RemoveFromRoot(int idElem) {
            root->Remove(idElem, ids);
        }
        ~BalancedTree() {
            root->RecursionRemove(ids);
            delete root;
        }
};
