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
  TreeNode *auxPadre = node->parent;

  if(node->left == NULL && node->right == NULL){
    if (auxPadre->left == NULL){
      auxPadre->right = NULL;
    }else{
      auxPadre->left = NULL;
    }return;
  }else if(node->left!=NULL&&node->right!=NULL){
    TreeNode *auxMinimum = minimum(node->right);
    node->pair->key = auxMinimum->pair->key;
    node->pair->value = auxMinimum->pair->value;
    node = NULL;

    removeNode(tree, auxMinimum);
    return;
  }else{
    if (node->left == NULL){
      if(auxPadre->left == NULL){
        auxPadre->left = node->left;
        auxPadre->left->parent = auxPadre;
      }else{
        auxPadre->right = node->right;
        auxPadre->right->parent = auxPadre;
      }
      node = NULL;
      return;
    }else{
      if(auxPadre->left == NULL){
        auxPadre->left = node->left;
        auxPadre->left = auxPadre;
      }else{
        auxPadre->right = node->right;
        auxPadre->right = auxPadre;
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
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
