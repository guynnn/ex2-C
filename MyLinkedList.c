#include "MyLinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * represents a Node in the linked list
 */
typedef struct Node
{
    char * data;
    struct Node * next;
} Node;

/**
 * represents the head of the linked list
 */
typedef struct _MyLinkedList
{
    int size;
    Node * first;
} Head;

int removeValue(MyLinkedListP l, char * val);


/**
 * @brief Allocates a new empty LinkedList
 * 			It is the caller's responsibility to free the returned LinkedList.
 *
 * RETURN VALUE:
 * @return a pointer to the new LinkedList, or NULL if the allocation failed.
 */
MyLinkedListP createList()
{
    MyLinkedListP list = (MyLinkedListP)malloc(sizeof(Head));
    if (list == NULL)
    {
        return NULL;
    }
    list->size = 0;
    list->first = NULL;
    return list;
}

/**
 * @brief print LinkedList l and it's contents- see school solution for the exact format.
 * @param l the LinkedList to print.
 */
void printList(MyLinkedListP l)
{
    if (l == NULL)
    {
        return;
    }
    if (l->size == 0)
    {
        printf("Empty!\n");
        return;
    }
    Node * current = l->first;
    int size = l->size;
    const char arrow[3] = "->";
    while (size-- > 0)
    {
        printf("'%s'%s", current->data, arrow);
        current = current->next;
    }
    printf("|| size:%d\n", l->size);
}

/**
 * @brief add val to the beginning of the list(the List may contain duplicates)
 * @param l the LinkedList
 * @param val the value - char *
 * RETURN VALUE:
 * @return true iff succeed
 */
bool insertFirst(MyLinkedListP list, char * data)
{
    if (list == NULL || data == NULL)
    {
        return false;
    }
    ++list->size;
    Node * toAdd = (Node*)malloc(sizeof(Node));
    if (toAdd == NULL)
    {
        return false;
    }
    toAdd->data = (char*)malloc(sizeof(strlen(data) + 1));
    if (toAdd->data == NULL)
    {
        return false;
    }
    strcpy(toAdd->data, data);
    toAdd->next = list->first;
    list->first = toAdd;
    return true;
}

/**
 * @brief get list size
 * @param l the LinkedList
 * RETURN VALUE:
 * @return number of elements in the list.  or MYLIST_ERROR_CODE if error occured
 */

int getSize(MyLinkedListP list)
{
    if (list == NULL)
    {
        return MYLIST_ERROR_CODE;
    }
    return list->size;
}

/**
 * @brief remove all the ocuurences of val in l
 * @param l the LinkedList
 * @param val the value - char *
 * @return number of elements that were removed. or MYLIST_ERROR_CODE if error occured
 */
int removeData(MyLinkedListP l, char * val)
{
    if (l == NULL || val == NULL)
    {
        return MYLIST_ERROR_CODE;
    }
    if (l->size == 0)
    {
        return 0;
    }
    int start = l->size;
    // removing the value that need to be removed one by one
    int again = removeValue(l, val);
    while (again == 1)
    {
        again = removeValue(l, val);
    }
    int end = l->size;
    return start - end;
}

/**
 * @brief remove the first ocuurence of val in l
 * @param l the LinkedList
 * @param val the value - char *
 * @return 1 if some element has been removed, 0 otherwise
 */
int removeValue(MyLinkedListP l, char * val)
{
    Node * current = l->first;
    if (strcmp(l->first->data, val) == 0)
    {
        if (l->size == 1)
        {
            free(l->first->data);
            free(l->first);
            l->first = NULL;
            l->size = 0;
            return 0;
        }
        Node * temp = l->first;
        l->first = l->first->next;
        free(temp->data);
        free(temp);
        l->size--;
        return 1;
    }
    current = current->next;
    Node * previous = l->first;
    int loops = l->size - 1;
    while (loops-- > 0)
    {
        if (strcmp(current->data, val) == 0)
        {
            previous->next = current->next;
            free(current->data);
            free(current);
            l->size--;
            return 1;
        }
        current = current->next;
        previous = previous->next;
    }
    return 0;
}

/**
 * @brief Allocates a new MyLinkedList with the same values as l. It is the caller's
 * 	      responsibility to free the returned LinkedList.
 * @param l the MyLinkedListP to clone.
 * @return a pointer to the new LinkedList, or NULL if the allocation failed.
 */
MyLinkedListP cloneList(MyLinkedListP source)
{
    MyLinkedListP clone = createList();
    int loops = source->size;
    Node * toCopy = source->first;
    Node ** toAdd = &clone->first;
    while (loops-- > 0)
    {
        *toAdd = (Node*)malloc(sizeof(Node));
        if (*toAdd == NULL)
        {
            return NULL;
        }
        (*toAdd)->data = (char*)malloc(sizeof(strlen(toCopy->data)));
        if ((*toAdd)->data == NULL)
        {
            return NULL;
        }
        strcpy((*toAdd)->data, toCopy->data);
        toAdd = &(*toAdd)->next;
        toCopy = toCopy->next;
    }
    clone->size = source->size;
    return clone;
}

/**
 * @brief search val in the list
 * @param l the LinkedList
 * @param val the value to add
 * @return the number of val ocuurences in the list.  or MYLIST_ERROR_CODE if error occured
 */
int isInList(MyLinkedListP list, char * val)
{
    if (list == NULL || val == NULL)
    {
        return MYLIST_ERROR_CODE;
    }
    int counter = 0;
    int size = list->size;
    Node * current = list->first;
    while (size-- > 0)
    {
        if (strcmp(current->data, val) == 0)
        {
            counter++;
        }
        current = current->next;
    }
    return counter;
}

/**
 * @brief Returns size in bytes of l and all it's contents
 *		  equivalent to sum of sizeof for all the list contents
 * @param l the LinkedList
 * @return the allocated size for l
 */
int getSizeOf(MyLinkedListP list)
{
    int size = list->size;
    int sum = sizeof(Head);
    Node * current = list->first;
    while (size-- > 0)
    {
        sum += sizeof(current) + strlen(current->data) + 1;
        current = current->next;
    }
    return sum;
}

/**
 * @brief Frees the memory and resources allocated to LinkedList l.
 * @param l the LinkedList to free.
 * If l is NULL, no operation is performed.
 */
void freeList(MyLinkedListP list)
{
    if (list == NULL)
    {
        return;
    }
    Node * current = list->first;
    int size = list->size;
    while (size-- > 0)
    {
        Node * temp = current;
        free(current->data);
        current = current->next;
        free(temp);
    }
    list->size = 0;
    free(list);
}
