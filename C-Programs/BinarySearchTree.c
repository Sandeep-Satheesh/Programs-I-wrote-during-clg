#include<stdio.h>
#include<stdlib.h>

/*Program to implement insertion and deletion of new nodes in a binary search tree (BST)
Assumption(s):
1. All elements in the BST are unique.
2. The data elements in each node of the BST is an integer.
*/

/*Definition of node element in the BST*/
struct node {
	int data;
	struct node *left, *right;
};

//Aliasing the structure type so that I can save some typing effort :P
typedef struct node node;


//Creates an empty new node and returns it (if there was enough memory. Otherwise returns NULL.
node *newNode(int data) {
	node *t = (node *)malloc(sizeof(node));
	if (t) {
		t->data = data;
		t->left = t->right = NULL;
	}
	else {
		printf("\n\nCOULD NOT ALLOCATE MEMORY!!!!");
        exit(0); //Terminate program...
	}
	return t;
}

//Inserts a newly created node into the binary search tree.
node *insertNode(node *root, int data) {
	if (!root) {
		root = newNode(data); //We don't have a subtree rooted by this node.
		printf("\n%d inserted into tree.", data);
	}
	else if (root->data < data) root->left = insertNode(root->left, data); //All elements lesser than root go to its left.
	else if (root->data > data) root->right = insertNode(root->right, data); //All elements greater than root go to its right.

	return root; //Recursive call ensures that the above if-conditions work.
}

//Finds the minimum element by traversing from top to bottom, and keeps going to the left subtree.
node *findMin (node *root) {
	while (root->left != NULL)
		root = root->left;
	return root;
}

//Deletion routine. This one might be tricky.

node *deleteNode(node *root, int data) {
	if (root->data < data) root = deleteNode (root->left, data); //Keep going to the left subtree and return the address of the next node after deletion.
	else if (root-> data > data) root = deleteNode (root->right, data); //Similar story for the right.
	else {
		if (!root->left)
			if (!root->right) {
				//it's got no children, can delete directly.
				free(root);
				root = NULL;
			}
			else {
				//it's got a right child, so disconnect the node to be deleted from the tree, and then delete it.
				node *temp = root; //Holding the address of the node to be deleted later.
				root = root->right; //Connecting the node of the right subtree to the root.
				free(temp); //Deleting the node.
			}
		else if (!root->right) {
			//it's got a left child. Similar story as above.
			node *temp = root;
			root = root->left;
			free(temp);
		}
		else {
			//it's got both children. So we find the smallest element greater than root, and then replace that with the node to be deleted.
			node *min = findMin(root->right);
			root->data = min->data;
			deleteNode(min, root->data);
		}
	}
	return root;
}
//Inorder traversal routine. Traverse the left subtree, then the root, then the right.
void inOrder(node *root) {

	if (!root) return;
	inOrder(root->left);
	printf("%d ", root->data);
	inOrder(root->right);
}

//Preorder traversal routine. Traverse as: root, then the left subtree, then the right.
void preOrder(node *root) {
	if (!root) return;
	printf("%d ", root->data);
	preOrder(root->left);
	preOrder(root->right);
}

//Postorder traversal routine. Traverse as: root, then the left subtree, then the right.
void postOrder(node *root) {
	if (!root) return;
	postOrder(root->left);
	postOrder(root->right);
	printf("%d ", root->data);
}

void releaseMem (node *root) {
    //Subroutine to release used memory for the tree. Uses a post-order traversal algorithm to free the subtrees first, and then the root.
    if (!root) return;
    if (root->left) releaseMem(root->left);
    if (root->right) releaseMem(root->right);

    free(root);
}
//Main function to interact with user.
int main() {
	int ch, e;
	node *root = NULL; //Root element is initially NULL.

	//Display a menu.
	printf("\n\nMenu\n1 - Insertion"
        "\n2 - Deletion"
        "\n3 - Inorder Traversal"
        "\n4 - Preorder Traversal"
        "\n5 - Postorder Traversal"
        "\n6 - Exit");
	do {
		printf("\n\nEnter choice (1-6): ");
		scanf("%d", &ch);

		switch (ch) {
            case 1:
               	printf("\nEnter element to insert: ");
                scanf("%d", &e);
                root = insertNode(root,e);
                break;
            case 2:
                if (root == NULL) printf("\nThe tree is empty!");
                else {
                    printf("\nEnter element to delete: ");
                    scanf("%d", &e);
                    root = deleteNode(root,e);
                }
                break;
            case 3:
                if (root == NULL) printf("\nThe tree is empty!");
                else {
                    printf("\nInorder traversal: ");
                    inOrder(root);
                }
                break;
            case 4:
                if (root == NULL) printf("\nThe tree is empty!");
                else {
                    printf("\nPreorder traversal: ");
                    preOrder(root);
                    break;
                }
                break;
            case 5:
                if (root == NULL) printf("\nThe tree is empty!");
                else {
                    printf("\nPostorder traversal: ");
                    postOrder(root);
                }
                break;
            case 6:
                printf("\nExiting...\n\n");
                	//Cleanup time.
                if (root) releaseMem(root);
                return 0;
            default:
                printf("\n\nInvalid option!");
		}

	} while (ch != 6);
	return 0;
}
