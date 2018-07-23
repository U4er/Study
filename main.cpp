#include<stdio.h>
#include<malloc.h>

#define OK 1
#define ERROR 0
#define RED 0
#define BLACK 1
#define LEFT 0
#define RIGHT 1
#define status int

enum {PREORDERVISIT,INORDERVISIT,POSTORDERVISIT};

typedef int Type;

typedef struct rbnode
{
    Type key;
    struct rbnode* parent;
    struct rbnode* lchild;
    struct rbnode* rchild;
    char color;
}Node,*RBTree;


void LeftRotate(RBTree &T,RBTree node) {
    RBTree X = node->rchild;
    node->rchild = X->lchild;
    if (X->lchild) X->lchild->parent=node;
    X->lchild = node;
    X->parent = node->parent;
    node->parent = X;
    if (!X->parent) T = X;
    else {
        if (X->parent->lchild==node) X->parent->lchild = X;
        else X->parent->rchild = X;
    }
}

void RightRotate(RBTree &T,RBTree node) {// whther need "&" or not
    RBTree X = node->lchild;
    node->lchild = X->rchild;
    if (X->rchild) X->rchild->parent=node;
    X->rchild = node;
    X->parent = node->parent;
    node->parent = X;
    if (!X->parent) T=X;
    else {
        if (X->parent->lchild==node) X->parent->lchild = X;
        else X->parent->rchild = X;
    }
}

void ColorSwap (RBTree A, RBTree B) {
    char c = A->color;
    A->color = B->color;
    B->color = c;
}

int IsRed (RBTree A) {
    if (A&&A->color==RED) return 1;
    else return 0;
}

void InitRBTree(RBTree &T){
    T=NULL;
}

void InsertFixup (RBTree &T,RBTree node) {
    RBTree P,G;
    while ((P=node->parent)&&IsRed(P)) {
        G=P->parent;
        if (IsRed(G->lchild)&&IsRed(G->rchild)) {
            G->color=RED;
            G->lchild->color=G->rchild->color=BLACK;
            node=G;
            continue;
        }
        if (P==G->lchild) {
            if (node==P->rchild) {
                LeftRotate(T,P);
                node=P;
                P=node->parent;
            }
            ColorSwap(G,P);
            RightRotate(T,G);
        }
        else {
            if (node==P->lchild) {
                RightRotate(T,P);
                node=P;
                P=node->parent;
            }
            ColorSwap(G,P);
            LeftRotate(T,G);
        }
    }
    T->color=BLACK;



}

status RBTreeInsert(RBTree &T, Type k){
    RBTree node = (RBTree) malloc(sizeof(Node));
    if (!node) return ERROR;
    node->color=RED;
    node->key=k;
    node->lchild=node->rchild=node->parent=NULL;

    RBTree y=NULL;
    RBTree x=T;
    while (x) {
        y=x;
        if (k<x->key) x=x->lchild;
        else x=x->rchild;
    }
    if (y) {
        if (y->key>k) y->lchild=node;
        else y->rchild=node;
        node->parent=y;
    }
    else T=node;
    InsertFixup (T,node);
    return OK;
}

int IsLchild (RBTree A) {
    return A->parent->lchild == A;
}

RBTree RBTreeSearch (RBTree T,Type k) {
    if (!T) return NULL;
    while (T) {
    if (T->key<k) T = T->rchild;
        else if (T->key>k) T = T->lchild;
        else break;
    }
    return T;
}

void DeleteFixup (RBTree &T, RBTree node) {
    RBTree P,B;
    int flag = 1, d;
    while ((P = node->parent)&&!IsRed(node)) {
        if ((node->lchild||node->rchild)&&flag) {
            if (node->lchild) ColorSwap(node,node->lchild);
            else ColorSwap(node,node->rchild);
            break;
        }

        B=P->rchild;
        d=LEFT;
        if (!IsLchild(node)) {
            B=P->lchild;
            d=RIGHT;
        }

        if (B->lchild||B->rchild) {
            if (d==LEFT) {
                if (IsRed(B)) {
                    ColorSwap(P,B);
                    LeftRotate(T,P);
                }
                else if (IsRed(B->lchild)||IsRed(B->rchild)){
                    if (B->rchild) {
                        B->rchild->color=BLACK;
                        ColorSwap(P,B);
                        LeftRotate(T,P);
                        break;
                    }
                    else {
                        ColorSwap(B,B->lchild);
                        RightRotate(T,B);
                    }
                }
                else {
                    B->color=RED;
                    node=P;
                    flag=0;
                }
            }
            else {
                if (IsRed(B)) {
                    ColorSwap(P,B);
                    RightRotate(T,P);
                }
                else if (IsRed(B->lchild)||IsRed(B->rchild)){
                    if (B->lchild) {
                        B->lchild->color=BLACK;
                        ColorSwap(P,B);
                        RightRotate(T,P);
                        break;
                    }
                    else {
                        ColorSwap(B,B->rchild);
                        LeftRotate(T,B);
                    }
                }
                else {
                    B->color=RED;
                    node=P;
                    flag=0;
                }
            }
        }

        else {
            if (IsRed(P)) {
                ColorSwap(P,B);
                break;
            }
            else {
                B->color=RED;
                node=P;
                flag=0;
            }
        }
    }
}

status RBTreeDelete (RBTree &T,Type k) {
    RBTree node = RBTreeSearch(T,k);
    if (!node) {
        printf("The element is not exist !");
        return ERROR;
    }
    if (node->lchild&&node->rchild) {
        RBTree p = node->rchild;
        while (p->lchild) p = p->lchild;
        node->key = p->key;
        node = p;
    }
    DeleteFixup(T ,node);
    if (node->lchild) {
        node->lchild->parent = node->parent;
        if (node->parent) {
            if (IsLchild(node)) node->parent->lchild = node->lchild;
            else node->parent->rchild = node->lchild;
        }
        else T = node->lchild;
    }
    else {
        if (node->rchild)
            node->rchild->parent = node->parent;
        if (node->parent) {
            if (IsLchild(node)) node->parent->lchild = node->rchild;
            else node->parent->rchild = node->rchild;
        }
        else T = node->rchild;
    }
    if (T) T->color=BLACK;
    free(node);
    return OK;
}

void IsElementExist (RBTree T,Type k) {
    if (RBTreeSearch(T,k)) printf("Exist");
    else printf("The element is not exist !");
}

status PreorderVisit (RBTree T) {
    if (!T) return ERROR;
    printf("%d,%d ",T->key,T->color);
    PreorderVisit(T->lchild);
    PreorderVisit(T->rchild);
    return OK;
}

status InorderVisit (RBTree T) {
    if (!T) return ERROR;
    InorderVisit(T->lchild);
    printf("%d,%d ",T->key,T->color);
    InorderVisit(T->rchild);
    return OK;
}

status PostorderVisit (RBTree T) {
    if (!T) return ERROR;
    PostorderVisit(T->lchild);
    PostorderVisit(T->rchild);
    printf("%d,%d ",T->key,T->color);
    return OK;
}

void RBTreeVisit (RBTree T, int way) {
    int s;
    switch (way) {
        case 0: s=PreorderVisit(T);break;
        case 1: s=InorderVisit(T);break;
        s=PostorderVisit(T);
    }
    if (s) printf("\n");
    else printf("The Tree is empty!");
}

int main ()
{
    RBTree T;
    InitRBTree(T);
//    int x=0;
//    do {
//        scanf("%d",&x);
//        if (x>0) RBTreeInsert(T,x);
//    }while(x>0);
    int a[20]={12,1,9,2,0,11,7,19,4,15,18,5,14,13,10,16,6,3,8,17};
    for (int i=0;i<20;i++) RBTreeInsert(T,a[i]);
    RBTreeVisit(T,1);
//    do {
//        scanf("%d",&x);
//        if (x>0) RBTreeDelete(T,x);
//        RBTreeVisit(T,1);
//    }while (x>0);
    for (int i=0;i<20;i++) {
        RBTreeDelete(T,a[i]);
        RBTreeVisit(T,1);
    }
    return 0;
}
