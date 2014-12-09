/**
 * Neel Shah
 *
 * A test function for the timer wheel linked list
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int
main(void)
{
        struct linked_list *ll = ll_init();

        if (!ll) { printf("Error ll create\n"); return -1; }

        printf("list size = %d\n", ll->size);

        struct list_node *node1 = malloc(sizeof(struct list_node));
        node1->timerid = 3;
        node1->expiretime = 2;
        node1->tickoffset = 4;
        node1->next = NULL;

        printf("insert result = %d\n", ll_insert(ll, node1));

        struct list_node *node2 = malloc(sizeof(struct list_node));
        node2->timerid = 4;
        node2->expiretime = 7;
        node2->tickoffset = 6;
        node2->next = NULL;

        printf("insert result = %d\n", ll_insert(ll, node2));

        struct list_node *node3 = malloc(sizeof(struct list_node));
        node3->timerid = 6;
        node3->expiretime = 6;
        node3->tickoffset = 8;
        node3->next = NULL;

        printf("insert resuasdflt = %d\n", ll_insert(ll, node3));

        struct list_node *node4 = malloc(sizeof(struct list_node));
        node4->timerid = 67;
        node4->expiretime = 234;
        node4->tickoffset = 32;
        node4->next = NULL;

        printf("insert resulffffft = %d\n", ll_insert(ll, node4));

        struct list_node *node5 = malloc(sizeof(struct list_node));
        node5->timerid = 87;
        node5->expiretime = 1;
        node5->tickoffset = 3456;
        node5->next = NULL;

        printf("insert dick resuldddt = %d\n", ll_insert(ll, node5));

        struct list_node *temp;

        temp = ll->head;
        while (temp != NULL) {
                printf("%d\n", temp->expiretime);
                temp = temp->next;
        }

        struct list_node *rem = ll_remove_first(ll);

        if (rem == NULL)
                printf("failed\n");
        else
                printf("success\n");

        temp = ll->head;
        while (temp != NULL) {
                printf("%d\n", temp->expiretime);
                temp = temp->next;
        }
}
