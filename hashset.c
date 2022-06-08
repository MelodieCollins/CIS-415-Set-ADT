#include "hashset.h"  /* the .h file does NOT reside in /usr/local/include/ADTs */
#include <stdlib.h>
/* any other includes needed by your code */
#include "ADTs/iterator.h"
#include <stdio.h>

//#define UNUSED __attribute__((unused))

typedef struct s_data {
    /* definitions of the data members of self */
	void (*freeValue)(void *element);
	int (*cmpFxn)(void *first, void *second);
	long capacity;
	double loadFactor;
	long (*hashFxn)(void *, long);
	long size;
	void **array;
} SData;

/*
 * important - remove UNUSED attributed in signatures when you flesh out the
 * methods
 */


static void s_destroy(const Set *s) {
    /* implement the destroy() method */

    //free each element in s

    SData *sd = (SData *)s->self;
    for(long i = 0L; i < sd->capacity; i++) {
        if(sd->array[i] == NULL)
		continue;
	if(sd->freeValue != NULL)
	        sd->freeValue(sd->array[i]);
    }
    free(sd->array);
    free(sd);
    free((void *)s);
    return;
}

static void s_clear(const Set *s) {
    /* implement the clear() method */

    //set all value of array to NULL
    //free each element in s

    SData *sd = (SData *)s->self;
    for(long i = 0L; i < sd->capacity; i++) {
    	if(sd->array[i] == NULL)
		continue;
	if(sd->freeValue != NULL)
		sd->freeValue(sd->array[i]);
	sd->array[i] = NULL;
    }
    sd->size = 0;
    return;
}

static int add_helper(void **array, long len, void *member, long (*hashFxn)(void *, long), int (*cmpFxn)(void *first, void *second)) {

    long hash = hashFxn(member, len);
    if(array[hash] == NULL) {
	    array[hash] = member;
	    return 1;
    }
    if(cmpFxn(array[hash], member) == 0) {
	    return 0;
    }
    for(long i = 0L + i; i < len + i; i++){
	    
	    long index = i%(len);
	    if(array[index] == NULL) {
		    array[index] = member;
		    return 1;
	    }
    	    if(cmpFxn(array[index], member) == 0) {
	        return 0;
    	    }

    }
    return 0;
}

static void **create_empty(long size) {

    	    void **arr = (void **)malloc(size * sizeof(void *));
            if(arr == NULL)
	        return NULL;
	    for(long i = 0L; i < size; i++)
		    arr[i] = NULL;
	    return arr;
}

static int s_add(const Set *s, void *member) {
    /* implement the add() method */

    //checks size not bigger than capacity * loadFactor, if bigger, capacity *= 2
    //Adds value to set if not already present
    //returns 1 if value added, sd->size++
    //returns 0 if not added or value was already present

    SData *sd = (SData *)s->self;

    // If array is too full, create a new, bigger  one and re-add members  into it
    if(((sd->size)/(sd->capacity)) > sd->loadFactor) { 
	    long newCap = 2*sd->capacity;
    	    void **arr = create_empty(newCap);
            if(arr == NULL)
	        return 0;
    	    void **old = sd->array;
	    for(int i = 0; i < sd->capacity; i++) {
	        if(sd->array[i] == NULL)
		    continue;
    		long result = add_helper(arr, newCap, sd->array[i], sd->hashFxn, sd->cmpFxn);
		if(result == 0)
			return 0;
            }
	    sd->array = arr;
	    sd->capacity = newCap;
	    free(old);
    }

    long result = add_helper(sd->array, sd->capacity, member, sd->hashFxn, sd->cmpFxn);
    if (result == 1)
	    sd->size++;
    return result;
}

static int s_contains(const Set *s, void *member) {
    /* implement the contains() method */

    //return 1 if value contained in set
    //return 0 if not in set
   
    SData *sd = (SData *)s->self;
    if(sd->size == 0) {
	    return 0;
    }
    long first = sd->hashFxn(member, sd->capacity);
    for(long i = 0 + first; i < sd->capacity + first; i++){
	    long index = i%(sd->capacity);
	    if(sd->array[index] == NULL) {
		    continue;
	    }
	    if(sd->cmpFxn(sd->array[index],member) == 0)
		    return 1;

    }
    return 0;
}

static int s_isEmpty(const Set *s) {
    /* implement the isEmpty() method */

    //return 1 if s is empty
    //return 0 if not empty

    SData *sd = (SData *)s->self;
    return sd->size == 0;
}

static int s_remove(const Set *s, void *member) {
    /* implement the remove() method */

    //remove value from set if present
    //return 1 if present and removed, sd->size--
    //return 0 if not present
    //need to change capacity?
    
    SData *sd = (SData *)s->self;
    if(sd->size == 0)
	    return 0;

    long first = sd->hashFxn(member, sd->capacity);
    for(long i = 0 + first; i < sd->capacity + first; i++){
	    long index = i%(sd->capacity);
	    if(sd->array[index] == NULL) {
		    continue;
	    }
	    if(sd->cmpFxn(sd->array[index],member) == 0){
		    if(sd->freeValue != NULL) {
			    sd->freeValue(sd->array[index]);
		    }
		    sd->array[index] = NULL;
	    	    sd->size--;
		    return 1;
	    }
    }
    return 0;
}

static long s_size(const Set *s) {
    /* implement the size() method */

    //return number of elements in the set

    SData *sd = (SData *)s->self;
    return sd->size;
}

static void **s_toArray(const Set *s, long *len) {
    /* implement the toArray() method */

    //return elements of set as array of void* pointers of arbitrary order
    //return pointer to array
    //return NULL if error
    //return number of elements in array in *len 

    SData *sd = (SData *)s->self;
    long num = 0L;
    void **arr = (void **)malloc(sd->size * sizeof(void *));
    if(arr == NULL)
	    return NULL;
    for(int i = 0; i < sd->capacity; i++) {
	    if(sd->array[i] == NULL)
		continue;
	    arr[num] = sd->array[i];
	    num++;
    }
    *len = num;
    return arr;
}

static const Iterator *s_itCreate(const Set *s) {
    /* implement the itCreate() method */
    
    //create generic iterator to s
    //return pointer to iterator instance
    //return NULL if error
    long len;
    void **a = s->toArray(s, &len);
    return Iterator_create(len, a);
}

static Set template = {
    NULL, s_destroy, s_clear, s_add, s_contains, s_isEmpty, s_remove,
    s_size, s_toArray, s_itCreate
};

const Set *HashSet(void (*freeValue)(void*), int (*cmpFxn)(void*, void*),
                   long capacity, double loadFactor,
                   long (*hashFxn)(void *m, long N)
                  ) {
    /* construct a Set instance and return to the caller */
    
    Set *s = (Set *)malloc(sizeof(Set));
    if(s == NULL) {
	    return NULL;
    }
    SData *sd = (SData *)malloc(sizeof(SData));
    if(sd == NULL){
	    free(s);
	    return NULL;
    }
    double load = (loadFactor <= 0.0) ? DEFAULT_LOAD_FACTOR : loadFactor;
    long cap = (capacity <= 0L) ? DEFAULT_SET_CAPACITY : capacity;
    void **array = create_empty(cap);
    if(array == NULL) {
	    free(sd);
	    free(s);
	    return NULL;
    }

    sd->freeValue = freeValue;
    sd->cmpFxn = cmpFxn;
    sd->capacity = cap;
    sd->loadFactor = load;
    sd->hashFxn = hashFxn;
    sd->size = 0L;
    sd->array = array;
    *s = template;
    s->self = sd;
    return s;
}

