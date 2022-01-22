//
// AED, January 2022
//
// Solution of the second practical assignement (multi-ordered tree)
//
// Place your student numbers and names here
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AED_2021_A02.h"

// MAX_NAME_SIZE
// the custom tree node structure
//
// we want to maintain three ordered trees (using the same nodes!), so we need three left and three right pointers
// so, when inserting a new node we need to do it three times (one for each index), so we will end upo with 3 three roots
//

int ctr = 0;

typedef struct tree_node_s
{
  char name[ MAX_NAME_SIZE + 1];                               // index 0 data item
  char zip_code[MAX_ZIP_CODE_SIZE + 1];                        // index 1 data item
  char telephone_number[MAX_TELEPHONE_NUMBER_SIZE + 1];        // index 2 data item
  char social_security_number[MAX_SOCIAL_SECURITY_NUMBER + 1]; // index 3 data item
  struct tree_node_s *left[4];                                 // left pointers (one for each index) ---- left means smaller
  struct tree_node_s *right[4];                                // right pointers (one for each index) --- right means larger
}
tree_node_t;
//
// the node comparison function (do not change this)
//
int compare_tree_nodes(tree_node_t *node1,tree_node_t *node2,int main_idx)
{
  int i,c;
  for(i = 0;i < 3;i++)
  {
    if(main_idx == 0)
      c = strcmp(node1->name,node2->name);
    else if(main_idx == 1)
      c = strcmp(node1->zip_code,node2->zip_code);
    else if(main_idx == 2)
      c = strcmp(node1->telephone_number,node2->telephone_number);
    else
      c = strcmp(node1->social_security_number,node2->social_security_number);
    if(c != 0)
      return c; // different on this index, so return
    main_idx = (main_idx == 3) ? 0 : main_idx + 1; // advance to the next index
  }
  return 0;
}
//
// tree insertion routine (place your code here)
//
void tree_insert(tree_node_t **link, tree_node_t *person , int main_index){
  if(*link == NULL){ 
      (*link) = person;
  }
  else if(compare_tree_nodes(*link, person, main_index) > 0){  
      tree_insert(&((*link)->left[main_index]), person, main_index); 
    }
  else{
    tree_insert(&((*link)->right[main_index]), person, main_index);
  }
}
//
// tree search routine (place your code here)
//
tree_node_t *find(tree_node_t *link, tree_node_t *person, int main_index){
  if(link == NULL || compare_tree_nodes(link, person, main_index) == 0) {
    return link;
  }
  else if(compare_tree_nodes(link, person, main_index) > 0){
    return find(link->left[main_index],person, main_index);
  }
  else {
    return find(link->right[main_index],person, main_index);
  }
}
//
// tree depdth
//
int tree_depth(tree_node_t *link, int main_index) {
  if (link == NULL){return 0;}
  int ld = tree_depth(link->left[main_index], main_index);
  int rd = tree_depth(link->right[main_index], main_index);
  if (ld > rd) {return ld + 1;}
  else {return rd + 1;}
}
//
// list, i,e, traverse the tree (place your code here)
//
int list(tree_node_t *link, int main_index, char *compare){
  if(link != NULL){

    list(link->left[main_index], main_index, compare);
    
    char *search;
    if (main_index == 0){
      search = link->name;    
    } else if (main_index == 1){
      search = link->zip_code;
    } else if (main_index == 2){
      search = link->telephone_number;
    } else {
      search = link->social_security_number;
    }
    char sus[strlen(search)];
    strcpy(sus,search);
    if (main_index != 0) {
      int i = 0, j = 0;
      while (sus[i]) {
        if (sus[i] != ' ')
          sus[j++] = sus[i];
        i++;
      }
      sus[j] = '\0';
    }
    if(strcmp(compare,"NULL") == 0 || strstr(sus,compare)){
      ctr++;
      printf("Person #%d\n",ctr);
      printf("  name --------------------- %s\n",link->name);
      printf("  zip code ----------------- %s\n",link->zip_code);
      printf("  telephone number --------- %s\n",link->telephone_number);
      printf("  social security number --- %s\n",link->social_security_number);
    }
    list(link->right[main_index], main_index, compare);

  }
  return EXIT_SUCCESS;
}
//
// main program
//
int main(int argc,char **argv){
  
  // process the command line arguments
  // if(argc < 3){
  //   fprintf(stderr,"Usage: %s student_number number_of_persons [options ...]\n",argv[0]);
  //   fprintf(stderr,"Recognized options:\n");
  //   fprintf(stderr,"  -list[N]              # list the tree contents, sorted by key index N (the default is index 0)\n");
  //   // place a description of your own options here
  //   return 1;
  // }
  // int student_number = atoi(argv[1]);
  // if(student_number < 1 || student_number >= 1000000){
  //   fprintf(stderr,"Bad student number (%d) --- must be an integer belonging to [1,1000000{\n",student_number);
  //   return 1;
  // }
  FILE *graphs_0, *graphs_1, *graphs_2, *graphs_3;
  FILE *creation;
  FILE *search_0, *search_1, *search_2, *search_3;
  graphs_0 = fopen("depth_histogram00.txt", "a");
  graphs_1 = fopen("depth_histogram01.txt", "a");
  graphs_2 = fopen("depth_histogram02.txt", "a");
  graphs_3 = fopen("depth_histogram03.txt", "a");
  creation = fopen("creation_histogram.txt", "a");
  search_0 = fopen("search_histogram00.txt", "a");
  search_1 = fopen("search_histogram01.txt", "a");
  search_2 = fopen("search_histogram02.txt", "a");
  search_3 = fopen("search_histogram03.txt", "a");
  int n_persons = 100000;
  for (int student_number = 1; student_number <= 10000; student_number++) {
    printf("student -> %d\n", student_number);
    double dt;
    // generate all data
    tree_node_t *persons = (tree_node_t *)calloc((size_t)n_persons,sizeof(tree_node_t)); // arvore para por as pessoas
    if(persons == NULL){
      fprintf(stderr,"Output memory!\n");
      return 1;
    }
    aed_srandom(student_number);
    for(int i = 0;i < n_persons;i++){
      random_name(&(persons[i].name[0]));
      random_zip_code(&(persons[i].zip_code[0]));
      random_telephone_number(&(persons[i].telephone_number[0]));
      random_social_security_number(&(persons[i].social_security_number[0]));
      for(int j = 0;j < 4;j++)
        persons[i].left[j] = persons[i].right[j] = NULL; // make sure the pointers are initially NULL
    }
    // create the ordered binary trees
    dt = cpu_time();
    tree_node_t *roots[4]; // fou indices, four roots
    for(int main_index = 0;main_index < 4;main_index++){
      roots[main_index] = NULL;
    }
    for(int i = 0;i < n_persons;i++){
      for(int main_index = 0;main_index < 4;main_index++){
        tree_insert(&(roots[main_index]),&(persons[i]), main_index); // place your code here to insert &(persons[i]) in the tree with number main_index
      }
    }
    dt = cpu_time() - dt;
    fprintf(creation, "%17.15f\n", dt);
    // search the tree
    for(int main_index = 0;main_index < 4;main_index++){
      dt = cpu_time();
      for(int i = 0;i < n_persons;i++){
        tree_node_t n = persons[i]; // make a copy of the node data
        if(find(roots[main_index],&(n), main_index) != &(persons[i])) // place your code here to find a given person, searching for it using the tree with number main_index
        {
          fprintf(stderr,"person %d not found using index %d\n",i,main_index);
          return 1;
        }
      }
      dt = cpu_time() - dt;
      if (main_index == 0) {
        fprintf(search_0, "%17.15f\n", dt);
      }
      else if (main_index == 1) {
        fprintf(search_1, "%17.15f\n", dt);
      }
      else if (main_index == 2) {
        fprintf(search_2, "%17.15f\n", dt);
      }
      else {
        fprintf(search_3, "%17.15f\n", dt);
      }
    }
    // compute the largest tree depdth
    for(int main_index = 0;main_index < 4;main_index++){
      dt = cpu_time();
      int depth = tree_depth(roots[main_index], main_index); // place your code here to compute the depth of the tree with number main_index
      dt = cpu_time() - dt;
      if (main_index == 0) {
        fprintf(graphs_0, "%d\n", depth);
      }
      else if (main_index == 1) {
        fprintf(graphs_1, "%d\n", depth);
      }
      else if (main_index == 2) {
        fprintf(graphs_2, "%d\n", depth);
      }
      else {
        fprintf(graphs_3, "%d\n", depth);
      }
    }
    // process the command line optional arguments
    for(int i = 3;i < argc;i++){
      if(strncmp(argv[i],"-list",5) == 0){ // list all (optional)
        int main_index = atoi(&(argv[i][5]));
        if(main_index < 0)
          main_index = 0;
        if(main_index > 3)
          main_index = 3;
        printf("List of persons:\n");
        if ((i+1) < argc){
          (void)list(roots[main_index], main_index, argv[i+1]); // place your code here to traverse, in order, the tree with number main_index
        } else {
          (void)list(roots[main_index], main_index, "NULL"); // place your code here to traverse, in order, the tree with number main_index
        }
      }
      // place your own options here
      // I wanna commit unlive.
    }
    // clean up --- don't forget to test your program with valgrind, we don't want any memory leaks
    
    free(persons);
  }
  fclose(graphs_0);
  fclose(graphs_1);
  fclose(graphs_2);
  fclose(graphs_3);
  fclose(creation);
  fclose(search_0);
  fclose(search_1);
  fclose(search_2);
  fclose(search_3);
  return 0;
}