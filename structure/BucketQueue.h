//
// Created by 김태현 on 2021/07/27.
//

#ifndef SPA_COMPARE_BUCKETQUEUE_H
#define SPA_COMPARE_BUCKETQUEUE_H
#include<cstdlib>
template <class Key, class Value>
class BucketQueue{
private:
    /*
     * Theoretically, the maximum distance from the starting point to the location
     * is the Manhattan distance of the edge with maximum weight.
     */
    Value*** bucketList;
    int* bucketTop;
    int bucketListSize;

    // Store the index of bucket that has smallest key which is popped before.
    int minIndex = 0;
public:
    BucketQueue(int maxRow, int maxCol, int weightMean);
    ~BucketQueue();
    void Insert(Key key, Value *value);
    Value* PopMinimum();
    Key getMinimumKey();

};

template<class Key, class Value>
BucketQueue<Key, Value>::BucketQueue(int maxRow, int maxCol, int weightMean) {
    bucketList = (Value***)calloc((maxRow+maxCol)*(weightMean*2), sizeof( Value**));
    bucketTop = new int[(maxRow + maxCol) * (weightMean * 2)];
    for (int i = 0; i < (maxRow+maxCol)*(weightMean*2); i++){
        bucketList[i] = nullptr;
        bucketTop[i] = 0;
    }
    bucketListSize = (maxRow+maxCol)*(weightMean*2);
}

template<class Key, class Value>
BucketQueue<Key, Value>::~BucketQueue() {
    for (int i = 0; i < bucketListSize; i++){
        if (bucketTop[i] > 0){
            free(bucketList[i]);
        }
    }
    free(bucketList);
    delete bucketTop;
}

template<class Key, class Value>
void BucketQueue<Key, Value>::Insert(Key key, Value *value) {
    int newIndex = key;
    if (newIndex < minIndex){
        minIndex = newIndex;
    }
    if (bucketList[newIndex] == nullptr){
        bucketList[newIndex] = (Value**) malloc((sizeof (Value*))*1);
        bucketTop[newIndex] = 1;
    }
    else{
        bucketList[newIndex] = (Value**) realloc(bucketList[newIndex], (sizeof (Value*)) * (++bucketTop[newIndex]));
    }
    bucketList[newIndex][bucketTop[newIndex]-1] = value;
}

template<class Key, class Value>
Value* BucketQueue<Key, Value>::PopMinimum() {
    Value* v = bucketList[minIndex][bucketTop[minIndex]-1];
    if (bucketTop[minIndex] == 1){
        free(bucketList[minIndex]);
        bucketList[minIndex] = nullptr;
        bucketTop[minIndex]--;
    } else{
        bucketList[minIndex] = (Value**) realloc(bucketList[minIndex], (sizeof (Value*)) * (--bucketTop[minIndex]));
    }
    return v;
}


template<class Key, class Value>
Key BucketQueue<Key, Value>::getMinimumKey() {
    for (int i = minIndex; i < bucketListSize; ++i) {
        if (bucketTop[i] > 0){
            minIndex = i;
            return i;
        }
    }
}



#endif //SPA_COMPARE_BUCKETQUEUE_H
