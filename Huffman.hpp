#ifndef __HUFFMAN_HPP__
#define __HUFFMAN_HPP__


#include"ProtocolUtil.hpp"


class Huffman 
{
    private:
    weight _infos[256];

    public:

    Huffman()
    {
        for (int i = 1; i < 256; i++)
        {
            _infos[i]._ch = (unsigned char)i;                    
        }              
    }


    bool HuffmanEncode(const char* File_name)
    {
        std::string Source_name = File_name;
        if(int a = Source_name.rfind('.') != std::string::npos)
            if(Source_name.substr(a) == ".huffman")
            {
                std::cout<<"File compressed"<<std::endl;
                return false;
            }
        FILE* SOURCE = fopen(Source_name.c_str(),"rb");
        assert(SOURCE);
        int ch = fgetc(SOURCE);
        type charcount = 0;  //统计字符串出现总次数


        while(ch != EOF)
        {
            if(feof(SOURCE)) break;
            _infos[ch]._count++;
            ch = fgetc(SOURCE);
            charcount++;
        }
      

        HuffmanTree HT(_infos);
        HT.Coding(_infos);
        //HT.print();


        std::string compressname = File_name;
        compressname += ".huffman";
        FILE* COMPRESS = fopen(compressname.c_str(),"wb");
        assert(COMPRESS);
        //指针再次指向文件头
        fseek(SOURCE,0,SEEK_SET);

        //先写入配之信息
        std::string countStr;
        countStr = ProtocolUtil::IntoString(charcount);//写入文件总长度
        fputs(countStr.c_str(),COMPRESS);
        fputc('\n',COMPRESS);

        char put = 0;
        for(int i = 1; i < 256; ++i)
        {
            int tmp = _infos[i]._count;
            for(int j = 0; j < 32;++j)
            {
                put <<= 1;
                if((tmp>>(31 - j) & 1) == 1) 
                    put |= 1;
                if((j+1)%8 == 0) 
                {
                    fputc(put,COMPRESS);
                    put = 0;
                }
            }
        }


        //写入编码后信息
        int pos = 0;
        unsigned char value = 0;
        int ch1 = fgetc(SOURCE);
        while(ch1 != EOF)
        {
            if(feof(SOURCE)) 
                break;
            std::string& code = _infos[ch1]._code;
            for(size_t i = 0; i < code.size(); ++i)
            {
                value <<=1;
                if(code[i] == '1') 
                    value |= 1;
          
                if(++pos == 8)   //满8位写入文件
                {
                    fputc(value,COMPRESS);
                    value = 0;
                    pos = 0;
                }
            }
            ch1 = fgetc(SOURCE);
        }
        if(pos)  //最后的编码不满足一个字节
        {
            value = value<<(8-pos);
            fputc(value,COMPRESS);
        }



        fclose(SOURCE);
        fclose(COMPRESS);
        return true;
    }








    bool HuffmanDecode(const char* File_name)
    {
        //读取配置信息
        std::string compressname = File_name;
        if(int a = compressname.rfind('.') == std::string::npos)
        {
            return false;
        }
        else{
            if(compressname.substr(compressname.rfind('.')) != ".huffman")
            {
                return false;
            }
        }
        std::cout<<File_name<<std::endl;
        FILE* COMPRESS = fopen(compressname.c_str(),"rb");
        assert(COMPRESS);
        type charcount = 0;
        std::string line;
        ProtocolUtil::ReadLine(line,COMPRESS);  //读取总长度
        charcount = ProtocolUtil::StringtoInt(line);
        ProtocolUtil::ReadWeight(_infos,COMPRESS);//读取计数情况



        //重建Huffman树
        HuffmanTree HT(_infos);



        //写入解码文件
        std::string uncompressname = compressname.substr(0,compressname.rfind('.'));
        uncompressname += ".unhuffman";
        FILE* UNCOMPRESS = fopen(uncompressname.c_str(),"wb");
        assert(UNCOMPRESS);


        Node* root = HT.GetRoot();
        int cur = root[0]._right;
        char ch = fgetc(COMPRESS);
        int pos = 8;
        while(1)
        {
            --pos;
            if((ch >>pos) & 1) 
                cur = root[cur]._right;
            else 
                cur = root[cur]._left;

            if(root[cur]._left == 0 && root[cur]._right == 0)
            {
                fputc(root[cur]._weight._ch,UNCOMPRESS);
                cur = root[0]._right;
                charcount--;
            }
            if(pos == 0)
            {
                ch = fgetc(COMPRESS);
                pos = 8;
            }
            if(charcount == 0)
                break;
        }
      


        fclose(COMPRESS);
        fclose(UNCOMPRESS);
        return true;
    }

};
       

#endif
