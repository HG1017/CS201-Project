// a C program for the implementation of the construction of suffix tree from a
// given string input and also printing the linear time suffix array for the same.

/* Team details:-
    Group number 35
    Members:-
    1) Shivam Choubey (2019med1006)
    2) Harsh Gupta (2019med1008)
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define endl printf("\n")
#define MAX 2048

struct ukkonen_suffix_tree_node // structure for the node of a suffix tree
{
    int begin; // indicates the starting of an edge or link which connects a parent node to its child node
    int* close; // indicates the end of an edge or link which connects a parent node to its child node
    int status; // stores the index of the suffix for the path from root to leaf
    struct ukkonen_suffix_tree_node *child[MAX]; // connects a parent node to its child node
    struct ukkonen_suffix_tree_node* next; // indicates a link between two nodes

};

struct ukkonen_suffix_tree_node* root = NULL;
struct ukkonen_suffix_tree_node* prev_node = NULL;
struct ukkonen_suffix_tree_node* current_node = NULL;

char input[200];

int cnt = 0; // number of nodes in the tree
int curr_edge = -1;
int curr_len = 0;
int remain = 0; // number of suffixes remaining to be added to the tree
int input_size = -1;

int* divide = NULL;
int* root_last = NULL;
int leaf_last = -1;

struct ukkonen_suffix_tree_node* generate_node(int begin , int* close) // function to dynamically allocate memory for a new node and initialize its children pointers to NULL
{
    cnt = cnt + 1;
    struct ukkonen_suffix_tree_node* t;

    t = (struct ukkonen_suffix_tree_node*)malloc(sizeof(struct ukkonen_suffix_tree_node));

    for (int i = 0; i < MAX; i++)
    {
        t->child[i] = NULL;
    }

    t->next = root;

    t->begin = begin;

    t->close = close;

    t->status = -1;

    return t;
}

int length_of_edge(struct ukkonen_suffix_tree_node* t) // this function finds the length of edge in terms of characters from one node to the other
{
    if(t == NULL)
    {
        return -1;
    }

    int out;

    if (t->close == NULL)
    {
    	return 1;
    }
    out = *(t->close) - (t->begin);

    out = out + 1;

    return out;
}

int traverse_below(struct ukkonen_suffix_tree_node* t) // this functions helps in assigning relevant status to the node
{
    if (t == NULL)
    {
        return -1;
    }

    if(curr_len < length_of_edge(t))
    {
        return 0;
    }

    if ((length_of_edge(t) + curr_edge)>=input_size)
    {
        return 0;
    }

    curr_edge = (int)input[length_of_edge(t) + curr_edge] - (int)' ';

    curr_len = curr_len - length_of_edge(t);

    current_node = t;

    return 1;
}

void add_to_tree (int idx) // function for adding all the created suffixes and the nodes, including the leaves to the existing suffix tree
{
    leaf_last = idx;

    remain = remain + 1;

    prev_node = NULL;

    while(remain > 0)
    {
        if(curr_len == 0)
        {
            curr_edge = (int)input[idx] - (int)' ';
        }

        if(current_node->child[curr_edge] == NULL)
        {
            current_node->child[curr_edge] = generate_node(idx, &leaf_last);

            if(prev_node != NULL)
            {
                prev_node->next = current_node;

                prev_node = NULL;
            }
        }

        else
        {
            struct ukkonen_suffix_tree_node* dup;
            dup = current_node->child[curr_edge];

            if(traverse_below(dup) == 1)
            {
                continue;
            }

            if(input[curr_len + dup->begin] == input[idx])
            {
                if(prev_node != NULL && current_node != root)
                {
                    prev_node->next = current_node;

                    prev_node = NULL;
                }

                curr_len = curr_len + 1;
                break;
            }

            divide = (int*)malloc(sizeof(int));
            *divide = curr_len + dup->begin - 1;

            struct ukkonen_suffix_tree_node* n;
            n = generate_node(dup->begin, divide);
            current_node->child[curr_edge] = n;

            n->child[(int)input[idx] - (int)' '] = generate_node(idx, &leaf_last);
            dup->begin = dup->begin + curr_len;
            n->child[curr_edge] = dup;

            if(prev_node != NULL)
            {
                prev_node->next = n;
            }
            prev_node = n;
        }

        remain = remain - 1;

        if(current_node == root && curr_len > 0)
        {
            curr_len = curr_len - 1;

            curr_edge = (int)input[1 + idx - remain] - (int)' ';
        }

        else if(current_node != root)
        {
            current_node = current_node->next;
        }
    }
}

void print_it(int a, int b) // function for printing a particular suffix
{
    for (int i = a; i <= b; i++)
    {
       printf("%c", input[i]);
    }
}

int suffix_array[1000];
int arr_size = 0;

void DFS_on_suffix_tree(struct ukkonen_suffix_tree_node* dup, int h) // function for printing the suffixes of the tree in DFS manner
{
    int j = 0;

    if(dup == NULL)
    {
        return;
    }

    if(dup->begin != -1)
    {
        print_it(dup->begin, *(dup->close));
    }

    int leaf = 1;

    for (int i = 0; i < MAX; i++)
    {
        if(dup->child[i] != NULL)
        {
            if(leaf == 1 && dup->begin != -1)
            {
                printf(" [%d]", dup->status);
                endl;
                suffix_array[j++] = dup->status;
                arr_size++;
            }

            leaf = 0;
            DFS_on_suffix_tree(dup->child[i], h + length_of_edge(dup->child[i]));
        }
    }

    if(leaf == 1)
    {
        dup->status = input_size - h;
        printf(" [%d]", dup->status);
        endl;
        suffix_array[j++] = dup->status;
        arr_size++;
    }
}

void free_suffix_tree(struct ukkonen_suffix_tree_node* dup) // this function frees the space after we are done printing the details of the tree
{
    if(dup == NULL)
    {
        return;
    }

    for (int i = 0; i < MAX; i++)
    {
        if(dup->child[i] != NULL)
        {
            free_suffix_tree(dup->child[i]);
        }
    }
