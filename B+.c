#include <stdio.h>
#include <stdlib.h>
#include "B+.h"

#define DEPTH 128

BTree* initial(int order)
{
    BTree* re=(BTree*)malloc(sizeof(BTree));
    re->head=NULL;
    re->root=NULL;
    re->order=order;

    return re;
}

static void insertList(int* l,int n,int key)
{
    int i=n;
    for (;i>=0;i--)
    {
        if (i==0 || key > l[i-1]) break;
        l[i]=l[i-1];
    }
    l[i]=key;
}

static PtrToBNode insertinner(PtrToBNode B,int key,int order)
{
    PtrToBNode curr=B;
    PtrToBNode stack[DEPTH];
    int Istack[DEPTH];
    int top=0;

    while (1)
    {
        stack[top++]=curr;
        if (curr->leaf)
            break;

        int i;
        for (i=0;i<curr->num;i++)
        {
            if (key < curr->data[i])
                break;
        }
        Istack[top]=i;
        curr=curr->children[i];
    }

    PtrToBNode p=stack[top-1];
    insertList(p->data,p->num,key);
    p->num++;

    int up=0;
    PtrToBNode A1=NULL,A2=NULL;
    int flag=0;

    if (p->num > order)
    {
        int n1num=order+1-(order+1)/2;
        int n2num=(order+1)/2;

        A1=(PtrToBNode)malloc(sizeof(BNode));
        A2=(PtrToBNode)malloc(sizeof(BNode));
        A1->data=(int*)malloc(sizeof(int)*(order+1));
        A2->data=(int*)malloc(sizeof(int)*(order+1));
        A1->children=NULL;
        A2->children=NULL;
        A1->leaf=1;
        A2->leaf=1;

        for (int i=0;i<n1num;i++)
            A1->data[i]=p->data[i];
        for (int i=0;i<n2num;i++)
            A2->data[i]=p->data[n1num+i];
        A1->num=n1num;
        A2->num=n2num;

        A1->prev=p->prev;
        A1->next=A2;
        A2->prev=A1;
        A2->next=p->next;
        if (p->prev!=NULL)
            p->prev->next=A1;
        if (p->next!=NULL)
            p->next->prev=A2;

        up=A2->data[0];

        free(p->data);
        free(p);

        top--;
        flag=1;
    }

    while (flag==1)
    {
        if (top==0)
        {
            PtrToBNode newroot=(PtrToBNode)malloc(sizeof(BNode));
            newroot->data=(int*)malloc(sizeof(int)*(order+1));
            newroot->children=(PtrToBNode*)malloc(sizeof(PtrToBNode)*(order+2));
            newroot->leaf=0;
            newroot->num=1;
            newroot->data[0]=up;
            newroot->children[0]=A1;
            newroot->children[1]=A2;
            newroot->prev=NULL;
            newroot->next=NULL;

            return newroot;
        }

        p=stack[--top];
        int index=Istack[top+1];
        for (int i=p->num;i>=index;i--)
        {
            p->children[i+1]=p->children[i];
        }
        p->children[index]=A1;
        p->children[index+1]=A2;

        insertList(p->data,p->num,up);
        p->num++;

        if (p->num > order)
        {
            int n1num=order+1-(order+1)/2;
            int n2num=order-n1num;

            A1=(PtrToBNode)malloc(sizeof(BNode));
            A2=(PtrToBNode)malloc(sizeof(BNode));
            A1->data=(int*)malloc(sizeof(int)*(order+1));
            A2->data=(int*)malloc(sizeof(int)*(order+1));
            A1->children=(PtrToBNode*)malloc(sizeof(PtrToBNode)*(order+2));
            A2->children=(PtrToBNode*)malloc(sizeof(PtrToBNode)*(order+2));
            A1->leaf=0;
            A2->leaf=0;
            A1->prev=NULL;
            A2->prev=NULL;
            A1->next=NULL;
            A2->next=NULL;

            for (int i=0;i<n1num;i++)
                A1->data[i]=p->data[i];
            for (int i=0;i<n2num;i++)
                A2->data[i]=p->data[n1num+1+i];
            for (int i=0;i<=n1num;i++)
                A1->children[i]=p->children[i];
            for (int i=0;i<=n2num;i++)
                A2->children[i]=p->children[n1num+1+i];
            A1->num=n1num;
            A2->num=n2num;

            up=p->data[n1num];

            free(p->data);
            free(p->children);
            free(p);

            flag=1;
        }
        else
            flag=0;
    }

    return B;
}

BTree* insertBPT(BTree* B, int key)
{
    if (B->root ==NULL)
    {
        PtrToBNode newnode=(PtrToBNode)malloc(sizeof(BNode));

        newnode->data=(int*)malloc(sizeof(int)*(B->order+1));
        newnode->num=0;
        newnode->data[newnode->num++]=key;

        newnode->children=(PtrToBNode*)malloc(sizeof(PtrToBNode)*(B->order+2));
        for (int i=0;i<B->order+2;i++)
            newnode->children[i]=NULL;
        newnode->next=NULL;
        newnode->prev=NULL;

        newnode->leaf=1;

        B->head=newnode;
        B->root=newnode;

        return B;
    }

    B->root=insertinner(B->root,key,B->order);

    PtrToBNode p=B->root;
    while (!p->leaf)
        p=p->children[0];
    B->head=p;

    return B;
}

static void fixMinUp(PtrToBNode* stack,int* Istack,int top,int oldmin,int newmin)
{
    for (int t=top;t>=1;t--)
    {
        PtrToBNode parent=stack[t-1];
        int index=Istack[t];
        if (index>0 && parent->data[index-1]==oldmin)
            parent->data[index-1]=newmin;
    }
}

static PtrToBNode deleteinner(PtrToBNode B,int key,int order)
{
    PtrToBNode curr=B;
    PtrToBNode stack[DEPTH];
    int Istack[DEPTH];
    int top=0;

    while (1)
    {
        stack[top++]=curr;
        if (curr->leaf)
            break;

        int i;
        for (i=0;i<curr->num;i++)
        {
            if (key < curr->data[i])
                break;
        }
        Istack[top]=i;
        curr=curr->children[i];
    }

    PtrToBNode p=stack[--top];
    int i,flag=0;
    for (i=0;i<p->num;i++)
        if (p->data[i]==key)
            break;

    if (i==p->num)
        return B;

    for (int j=i;j<p->num-1;j++)
        p->data[j]=p->data[j+1];

    p->num--;

    /* 删掉的正好是最小键，祖先里指向它的分隔键要跟着改 */
    if (i==0 && p->num>0)
        fixMinUp(stack,Istack,top,key,p->data[0]);

    int min=order/2;

    flag=1;
    while (flag)
    {
        if (top==0 || p->num>=min)
        {
            flag=0;
            break;
        }

        PtrToBNode parent=stack[top-1];
        int index=Istack[top];

        if (p->leaf)
        {
            PtrToBNode L=(index>0)?parent->children[index-1]:NULL;
            PtrToBNode R=(index<parent->num)?parent->children[index+1]:NULL;

            if (L!=NULL && L->num>min)
            {
                /* 向左兄弟借最大的键 */
                int oldmin=parent->data[index-1];
                for (int j=p->num;j>0;j--)
                    p->data[j]=p->data[j-1];
                p->data[0]=L->data[--L->num];
                p->num++;
                parent->data[index-1]=p->data[0];
                fixMinUp(stack,Istack,top,oldmin,p->data[0]);
                flag=0;
            }
            else if (R!=NULL && R->num>min)
            {
                /* 向右兄弟借最小的键 */
                int wasEmpty=(p->num==0);
                p->data[p->num++]=R->data[0];
                for (int j=0;j<R->num-1;j++)
                    R->data[j]=R->data[j+1];
                R->num--;
                parent->data[index]=R->data[0];
                if (wasEmpty)
                    fixMinUp(stack,Istack,top,key,p->data[0]);
                flag=0;
            }
            else if (L!=NULL)
            {
                /* 并到左兄弟 */
                for (int j=0;j<p->num;j++)
                    L->data[L->num+j]=p->data[j];
                L->num+=p->num;
                L->next=p->next;
                if (p->next!=NULL)
                    p->next->prev=L;

                for (int j=index-1;j<parent->num-1;j++)
                    parent->data[j]=parent->data[j+1];
                for (int j=index;j<parent->num;j++)
                    parent->children[j]=parent->children[j+1];
                parent->num--;

                free(p->data);
                free(p);

                p=stack[--top];
            }
            else
            {
                /* index==0，没有左兄弟，把右兄弟并进来 */
                if (p->num==0)
                    fixMinUp(stack,Istack,top,key,R->data[0]);
                for (int j=0;j<R->num;j++)
                    p->data[p->num+j]=R->data[j];
                p->num+=R->num;
                p->next=R->next;
                if (R->next!=NULL)
                    R->next->prev=p;

                free(R->data);
                free(R);

                for (int j=0;j<parent->num-1;j++)
                    parent->data[j]=parent->data[j+1];
                for (int j=1;j<parent->num;j++)
                    parent->children[j]=parent->children[j+1];
                parent->num--;

                p=stack[--top];
            }
        }
        else
        {
            PtrToBNode L=(index>0)?parent->children[index-1]:NULL;
            PtrToBNode R=(index<parent->num)?parent->children[index+1]:NULL;

            if (L!=NULL && L->num>min)
            {
                /* 内部结点向左兄弟借：经父结点旋转 */
                for (int j=p->num;j>0;j--)
                    p->data[j]=p->data[j-1];
                for (int j=p->num+1;j>0;j--)
                    p->children[j]=p->children[j-1];
                p->data[0]=parent->data[index-1];
                p->children[0]=L->children[L->num];
                p->num++;
                parent->data[index-1]=L->data[--L->num];
                flag=0;
            }
            else if (R!=NULL && R->num>min)
            {
                /* 内部结点向右兄弟借 */
                p->data[p->num]=parent->data[index];
                p->children[p->num+1]=R->children[0];
                p->num++;
                parent->data[index]=R->data[0];
                for (int j=0;j<R->num-1;j++)
                    R->data[j]=R->data[j+1];
                for (int j=0;j<R->num;j++)
                    R->children[j]=R->children[j+1];
                R->num--;
                flag=0;
            }
            else if (L!=NULL)
            {
                /* 内部结点并到左兄弟：父键下拉夹中间 */
                int base=L->num;
                L->data[base]=parent->data[index-1];
                for (int j=0;j<=p->num;j++)
                    L->children[base+1+j]=p->children[j];
                for (int j=0;j<p->num;j++)
                    L->data[base+1+j]=p->data[j];
                L->num=base+1+p->num;

                for (int j=index-1;j<parent->num-1;j++)
                    parent->data[j]=parent->data[j+1];
                for (int j=index;j<parent->num;j++)
                    parent->children[j]=parent->children[j+1];
                parent->num--;

                free(p->data);
                free(p->children);
                free(p);

                p=stack[--top];
            }
            else
            {
                /* index==0，右兄弟并进来 */
                int base=p->num;
                p->data[base]=parent->data[index];
                for (int j=0;j<=R->num;j++)
                    p->children[base+1+j]=R->children[j];
                for (int j=0;j<R->num;j++)
                    p->data[base+1+j]=R->data[j];
                p->num=base+1+R->num;

                for (int j=0;j<parent->num-1;j++)
                    parent->data[j]=parent->data[j+1];
                for (int j=1;j<parent->num;j++)
                    parent->children[j]=parent->children[j+1];
                parent->num--;

                free(R->data);
                free(R->children);
                free(R);

                p=stack[--top];
            }
        }
    }

    /* 根：内部空了就降高，叶子空了就是空树 */
    if (!B->leaf && B->num==0)
    {
        PtrToBNode old=B;
        B=B->children[0];
        free(old->data);
        free(old->children);
        free(old);
    }

    if (B->leaf && B->num==0)
    {
        free(B->data);
        free(B->children);
        free(B);
        return NULL;
    }

    return B;
}

BTree* deleteBPT(BTree* B,int key)
{
    if (B->root==NULL)
        return NULL;

    B->root=deleteinner(B->root,key,B->order);

    if (B->root==NULL)
    {
        B->head=NULL;
        return B;
    }

    PtrToBNode p=B->root;
    while (!p->leaf)
        p=p->children[0];
    B->head=p;

    return B;
}