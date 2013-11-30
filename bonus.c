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

// The tree structure
typedef struct Tree {
    Node* root;
} Tree;

// Create the tree and give it the root
Tree* create_tree(Node* root) {
    Tree* temp;
    temp = malloc(sizeof(Tree));
    temp->root = root;
    return temp;
}

// Create a new node and assign its value
// return a pointer to the new node
Node* create_node(int element) {
    Node* tempNode;
    tempNode = malloc(sizeof(Node));
    tempNode->element = element;
    tempNode->left = NULL;
    tempNode->right = NULL;
    tempNode->parent = NULL;
    tempNode->count = 0;
    return tempNode;
}

/*
 * Walk the tree and print out each node
 */
void inorder_walk(Node* node) {
	if(node != NULL) {
        // first print the left subtree
        inorder_walk(node->left);

        // next print the middle node
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

        //finally print the right subtree
        inorder_walk(node->right);
    }
}

// Reduce the value of the counter if the node was not
// inserted into the tree
void update_count(Node* node) {
    while(node->parent != NULL) {
        node = node->parent;
        node->count--;
    }
}

// Swap two nodes right
Node* rotate_right(Node* h) {
	Node* x = h->left;

	int x_count = x->count;
	x->count = h->count;
	h->count = h->count - x_count - 1 + (x->right != NULL ? (x->right->count + 1) : 0);

	Node* h_parent = h->parent;
	h->left = x->right;

	if (x->right != NULL) {
		x->right->parent = h;
	}

	h->parent = x;
	x->right = h;
	x->parent = h_parent;

	return x;
}

// swap two nodes left
Node* rotate_left(Node* h) {
	Node* x = h->right;

	int x_count = x->count;
	x->count = h->count;
	h->count = h->count - x_count - 1 + (x->left != NULL ? (x->left->count + 1) : 0);

	Node* h_parent = h->parent;
	h->right = x->left;

	if (x->left != NULL) {
		x->left->parent = h;
	}

	h->parent = x;
	x->left = h;
	x->parent = h_parent;

	return x;
}

/**
 * Insert a node as a leaf (non-recursively)
 * Based on class notes.
 */
void BST_insert_element(Tree* tree, Node* node) {
	Node* y = NULL;
	Node* x = tree->root;
	short duplicate = 0;

	if (x == NULL) {
		tree->root = node;
	}
	else {
        // Non-recursive solution to add new nodes to the tree
		while (x != NULL) {
			y = x;
			x->count++;
			if (node->element < x->element) {
				x = x->left;
			}
            else if (node->element > x->element) {
                x = x->right;
            }
            else {
				duplicate = 1;
				x->count--;
				update_count(x);
				break;
			}
		}

        // Add new node to the tree if it is not a duplicate
		if (duplicate == 0) {
			if (node->element < y->element) {
				y->left = node;
				node->parent = y;
			}
            else {
				y->right = node;
				node->parent = y;
			}
		}
        else {
            printf("%d is already in the tree!\n", node->element);
        }
	}
}

/**
 * Always insert the new node as a leaf, but then rotate the node
 * up the tree until it is the root.
 * This function is based on the sample provided in the notes.
 */
Node* top(Node* h, Node* z) {
	Node* v;

	if (h == NULL) {
		return z;
	}

	h->count++;
	if (z->element < h->element) {
		v = top(h->left, z);

		if (v == NULL) {
			return v;
		}

		h->left = v;
		h = rotate_right(h);
	}
    else if (z->element > h->element) {
		v = top(h->right, z);

		if (v == NULL) {
			return v;
		}

		h->right = v;
		h = rotate_left(h);
	}
    else {
        printf("%d is already in the tree!\n", h->element);
		h->count--;
		update_count(h);
		return NULL;
	}
	return h;
}

Node* insertT(Tree* tree, Node* node) {
	Node* tmp = top(tree->root, node);

	if (tmp == NULL) {
		return tmp;
	}

	tree->root = tmp;
	return tree->root;
}

/*
 * Deletes a leaf node from the tree
 */
void BST_delete(Tree* tree, Node* node) {
	if (node->right != NULL || node->left != NULL) {
		printf("Must delete a leaf node!\n");
	}

    // if we delete the root
	if (node->parent == NULL) {
		tree->root = NULL;
	}
    else {
        // if the node is a leaf node...
		update_count(node);
		Node* parent = node->parent;

		if (parent->element > node->element) {
			parent->left = NULL;
		}
        else {
			parent->right = NULL;
		}
	}
}

// Replace a node with its single child node. 
// Based on the class notes
// Precondition: Cannot replace a node that has both a left and right child
void BST_replace(Tree* tree, Node* z, Node* a) {
	if (z->right != NULL && z->left != NULL) {
		printf("Cannot delete a node with a left child and a right child!\n");
	}

    // If the node is the root...
	if (z->parent == NULL) {
		tree->root = a;
		a->parent = NULL;
	}
    else {
		update_count(z);
		Node* y = z->parent;
		a->parent = y;

		if (z->element < a->element) {
			y->left = a;
		}
        else {
			y->right = a;
		}
	}
}

// Find the maximum node in the left subtree
// and rotate it up
Node* max_rot(Node* k) {
	Node* u = k;
	Node* v;

	while ((v = u->right) != NULL) {
		rotate_left(u);
		u = v;
	}
	return u;
}

// Find the minimum node in the right subtree
// and rotate it up
Node* min_rot(Node* k) {
	Node* u = k;
	Node* v;

	while ((v = u->left) != NULL) {
		rotate_right(u);
		u = v;
	}
	return u;
}

// Delete a node that has two child nodes
// This function is based on the sample function
// provided in the notes
// Precondition: The node to be deleted has at least two child nodes (a left and a right)
void BST_delete_2(Node* z, Node* y) {
    // since the node to be deleted requires a left and right child, 
    // make sure both exist before continuing
    if(z->left == NULL) {
        printf("left child does not exist!\n");
    }
    else if(z->right == NULL) {
        printf("right child does not exist!\n");
    }
    else {
        printf("z is not null.\n");
	    Node* k = z->left;
	    Node* n = z->right;
	    update_count(z);
	    Node* k_prime = max_rot(k);
	    Node* n_prime = min_rot(n);
	    k_prime->parent = y;

	    if (y->left->element == z->element) {
		    y->left = k_prime;
	    }
        else {
		    y->right = k_prime;
	    }

	    n_prime->parent = k_prime;
	    k_prime->right = n_prime;
	    k_prime->count += n_prime->count + 1;
    }
}

// Search the tree recursively and find a specific node
Node* BST_find(int key, Node* node) {
    // The node does not exist!
    if (node == NULL) {
        printf("%d is not in the tree!\n", key);
        return NULL;
    }

    // The node was found!
    if (node->element == key) {
        printf("Found %d in the tree!\n", key);
		return node;
	}
    else {
        // Continue searching in the left or the right subtree
	    if (node->element < key) {
		    return BST_find(key, node->right);
	    }
        else {
		    return BST_find(key, node->left);
	    }
    }
}

int main() {
	// Create the root of tree
    Node* root = create_node(3);
    Tree* tree = create_tree(root);

    Node* test_node = create_node(0);

    printf("Binary Search Tree Implementation\n");
    printf("CS 5084 Bonus Project\n");
    printf("By Trevor Hodde\n");

    // Insert as a leaf
    BST_insert_element(tree, create_node(2));
    BST_insert_element(tree, create_node(1));
    BST_insert_element(tree, create_node(5));
    BST_insert_element(tree, create_node(1));
    BST_insert_element(tree, create_node(9));
    BST_insert_element(tree, create_node(2));
    BST_insert_element(tree, create_node(4));

    //inorder_walk(tree->root);

    // Insert at the root
    insertT(tree, create_node(10));
    insertT(tree, create_node(6));
    insertT(tree, create_node(7));

    inorder_walk(tree->root);

    // Use these tests to prove that all the functionality works
    printf("\n\n-o-o-o-o-o-o-o-o-  Unit Tests  -o-o-o-o-o-o-o-o-\n\n");
    printf("\n**Testing find functionality...\n");
    printf("Searching for 4 in the tree...\n");
    test_node = BST_find(4, tree->root);
    // reset the test_node if it comes back NULL
    if (test_node == NULL) test_node = tree->root;

    printf("Searching for 12 in the tree...\n");
    test_node->right = BST_find(12, tree->root);
    // reset the test_node if it comes back NULL
    if (test_node == NULL) test_node = tree->root;

    // Add a bunch of new nodes to the tree to demonstrate add/delete functionality
    // as well as demonstrating how duplicate elements are handled
    printf("\n**Attempting to add some new elements...\n");
    insertT(tree, create_node(2));
    insertT(tree, create_node(11));
    insertT(tree, create_node(10));
    insertT(tree, create_node(8));
    insertT(tree, create_node(10));
    insertT(tree, create_node(10));
    insertT(tree, create_node(12));

    // Demonstrate how to delete leaf nodes
    printf("\n**Testing delete functionality...\n");
    printf("Attempting to delete element 10...\n");
    BST_delete(tree, BST_find(10, tree->root));
    printf("Attempting to delete element 2...\n");
    BST_delete(tree, BST_find(2, tree->root));
    printf("Element 2 was not deleted.\n");

    // Demonstrate how to replace 2 nodes
    printf("\n**Testing replace functionality...\n");
    printf("Replacing elements 3 and 4...\n");
    BST_replace(tree, BST_find(3, tree->root), BST_find(5, tree->root));

    // Demonstrate how to delete a node with 2 child nodes
    printf("\n**Testing delete 2 functionality...\n");
	BST_delete_2(BST_find(8, tree->root), BST_find(7, tree->root));
	
    printf("\nEnd of unit tests.\n");
    printf("Current state of the tree: \n");
    inorder_walk(tree->root);

    printf("\n");
    return 0;
}
