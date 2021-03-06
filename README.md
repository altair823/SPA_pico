# SPA comparing project for raspberry pi pico

라즈베리 파이 피코를 위한 SPA compare 프로젝트입니다.

[SPA_compare](https://github.com/altair823/SPA_compare)에 관해서는 링크를 참조하세요.



## Introduction

SPA_compare 프로젝트는 무작위 미로를 생성하고 여러 알고리즘이 미로의 최단거리를 찾는 시간을 비교하는 프로젝트입니다. SPA_pico 프로젝트는 이를 라즈베리 파이 피코에서 실행하는 프로젝트입니다. 

리소스가 부족한 마이크로 컨트롤러를 위한 프로젝트이기 때문에 C++를 사용하여 이전보다 효율적으로 코드를 작성하였습니다. 

보다 자세한 설명은 [Documentation](https://github.com/altair823/SPA_pico/raw/master/Documentation.pdf)을 참조하세요.  



## 구성

프로그램은 다음과 같은 절차를 실행합니다.

1.  주어진 크기의 미로를 생성합니다. 

2. 여러 알고리즘으로 출발점과 도착점까지의 최단거리를 탐색합니다. 

3. 각 알고리즘이 걸린 시간을 출력합니다. 

4. 1번부터 다시 시작합니다. 

실행하는 알고리즘은 다음과 같습니다. 

1. 순수한 다익스트라 알고리즘

2. 우선순위 큐를 사용하는 A* 알고리즘

3. 버킷 큐를 사용하는 A* 알고리즘



## 최적화

나름 객체지향을 충실히 구현한 이전 프로젝트와 달리, 리소스가 매우 부족한 마이크로 컨트롤러에서 구동되는 만큼 많은 최적화가 이루어졌습니다. 이를 통해 생성할 수 있는 미로의 최대 크기를 20x20 에서 125x125 까지 증가시킬 수 있었습니다. 

- 전체
  
  - 미로의 가로 세로 크기의 자료형을 따로 설정할 수 있습니다. 125x125 크기의 경우 일반적인 int 보다 unsigned char를 사용하여 더 적은 공간을 사용할 수 있습니다. 
  
  - 가중치의 자료형을 설정할 수 있습니다. 가중치의 최대값이 충분히 작다면 그만큼 공간을 아낄 수 있습니다. 
  
  - 인접한 칸을 가리키던 포인터를 삭제했습니다. 배열 인덱스 또는 함수를 사용하여 인접 칸에 접근해야 합니다. 
  
  - 작지만 매우 자주 호출되는 함수들을 매크로로 변경하였습니다. 
  
  - vector, set 과 같은 STL 컨테이너들을 동적으로 할당한 1 또는 2차원 배열로 바꾸었습니다. 
  
  - 최단 경로를 저장하지 않습니다. 단지 최단 경로의 거리와 탐색 시간만을 저장합니다. 

- 순수한 다익스트라 알고리즘
  
  - 방문했던 위치인지 확인하는 방법을 vector 탐색에서 2차원 bool 테이블 접근으로 변경했습니다. 

- 우선순위 큐를 사용하는 A* 알고리즘
  
  - <queue>의 priority_queue 를 사용하던 것을 완전 이진 힙 트리로 직접 구현하여 시간을 단축했습니다. 
  
  - 힙 트리에서 사용하는 배열의 크기를 동적으로 할당하여 공간을 절약하였습니다. 

- 버킷 큐를 사용하는 A* 알고리즘
  
  - 가중치의 이론적 최대값으로 큐의 크기를 초기화하던 것에서, 동적으로 큐의 크기를 할당하도록 하였습니다. 수행시간이 증가하였지만 공간복잡도가 크게 줄어들었습니다. 



## 기타 변경 사항

- [Doxygen]([Doxygen: Doxygen](https://www.doxygen.nl/index.html)) 을 사용하여 문서화할 수 있도록 하였습니다. 

- 
