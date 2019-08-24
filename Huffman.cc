
#include"Huffman.hpp"



void Guide()
{
  std::cout<<"Usage: ./Huffman method(encode/decode) filename"<<std::endl;
}

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    Guide();
    return -1;
  }

  Huffman h;  

  if(strcmp( argv[1], "encode") == 0)
    h.HuffmanEncode(argv[2]);
  else if(strcmp(argv[1], "decode") == 0)
    h.HuffmanDecode(argv[2]);
  else Guide();

  return 0; 

}
