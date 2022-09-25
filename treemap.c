#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap * map = (TreeMap *)malloc(sizeof(TreeMap));
  map->root=map->current=NULL;
  map->lower_than=lower_than;
  return map;

}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  if (tree==NULL) return;
  TreeNode *aux = tree->root;
  TreeNode *padre = NULL;
  TreeNode *nodo = createTreeNode(key, value);
  while (aux != NULL){
    padre = aux;
    if (tree->lower_than(key, aux->pair->key)){
      aux = aux->left;
    }else if(tree->lower_than(aux->pair->key, key)){
      aux=aux->right;
    }else{
      return;
    }
  }
  if (padre == NULL){
    tree->root = nodo;
    tree->current = nodo;
  }else if(tree->lower_than(nodo->pair->key, padre->pair->key)){
    nodo->parent = padre;
    padre->left = nodo;
    tree->current = nodo;
  }else{
    nodo->parent = padre;
    padre->left = nodo;
    tree->current = nodo;
  }
}

TreeNode * minimum(TreeNode * x){
  while(x->left!=NULL){
    x=x->left;
  }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  TreeNode *auxParent = node->parent;

	if (node->left == NULL && node->right == NULL) {
		if (auxParent->left == NULL)
			auxParent->right = NULL;
		else
			auxParent->left = NULL;
		return;
		
	} else if (node->left != NULL && node->right != NULL) {
		TreeNode *auxMini = minimum(node->right);

		node->pair = auxMini->pair;
		node = NULL;

		removeNode(tree, auxMini);
		return;
	} else {
		if (node->left == NULL) {
			if (auxParent->left == NULL) {
				auxParent->left = node->left;
				auxParent->left->parent = auxParent;
			} else {
				auxParent->right = node->right;
				auxParent->right->parent = auxParent;
			}
			node = NULL;
			return;
		} else {
			if (auxParent->left == NULL) {
				auxParent->left = node->left;
				auxParent->left = auxParent;
			} else {
				auxParent->right = node->left;
				auxParent->right->parent = auxParent;
			}
			node = NULL;
			return;
		}
	}
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  if(tree==NULL||tree->root==NULL) return NULL;
  TreeNode *aux = tree->root;

  while(aux!=NULL){
    if (tree->lower_than(key, aux->pair->key)){
      aux= aux->left;
    }else if (tree->lower_than(aux->pair->key, key)){
      aux = aux->right;
    }else break;
  }
  tree->current = aux;
  if (aux == NULL) return NULL;
  return aux->pair;
  
}


Pair * upperBound(TreeMap * tree, void* key) {
  tree->current = tree->root;
  TreeNode * upBoundnode = NULL;
  
  while(tree->current!=NULL){
    if(is_equal(tree, tree->current->pair->key, key)==1){
      return tree->current->pair;
    }else if(tree->lower_than(tree->current->pair->key, key) == 1){
      tree->current = tree->current->right;
    }else{
      upBoundnode = tree->current;
      tree->current = tree->current->left;
    }
    
    if(tree->current == NULL && upBoundnode == NULL){
      return NULL;
    }
  }
  return upBoundnode->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode * firstNode = minimum(tree->root);
  return firstNode->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if(tree->current->right!=NULL){
    tree->current = minimum(tree->current->right);
    return tree->current->pair;
  }
  TreeNode *node = tree->current->parent;
  while(node!=NULL && tree->current==node->right){
    tree->current=node;
    node=node->parent;
  }
  if(node!=NULL){
    tree->current=node;
    return node->pair;
  }
  return NULL;
}
