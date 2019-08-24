#ifndef __PROTOCOLUTIL_HPP__
#define __PROTOCOLUTIL_HPP__


#include<string>
#include<string.h>
#include<iostream>
#include<assert.h>

typedef long long type;


struct weight{
    unsigned char _ch;
    type _count;
    std::string _code;
    weight()
        :_ch(0),
        _count(0),
        _code("")
    {}
};



struct Node{
    weight _weight;
    int _left;
    int _right;
    int _parent;
    Node()
        :_left(0),
        _right(0),
        _parent(0)
    {}
};



class Heap{
  public:
    int* heap;
    int size;
    const Node* _root;
    Heap(const Node* _Root,int num)
    {
        _root = _Root;
        heap = new int[num + 1];
        int i = 0;
        for(i = 1; i <= num; ++i)
        {
            heap[i] = i;
        }
        size = num;
        
        int start = size/2;
        while(start > 0)
        {
            HeapAdjust(start);
            start--;
        }

    }


   void PopNode(int& one)
   {
      one = heap[1];
      swap(1,size);
      size--;
      HeapAdjust(1);
   }



   void PushNode(int m)
   {
      size++;
      heap[size] = m;
      int parent = size/2;
      int child = size;
      while(parent > 0)
      {
          if(_root[heap[parent]]._weight._count > _root[heap[child]]._weight._count)
          {  
              swap(parent,child);
              child = parent;
              parent = child/2;
          }
          else 
              break;

      }
   }



    void HeapAdjust(int start)
    {
        int parent = start;
        int child = start*2;
        while(child <= size)
        {
            if(child + 1 <= size && _root[heap[child]]._weight._count > _root[heap[child + 1]]._weight._count)
                child++;
            if(_root[heap[parent]]._weight._count > _root[heap[child]]._weight._count)
                swap(child,parent);
            else 
                break;
            parent = child;
            child *=2;
        }
    }
    

    void swap(int a,int b)
    {
        int tmp = heap[a];
        heap[a] = heap[b];
        heap[b] = tmp;
    }


    ~Heap()
    {
      delete[] heap;
    }
};




class HuffmanTree{

    private:
    Node* _Root;
    int num;

    public:
    HuffmanTree(weight _infos[])
    {
        int i = 0;
        int j = 0;
        int one,two;
        num = 0;

        for(i = 1; i < 256; ++i)
        {
            if(_infos[i]._count != 0)
                num++;
        }

        _Root = new Node[num*2];
        Heap MinHeap(_Root,num);

        for(i = 1,j = 0; j < num; ++i)
        {
            if(_infos[i]._count != 0)
            {
                j++;
                _Root[j]._weight._ch = _infos[i]._ch;
                _Root[j]._weight._count = _infos[i]._count;
            }
        }


        for(i = num+1; i < 2*num; ++i)
        {
            //Select(i - 1,one,two);
            MinHeap.PopNode(one);
            MinHeap.PopNode(two);

  
            _Root[one]._parent = i;
            _Root[two]._parent = i;
            _Root[i]._left = one;
            _Root[i]._right = two;
            _Root[i]._weight._count = _Root[one]._weight._count + _Root[two]._weight._count;

            MinHeap.PushNode(i);
        }
        _Root[0]._right = 2*num - 1;
    }



    void Coding(weight _infos[])
    {
        for(int i = 1; i <= num; ++i)
        {
            std::string tmp = "";
            for(int j = i,f = _Root[j]._parent; f != 0; j = f,f = _Root[f]._parent)
            {
                if(_Root[f]._left == j)  
                    tmp = '0' + tmp;
                else 
                    tmp = '1' + tmp;
            }

            _Root[i]._weight._code = tmp;
            _infos[_Root[i]._weight._ch]._code= tmp;
        }
    }




    Node* GetRoot()
    {
        return _Root;
    }



    void print()
    {
        std::cout<<num<<std::endl;
        for(int i = 1; i <= num; ++i)
        {
            std::cout<<_Root[i]._weight._ch <<'='<<_Root[i]._weight._count<<"    "<<_Root[i]._weight._code<<std::endl;
        }
    }


    ~HuffmanTree()
    {
        delete[] _Root;
    }


    private:


    void Select(int end,int &a,int &b)
    {
        int flag = 0;
        int i = 0;

        for(i = 1; i <= end; ++i){
            if(flag != 0)
            {
                if(_Root[i]._parent == 0)
                {
                    b = i;
                    if(_Root[b]._weight._count < _Root[a]._weight._count)
                    {
                        int tmp = a;
                        a = b;
                        b = tmp;
                    }
                    break;
                }
            }
            else if(_Root[i]._parent == 0)
            {
                a = i;
                flag = i;
            }
        }

        for(; i <= end; ++i)
        {
            if(_Root[i]._parent == 0)
                if(_Root[i]._weight._count < _Root[b]._weight._count && i != a)
                {    
                    if(_Root[i]._weight._count < _Root[a]._weight._count)
                    {
                        b = a;
                        a = i;
                    }
                    else b = i; 
                }
        }
    }
};





class ProtocolUtil{
 
    public:

    static void print(weight _infos[])
    {
        for(int i = 1; i< 256; ++i)
        {
            std::cout<<i<<" "<<_infos[i]._ch<<'='<<_infos[i]._count<<std::endl;
        }
    }



    static std::string IntoString(type _charcount)
    {
        std::cout<<_charcount;
        std::string tmp = "";
        char a = 0;
        while(_charcount>0)
        {
            a = _charcount%10 + '0';
            tmp = a + tmp;
            _charcount /= 10;
        }
        std::cout<<" to "<<tmp<<std::endl;
        return tmp;
    }



    static type StringtoInt(const std::string line)
    {
        type tmp = 0;
        for(size_t i = 0; i < line.size(); ++i)
        {
            tmp *= 10;
            tmp += line[i] - '0';
        }

        return tmp;
    }




    static void ReadLine(std::string & line,FILE* & COMPRESS)
    {
        char ch = fgetc(COMPRESS);
        while(ch != '\n')
        {
            line += ch;
            ch = fgetc(COMPRESS);
        }
    }



    static void ReadWeight(weight _infos[],FILE* COMPRESS)
    {
        for(int i = 1; i < 256; ++i)
        {
            int tmp = 0;
            for(int j = 0; j < 4; ++j)
            {
              tmp <<= 8;
              tmp |= fgetc(COMPRESS);
            }
            _infos[i]._count = tmp;
        }
    }

    
};



#endif
