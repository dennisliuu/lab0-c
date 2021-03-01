#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

// For replacing 'strcpy'
#ifndef strlcpy
#define strlcpy(dst, src, sz) snprintf((dst), (sz), "%s", (src))
#endif

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (q == NULL)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q == NULL)
        return;

    while (q->head != NULL) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    char *tmp = malloc(sizeof(char) * strlen(s) + 1);
    if (tmp == NULL) {
        free(newh);
        return false;
    }

    newh->value = tmp;
    strncpy(newh->value, s, (strlen(s) + 1));
    newh->next = q->head;
    q->head = newh;

    // If queue is empty, insert head also should setup tail
    if (q->size == 0)
        q->tail = newh;

    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    char *tmp = malloc(sizeof(char) * (strlen(s) + 1));
    if (tmp == NULL) {
        // remember to release newh!!
        free(newh);
        return false;
    }

    newh->value = tmp;
    strlcpy(newh->value, s, (strlen(s) + 1));

    if (q->size == 0) {
        q->tail = newh;
        q->head = newh;
        return true;
    } else {
        newh->next = NULL;
        q->tail->next = newh;
        q->tail = newh;
    }

    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    if (q == NULL || q->head == NULL)
        return false;

    if (bufsize > strlen(q->head->value)) {
        strncpy(sp, q->head->value, strlen(q->head->value));
        sp[strlen(q->head->value)] = '\0';
    } else {
        strncpy(sp, q->head->value, bufsize);
        sp[bufsize] = '\0';
    }

    // Free allocated memory
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    tmp->next = NULL;
    free(tmp->value);
    free(tmp);

    q->size--;
    if (q->size == 0)
        q->tail = NULL;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q == NULL)
        return 0;

    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if ((q == NULL) || (q->size == 0))
        return;

    list_ele_t *cur = q->head;
    list_ele_t *next = cur->next;
    list_ele_t *tmp;

    q->tail = q->head;

    while (next != NULL) {
        tmp = next->next;
        next->next = cur;
        cur = next;
        next = tmp;
    }
    q->tail->next = NULL;
    q->head = cur;
}

void merge_sorted_list(list_ele_t *a, list_ele_t *b, list_ele_t **tmp)
{
    while (a && b) {
        if (strcmp(a->value, b->value) < 0) {
            *tmp = a;
            a = a->next;
        } else {
            *tmp = b;
            b = b->next;
        }
        tmp = &((*tmp)->next);
    }

    *tmp = a ? a : b;
}

void merge_sort(list_ele_t **head)
{
    if (*head == NULL || (*head)->next == NULL)
        return;

    list_ele_t *fast = (*head)->next;
    list_ele_t *slow = (*head);

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    slow = *head;

    merge_sort(&fast);
    merge_sort(&slow);

    *head = NULL;
    list_ele_t **tmp = head;

    merge_sorted_list(fast, slow, tmp);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q == NULL || q->head == NULL || q->size == 1)
        return;

    merge_sort(&q->head);

    while (q->tail->next != NULL)
        q->tail = q->tail->next;
}
