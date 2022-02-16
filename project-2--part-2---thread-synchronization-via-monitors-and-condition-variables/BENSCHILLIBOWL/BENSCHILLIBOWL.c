#include "BENSCHILLIBOWL.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

bool IsEmpty(BENSCHILLIBOWL* bcb);
bool IsFull(BENSCHILLIBOWL* bcb);
void AddOrderToBack(Order **orders, Order *order);

MenuItem BENSCHILLIBOWLMenu[] = { 
    "BensChilli", 
    "BensHalfSmoke", 
    "BensHotDog", 
    "BensChilliCheeseFries", 
    "BensShake",
    "BensHotCakes",
    "BensCake",
    "BensHamburger",
    "BensVeggieBurger",
    "BensOnionRings",
};
int BENSCHILLIBOWLMenuLength = 10;

/* Select a random item from the Menu and return it */
MenuItem PickRandomMenuItem() {
    int rand_num = ((rand() % BENSCHILLIBOWLMenuLength) + 1);
    return BENSCHILLIBOWLMenuLength[rand_num];
}

/* Allocate memory for the Restaurant, then create the mutex and condition variables needed to instantiate the Restaurant */

BENSCHILLIBOWL* OpenRestaurant(int max_size, int expected_num_orders) {
    BENSCHILLIBOWL *diner = (BENSCHILLIBOWL*) malloc(sizeof(BENSCHILLIBOWL));
    diner->orders = NULL;
    diner->current_size = 0;
    diner->max_size = max_size;
    diner->next_order_number = 1;
    diner->orders_handled = 0;
    diner->expected_num_orders = expected_num_orders;
    
    pthread_mutex_init(&(diner->mutex), NULL);
    pthread_cond_init(&(diner->can_get_orders), NULL);
    pthread_cond_init(&(diner->can_add_orders), NULL);

    printf("Restaurant is open!\n");

    //return NULL;

}


/* check that the number of orders received is equal to the number handled (ie.fullfilled). Remember to deallocate your resources */

void CloseRestaurant(BENSCHILLIBOWL* bcb) {
    if((bcb->current_size) == (bcb->expected_num_orders - bcb->orders_handled)){
        free(bcb);
        pthread_mutex_destroy(&(bcb->mutex));
        printf("Restaurant is closed!\n");
    }
    else{
        printf("All orders not filled, restaurant still open.\n");
    }
}

/* add an order to the back of queue */
int AddOrder(BENSCHILLIBOWL* bcb, Order* order) {
    pthread_mutex_lock(&(bcb->mutex));

    while(isFull(bcb) == true){
        pthread_cond_wait(&(bcb->can_add_orders), &(bcb->mutex));
    }

    order->order_number = bcb ->next_order_number;
    AddOrderToBack(&(bcb->orders), order);
    return order->order_number;
    return -1;
}

/* remove an order from the queue */
Order *GetOrder(BENSCHILLIBOWL* bcb) {
    pthread_mutex_lock(&(bcb->mutex));

    while(isEmpty(bcb) == true){
        if(bcb->orders_handled >= bcb->expected_num_orders){
            pthread_mutex_unlock(&(bcb->mutex));
            return NULL;
        }
        pthread_cond_wait(&(bcb->can_get_orders));
    }
    Order *order = bcb->orders;
    bcb->orders = bcb->orders->next;

    pthread_cond_broadcast(&(bcb->can_add_orders));
    pthread_mutex_unlock(&(bcb->mutex));

    bcb->current_size--;
    bcb->orders_handled++;
    return order;
}

// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(BENSCHILLIBOWL* bcb) {
    if(bcb->current_size > 0){
        return false;
    }
    else{
        return true;
    }
}

bool IsFull(BENSCHILLIBOWL* bcb) {
    if(bcb->current_size < bcb->max_size){
        return false;
    }
    else{
        return true;
    }
}

/* this methods adds order to rear of queue */
void AddOrderToBack(Order **orders, Order *order) {
    if(*orders == NULL){
        *orders = order;
    }
    else{
        Order *c;
        
        while(c->next != NULL){
            c = c->next;
        }
        c->next = order;
    }
}

