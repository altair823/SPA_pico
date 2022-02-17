/**
 * @file BucketQueue.h
 * @date 2022/02/17
 * @author altair823
 * @version 1.0
 * @brief Bucket queue implementation.
 */

#ifndef SPA_COMPARE_BUCKET_QUEUE_H
#define SPA_COMPARE_BUCKET_QUEUE_H

/**
 * @def DEFAULT_B_QUEUE_CAP
 * The initial default capacity of the bucket queue.
 */
#define DEFAULT_B_QUEUE_CAP 10

/**
 * @def DEFAULT_BUCKET_CAP
 * The initial default capacity of each bucket.
 */
#define DEFAULT_BUCKET_CAP 4

/**
 * Bucket queue class that sorts values using a key as an index.
 * @tparam Key Data type for key.
 * @tparam Value Data type for Value.
 * @tparam T Data type for index.
 */
template <class Key, class Value>
class BucketQueue{
private:
    Value** bucketList;
    unsigned char *bucketTop;
    unsigned char *bucketCap;
    int bucketListSize;

    // Store the index of bucket that has the smallest key which is popped before.
    int minIndex = 0;
public:

    /**
     * Constructor for bucket queue.
     */
    BucketQueue();

    /**
     * Destructor for bucket queue.
     */
    ~BucketQueue();

    /**
     * Push a new key, value pair to the queue.
     * @param key A key of new data.
     * @param value A value of new data.
     */
    void push(Key key, Value &value);

    /**
     * Delete the data at the top.
     */
    void pop();

    /**
     * Return the value of data at the top.
     * @return The value of data at the top.
     */
    Value top();
};

template<class Key, class Value>
BucketQueue<Key, Value>::BucketQueue():
bucketList(new Value* [DEFAULT_B_QUEUE_CAP]),
bucketTop(new unsigned char[DEFAULT_B_QUEUE_CAP]),
bucketCap(new unsigned char[DEFAULT_B_QUEUE_CAP]){
    for (int i = 0; i < DEFAULT_B_QUEUE_CAP; i++){
        bucketCap[i] = DEFAULT_BUCKET_CAP;
        bucketList[i] = new Value[bucketCap[i]];
        bucketTop[i] = 0;
    }
    bucketListSize = DEFAULT_B_QUEUE_CAP;
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
void BucketQueue<Key, Value>::push(Key key, Value &value) {
    int newIndex = key;
    if (newIndex < minIndex){
        minIndex = newIndex;
    }
    if (bucketListSize <= newIndex && bucketListSize >= DEFAULT_B_QUEUE_CAP){
        auto t_queue = new Value*[(newIndex+1)*2];
        auto t_top = new unsigned char[(newIndex+1)*2];
        auto t_cap = new unsigned char[(newIndex+1)*2];
        for (int i = 0; i < (newIndex+1)*2; i++) {
            if (i < bucketListSize) {
                t_queue[i] = new Value[bucketCap[i]];
                for (int j = 0; j < bucketTop[i]; j++) {
                    t_queue[i][j] = bucketList[i][j];
                }
                t_top[i] = bucketTop[i];
                t_cap[i] = bucketCap[i];
                delete[] bucketList[i];
            } else{
                t_queue[i] = new Value[DEFAULT_BUCKET_CAP];
                t_top[i] = 0;
                t_cap[i] = DEFAULT_BUCKET_CAP;
            }
        }
        delete[] bucketList;
        delete[] bucketTop;
        delete[] bucketCap;
        bucketList = t_queue;
        bucketListSize = (newIndex+1)*2;
        bucketTop = t_top;
        bucketCap = t_cap;
    }
    if (bucketTop[newIndex] >= bucketCap[newIndex]){
        if (bucketCap[newIndex] < DEFAULT_BUCKET_CAP){
            bucketCap[newIndex] = DEFAULT_BUCKET_CAP;
        } else {
            bucketCap[newIndex] *= 2;
        }
        auto t_bucket = new Value[bucketCap[newIndex]];
        for (int i = 0; i < bucketTop[newIndex]; i++){
            t_bucket[i] = bucketList[newIndex][i];
        }
        delete[] bucketList[newIndex];
        bucketList[newIndex] = t_bucket;
    }
    bucketList[newIndex][bucketTop[newIndex]++] = value;
}

template<class Key, class Value>
void BucketQueue<Key, Value>::pop() {
    for (int i = minIndex; i < bucketListSize; ++i) {
        if (bucketTop[i] > 0) {
            minIndex = i;
            break;
        }
    }
    bucketTop[minIndex] = bucketTop[minIndex] > 0 ? bucketTop[minIndex] - 1 : 0;
    if (bucketTop[minIndex] == 0){
        delete[] bucketList[minIndex];
        bucketList[minIndex] = new Value[DEFAULT_BUCKET_CAP];
        bucketCap[minIndex] = DEFAULT_BUCKET_CAP;
    }
}

template<class Key, class Value>
Value BucketQueue<Key, Value>::top() {
    for (int i = minIndex; i < bucketListSize; ++i) {
        if (bucketTop[i] > 0) {
            minIndex = i;
            break;
        }
    }
    return bucketList[minIndex][bucketTop[minIndex] - 1];
}

#endif //SPA_COMPARE_BUCKET_QUEUE_H
