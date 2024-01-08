#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include <stdlib.h>
#define MAX_SIZE 50


typedef enum { head, entry } tagfield;
typedef  struct  matrix_node* matrix_pointer;
struct  entry_node {
    int row;
    int col;
    int value;
};
struct matrix_node {
    matrix_pointer down;
    matrix_pointer right;
    tagfield tag;
    union {
        matrix_pointer next;
        struct entry_node entry;
    } u;
};
matrix_pointer hdnode[MAX_SIZE];

matrix_pointer mread() {
    int num_rows, num_cols, num_terms, num_heads, i;
    int row, col, value, current_row;
    matrix_pointer temp, last, node;

    /*Enter the number of rows, columns and number if nonzero terms: */
    FILE* ifp;
    ifp = fopen("HW4_20202106./input.txt", "r");
    if (ifp == NULL) {
        fprintf(stderr, "File is not open\n");
        exit(1);
    }

    num_rows = 7; num_cols = 7;
    num_terms = 7;

    num_heads = (num_cols > num_rows) ? num_cols : num_rows;

    /*set up header node for the list of header nudes*/
    node = (matrix_pointer)malloc(sizeof(struct matrix_node)); node->tag = entry;
    node->u.entry.row = num_rows;
    node->u.entry.col = num_cols;

    if (!num_heads) node->right = node;
    else { /* initialize the header nodes */
        for (i = 0; i < num_heads; i++) {
            temp = (matrix_pointer)malloc(sizeof(struct matrix_node));
            hdnode[i] = temp; hdnode[i]->tag = head;
            hdnode[i]->right = temp; hdnode[i]->u.next = temp;
        }
        current_row = 0;
        last = hdnode[0]; /* last node in current row */
        for (i = 0; i < num_terms; i++) {
            fscanf(ifp, "%d %d %d", &row, &col, &value);
            if (row > current_row) { /* close current row */
                last->right = hdnode[current_row];
                current_row = row;  last = hdnode[row];
            }
            temp = (matrix_pointer)malloc(sizeof(struct matrix_node));  temp->tag = entry;
            temp->u.entry.row = row;  temp->u.entry.col = col;
            temp->u.entry.value = value;
            last->right = temp; /* link into row list */
            last = temp;
            hdnode[col]->u.next->down = temp; /* link into column list */
            hdnode[col]->u.next = temp;
        }


        /* close last row */
        last->right = hdnode[current_row];
        /* close all column lists */
        for (i = 0; i < num_cols; i++)
            hdnode[i]->u.next->down = hdnode[i];
        /* link all header nodes together */
        for (i = 0; i < num_heads - 1; i++)
            hdnode[i]->u.next = hdnode[i + 1];
        hdnode[num_heads - 1]->u.next = node;
        node->right = hdnode[0];
    }
    fclose(ifp);
    return node;
}
void swap(int* x, int* y) {
    int wswap;

    wswap = *x;
    *x = *y;
    *y = wswap;
}
void mtranspose(matrix_pointer node) {
    int i, j;
    int work,count=0;


    matrix_pointer temp, temp_i, temp_j, pt;

    for (i = 0; i < node->u.entry.row; i++) {
        for (temp = hdnode[i]->right; temp != hdnode[i]; temp = temp->right) {


            swap(&temp->u.entry.col, &temp->u.entry.row);

        }
    }

  
    for (i = 1; i < node->u.entry.row - 1; i++) {
        
        for (temp_i = hdnode[i + 1]->right; temp_i != hdnode[i + 1]; temp_i = temp_i->right) {
                        for (j = 2; j < node->u.entry.row - 1; j++) {
                for (temp_j = hdnode[j]->right; temp_j != hdnode[j]; temp_j = temp_j->right) {   
                    count++;
                    if ((temp_i->u.entry.row < temp_j->u.entry.row)&&count%7!=1) {
                        
                        swap(&temp_i->u.entry.row, &temp_j->u.entry.row);
                        swap(&temp_i->u.entry.col, &temp_j->u.entry.col);
                        swap(&temp_i->u.entry.value, &temp_j->u.entry.value);



                    }
                }
            }
        }
    }
}

void mwrite(matrix_pointer node)
{ /* print out the matrix in row major form */
    int i;
    FILE* ofp;
    ofp = fopen("HW4_20202106./output.txt", "w");
    if(ofp==NULL){
        printf("output.txt is not open");
        exit(1);
    }

    matrix_pointer temp;

    for (i = 0; i < node->u.entry.row; i++) {
        /* print out the entries in each row */
        for (temp = hdnode[i]->right; temp != hdnode[i]; temp = temp->right)
            fprintf(ofp, " %d %d %d\n",
                temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);

    }
    fclose(ofp);


}





int main(void) {
    matrix_pointer node;;

    node = mread();
    mtranspose(node);
    mwrite(node);
    return 0;

}
