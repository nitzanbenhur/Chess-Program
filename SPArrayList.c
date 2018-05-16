//
// Created by itai on 16/05/17.
//


#include <stdlib.h>
#include <stdio.h>
#include "SPArrayList.h"
#define UNDEFINED -1
#define NNul(x) if(x == NULL) {return UNDEFINED;}
SPArrayList* spArrayListCreate(){
    SPArrayList* array;
    array = malloc(sizeof(SPArrayList));
    if(array != NULL){
        array->maxSize = MAX;
        array->actualSize = 0;
    }else{
        printf("Error: malloc has failed");
        return NULL;
    }
    return array;
}

SPArrayList* spArrayListCopy(SPArrayList* src){
    if(src == NULL){
        return NULL;
    }

    SPArrayList* copy;
    copy = spArrayListCreate();
    if (copy != NULL){
        copy->actualSize = src->actualSize;
	int i;
        for(i=0;i<src->maxSize;i++){
            copy->elements[i] = src->elements[i];
        }
    }
    return copy;
}

void spArrayListDestroy(SPArrayList* src){
    if(src != NULL) {
        free(src);
        src=NULL;
    }
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
    if(src == NULL){
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }

    src->actualSize = 0;
    return SP_ARRAY_LIST_SUCCESS;
}


SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, SPCommand elem, int index){
    if(src == NULL || index < 0 || index > src->actualSize){
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if(src->actualSize == src->maxSize){
        return SP_ARRAY_LIST_FULL;
    }
    int i;
    for(i=src->actualSize;i>=index;i--){
        if(i!=index) {
            src->elements[i] = src->elements[i - 1];
        }else{
            src->elements[i] = elem;
        }
    }
    src->actualSize++;
    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, SPCommand elem){
    if(src->actualSize == src->maxSize){
        spArrayListRemoveLast(src);
    }
    return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, SPCommand elem){
    return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
    if(src == NULL || index < 0 || index >= src->actualSize){
        return SP_ARRAY_LIST_INVALID_ARGUMENT;
    }
    if(src->actualSize == 0){
        return SP_ARRAY_LIST_EMPTY;
    }
    int i;
    for(i=index;i<src->actualSize-1;i++){
        src->elements[i] = src->elements[i + 1];
    }
    src->actualSize--;
    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
    return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
    return spArrayListRemoveAt(src, src->actualSize-1);
}

SPCommand spArrayListGetAt(SPArrayList* src, int index){
    if(src == NULL || index <0 || index >= src->actualSize){
        SPCommand a;
        a.cmd=SP_INVALID_LINE;
        return a;
    }
    return src->elements[index];
}

SPCommand spArrayListGetFirst(SPArrayList* src){
    return spArrayListGetAt(src, 0);
}
SPCommand spArrayListGetLast(SPArrayList* src){
    return spArrayListGetAt(src, src->actualSize-1);
}
int spArrayListMaxCapacity(SPArrayList* src){
    NNul(src);
    return src->maxSize;
}
int spArrayListSize(SPArrayList* src){
    NNul(src);
    return src->actualSize;
}
bool spArrayListIsEmpty(SPArrayList* src){
    NNul(src);
    return src->actualSize == 0;
}
bool spArrayListIsFull(SPArrayList* src){
    NNul(src);
    return src->actualSize == src->maxSize;
}

