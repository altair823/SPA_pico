//
// Created by 김태현 on 2022/02/16.
//

#ifndef SPA_PICO_PRIORITY_QUEUE_H
#define SPA_PICO_PRIORITY_QUEUE_H

#define DEFAULT_QUEUE_CAP 3
#define SET_DATA(key, value, index) keyData[index] = key; \
                                    valueData[index] = &value
#define SWAP(indexA, indexB)        auto tmpK = keyData[indexA]; \
                                    keyData[indexA] = keyData[indexB]; \
                                    keyData[indexB] = tmpK;     \
                                    auto tmpV = valueData[indexA];    \
                                    valueData[indexA] = valueData[indexB]; \
                                    valueData[indexB] = tmpV

template <typename Key, typename Value>
class PriorityQueue{
private:
    Key* keyData;
    Value** valueData;
    int size;
    int back;
public:
    PriorityQueue();
    ~PriorityQueue();
    void push(Key key, Value &value);
    void pop();
    Value* top();
};

template<typename Key, typename Value>
PriorityQueue<Key, Value>::PriorityQueue():
keyData(new Key[DEFAULT_QUEUE_CAP]),
valueData(new Value*[DEFAULT_QUEUE_CAP]),
size(DEFAULT_QUEUE_CAP),
back(0) {}

template<typename Key, typename Value>
PriorityQueue<Key, Value>::~PriorityQueue() {
    delete[] keyData;
    delete[] valueData;
}

template<typename Key, typename Value>
void PriorityQueue<Key, Value>::push(Key key, Value &value) {
    if (size == back) {
        size *= 2;

        auto tempKeyData = new Key[size];
        auto tempValueData = new Value *[size];
        for (int i = 0; i < back; i++) {
            tempKeyData[i] = keyData[i];
            tempValueData[i] = valueData[i];
        }
        delete[] keyData;
        delete[] valueData;
        keyData = tempKeyData;
        valueData = tempValueData;
    }

    SET_DATA(key, value, back);
    int parent = (back - 1) / 2;
    int child = back;
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
        while (!placed && child < back) {
            if (child < back - 1 && keyData[child] < keyData[child + 1])
                child += 1;
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
Value *PriorityQueue<Key, Value>::top() {
    if (back != 0){
        return valueData[0];
    } else {
        return nullptr;
    }
}

#endif //SPA_PICO_PRIORITY_QUEUE_H
