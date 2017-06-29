struct node
{
  void *data;
  
  struct node *l,*r;   /* BST pointers */
  struct node *next;   /* Linked list for traverse etc */
};

enum llorder {O_NONE,O_IN,O_PRE,O_POST,O_LEVEL};

struct dict
{
  struct node *head;
  enum llorder order;
  int (*compare)(void *a, void *b);
};

struct dict *new_dict(int (*compare)(void *a, void *b));
void free_dict(struct dict *dict);

int add_unique_node(struct dict *d,void *data);
void add_node(struct dict *d,void *data);
void *change_node(struct dict *d,void *data);
void *find_node(struct dict *d,void *k);
void *delete_node(struct dict *d,void *data);
void relink_inorder(struct dict *d);
void visit_nodes(struct dict *d,void (*visit)(void *data));
void balance_tree(struct dict *d);
