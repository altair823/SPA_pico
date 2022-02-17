/**
 * @file PriorityQueue.h
 * @date 2022/02/17
 * @author altair823
 * @version 1.0
 * @brief Priority queue implementation using complete binary heap tree.
 */

#ifndef SPA_PICO_PRIORITY_QUEUE_H
#define SPA_PICO_PRIORITY_QUEUE_H

/**
 * @def DEFAULT_P_QUEUE_CAP
 * The initial default capacity of data array in the queue.
 */
#define DEFAULT_P_QUEUE_CAP 3

/**
 * @def SET_DATA(key, value, index)
 * Set a key-value data to the array element in given index.
 */
#define SET_DATA(key, value, index) keyData[index] = key; \
                                    valueData[index] = value

/**
 * @def SWAP(indexA, indexB)
 * Swap the two data at index A and B.
 */
#define SWAP(indexA, indexB)        auto tmpK = keyData[indexA]; \
                                    keyData[indexA] = keyData[indexB]; \
                                    keyData[indexB] = tmpK;     \
                                    auto tmpV = valueData[indexA];    \
                                    valueData[indexA] = valueData[indexB]; \
                                    valueData[indexB] = tmpV

/**
 * Priority Queue class that is implemented with complete binary heap tree.
 * Values in the queue is sorted using keys.
 * @tparam Key Data type for key.
 * @tparam Value Data type for Value.
 */
template <typename Key, typename Value>
class PriorityQueue{
private:

    /**
     * Data array that contains key of data.
     */
    Key* keyData;

    /**
     * Data array that contains value of data.
     */
    Value* valueData;

    /**
     * The size of arrays in the queue.
     */
    int capacity;

    /**
     * Indicates the index of the data at the end of the array.
     */
    int back;
public:
    /**
     * Constructor a new priority queue.
     */
    PriorityQueue();

    /**
     * Destructor a existing priority queue.
     */
    ~PriorityQueue();

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

template<typename Key, typename Value>
PriorityQueue<Key, Value>::PriorityQueue():
keyData(new Key[DEFAULT_P_QUEUE_CAP]),
valueData(new Value[DEFAULT_P_QUEUE_CAP]),
capacity(DEFAULT_P_QUEUE_CAP),
back(0) {}

template<typename Key, typename Value>
PriorityQueue<Key, Value>::~PriorityQueue() {
    delete[] keyData;
    delete[] valueData;
}

template<typename Key, typename Value>
void PriorityQueue<Key, Value>::push(Key key, Value &value) {
    /* When the data array in the queue full, double the capacity of both arrays. */
    if (capacity == back) {
        capacity *= 2;

        auto tempKeyData = new Key[capacity];
        auto tempValueData = new Value [capacity];
        for (int i = 0; i < back; i++) {
            tempKeyData[i] = keyData[i];
            tempValueData[i] = valueData[i];
        }
        delete[] keyData;
        delete[] valueData;
        keyData = tempKeyData;
        valueData = tempValueData;
    }

    /* Appends new data to the end of the array. */
    SET_DATA(key, value, back);
    int parent = (back - 1) / 2;
    int child = back;

    /* Heapify all nodes. */
    while (parent >= 0 && keyData[parent] < keyData[child]) {
        SWAP(parent, child);
        child = parent;
        parent = (child - 1) / 2;
    }
    back++;
}

template<typename Key, typename Value>
void PriorityQueue<Key, Value>::pop() {
    if (back > 0) {
        back--;
        keyData[0] = keyData[back];
        valueData[0] = valueData[back];
        int parent = 0;
        int child = parent * 2 + 1;
        bool placed = false;

        /* Heapify all nodes. */
        while (!placed && child < back) {
            if (child < back - 1 && keyData[child] < keyData[child + 1])
                child += 1;
            /* Heapify complete. */
            if (keyData[parent] >= keyData[child])
                placed = true;
            else {
                SWAP(parent, child);
            }
            parent = child;
            child = parent * 2 + 1;
        }
    }
}

template<typename Key, typename Value>
Value PriorityQueue<Key, Value>::top() {
    if (back != 0){
        return valueData[0];
    } else {
        return nullptr;
    }
}

#endif //SPA_PICO_PRIORITY_QUEUE_H
