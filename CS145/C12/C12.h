struct anode {

  int key;

  char *value;

  struct anode *next;

};

 

struct hash {

    int size;

    struct anode **table;

};