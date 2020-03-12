#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 100

struct MyFrame{
    uint8_t* data;
    size_t length;
    int width;
    int height;
    int format;
};

class flame_queue{
private:
    mutable std::mutex mut;
    std::queue<MyFrame> data_queue;
    std::condition_variable data_cond;
public:
    flame_queue(){}
    void push(const MyFrame &frame){
//        printf("    %d data:%u, length:%zu\n", __LINE__, frame.data[0], frame.length);
        if(frame.data == nullptr || frame.length == 0){
            return;
        }
        std::lock_guard<std::mutex> lk(mut);
        if( data_queue.size() >= MAX_SIZE ) {
            printf("size:%zu\n", data_queue.size());
            MyFrame& top_frame = data_queue.front();
            if( top_frame.data != NULL ){
                delete top_frame.data;
            }
            data_queue.pop();
        }
//        printf("%d data:%u, length:%zu\n", __LINE__, frame.data[0], frame.length);
        MyFrame store_frame;
        store_frame.data = new uint8_t[frame.length]{0};
//        printf("%d data:%u, length:%zu\n", __LINE__, frame.data[0], frame.length);
        memcpy(store_frame.data, frame.data, frame.length );
        store_frame.length = frame.length;
        store_frame.width = frame.width;
        store_frame.height = frame.height;
        store_frame.format = frame.format;
        data_queue.push(store_frame);
        data_cond.notify_one();
    }

    void wait_and_pop(MyFrame& frame){
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]{return !data_queue.empty();});
        const MyFrame store_frame = data_queue.front();
        if( store_frame.data != nullptr ){
            memcpy(frame.data, store_frame.data, store_frame.length);
            frame.length = store_frame.length;
            frame.width = store_frame.width;
            frame.height = store_frame.height;
            frame.format = store_frame.format;
            delete store_frame.data;
        }
        data_queue.pop();
    }

    bool empty()const{
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};
