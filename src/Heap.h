/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */

#ifndef  _HEAP_H_
#define  _HEAP_H_


#include "TinyDefine.h"

namespace Tiny {

    /*tiny_forceinline*/ void  upHeap(HeapVec& heap, int k)
    {
        TimerHeap*  he  = heap[k];

        for ( ; ; ) {
            int p = k >> 1;
            if (!p || heap[p]->getAt() <= he->getAt())
				break;

            heap[k] = heap[p];
            heap[k]->setActive(k);

            k = p;
        }

        heap[k] = he;
        he->setActive(k);
    }

    /*tiny_forceinline*/  void downHeap(HeapVec& heap, int N, int k)
    {
        TimerHeap* he = heap[k];
        for ( ; ; ) {
            int c = k << 1;

            if (c >= N + 1)
				break;
            c += c + 1 <  N + 1 && heap[c]->getAt() > heap[c+1]->getAt() ? 1 : 0;

            if (he->getAt() < heap[c]->getAt())
				break;

            heap[k] = heap [c];
            heap[k]->setActive(k);

            k =  c;
        }

        heap[k] = he;
        he->setActive(k);
    }

    tiny_forceinline void adjustHeap(HeapVec& heap, int N, int k)
    {
        if (k > 1 && heap[k]->getActive() <= heap[k >> 1]->getActive()) {
            upHeap(heap, k);
        } else {
            downHeap(heap, N, k);
        }
    }

    tiny_forceinline void reHeap(HeapVec& heap, int N)
    {
        for (int i = 0; i < N; ++i) {
            upHeap(heap, i + 1);
        }

    }

};

#endif //_HEAP_H_










