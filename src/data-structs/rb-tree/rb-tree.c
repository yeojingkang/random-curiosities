#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define NODE_ID int32_t

#define INIT_CAPACITY 16
#define GROWTH_FACTOR 2
#define NULL_NODE -1

#define ERR_NULL_TREE 1
#define ERR_INVALID_ROT 2

enum rb_color
{
    BLACK,
    RED
};

struct rb_node
{
    int32_t key;
    union
    {
        NODE_ID next_free;
        enum rb_color color;
    };
    NODE_ID parent;
    NODE_ID left;
    NODE_ID right;
};

struct rb_tree
{
    struct rb_node *data;
    NODE_ID root;
    int32_t capacity;
    NODE_ID free_list;
};

/* Internal functions */
/****************************************************/
int32_t rb_resize(struct rb_tree *tree, int32_t capacity)
{
    if (tree == NULL || capacity <= tree->capacity)
        return ERR_NULL_TREE;

    struct rb_node *new_data = malloc(sizeof(struct rb_node) * capacity);

    // Construct a free list of the new nodes
    for (int32_t i = tree->capacity; i < capacity - 1; ++i)
        new_data[i].next_free = i + 1;
    new_data[capacity - 1].next_free = NULL_NODE;

    if (tree->data != NULL)
    {
        memcpy(new_data, tree->data, sizeof(struct rb_node) * tree->capacity);

        if (tree->free_list != NULL_NODE)
            new_data[capacity - 1].next_free = tree->free_list;

        free(tree->data);
    }

    tree->free_list = tree->capacity;
    tree->data = new_data;
    tree->capacity = capacity;

    return 0;
}

NODE_ID rb_alloc_node(struct rb_tree *tree)
{
    if (tree == NULL)
        return NULL_NODE;

    if (tree->free_list == NULL_NODE)
        rb_resize(tree, tree->capacity * GROWTH_FACTOR);

    const NODE_ID new_node_id = tree->free_list;
    tree->free_list = tree->data[new_node_id].next_free;

    return new_node_id;
}

void rb_print_inorder(const struct rb_tree *tree, NODE_ID id)
{
    if (id == NULL_NODE)
        return;

    const struct rb_node *node = tree->data + id;
    rb_print_inorder(tree, node->left);
    printf("%d%c ", node->key, node->color == RED ? 'r' : 'b');
    rb_print_inorder(tree, node->right);
}

int32_t rb_rot_l(struct rb_tree *tree, NODE_ID id)
{
    if (tree == NULL)
        return ERR_NULL_TREE;

    struct rb_node *node = tree->data + id;

    if (node->right == NULL_NODE)
        return ERR_INVALID_ROT;

    NODE_ID parent_id = node->parent;
    NODE_ID right_id = node->right;
    struct rb_node *right_child = tree->data + right_id;
    NODE_ID grandchild_id = right_child->left;

    right_child->parent = parent_id;
    node->parent = right_id;
    node->right = grandchild_id;
    right_child->left = id;

    // Relink parent
    if (parent_id != NULL_NODE)
    {
        struct rb_node *parent = tree->data + parent_id;
        if (parent->left == id)
            parent->left = right_id;
        else
            parent->right = right_id;
    }
    else
    {
        // Parent is root node
        tree->root = right_id;
    }

    // Relink grandchild
    if (grandchild_id != NULL_NODE)
        tree->data[grandchild_id].parent = id;

    return 0;
}

int32_t rb_rot_r(struct rb_tree *tree, NODE_ID id)
{
    if (tree == NULL)
        return ERR_NULL_TREE;

    struct rb_node *node = tree->data + id;

    if (node->left == NULL_NODE)
        return ERR_INVALID_ROT;

    NODE_ID parent_id = node->parent;
    NODE_ID left_id = node->left;
    struct rb_node *left_child = tree->data + left_id;
    NODE_ID grandchild_id = left_child->right;

    left_child->parent = parent_id;
    node->parent = left_id;
    node->left = grandchild_id;
    left_child->right = id;

    // Relink parent
    if (parent_id != NULL_NODE)
    {
        struct rb_node *parent = tree->data + parent_id;
        if (parent->left == id)
            parent->left = left_id;
        else
            parent->right = left_id;
    }
    else
    {
        // Parent is root node
        tree->root = left_id;
    }

    // Relink grandchild
    if (grandchild_id != NULL_NODE)
        tree->data[grandchild_id].parent = id;

    return 0;
}

void rb_ins_fix(struct rb_tree *tree, NODE_ID id)
{
    if (tree == NULL)
        return;

    while (id != NULL_NODE)
    {
        struct rb_node *node = tree->data + id;

        // Case 1: Node is root
        if (node->parent == NULL_NODE)
        {
            node->color = BLACK;
            return;
        }

        struct rb_node *parent_node = tree->data + node->parent;

        // Case 2: Parent is black
        if (parent_node->color == BLACK)
            return;

        NODE_ID grandparent = parent_node->parent;

        if (grandparent == NULL_NODE)
        {
            // Parent is root
            id = node->parent;
            continue;
        }

        struct rb_node *grandparent_node = tree->data + grandparent;
        NODE_ID uncle = grandparent_node->left == node->parent
            ? grandparent_node->right
            : grandparent_node->left;
        struct rb_node *uncle_node = uncle == NULL_NODE ? NULL : tree->data + uncle;

        // Case 3: Parent is red, Uncle is red
        if (uncle_node && uncle_node->color == RED)
        {
            parent_node->color = BLACK;
            uncle_node->color = BLACK;
            grandparent_node->color = RED;
            id = grandparent;
            continue;
        }

        // Case 4: Parent is red, Uncle is black
        if (parent_node->right == id && grandparent_node->left == node->parent)
            rb_rot_l(tree, node->parent);
        else if (parent_node->left == id && grandparent_node->right == node->parent)
            rb_rot_r(tree, node->parent);

        if (parent_node->left == id && grandparent_node->left == node->parent)
            rb_rot_r(tree, grandparent);
        else
            rb_rot_l(tree, grandparent);

        // Switch grandparent and parent color
        enum rb_color tmp = grandparent_node->color;
        grandparent_node->color = parent_node->color;
        parent_node->color = tmp;

        id = grandparent;
    }
}
/****************************************************/



int32_t rb_init(struct rb_tree *tree)
{
    if (tree == NULL)
        return ERR_NULL_TREE;

    tree->data = NULL;
    tree->root = NULL_NODE;
    tree->capacity = 0;
    tree->free_list = NULL_NODE;
    return rb_resize(tree, INIT_CAPACITY);
}

void rb_free(struct rb_tree *tree)
{
    free(tree->data);
    tree->capacity = 0;
}

int32_t rb_insert(struct rb_tree *tree, int32_t val)
{
    if (tree == NULL)
        return ERR_NULL_TREE;

    // Find insertion point
    uint8_t ins_dir = 0; // 0 - left, 1 - right
    NODE_ID curr = tree->root;
    NODE_ID parent = NULL_NODE;
    while (curr != NULL_NODE)
    {
        struct rb_node *node = tree->data + curr;
        parent = curr;
        if (val < node->key)
        {
            curr = node->left;
            ins_dir = 0;
        }
        else if (val > node->key)
        {
            curr = node->right;
            ins_dir = 1;
        }
        else
            return 0; // Value already exists
    }

    // Create and insert the new node
    NODE_ID new_node_id = rb_alloc_node(tree);
    struct rb_node *new_node = tree->data + new_node_id;

    new_node->parent = parent;
    new_node->key = val;
    new_node->color = RED;
    new_node->left = NULL_NODE;
    new_node->right = NULL_NODE;

    if (tree->root == NULL_NODE)
        tree->root = new_node_id;
    else if (ins_dir == 0)
        tree->data[parent].left = new_node_id;
    else
        tree->data[parent].right = new_node_id;

    rb_ins_fix(tree, new_node_id);

    return 0;
}

NODE_ID rb_find(const struct rb_tree *tree, int32_t val)
{
    NODE_ID curr = tree->root;
    while (curr != NULL_NODE)
    {
        const struct rb_node *node = tree->data + curr;
        if (node->key == val)
            return curr;
        if (val < node->key)
            curr = node->left;
        else
            curr = node->right;
    }
    return NULL_NODE;
}

void rb_print(const struct rb_tree *tree)
{
    if (tree == NULL)
        return;

    rb_print_inorder(tree, tree->root);
}

// Tree printing
/*********************************/
struct queue_node
{
    NODE_ID id;
    uint32_t level;
    uint32_t pos;
};

uint32_t rb_tree_height(const struct rb_tree *tree, NODE_ID id)
{
    if (id == NULL_NODE)
        return 0;

    struct rb_node *node = tree->data + id;

    uint32_t lh = rb_tree_height(tree, node->left);
    uint32_t rh = rb_tree_height(tree, node->right);
    return (lh > rh ? lh : rh) + 1;
}

void print_indent(uint32_t indent)
{
    if (indent > 0)
        printf("%*c", indent << 1, ' ');
}

// If key has >1 digits, the tree will look off
void rb_pretty_print(const struct rb_tree *tree)
{
    if (tree == NULL || tree->root == NULL_NODE)
        return;

    uint32_t height = rb_tree_height(tree, tree->root);
    printf("tree height: %d\n", height);

    // Max possible nodes is 2 * capacity (accounting for nil nodes)
    struct queue_node *queue = (struct queue_node*)malloc((tree->capacity << 1) * sizeof(struct queue_node));
    uint32_t size = 0;
    uint32_t level = 0;
    uint32_t curr = 0;
    uint32_t curr_pos = 0;

    uint32_t indent = (1 << height) - 1;
    queue[size++] = (struct queue_node){ tree->root, level, indent };

    while (curr < size)
    {
        if (level < queue[curr].level)
        {
            // Reached next level
            printf("\n");
            level = queue[curr].level;
            indent = (1 << (height - level)) - 1;
            curr_pos = 0;
        }

        // Print indentation to node's pos
        print_indent(queue[curr].pos - curr_pos);
        curr_pos = queue[curr].pos + 1;

        if (queue[curr].id == NULL_NODE)
        {
            printf("xx");
        }
        else
        {
            struct rb_node *node = tree->data + queue[curr].id;
            printf("%d%c", node->key, node->color == RED ? 'r' : 'b');

            const uint32_t pos_diff = (indent >> 1) + 1;

            queue[size++] = (struct queue_node){ node->left, queue[curr].level + 1, queue[curr].pos - pos_diff };
            queue[size++] = (struct queue_node){ node->right, queue[curr].level + 1, queue[curr].pos + pos_diff };
        }

        ++curr;
    }

    free(queue);
}
/*********************************/

#define rb_contains(tree, val) rb_index_of(tree, val) != NULL_NODE

int main()
{
    struct rb_tree rb;
    rb_init(&rb);

    for (int i = 1; i < 20; ++i)
    {
        rb_insert(&rb, i);
        rb_print(&rb);
        printf("\n");
    }

    rb_pretty_print(&rb);
    printf("\n\n");

    rb_free(&rb);
    return 0;
}
