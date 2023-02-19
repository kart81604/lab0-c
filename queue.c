#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *li =
        (struct list_head *) malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(li);
    return li;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, l, list) {
        q_release_element(entry);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new = (element_t *) malloc(sizeof(element_t));
    char *new_s = (char *) malloc((strlen(s) + 1) * sizeof(char));
    if (!new || !new_s)
        return false;
    memcpy(new_s, s, strlen(s) + 1);
    new->value = new_s;
    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new = (element_t *) malloc(sizeof(element_t));
    char *new_s = (char *) malloc((strlen(s) + 1) * sizeof(char));
    if (!new || !new_s)
        return false;
    memcpy(new_s, s, strlen(s) + 1);
    new->value = new_s;
    list_add_tail(&new->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *ele = list_entry(head->next, element_t, list);
    list_del_init(&ele->list);
    if (sp != NULL) {
        strncpy(sp, ele->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *ele = list_entry(head->prev, element_t, list);
    list_del_init(&ele->list);
    if (sp != NULL) {
        strncpy(sp, ele->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head)
        return false;
    struct list_head **forward, **back;
    forward = &head->next;
    back = &head->prev;
    while (*forward != *back && *forward != (*back)->next) {
        forward = &(*forward)->next;
        back = &(*back)->prev;
    }
    element_t *cur = list_entry(*forward, element_t, list);
    list_del_init(*forward);
    q_release_element(cur);
    return true;
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head)
        return false;
    struct list_head **cur, **test;
    cur = &head->next;
    while (cur != &head) {
        test = &(*cur)->next;
        while (test != &head &&
               !strcmp(list_entry(*cur, element_t, list)->value,
                       list_entry(*test, element_t, list)->value)) {
            list_del_init(*test);
            q_release_element(list_entry(*test, element_t, list));
            test = &(*cur)->next;
        }
        cur = test;
    }
    return true;
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *cur;
    cur = head->next;
    while (cur != head && cur->next != head) {
        struct list_head *tmp = cur->next;
        list_del(cur);
        cur->next = tmp->next;
        cur->prev = tmp;
        tmp->next->prev = cur;
        tmp->next = cur;
        cur = cur->next;
    }
    return;
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
