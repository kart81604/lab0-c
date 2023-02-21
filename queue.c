#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */
struct list_head *merge_two_list(struct list_head *L1, struct list_head *L2)
{
    struct list_head *head = NULL, **node, *tmp;
    int len_L1 = q_size(L1) + 1, len_L2 = q_size(L2) + 1;

    node = strcmp(list_entry(L1, element_t, list)->value,
                  list_entry(L2, element_t, list)->value) < 0
               ? &L1
               : &L2;
    strcmp(list_entry(L1, element_t, list)->value,
           list_entry(L2, element_t, list)->value) < 0
        ? len_L1--
        : len_L2--;
    head = *node;
    *node = (*node)->next;
    list_del_init(head);
    while (len_L1 > 0 && len_L2 > 0) {
        node = strcmp(list_entry(L1, element_t, list)->value,
                      list_entry(L2, element_t, list)->value) < 0
                   ? &L1
                   : &L2;
        strcmp(list_entry(L1, element_t, list)->value,
               list_entry(L2, element_t, list)->value) < 0
            ? len_L1--
            : len_L2--;
        *node = (*node)->next;
        tmp = (*node)->prev;
        list_del_init(tmp);
        list_add_tail(tmp, head);
    }
    if (len_L1 <= 0) {
        tmp = L2->prev;
        L2->prev->next = head;
        L2->prev = head->prev;
        head->prev->next = L2;
        head->prev = tmp;
    }
    if (len_L2 <= 0) {
        tmp = L1->prev;
        L1->prev->next = head;
        L1->prev = head->prev;
        head->prev->next = L1;
        head->prev = tmp;
    }
    return head;
}

struct list_head *merge_sort(struct list_head *start)
{
    if (list_empty(start) != 0)
        return start;
    struct list_head *forward = start, *back = start->prev, *tail = start->prev;
    do {
        forward = forward->next;
        back = back->prev;
    } while (forward != back && forward->prev != back);
    if (forward == back)
        forward = forward->next;
    start->prev = back;
    back->next = start;
    forward->prev = tail;
    tail->next = forward;
    return merge_two_list(merge_sort(start), merge_sort(forward));
}

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *li =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (!li)
        return NULL;
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
    if (!new || !new_s || !head)
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
    struct list_head *forward, *back;
    forward = head->next;
    back = head->prev;
    while (forward != back && forward != back->next) {
        forward = forward->next;
        back = back->prev;
    }
    element_t *cur = list_entry(forward, element_t, list);
    list_del_init(forward);
    q_release_element(cur);
    return true;
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head)
        return false;
    struct list_head *cur, *del;
    int count = 0;
    cur = head->next;
    while (cur != head) {
        struct list_head *test = cur->next;
        while (test != head &&
               strcmp(list_entry(cur, element_t, list)->value,
                      list_entry(test, element_t, list)->value) == 0) {
            count++;
            list_del_init(test);
            q_release_element(list_entry(test, element_t, list));
            test = cur->next;
        }
        cur = cur->next;
        if (count) {
            del = cur->prev;
            list_del_init(del);
            q_release_element(list_entry(del, element_t, list));
            count = 0;
        }
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
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) != 0)
        return;
    struct list_head *cur = head->next;
    while (cur != head) {
        cur = cur->next;
        list_move(cur->prev, head);
    }
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) != 0 || list_is_singular(head) != 0 || k < 2)
        return;
    int len = q_size(head);
    struct list_head *cur_head = head;
    struct list_head *cur = cur_head->next;
    while (len - k > -1) {
        for (int i = 0; i < k; i++) {
            cur = cur->next;
            list_move(cur->prev, cur_head);
        }
        cur_head = cur->prev;
        cur = cur_head->next;
        len = len - k;
    }
    return;
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) != 0)
        return;

    struct list_head *start = head->next;
    list_del_init(head);
    struct list_head *init = merge_sort(start);
    head->next = init;
    head->prev = init->prev;
    init->prev->next = head;
    init->prev = head;
    return;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head) != 0)
        return 0;
    struct list_head *cur = head->next->next, *del;
    while (cur != head) {
        while (cur->prev != head &&
               strcmp(list_entry(cur, element_t, list)->value,
                      list_entry(cur->prev, element_t, list)->value) >= 0) {
            del = cur->prev;
            list_del_init(del);
            q_release_element(list_entry(del, element_t, list));
        }
        cur = cur->next;
    }
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    if (!head || list_empty(head) != 0)
        return 0;
    struct list_head *ret_head, *cur_head = head->next->next, *sorted;
    ret_head = list_entry(head->next, queue_contex_t, chain)->q;
    sorted = ret_head->next;
    list_del_init(ret_head);
    while (cur_head != head) {
        struct list_head *first =
            list_entry(cur_head, queue_contex_t, chain)->q->next;
        list_del_init(first->prev);
        sorted = merge_two_list(sorted, first);
        cur_head = cur_head->next;
    }
    ret_head->next = sorted;
    ret_head->prev = sorted->prev;
    sorted->prev->next = ret_head;
    sorted->prev = ret_head;
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return q_size(ret_head);
}
