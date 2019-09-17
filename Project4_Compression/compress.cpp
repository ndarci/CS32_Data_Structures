// compress.cpp

#include <string>
#include <list>
#include <functional>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

const int CHUNK_SIZE = 8;
const int NUMBUCKETS = 15331;

struct Chunk
{
    string data;
    list<int> positions;
};

class hashTable
{
public:
    hashTable() {};
    ~hashTable();
    void insert(string data, int pos);
    bool search(const string& target);
    list<Chunk> getList(const string& target) const;
private:
    list<Chunk> m_table[NUMBUCKETS];
};

hashTable::~hashTable()
{
    // deallocate everything
    for(int i = 0; i < NUMBUCKETS; i++)
    {
        list<Chunk>::iterator it = m_table[i].begin();
        while(it != m_table[i].end())
        {
            it->positions.clear();
            it++;
        }
        m_table[i].clear();
    }
}

void hashTable::insert(string data, int pos)
{
    // get the bucket to drop this string into
    unsigned long moduloMe = hash<string>()(data);
    unsigned long putMeHere = moduloMe % NUMBUCKETS;
    list<Chunk>::iterator it = m_table[putMeHere].begin();
    while( it != m_table[putMeHere].end() )
    {
        // if this string was already inserted,
        // just add this position to the existing chunk's list of positions
        if(it->data == data)
        {
            it->positions.push_back(pos);
            return;
        }
        it++;
    }
    // if this is a new string for this bucket,
    // create a new Chunk with a one-int long position list
    Chunk newChunk;
    newChunk.data = data;
    newChunk.positions.push_back(pos);
    m_table[putMeHere].push_back(newChunk);
}

bool hashTable::search(const string& target)
{
    // jump to the bucket this string hashes to and search for it
    unsigned long moduloMe = hash<string>()(target);
    unsigned long bucketToSearch = moduloMe % NUMBUCKETS;
    list<Chunk>::iterator it = m_table[bucketToSearch].begin();
    while( it != m_table[bucketToSearch].end() )
    {
        if( it->data == target )
        {
            return true;
        }
        it++;
    }
    return false;
}

list<Chunk> hashTable::getList(const string& target) const
{
    unsigned long moduloMe = hash<string>()(target);
    unsigned long bucketToReturn = moduloMe % NUMBUCKETS;
    return m_table[bucketToReturn];
}

string copyCommand(unsigned long len, unsigned long pos)
{
    // return a copy command
    string returnMe = "C" + to_string(len) + "," + to_string(pos);
    return returnMe;
}

string addCommand(string data)
{
    // return an add command
    string returnMe = "A" + to_string(data.size()) + ":" + data;
    return returnMe;
}

unsigned long longestMatch(const string& oldF, const string& newF, int newFilePos, list<Chunk> l, int& bestPos, string target)
{
    // find the Chunk inside this bucket that has the target string in it
    list<Chunk>::iterator it = l.begin();
    while(it != l.end() )
    {
        if(it->data == target)
            break;
        it++;
    }
    unsigned long bestOne = it -> data.size();
    unsigned long i = 0;
    int bestPosHolder = it->positions.front();
    list<int>::iterator posI = it->positions.begin();
    // for every position this string occurs at,
    // see how long of a copy can be made off the old file
    while(posI != it->positions.end())
    {
        i = *posI + it->data.size();
        unsigned long nFP = newFilePos + it->data.size();
        unsigned long count = it->data.size();
        while( i < oldF.size() && nFP < newF.size() )
        {
            if(oldF[i] != newF[nFP])
                break;
            count++;
            i++;
            nFP++;
        }
        if(count > bestOne)
        {
            bestOne = count;
            bestPosHolder = *posI;
        }
        posI++;
    }
    // choose the longest copy possible and return its length and position
    bestPos = bestPosHolder;
    return bestOne;
}

void createDiff(istream& fold, istream& fnew, ostream& fdiff)
{
    hashTable* hashPtr = new hashTable();
    string oldFile = "";
    string newFile = "";
    if( !fold || !fnew || !fdiff )
    {
        cerr << "Error! Cannot open or create one of the required files." << endl;
        delete hashPtr;
        return;
    }
    // read files into strings
    char c;
    while( fold.get(c) )
        oldFile += c;
    while( fnew.get(c) )
        newFile += c;
    unsigned long numChunks = oldFile.size() + 1 - CHUNK_SIZE;
    for(int i = 0; i < numChunks; i++)
    {
        // put each chunk in the hash table
        string addToHash(oldFile, i, CHUNK_SIZE);
        hashPtr -> insert(addToHash, i);
    }
    int j = 0;
    string addMe = "";
    unsigned long copyLen = 0;
    int copyPos = 0;
    // go through every character in newFile, chunk by chunk or one by one
    while( j < newFile.size() )
    {
        string newFileChunk(newFile, j, CHUNK_SIZE);
        if( hashPtr -> search(newFileChunk) == true )
        {
            // this string exists in the old file...
            if(addMe != "")
            {
                // write an add command with whatever string we've queued up to add
                if(addMe[0] != '\0')
                    fdiff << addCommand(addMe);
                addMe = "";
            }
            list<Chunk> thisList = hashPtr -> getList(newFileChunk);
            int bestPos = 0;
            // use longestMatch to find the most efficient copy command to write
            copyLen = longestMatch(oldFile, newFile, j, thisList, bestPos, newFileChunk);
            copyPos = bestPos;
            fdiff << copyCommand(copyLen, copyPos);
            j += copyLen;
            if( j < newFile.size() )
            {
                // make sure the next chunk also can't be copied
                string nextChunk(newFile, j, CHUNK_SIZE);
                if( hashPtr -> search(nextChunk) == false && j < newFile.size())
                {
                    // if it can't be copied, add the first character that doesn't match
                    // to the add queue and loop back to the top, checking the next chunk
                    addMe += newFile[j];
                    j++;
                }
            }
        }
        else
        {
            if(j < newFile.size())
            {
                addMe += newFile[j];
                j++;
            }
        }
    }
    // we're at the end of the file...
    // write a command to dump whatever was in the add queue
    if( addMe != "" )
    {
        if(addMe[0] != '\0')
            fdiff << addCommand(addMe);
    }
    delete hashPtr;
}

bool getInt(istream& inf, int& n)
{
    char ch;
    if (!inf.get(ch)  ||  !isascii(ch)  ||  !isdigit(ch))
        return false;
    inf.unget();
    inf >> n;
    return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset)
{
    if (!inf.get(cmd))
    {
        cmd = 'x';  // signals end of file
        return true;
    }
    char ch;
    switch (cmd)
    {
        case 'A':
            return getInt(inf, length) && inf.get(ch) && ch == ':';
        case 'C':
            return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
        case '\r':
        case '\n':
            return true;
        default:
            length = -1;
            return true;
    }
    return false;
}

bool getAddString(istream& inf, string& addMe, int length)
{
    // extracts the string to add after Alen: from the diff file
    char ch;
    if( !inf.get(ch) )
        return false;
    addMe += ch;
    int i = 0;
    while( i < length - 1 && inf.get(ch))
    {
        addMe += ch;
        i++;
    }
    return true;
}

bool applyDiff(istream& fold, istream& fdiff, ostream& fnew)
{
    if( !fold || !fdiff || !fnew )
    {
        cerr << "Error! Cannot open or create one of the required files." << endl;
        return false;
    }
    string oldFile = "";
    char c;
    while( fold.get(c) )
        oldFile += c;
    char cmd;
    int len;
    int pos;
    string addTheseChars;
    // read all the commands from the diff file and execute them
    while(getCommand(fdiff, cmd, len, pos))
    {
        if(len == -1 || pos + len > oldFile.size() )
            return false;
        switch(cmd)
        {
            case 'x':
                return true;
            case 'C':
            {
                string copyMe(oldFile, pos, len);
                fnew << copyMe;
                break;
            }
            case 'A':
            {
                addTheseChars = "";
                if(!getAddString(fdiff, addTheseChars, len))
                    return false;
                fnew << addTheseChars;
                break;
            }
        }
    }
    return true;
}

int main()
{
    ifstream fold("/Users/niko/Desktop/testOld.txt");
    ifstream fnew("/Users/niko/Desktop/warandpeace2.txt");
    ofstream fdiffWrite("/Users/niko/Desktop/diffWrite.txt");
    ifstream fdiffRead("/Users/niko/Desktop/testDiff.txt");
    ofstream fout("/Users/niko/Desktop/out.txt");
//    createDiff(fold, fnew, fdiffWrite);
    assert(!applyDiff(fold, fdiffRead, fout));
}

//bool runtest(string oldName, string newName, string diffName, string newName2)
//{
//    if (diffName == oldName  ||  diffName == newName  ||
//        newName2 == oldName  ||  newName2 == diffName  ||
//        newName2 == newName)
//    {
//        cerr << "Files used for output must have names distinct from other files" << endl;
//        return false;
//    }
//    ifstream oldFile(oldName, ios::binary);
//    if (!oldFile)
//    {
//        cerr << "Cannot open " << oldName << endl;
//        return false;
//    }
//    ifstream newFile(newName, ios::binary);
//    if (!newFile)
//    {
//        cerr << "Cannot open " << newName << endl;
//        return false;
//    }
//    ofstream diffFile(diffName, ios::binary);
//    if (!diffFile)
//    {
//        cerr << "Cannot create " << diffName << endl;
//        return false;
//    }
//    createDiff(oldFile, newFile, diffFile);
//    diffFile.close();
//
//    oldFile.clear();   // clear the end of file condition
//    oldFile.seekg(0);  // reset back to beginning of the file
//    ifstream diffFile2(diffName, ios::binary);
//    if (!diffFile2)
//    {
//        cerr << "Cannot read the " << diffName << " that was just created!" << endl;
//        return false;
//    }
//    ofstream newFile2(newName2, ios::binary);
//    if (!newFile2)
//    {
//        cerr << "Cannot create " << newName2 << endl;
//        return false;
//    }
//    assert(applyDiff(oldFile, diffFile2, newFile2));
//    newFile2.close();
//
//    newFile.clear();
//    newFile.seekg(0);
//    ifstream newFile3(newName2, ios::binary);
//    if (!newFile)
//    {
//        cerr << "Cannot open " << newName2 << endl;
//        return false;
//    }
//    if ( ! equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
//                 istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
//    {
//
//        cerr << newName2 << " is not identical to " << newName
//        << "; test FAILED" << endl;
//        return false;
//    }
//    return true;
//}
//
//int main()
//{
//    assert(runtest("/Users/niko/Desktop/smallmart1.txt", "/Users/niko/Desktop/smallmart2.txt", "/Users/niko/Desktop/diffWrite.txt", "/Users/niko/Desktop/out.txt"));
//    assert(runtest("/Users/niko/Desktop/greeneggs1.txt", "/Users/niko/Desktop/greeneggs2.txt", "/Users/niko/Desktop/diffWrite.txt", "/Users/niko/Desktop/out.txt"));
//    assert(runtest("/Users/niko/Desktop/warandpeace1.txt", "/Users/niko/Desktop/warandpeace2.txt", "/Users/niko/Desktop/diffWrite.txt", "/Users/niko/Desktop/out.txt"));
//    assert(runtest("/Users/niko/Desktop/strange1.txt", "/Users/niko/Desktop/strange2.txt", "/Users/niko/Desktop/diffWrite.txt", "/Users/niko/Desktop/out.txt"));
//    cerr << "Test PASSED" << endl;
//}
