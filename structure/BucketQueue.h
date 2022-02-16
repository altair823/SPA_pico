//
// Created by 김태현 on 2021/07/27.
//

#ifndef SPA_COMPARE_BUCKET_QUEUE_H
#define SPA_COMPARE_BUCKET_QUEUE_H

#define DEFAULT_CAP 4

template <class Key, class Value>
class BucketQueue{
private:
    /*
     * Theoretically, the maximum distance from the starting point to the location
     * is the Manhattan distance of the edge with maximum weight.
     */
    Value*** bucketList;
    unsigned char* bucketTop;
    unsigned char* bucketCap;
    int bucketListSize;

    // Store the index of bucket that has the smallest key which is popped before.
    int minIndex = 0;
public:
    BucketQueue(int maxRow, int maxCol, int weightMean);
    ~BucketQueue();
    void Insert(Key key, Value &value);
    Value* PopMinimum();
};

template<class Key, class Value>
BucketQueue<Key, Value>::BucketQueue(int maxRow, int maxCol, int weightMean) {
    bucketList = new Value**[(maxRow+maxCol)*(weightMean*2)];
    bucketTop = new unsigned char[(maxRow + maxCol) * (weightMean * 2)];
    bucketCap = new unsigned char[(maxRow + maxCol) * (weightMean * 2)];
    for (int i = 0; i < (maxRow+maxCol)*(weightMean*2); i++){
        bucketList[i] = new Value*[DEFAULT_CAP];
        bucketTop[i] = 0;
        bucketCap[i] = DEFAULT_CAP;
    }
    bucketListSize = (maxRow+maxCol)*(weightMean*2);
}

template<class Key, class Value>
BucketQueue<Key, Value>::~BucketQueue() {
    for (int i = 0; i < bucketListSize; i++){
        if (bucketCap[i] > 0){
            delete[] bucketList[i];
        }
    }
    delete[] bucketList;
    delete[] bucketTop;
    delete[] bucketCap;
}

template<class Key, class Value>
void BucketQueue<Key, Value>::Insert(Key key, Value &value) {
    int newIndex = key;
    if (newIndex < minIndex){
        minIndex = newIndex;
    }

    if (bucketTop[newIndex] >= bucketCap[newIndex]){
        if (bucketCap[newIndex] < DEFAULT_CAP){
            bucketCap[newIndex] = DEFAULT_CAP;
        } else {
            bucketCap[newIndex] *= 2;
        }
        auto t_bucket = new Value*[bucketCap[newIndex]];
        for (int i = 0; i < bucketTop[newIndex]; i++){
            t_bucket[i] = bucketList[newIndex][i];
        }
        delete[] bucketList[newIndex];
        bucketList[newIndex] = t_bucket;
    }
    bucketList[newIndex][bucketTop[newIndex]++] = &value;
}

template<class Key, class Value>
Value* BucketQueue<Key, Value>::PopMinimum() {
    for (int i = minIndex; i < bucketListSize; ++i) {
        if (bucketTop[i] > 0) {
            minIndex = i;
            break;
        }
    }
    Value *v = bucketList[minIndex][bucketTop[minIndex] - 1];
    bucketTop[minIndex] = bucketTop[minIndex] > 0 ? bucketTop[minIndex] - 1 : 0;
    if (bucketTop[minIndex] == 0){
        delete[] bucketList[minIndex];
        bucketList[minIndex] = new Value*[DEFAULT_CAP];
        bucketCap[minIndex] = DEFAULT_CAP;
    }
    return v;
}

#endif //SPA_COMPARE_BUCKET_QUEUE_H
