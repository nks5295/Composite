/**
 * Neel Shah
 *
 * LinkedList to hold timer information
 *
 * Ordered by earliest deadline first based on expiration time
 *
 * Structs contain:
 *      - unsigned int timerid
 *      - unsigned int expirtime (current time + ticks)
 *      - unsigned int tickoffset
 *      - struct list_node next
 *
 * Functions:
 *      - init          (initializes a list)
 *      - insert        (inserts a value into the list following edf order)
 *      - remove        (removes first value off of list)
 *      - size          (returns the size of the list)
 */

#include <stdio.h>
#include <stdlib.h>

struct list_node {
        unsigned int timerid;       // id of timer that this belongs to
        unsigned int expiretime;    // current time + ticks
        unsigned int tickoffset;    // ticks
        struct list_node *next;
};


struct linked_list {
        struct list_node *head;
        unsigned int size;
};

struct linked_list*
ll_init(void)
{
        struct linked_list *ll = malloc( sizeof(struct linked_list) );

        if (ll == NULL) return NULL;

        ll->head = NULL;
        ll->size = 0;

        return ll;
}

int
ll_destroy(struct linked_list *ll)
{
        if (ll == NULL) return -1;

        if (ll->head == NULL){
                free(ll);
                return 0;
        } else {
                while (ll->head != NULL){
                        struct list_node *temp;
                        temp = ll->head;
                        ll->head = temp->next;
                        free(temp);
                }
                free(ll);
                return 0;
        }
        return -1;
}

int
ll_insert(struct linked_list *ll, struct list_node *n)
{
        if (ll == NULL || n == NULL) return -1;

        struct list_node *curr, *prev;

        curr = ll->head;
        prev = NULL;

        while (curr != NULL && n->expiretime > curr->expiretime) {
                prev = curr;
                curr = curr->next;
        }

        if (prev == NULL) {
                n->next = ll->head;
                ll->head = n;
                ll->size++;
        } else {
                prev->next = n;
                n->next = curr;
                ll->size++;
        }
        return 0;
}

int
ll_size(struct linked_list *ll)
{
        if (ll == NULL) return -1;

        return ll->size;
}

unsigned int
ll_remove_first(struct linked_list *ll)
{
        if (ll == NULL || ll->size == 0) return -1;

        unsigned int timerid_ret;
        struct list_node *temp = ll->head;

        ll->head = ll->head->next;

        timerid_ret = temp->timerid;

        free(temp);

        return timerid_ret;
}

struct list_node*
ll_remove_timerid(struct linked_list *ll, unsigned int tid)
{
        if (ll == NULL || tid < 0 || ll->size == 0) return NULL;

        struct list_node *curr, *prev;
        curr = ll->head;
        prev = NULL;

        while (curr != NULL) {
                if (curr->timerid == tid) {
                        if (prev == NULL) {
                                ll->head = curr->next;
                                curr->next = NULL;
                                return curr;
                        } else {
                                prev->next = curr->next;
                                curr->next = NULL;
                                return curr;
                        }
                } else {
                        prev = curr;
                        curr = curr->next;
                }
        }

        return NULL;
}
