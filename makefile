.PHONY: Huffman clean

Huffman: Huffman.cc
	g++ $^ -o $@ -std=c++11

clean:
	rm Huffman 
