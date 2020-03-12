#include "queue.h"

void make_data(flame_queue& tq, const MyFrame &frame){
  tq.push(frame);
}
void get_data(flame_queue& tq, MyFrame& f1){
  tq.wait_and_pop(f1);
}

int main(){
  flame_queue q1;
  uint8_t d1[] = {1,2,3,4};
  MyFrame f1;
  f1.data = d1;
  f1.length = 4;

  uint8_t d2[] = {5,6,7,4};
  MyFrame f2;
  f2.data = d2;
  f2.length = 4;

  printf("q:%d\n", q1.empty());

  std::thread t1(make_data, std::ref(q1), f1);
  std::thread t2(make_data, std::ref(q1), f2);
  std::thread t3(get_data, std::ref(q1),std::ref(f2));
  t1.join();
  t2.join();
  t3.join();
  printf("%u %u\n", d1[0], d2[0]);

}
