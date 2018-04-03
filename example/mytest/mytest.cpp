//
// Created by hou guoli on 2018/2/24.
//

#include <iostream>
#include <string>
#include <fstream>

#include <rapidjson/document.h>
//#include <rapidjson/filestream.h>
#include <rapidjson/error/en.h>
#include "rapidjson/reader.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"

using namespace std;
using namespace rapidjson;

//void testRapidJson()
//{
//    // Prepare reader and input stream.
//    Reader reader;
//    char readBuffer[65536];
//    FileReadStream is(stdin, readBuffer, sizeof(readBuffer));
//
//    // Prepare writer and output stream.
//    char writeBuffer[65536];
//    FileWriteStream os(stdout, writeBuffer, sizeof(writeBuffer));
//    PrettyWriter<FileWriteStream> writer(os);
//
//    // JSON reader parse from the input stream and let writer generate the output.
//    if (!reader.Parse<kParseValidateEncodingFlag>(is, writer)) {
//        fprintf(stderr, "\nError(%u): %s\n", static_cast<unsigned>(reader.GetErrorOffset()), GetParseError_En(reader.GetParseErrorCode()));
//        return;
//    }
//
//    return;
//}

void testParserJson()
{
    //json格式字符串
    // {
    // "name":"qfl",
    // "age":20,
    // "letter":["a","b","c"],
    // "location": {"province":"fujian","city":"xiamen","number":16}
    // "book":[{"name":"book1", "isbn":"123"},{"name":"book2","isbn":"456"}],
    // "healthy":true,
    // }
    const char *json = "{\"name\":\"qfl\", \"age\":20,\"letter\":[\"a\",\"b\",\"c\"],\"location\": {\"province\":\"fujian\",\"city\":\"xiamen\",\"number\":16},\"book\":[{\"name\":\"book1\", \"isbn\":\"123\"},{\"name\":\"book2\",\"isbn\":\"456\"}], \"healthy\":true}";

//    const char *json = "{\"hello\": \"world\",\"t\": true ,\"f\": false,\"n\": null,\"i\": 123,\"pi\": 3.1416,\"a\": [1, 2, 3, 4]}";



    rapidjson::Document newDoc;
    rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson::CrtAllocator> newDoc1;
    newDoc.Parse(json);
    if( newDoc.HasParseError())
    {
        printf("Json Parse error:%d", newDoc.GetParseError());
    }
    else{
        if (newDoc.HasMember("name")) {
            printf("name:%s\n", newDoc["name"].GetString()); //必须要获取对应的数据类型，rapidjson不会帮你转换类型
        }
        else {}

        if (newDoc.HasMember("age")) {
            const rapidjson::Value& value = newDoc["age"];
            printf("age:%d\n", value.GetInt());  //获取正确的类型
        }

        if (newDoc.HasMember("letter")) {
            rapidjson::Value letter;    //使用一个新的rapidjson::Value来存放array的内容
            letter = newDoc["letter"];

            //确保它是一个Array，而且有内容
            if (letter.IsArray() && !letter.Empty()) {
                //遍历Array中的内容
                for (rapidjson::SizeType i = 0; i < letter.Size(); i++) {
                    printf("letter:%s\n", letter[i].GetString());
                }
            }
            else {}
        }

        if (newDoc.HasMember("location")) {
            rapidjson::Value location;      //使用一个新的rapidjson::Value来存放object
            location = newDoc["location"];

            //确保它是一个Object
            if (location.IsObject()) {

                if (location.HasMember("province")) {
                    printf("location:province:%s\n", location["province"].GetString());
                }
                else {}
                if (location.HasMember("city")) {
                    printf("location:city:%s\n", location["city"].GetString());
                }
                else {}
                if (location.HasMember("number")) {
                    printf("location:number:%d\n", location["number"].GetInt());
                }
                else {}
            }
            else {}
        }
        else {}

        //book是一个包含了2个object的array。按照上面的步骤来取值就行
        if (newDoc.HasMember("book")) {
            rapidjson::Value book;
            book = newDoc["book"];

            //先取Array
            if (book.IsArray() && !book.Empty()) {

                rapidjson::Value tempBook;
                for (rapidjson::SizeType i = 0; i < book.Size(); i++) {
                    tempBook = book[i]; //Array中每个元素又是一个Object

                    if (tempBook.IsObject()) {

                        if (tempBook.HasMember("name") && tempBook.HasMember("isbn")) {
                            printf("book:%d:name:%s, isbn:%s\n", i, tempBook["name"].GetString(), tempBook["isbn"].GetString());
                        }
                        else {}
                    }
                    else {}
                }
            }
            else {}
        }
        else {}

        if (newDoc.HasMember("healthy")) {
            if (newDoc["healthy"].GetBool()) {
                printf("healthy:true\n");
            }
            else {
                printf("healthy:false\n");
            }
        }
        else {}
    }

}


// {
//     "author": "tashaxing",
//     "number": [
//         0,
//         1,
//         2
//     ],
//     "language": {
//         "language1": "C++",
//         "language2": "java"
//     },
//     "information": [
//         {
//             "hobby": "drawing"
//         },
//         {
//             "height": 1.71
//         }
//     ]
// }


//写json文件
void json_write()
{
    Document doc;
    doc.SetObject();
    Document::AllocatorType &allocator=doc.GetAllocator(); //获取分配器
    //1.添加字符串对象
    doc.AddMember("author","tashaxing",allocator);
    //2.添加数组对象
    Value array1(kArrayType);
    for(int i=0;i<3;i++)
    {
        Value int_object(kObjectType);
        int_object.SetInt(i);
        array1.PushBack(int_object,allocator);
    }
    doc.AddMember("number",array1,allocator);
    //3.添加复合对象
    Value object(kObjectType);
    object.AddMember("language1","C++",allocator);
    object.AddMember("language2","java",allocator);
    doc.AddMember("language",object,allocator);
    //4.添加对象数组和复合对象的组合
    Value array2(kArrayType);
    Value object1(kObjectType);
    object1.AddMember("hobby","drawing",allocator);
    array2.PushBack(object1,allocator);
    Value object2(kObjectType);
    object2.AddMember("height",1.71,allocator);
    array2.PushBack(object2,allocator);
    doc.AddMember("information",array2,allocator);
    StringBuffer buffer;
    PrettyWriter<StringBuffer> pretty_writer(buffer);  //PrettyWriter是格式化的json，如果是Writer则是换行空格压缩后的json
    doc.Accept(pretty_writer);
    //打印到屏幕
    cout<<"the json output:"<<endl;
    cout<<buffer.GetString()<<endl;
    //输出到文件
    ofstream fout;
    fout.open("test.json");    //可以使绝对和相对路径，用\\隔开目录，test, test.json, test.txt 都行，不局限于文件格式后缀，只要是文本文档
    fout<<buffer.GetString();
    fout.close();
}

//读json文件
void json_read()
{
    cout<<"the json read:"<<endl;
    ifstream fin;
    fin.open("test.json");
    string str;
    string str_in="";
    while(getline(fin,str))    //一行一行地读到字符串str_in中
    {
        str_in=str_in+str+'\n';
    }
    //解析并打印出来
    Document document;
    document.Parse<0>(str_in.c_str());

    Value &node1=document["author"];
    cout<<"author: "<<node1.GetString()<<endl;

    Value &node2=document["number"];
    cout<<"number: "<<endl;
    if(node2.IsArray())
    {
        for(int i=0;i<node2.Size();i++)
            cout<<'\t'<<node2[i].GetInt()<<endl;
    }

    Value &node3=document["language"];
    cout<<"language: "<<endl;
    Value &tmp=node3["language1"];
    cout<<'\t'<<"language1: "<<tmp.GetString()<<endl;
    tmp=node3["language2"];
    cout<<'\t'<<"language2: "<<tmp.GetString()<<endl;

    Value &node4=document["information"];
    cout<<"information: "<<endl;
    if(node4.IsArray())
    {
        int i=0;
        Value &data=node4[i];   //注意，此处下表索引只能用变量，不能用常量，例如node[0]编译错误
        cout<<'\t'<<"hobby: "<<data["hobby"].GetString()<<endl;
        i=1;
        data=node4[i];
        cout<<'\t'<<"height: "<<data["height"].GetDouble()<<endl;
    }

}

int main(int, char*[])
{
//    testParserJson();
    json_write();
    json_read();
    return 1;
}