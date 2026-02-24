#include <gtest/gtest.h>
#include "BalancedTree.h"
#include "CalculationNode.h"
#include "ZMQFunctions.h"

TEST(BalancedTreeTest, AddAndExist) {
    BalancedTree tree;
    tree.AddInTree(1, -1);
    EXPECT_TRUE(tree.Exist(1));
    EXPECT_FALSE(tree.Exist(2));
}

TEST(BalancedTreeTest, FindID) {
    BalancedTree tree;
    tree.AddInTree(1, -1);
    tree.AddInTree(2, 1);
    tree.AddInTree(3, 2);
    tree.AddInTree(4, -1);
    EXPECT_EQ(tree.FindID(), 1);
}

TEST(CalculationNodeTest, Ping) {
    CalculationNode node1(1, 5555, -1);
    CalculationNode node2(2, 5555, -1);
    EXPECT_EQ(node1.ping(1), "Ok: 1");
    EXPECT_EQ(node2.ping(2), "Ok: 1");
    EXPECT_EQ(node2.ping(1), "Ok: 0");
}

TEST(CalculationNodeTest, Exec) {
    CalculationNode node(1, 5555, -1);
    std::string result = node.exec("3 1 2 3");
    EXPECT_EQ(result, "Ok: 1: 6");
}

TEST(ZMQFunctionsTest, SendAndReceive) {
    zmq::context_t context;
    zmq::socket_t sender(context, ZMQ_PAIR);
    zmq::socket_t receiver(context, ZMQ_PAIR);

    int port = bind(sender, 5555);
    connect(receiver, port);

    send_message(sender, "test message");
    std::string message = receive_message(receiver);

    EXPECT_EQ(message, "test message");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}