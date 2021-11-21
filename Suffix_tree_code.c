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
    if(dup->status == -1)
    {
        free(dup->close);
    }

    free(dup);
}

void generate_suffix_tree(void) // function for building the appropriate suffix tree for a given string input
{
    input_size = strlen(input);

    root_last = (int*)malloc(sizeof(int));
    *root_last = -1;

    root = generate_node(-1, root_last);

    current_node = root;

    for (int i = 0; i < input_size; i++)
    {
        add_to_tree(i);
    }

    int h = 0;

    DFS_on_suffix_tree(root, h);

}

void travel(struct ukkonen_suffix_tree_node *t, int suffix_array[], int *ind) // function for traveling through the suffixes or the edges of the tree
{
    if(t == NULL)
    {
        return;
    }

    if(t->status == -1)
    {
        for (int i = 0; i < MAX; i++)
        {
            if(t->child[i] != NULL)
            {
                travel(t->child[i], suffix_array, ind);
            }
        }
    }

    else if(t->status > -1 && t->status < input_size)
    {
        suffix_array[(*ind)++] = t->status;
    }
}

void generate_suffix_array(int suffix_array[]) // function builds the linear time suffix array for the suffix tree
{
    for(int i = 0; i < input_size; i++)
    {
        suffix_array[i] = -1;
    }
    int ind = 0;
    travel(root, suffix_array, &ind);

    printf("Suffix Array for String ");

    for(int i = 0; i < input_size; i++)
    {
        printf("%c", input[i]);
    }

    printf(" is: ");

    for(int i = 0; i< input_size; i++)
    {
        if(suffix_array[i] != -1)
        {
            printf("%d ", suffix_array[i]);
        }
    }
    endl;
}

int go_through(char *c, int ind, int b, int e)
{
    if(c == NULL)
    {
        return 0;
    }

    int i;

    for (i = b; i <= e && c[ind]!= '\0'; i++, ind++)
    {
        if (input[i]!= c[ind])
        {
            return -1;
        }
    }

    if (c[ind]== '\0')
    {
        return 1;
    }

    return 0;
}

int leaf_counting (struct ukkonen_suffix_tree_node* t)
{
    if (t == NULL)
    {
        return 0;
    }

    if (t->status > -1)
    {
        endl;
        printf("Found at position: %d", t->status);

        return 1;
    }

    int out = 0,i;
    for (i = 0; i < MAX; i++)
    {
        if(t->child[i]!= NULL)
        {
            out = out + leaf_counting(t->child[i]);
        }
    }

    return out;
}

int no_of_leaves(struct ukkonen_suffix_tree_node* t)
{
    if (t == NULL)
    {
        return 0;
    }

    return leaf_counting(t);
}

int travel_for_search(struct ukkonen_suffix_tree_node* t, char* c, int ind)
{
    if (t== NULL || c == NULL)
    {
        return -1;
    }

    int out = -1;

    if(t->close == NULL)
        {
            return -1;
        }

    if (t->begin != -1)
    {
        if(t->close == NULL)
        {
            return -1;
        }
        out = go_through(c, ind, t->begin, *(t->close));

        if (out== -1)
        {
            return -1;
        }
        if (out == 1)
        {
            if (t->status > -1)
            {
                endl;
                printf("Substring count: 1 and position: %d", t->status);
            }
            else
            {
                endl;
                printf("Substring count: %d", no_of_leaves(t));
            }

            return 1;
        }
    }

    ind = ind + length_of_edge(t);

    if (t->child[c[ind]]!= NULL)
    {
        return travel_for_search(t->child[c[ind]], c, ind);
    }

    else
    {
        return -1;
    }
}

void verify_substring (char* c)
{
    if (c == NULL)
    {
        return;
    }

    int out = travel_for_search(root, c, 0);

    if (out == 1)
    {
        endl;
        printf("Pattern <%s> is a substring", c);
        endl;
    }

    else
    {
        endl;
        printf("Pattern <%s> is not a substring", c);
        endl;
    }
}

int main()
{
   int size;

	printf("The following program takes a string as an input and prints the suffixes present on the edges of the suffix tree and its corresponding linear time suffix array.");
	endl;

    printf("Please enter the string (length should be less than 200). Press Enter when finished: ");

    char str[200];

	scanf("%[^\n]s",str);
	endl;

	strcpy(input,str);

	input[strlen(str)]='$';

	printf("The suffixes present in the suffix tree of the input string are:");
	endl;

    generate_suffix_tree();
    endl;

    printf("The number of nodes in the suffix tree of string %s is %d", str, cnt);
    endl;

    input_size--;

    int suffix_array =(int) malloc(sizeof(int) * input_size);

    generate_suffix_array(suffix_array);

    free_suffix_tree(root);
    free(suffix_array);

    return 0;
}
