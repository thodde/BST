/*************************************
 * Author: Trevor Hodde
 * 
 * CS 5084 Bonus Project
 *************************************/

#include <stdlib.h>
#include <stdio.h>

// Create the node object
// Also add the pointer to the parent node
// as well as the count variable for the number
// of nodes in its subtree
typedef struct Node {
    int element;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int count;
} Node;

Node* tree_ptr;
Node* root;

// Create the first node in the tree and make it the root
Node* create_tree() {
    Node* tempNode;
    tempNode = malloc(sizeof(Node));
    tempNode->element = 0;
    tempNode->left = NULL;
    tempNode->right = NULL;
    return tempNode;
}

// Create a new node and assign its parent and its value
// return the pointer to the new node
Node* create_node(Node* parent, int x) {
    Node* tempNode;
    tempNode = malloc(sizeof(Node));
    tempNode->element = x;
    tempNode->left = NULL;
    tempNode->right = NULL;
    tempNode->parent = parent;
    tempNode->count = 0;
    return tempNode;
}

// Reduce the value of the counter if the node was not
// inserted into the tree
void decrease_count(Node* node) {
    while(node->parent != NULL) {
        node = node->parent;
        node->count--;
    }
}

int determine_count(Node* node) {
    int count;

    if(node == NULL) {
        return 0;
    }

    count = 1 + determine_count(node->left) + determine_count(node->right);
    return count;
}

Node* rotate_left(Node* node) {
    Node* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    tmp->parent = node->parent;
    tmp->right = NULL;
    node->parent = tmp;
    //tmp->parent->left = tmp;
    node->left = NULL;
    node->count--;
    tmp->count++;
    return tmp;
}

Node* rotate_right(Node* node) {
    Node* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    tmp->parent = node->parent;
    tmp->left = NULL;
    node->parent = tmp;
    //tmp->parent->right = tmp;
    node->right = NULL;
    node->count--;
    tmp->count++;
    return tmp;
}

Node* BST_insert_element(Node* parent, Node* node, int x) {
    //insert element to Tree
    if(node == NULL) {
        return create_node(parent, x);
    }
    else {
	    node->count++;
        if(x < node->element) {
            node->left = BST_insert_element(node, node->left, x);
	        return node;
        }
        else if(x > node->element) {
            node->right =  BST_insert_element(node, node->right, x);
	        return node;
        }
        else if(x == node->element) {
            printf("Oops! %d is already present in the tree.\n", node->element);
            node->count--;
            decrease_count(node);
            return node;
        }
    }
    return NULL;
}

/**
 * Always insert the new node as a leaf, but then rotate the node
 * up the tree until it is the root.
 */
Node* BST_insert_at_root(Node* parent, Node* node, int x) {
	if(node == NULL) {
	    return create_node(parent, x);
	}
	else {
	    if(x < node->element) {
            node->left = BST_insert_at_root(node, node->left, x);
            node = rotate_right(node);
            node->count = determine_count(node);
	    }
	    else if(x > node->element) {
            node->right = BST_insert_at_root(node, node->right, x);
            node = rotate_left(node);
            node->count = determine_count(node);
	    }
	    else {
            printf("Oops! %d is already present in the tree\n", node->element);
            //node->count = determine_count(node);
            //decrease_count(node);
	    }

	    // Make sure the tree knows that the new node is the root
	    root = node;
	    root->parent = NULL;
	    root->count = determine_count(root);
	    return node;
	}
}

// something is not quite right with this function...
void BST_delete(Node* root, Node* node, int x) {
    if(node == NULL) {
        printf("Node does not exist in this tree.\n");
    }

    Node* temp = node;
    if(x < node->element) {
        BST_delete(root, node->left, x);
    }
    else if(x > node->element) {
        BST_delete(root, node->right, x);
    }
    else {
        //We must be on the correct node
        // Handle the 3 cases here: 0 children, 1 child, 2 children
        if(node->left == NULL) {
            node = node->right;
            free(temp);
        }
        else if(node-> right == NULL) {
            node = node->left;
            free(temp);
        }
        else {
            temp = node->left;
            while(temp->right != NULL) {
                temp = temp->right;
            }
            root->element = temp->element;
            BST_delete(root, node->left, temp->element);
        }
    }
}

Node* display_tree(Node* node) {
    if(node == NULL) {
        return NULL;
    }

    display_tree(node->left);
    printf("-----------------------\n");
    printf("Node value: %d\n", node->element); 
 
    if(node->parent == NULL) {
	    printf("Parent is NULL\n");
    }
    else {
        printf("Parent value: %d\n", node->parent->element);
    }

    printf("Subtree count: %d\n", node->count);
    printf("-----------------------\n");
    display_tree(node->right);
    return node;
}

int main() {
    //Create the root of tree
    tree_ptr = create_tree();

    root = tree_ptr;

    // Assign the first element as the root of the tree
    root->element = 3;
    root->parent = NULL;
    printf("Root of tree: %d\n", root->element);

    printf("---Leaf insertion---\n");
    // Insert as a leaf
    BST_insert_element(NULL, root, 2);
    BST_insert_element(NULL, root, 1);
    BST_insert_element(NULL, root, 5);
    BST_insert_element(NULL, root, 1);
    BST_insert_element(NULL, root, 3);
    BST_insert_element(NULL, root, 2);
    BST_insert_element(NULL, root, 4);

    display_tree(root);

    tree_ptr = create_tree();
    root = tree_ptr;
    root->element = 3;
    root->parent = NULL;

    printf("\n---Root insertion---\n");
    // Insert at the root
    BST_insert_at_root(NULL, root, 2);
    BST_insert_at_root(NULL, root, 1);
    BST_insert_at_root(NULL, root, 5);
    BST_insert_at_root(NULL, root, 1);
    BST_insert_at_root(NULL, root, 3);
    BST_insert_at_root(NULL, root, 2);
    BST_insert_at_root(NULL, root, 4);

    display_tree(root);

    //printf("Removing element 5.\n");
    //BST_delete(root, root, 5);
    //display_tree(root);

    printf("\n");

    return 0;
}