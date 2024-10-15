#include <bits/stdc++.h>
#include <iostream>
using namespace std;

template <typename T, int ORDER>
class BTreeNode
{
public:
    T keys[ORDER - 1];
    BTreeNode<T, ORDER> *children[ORDER];

    int n; // n is the current number of keys
    bool leaf;

    BTreeNode(bool isLeaf = true) : n(0), leaf(isLeaf)
    {
        for (int i = 0; i < ORDER; i++)
        {
            children[i] = nullptr;
        }
    }
};

template <typename T, int ORDER>
class BTree
{
private:
    BTree<T, ORDER> *root;

    // function to split a full child node
    void splitChild(BTree<T, ORDER> *x, int i)
    {
        BTree<T, ORDER> *y = x->children[i];
        BTree<T, ORDER> *z = new BTreeNode<T, ORDER>(y->leaf);
        z->n = ORDER / 2 - 1;

        for (int j = 0; j < ORDER / 2; j++)
        {
            z->keys[j] = y->keys[j + ORDER / 2];
        }

        if (!y->leaf)
        {
            for (int j = 0; j < ORDER / 2; j++)
            {
                z->children[j] = y->children[j + ORDER / 2];
            }
        }
        y->n = ORDER / 2 - 1;

        for (int j = x->n; j >= i + 1; j--)
        {
            x->children[j + 1] = x->children[j];
        }

        x->children[i + 1] = z;

        for (int j = x->n - 1; j >= i; j--)
        {
            x->keys[j + 1] = x->keys[j];
        }
        x->keys[i] = y->keys[ORDER / 2 - 1];
        x->n = x->n + 1;
    }
    // Inset a key in a non-full node
    void insertNonFull(BTree<T, ORDER> *x, T k)
    {
        int i = x->n - 1;

        if (x->leaf)
        {
            while (i >= 0 && k < x->keys[i])
            {
                x->keys[i + 1] = x->keys[i];
                i--;
            }
            x->keys[i + 1] = k;
            x->n = x->n + 1;
        }
        else
        {
            while (i >= 0 && k < x->keys[i])
            {
                i--;
            }
            i++;
            if (x->children[i]->n == ORDER - 1)
            {
                splitChild(x, i);
                if (k > x->keys[i])
                    i++;
            }
            insertNonFull(x->children[i, k]);
        }
    }

    // to traverse the tree

    void traverse(BTree<T, ORDER> *x)
    {
        int i;
        for (i = 0; i < x->n; i++)
        {
            if (!x->leaf)
            {
                traverse(x->children[i]);
                cout << "" << x->keys[i];
            }
        }
        if (!x->leaf)
        {
            traverse(x->children[i]);
        }
    }

    // search a key
    BTree<T, ORDER> *search(BTree<T, ORDER> *x, T k)
    {
        int i = 0;
        while (i < x->n && k > x->keys[i])
            i++;

        if (i < x->n && k == x->keys[i])
            return x;

        if (x->leaf)
            return nullptr;

        return search(x->children[i], k);
    }

    // Find the predecessor

    T getPredecessor(BTree<T, ORDER> *node, int index)
    {
        BTree<T, ORDER> *current = node->children[index];
        while (!current->leaf)
            current = current->children[current->n];
        return current->keys[current->n - 1];
    }

    // Find successor

    T getSuccessor(BTree<T, ORDER> *node, int index)
    {
        BTree<T, ORDER> *current = node->children[index + 1];
        while (!current->leaf)
            current = current->children[0];
        return current->keys[0];
    }

    // fill child node

    void fill(BTree<T, ORDER> *node, int index)
    {
        if (index != 0 && node->children[index - 1]->n >= ORDER / 2)
            borrowFromPrev(node, index);
        else if (index != node->n && node->children[index + 1]->n >= ORDER / 2)
            borrowFromNexr(node, index);
        else
        {
            if (index != node->n)
                merge(node, index);
            else
                merge(node, index - 1);
        }
    }

    // borrow from previous

    void borrowFromPrev(BTree<T, ORDER> *node, int index)
    {
        BTree<T, ORDER> *child = node->children[index];
        BTree<T, ORDER> *sibling = node->children[index - 1];

        for (int i = child->n - 1; i >= 0; --i)
        {
            child->keys[i + 1] = child->keys[i];
        }
        if (!child->leaf)
        {
            for (int i = child->n; i >= 0; --i)
                child->children[i + 1] = child->children[i];
        }
        child->keys[0] = node->keys[index - 1];

        if (!child->leaf)
        {
            child->children[0] = sibling->children[sibling->n - 1];
        }
        node->keys[index - 1] = sibling->keys[sibling->n - 1];
        child->n += 1;
        sibling->n -= 1;
    }

    // to borrow from next

    void borrowFromNext(BTree<T, ORDER> *node, int index)
    {
        BTree<T, ORDER> *child = node->children[index];
        BTree<T, ORDER> *sibling = node->children[index + 1];

        child->keys[child->n] = node->keys[index];

        if (!node->leaf)
            child->children[child->n + 1] = sibling[0];
        node->keys[index] = sibling->keys[0];

        for (int i = 0; i < sibling->n; ++i)
        {
            sibling->keys[i - 1] = sibling->keys[i];
        }
        if (!sibling->leaf)
        {
            for (int i = 1; i <= sibling->n; ++i)
            {
                sibling->children[i - 1] = sibling->children[i];
            }
        }
        child->n += 1;
        sibling->n -= 1;
    }

    // merge two nodes

    void merge(BTree<T, ORDER> *node, int index)
    {
        BTree<T, ORDER> *child = node->children[index];
        BTree<T, ORDER> *sibling = node->children[index + 1];

        child->keys[ORDER / 2 - 1] = node->keys[index];

        for (int i = 0; i <= sibling->n; i++)
        {
            child->children[i + ORDER / 2] = sibling->keys[i];
        }
        if (!child->leaf)
        {
            for (int i = 0; i <= sibling->n; ++i)
            {
                child->children[i + ORDER / 2] = sibling->children[i];
            }
        }

        for (int i = index + 1; i < node->n; ++i)
            node->keys[i - 1] = node->keys[i];

        for (int i = index + 2; i < node->n; ++i)
            node->children[i - 1] = node->children[i];

        child->n += sibling->n + 1;
        node->n--;

        delete sibling;
    }

    // remove key from non-leaf

    void removeFromNonLeaf(BTree<T, ORDER> *node, int index)
    {
        T k = node->keys[index];

        if (node->children[index]->n >= ORDER / 2)
        {
            T pred = getPredecessor(node, index);
            node->keys[index] = pred;
            remove(node->children[index], pred);
        }
        else if (node->children[index + 1]->n >= ORDER / 2)
        {
            T succ = getSuccessor(node, index);
            node->keys[index] = succ;
            remove(node->children[index + 1], succ);
        }
        else
        {
            merge(node, index);
            remove(node->children[index], k);
        }
    }
    // remove from leaf node

    void removeFromLeaf(BTree<T, ORDER> *node, int index)
    {
        for (int i = index + 1; i < node->n; ++i)
            node->keys[i - 1] = node->keys[i];
        node->n--;
    }

    // remove a key from tree

    void remove(BTree<T, ORDER> *node, T k)
    {
        int index = 0;
        while (index < node->n && node->keys[index] < k)
            ++index;

        if (index < node->n && node->keys[index] == k)
        {
            if (node->leaf)
                removeFromLeaf(node, index);
            else
            {
                removeFromNonLeaf(node, index);
            }
        }
        else
        {
            if (node->leaf)
            {
                cout << "The Key" << k << "is not present in the tree\n";
                return;
            }

            bool flag = ((index == node->n) ? true : false);
            if (node->children[index]->n < ORDER / 2)
                fill(node, index);
            if (flag && index > node->n)
                remove(node->children[index - 1], k);
            else
                remove(node->children[index], k);
        }
    }

public:
    BTree() { root = new BTreeNode<T, ORDER>(true); }

    // insertion
    void insert(T k)
    {
        if (root->n == ORDER - 1)
        {
            BTreeNode<T, ORDER> *s = new BTreeNode<T, ORDER>(false);
            s->children[0] = root;
            root = s;
            splitChild(s, 0);
            insertNonFull(s, k);
        }
        else
        {
            insertNonFull(root, k);
        }
    }
    // traversal

    void traverse()
    {
        if (root != nullptr)
            traverse(root);
    }

    // search
    BTreeNode<T, ORDER> *search(T k)
    {
        return (root == nullptr) ? nullptr : search(root, k);
    }
    // remove
    void remove(T k)
    {
        if (!root)
        {
            cout << "The tree is empty!\n";
            return;
        }
        remove(root, k);
        if (root->n == 0)
        {
            BTreeNode<T, ORDER> *temp = root;
            if (root->leaf)
                root = nullptr;
            else
                root = root->children[0];
            delete temp;
        }
    }
};

void printMenu()
{
    cout << "\nB-tree operations:\n";
    cout << "1. Insert a key\n";
    cout << "2. Search for a key\n";
    cout << "3. Display the B-tree (in-order traversal)\n";
    cout << "4. Remove a key.\n";
    cout << "5. Exit.\n";
    cout << "Enter your choice: ";
}

int main()
{
    int choice;
    BTree<int, 3> rooot;

    while (true)
    {
        printMenu();

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int key;
            cout << "Enter the key to insert:";
            cin >> key;
            rooot.insert(key);
            break;
        }

        case 2:
        {
            int key;
            cout << "Enter the key to search:";
            cin >> key;
            BTreeNode<int, 3> *result = rooot.search(key);
            if (result != nullptr)
                cout << "Key" << key << "found at node." << result<< endl;
            else
                cout << "Key" << key << "not found.\n";
            break;
        }

        case 3:
        {
            cout << "B-Tree in-order traversal:";
            rooot.traverse();
            cout << endl;
            break;
        }

        case 4:
        {
            int key;
            cout << "Enter the key to remove:";
            cin >> key;
            rooot.remove(key);
            break;
        }

        case 5:
        {
            cout << "Exiting.." << endl;
        }
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
}
