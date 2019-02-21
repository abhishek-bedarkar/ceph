#include<iostream>
#include<fstream>
#include <cstring>
#include <string>
#include <iomanip>
#include <sstream>
#include<openssl/sha.h>
#include<stdio.h>

#define SIZE 128*1024
using namespace std;

class BlockHashTable;
class Node
{
    string fName; //256 bit name of the file blocks
    long ref_counter;//counter for duplicate file blocks
    Node *nextFileBlock;

public:
    Node();
    Node(string fName, long ref_counter);

    friend class BlockHashTable;
};//Node

Node :: Node()
{
    this->fName = "";
    this->ref_counter = 0;
    this->nextFileBlock = NULL;
}//default constructor


Node :: Node(string fName, long ref_counter)
{
    this->fName = fName;
    this->ref_counter = ref_counter;
    this->nextFileBlock = NULL;
}//parameterized constructor

bool checkCollision(FileBlockTable *fbt,long long block_Name)
{   
    if(fbt[block_Name] == NULL)
        return false;
    else
        return true;
}//checkCollision

bool isDuplicate(FileBlockTable *fbt,long long block_Name, string file256)
{
    for(Node *block = fbt[block_Name]; block != NULL; block = block->nextFileBlock)
    {
        if(block->fName == file256)
        {
            block->ref_counter++;
            return true;
        }//if duplicate block found

        else//else duplicate block not found
            return false;
    }//for searching for duplicacy
}//isDuplicate

void addFileBlock(FileBlockTable *fbt,long long block_Name, string file256)
{
    for(Node *block = fbt[block_Name]; block->nextFileBlock != NULL; block = block->nextFileBlock);
    block->nextFileBlock = new Node(file256, 0);
}//addFileBlock

void newFileBlock(FileBlockTable *fbt, long long block_Name, string file256)
{
    if(checkCollision(fbt, block_Name))
    {
        if(!isDuplicate(fbt, block_Name, file256))
            addFileBlock(fbt, block_Name, file256);
            //if unique 256bit hash value
    }//if 33bit hash value exists

    else//add file block to unused index
        addFileBlock(fbt, block_Name, file256);    
}//newFileBlock

class FileBlockTable
{
    long long block_Name;//33bit decimal version filename
    Node *block_Head;

public:
    FileBlockTable();
    FileBlockTable(long long block_Name, Node* block_Head);
};//BlockHashTable

FileBlockTable :: FileBlockTable()
{
    this->block_Name = 0;
    this->block_Head = NULL;
}//def constr

FileBlockTable :: FileBlockTable(long long block_Name, Node* block_Head)
{
    this->block_Name = block_Name;
    this->block_Head = block_Head;
}//par constr

string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }//for

    //cout<<"The hash value is: "<<ss.str();
    return ss.str();
}//sha256

int getHexValue(char a)
{
    switch(a)
    {
        case '0': return 0x0;
        case '1': return 0x1;
        case '2': return 0x2;
        case '3': return 0x3;
        case '4': return 0x4;
        case '5': return 0x5;
        case '6': return 0x6;
        case '7': return 0x7;
        case '8': return 0x8;
        case '9': return 0x9;
        case 'a': return 0xA;
        case 'b': return 0xB;
        case 'c': return 0xC;
        case 'd': return 0xD;
        case 'e': return 0xE;
        case 'f': return 0xF;
    }//switch
}//getHexValue

long long int convertHash(string hashValue)
{
    //cout<<hashValue<<"\n";
    
    string hash33 = hashValue.substr (55, 9);
    char a = hash33[0];
    int x, hex = getHexValue(a); 
    long long int final_binary = hex & 0x1;

    for(int i =1; i <9; i++)
    {
        x = getHexValue(hash33[i]);
        final_binary = final_binary << 4;
        final_binary = final_binary | x;        
    }//for
    //cout<<x<<" "<<hash33[0]<<endl;
    return final_binary;
}//convertHash

//bootup
void load_HashTable()
{

}//loadHashTable

//backup
void store_HashTable()
{

}//storeHashTable

int main()
{
    string fileName;
    fstream fin, fout;
    char buffer[SIZE];
    int ch;

    cout<<"\nEnter File Name : ";
    cin>>fileName;

    fin.open(fileName, ios::in | ios::binary);
    while(fin)
    {
        //reading 128K from file
        //in a buffer and sending it to hash
        //strcpy(buffer,"\0");
        fin.read(buffer, SIZE);
        
        string *s = new string(buffer);
            
        //sha256 algorithm
        
        string hashValue = sha256(*s);
        //printf("\nThe data  is: %s",buffer);
        //printf("\nThe Hash value is: %s",hashValue);
        
        //cout<<"Hash value is: "<<hashValue<<endl;
        //scanf("%d",&ch);
        long long newHashValue = convertHash(hashValue);
        cout<<newHashValue<<"\n";
        
        
        fout.open(hashValue, ios::out | ios::binary);
        fout.write(buffer, fin.gcount());
        fout.close();
    }//while
    
    return 0;
}//main