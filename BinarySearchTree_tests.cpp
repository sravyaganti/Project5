// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;

// Test with empty tree
TEST(test_emptyTree) {
    // empty, size, height
    BinarySearchTree<int> b;
    ASSERT_TRUE(b.empty());
    ASSERT_TRUE(b.size() == 0);
    ASSERT_TRUE(b.height() == 0);

    //find
    const int search = 3; 
    ASSERT_EQUAL(b.find(search), b.end());

    //preorder
    ostringstream oss_preorder;
    b.traverse_preorder(oss_preorder);
    cout << "preorder" << endl;
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "");

    // min and max
    ASSERT_EQUAL(b.min_element(), b.end());
    ASSERT_EQUAL(b.max_element(), b.end());

    // in order
    ostringstream oss_order;
    b.traverse_inorder(oss_order);
    cout << "order" << endl;
    cout << oss_order.str() << endl << endl;
    ASSERT_TRUE(oss_order.str() == "");

    // copy
    BinarySearchTree<int> copy;
    copy = b; 
    ostringstream oss_copy;
    copy.traverse_preorder(oss_copy);
    ASSERT_TRUE(oss_copy.str() == "");

    //min greater than
    ASSERT_EQUAL(b.min_greater_than(2), b.end());

    //insert
    b.insert(5); 
    ASSERT_EQUAL(5, *b.find(5));

}

TEST(test_oneElt) {
    BinarySearchTree<int> b;
    b.insert(5);
    ASSERT_EQUAL(*b.min_greater_than(2), 5);
    ASSERT_TRUE(b.min_element() == b.begin());
    ASSERT_TRUE(b.max_element() == b.begin());
    ASSERT_FALSE(b.empty());
    ASSERT_TRUE(b.size() == 1);
    ASSERT_TRUE(b.height() == 1);
    ostringstream oss_preorder;
    b.traverse_preorder(oss_preorder);
    cout << "preorder" << endl;
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "5 ");
    ostringstream oss_order;
    b.traverse_inorder(oss_order);
    cout << "order" << endl;
    cout << oss_order.str() << endl << endl;
    ASSERT_TRUE(oss_order.str() == "5 ");
    BinarySearchTree<int> copy;
    copy = b; 
    ostringstream oss_copy;
    copy.traverse_preorder(oss_copy);
    ASSERT_TRUE(oss_copy.str() == "5 ");
}

// Test with tree of elements 5, 3, 7
TEST(test_multipleElts) {
    BinarySearchTree<int> b;
    b.insert(5); 
    b.insert(3);
    b.insert(7); 
    ASSERT_FALSE(b.empty());
    ASSERT_TRUE(b.size() == 3);
    ASSERT_TRUE(b.height() == 2);

    // making copy
    BinarySearchTree<int> copy;
    copy = b; 
    ostringstream oss_copy;
    copy.traverse_preorder(oss_copy);
    ASSERT_TRUE(oss_copy.str() == "5 3 7 ");

    // find
    const int search = 4;
    ASSERT_EQUAL(b.find(search), b.end());

    ASSERT_TRUE(b.check_sorting_invariant()); 
    ASSERT_EQUAL(*b.max_element(), 7); 
    ASSERT_EQUAL(*b.min_element(), 3); 
    b.insert(8); 
    ASSERT_EQUAL(*b.find(8), 8); 
}


// Return node the elemnt is at
TEST(test_find_hasElt) {
    BinarySearchTree<int> b;
    b.insert(5); 
    const int search = 5;
    ASSERT_EQUAL(b.find(search), b.begin());
}


TEST(test_preorder_normal) {
    BinarySearchTree<int> b;
    b.insert(3); 
    b.insert(2); 
    b.insert(5); 
    ostringstream oss_preorder;
    b.traverse_preorder(oss_preorder);
    cout << "preorder" << endl;
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "3 2 5 ");
}

TEST(test_inorder_normal) {
    BinarySearchTree<int> b;
    b.insert(3); 
    b.insert(2); 
    b.insert(5); 
    ostringstream oss_preorder;
    b.traverse_inorder(oss_preorder);
    cout << "preorder" << endl;
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "2 3 5 ");
}

TEST(test_minGreater_normal) {
    BinarySearchTree<int> b;
    b.insert(3); 
    b.insert(2); 
    b.insert(5); 
    ASSERT_TRUE(*b.min_greater_than(2) == 3);
    ASSERT_EQUAL(b.min_greater_than(100), b.end()); 
}

TEST_MAIN()
