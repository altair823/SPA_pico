//
// Created by 김태현 on 2022/02/13.
//

#ifndef SPA_PICO_COUNTER_H
#define SPA_PICO_COUNTER_H

#include <vector>
#include <memory>

void updateCounter(std::vector<char>& counter){
    for (int i = 0; i < counter.size(); i++){
        if (counter[i] >= 10){
            counter[i] = i%10;
            if (counter.size() <= i+1){
                counter.push_back(1);
            } else{
                counter[i+1]++;
            }
        }
    }
}

void increaseCounter(std::vector<char>& counter){
    if (++counter[0] >= 10){
        updateCounter(counter);
    }
}

std::unique_ptr<char[]> counterToString(std::vector<char>& counter){
    auto str  = std::make_unique<char[]>(counter.size()+1);
    for (auto i = (long) counter.size() - 1; i >= 0 ; i--){
        str[long(counter.size() - i - 1)] = (char) (counter[i] + 48);
    }
    str[counter.size()] = '\0';
    return str;
}


#endif //SPA_PICO_COUNTER_H
