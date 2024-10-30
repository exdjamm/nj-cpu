from random import randint
from sys import argv
from time import time

def up_down_update_1(heap, size, node):
    index = node
    l = index*2 + 1
    r = index*2 + 2

    while l < size:
        lesser = l

        if( r < size):
            if heap[r] < heap[l]:
                lesser = r
        else:
            if heap[l] < heap[index]:
                temp = heap[index]
                heap[index] = heap[l]
                heap[l] = temp
            break
        
        if heap[lesser] > heap[index]:
            break

        temp = heap[index]
        heap[index] = heap[lesser]
        heap[lesser] = temp

        index = lesser
        
        l = index*2 + 1
        r = index*2 + 2
        pass

    pass

def up_down_update_2(heap, size, node):
    index = node
    l = index*2 + 1
    r = index*2 + 2

    if( l >= size):
        return
    
    if(r >= size):
        if heap[l] < heap[index]:
            temp = heap[index]
            heap[index] = heap[l]
            heap[l] = temp
        return

    less = heap[r] < heap[index] or heap[l] < heap[index]

    while less:
        if( l >= size):
            break
        
        if(r >= size):
            if heap[l] < heap[index]:
                temp = heap[index]
                heap[index] = heap[l]
                heap[l] = temp
            break

        lesser = r

        if heap[r] > heap[l]:
            lesser = l

        temp = heap[index]
        heap[index] = heap[lesser]
        heap[lesser] = temp

        index = lesser
        
        l = index*2 + 1
        r = index*2 + 2

        if( l >= size):
            break
        
        if(r >= size):
            if heap[l] < heap[index]:
                temp = heap[index]
                heap[index] = heap[l]
                heap[l] = temp
            break

        less = heap[r] < heap[index] or heap[l] < heap[index]

    pass

def heapify(heap1, heap2, size):
    timeh1 = 0
    timeh2 = 0

    init = time()
    for i in range(int(size/2), -1, -1):
        up_down_update_1(heap1, size, i)
    timeh1 = time() - init

    init = time()
    for i in range(int(size/2), -1, -1):
        up_down_update_2(heap2, size, i)
    timeh2 = time() - init

    print(f"H1: {timeh1} H2: {timeh2}")

if __name__ == "__main__":
    N = int(argv[1])

    items = [ randint(0, 100) for i in range(N) ]
    heap_1 = []
    heap_2 = []

    for i in items:
        heap_1.append(i)
        heap_2.append(i)

    # print(len(items), len(heap_1), len(heap_2))

    heapify(heap_1, heap_2, N)
    # print(heap_1, heap_2)

    for i, j in zip(heap_1, heap_2):
        if i != j:
            print(f": ( or : )? {i} {j}")
            break