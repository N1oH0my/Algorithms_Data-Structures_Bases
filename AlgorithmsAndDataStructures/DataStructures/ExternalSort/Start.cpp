//------------------------------------INCLUDE---------------------------------------------------------------------//
#include <string>
#include <fstream>
#include <iostream>
//-------------------------------------USING----------------------------------------------------------------------//
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::to_string;
//------------------------------------STRUCTURES------------------------------------------------------------------//
struct MyVeryGoodPair
{
private:
    int number;
    char symbol;

public:
    MyVeryGoodPair() :number(0), symbol(' ') {};
    MyVeryGoodPair(const int& _number, const char& _symbol) :number(_number), symbol(_symbol) {};
    MyVeryGoodPair(const MyVeryGoodPair& A) { number = A.number; symbol = A.symbol; };
    ~MyVeryGoodPair() {};
    void RemakeMyVeryGoodPair(const int& _number, const char& _symbol) { number = _number; symbol = _symbol; };

    void SetNumber(const int& _number) { number = _number; };
    void SetSymbol(const char& _symbol) { symbol = _symbol; };
    int GetNumber() { return number; };
    char GetSymbol() { return symbol; };
};

struct FileData {
    string filename;
    ifstream infile;
    ofstream outfile;
    bool eof = false;
    bool eos = false;
    MyVeryGoodPair elem;
    FileData() {};
    FileData(FileData& A) { filename = A.filename; eof = A.eof; eos = A.eos; elem.SetNumber(A.elem.GetNumber()); elem.SetSymbol(A.elem.GetSymbol()); }
};

//-----------------------------------FOR-ELEMENTS-----------------------------------------------------------------//
void ReadNext(FileData& binFile) {
    int prev = binFile.elem.GetNumber();

    if (binFile.infile.peek() != EOF)
    {
        char symbol = ' ';
        std::string Number = "";
        binFile.infile.read(&symbol, sizeof(char));
        while (symbol >= '0' && symbol <= '9' || symbol == '-')
        {
            Number += symbol;
            binFile.infile.read(&symbol, sizeof(char));
        }
        binFile.elem.SetSymbol(symbol);
        binFile.elem.SetNumber(std::stoi(Number));

        if (prev > binFile.elem.GetNumber()) 
        {
            binFile.eos = true;
        }
    }
    else 
    {
        binFile.eof = true;
        binFile.eos = true;
    }
}

void CopyElement(FileData& binFileIn, FileData& fileOut) {
    string Number = to_string(binFileIn.elem.GetNumber());
    size_t length = Number.length();
    for (size_t i = 0; i < length; ++i)
    {
        fileOut.outfile.write(&Number[i], sizeof(char));
    }
    char symbol = binFileIn.elem.GetSymbol();
    fileOut.outfile.write(&symbol, sizeof(char));
    ReadNext(binFileIn);
}

void copyRun(FileData& binFileIn, FileData& fileOut)
{
    while (!binFileIn.eos) 
    {
        CopyElement(binFileIn, fileOut);
    }
}
//----------------------------------OPEN-CLOSE--------------------------------------------------------------------//
void OpenForBinaryReading(FileData& binFile) {
    binFile.infile.open(binFile.filename, std::ifstream::binary);
    binFile.infile.seekg(0, binFile.infile.beg);

    if (binFile.infile.peek() != EOF)
    {
        char symbol = ' ';
        std::string Number = "";
        binFile.infile.read(&symbol, sizeof(char));
        while (symbol >= '0' && symbol <= '9' || symbol == '-')
        {
            Number += symbol;
            binFile.infile.read(&symbol, sizeof(char));
        }
        binFile.elem.SetSymbol(symbol);
        binFile.elem.SetNumber(std::stoi(Number));
    }
    else 
    {
        binFile.eof = true;
        binFile.eos = true;
    }
}

void OpenForBinaryWriting(FileData& binFile) {
    binFile.outfile.open(binFile.filename, ofstream::binary);
}
void CloseIn(FileData& file) {
    file.eof = false;
    file.eos = false;
    file.infile.close();
}
void CloseOut(FileData& file) {
    file.eof = false;
    file.eos = false;
    file.outfile.close();
}
//--------------------------------FOR-FILEDATA--------------------------------------------------------------------//
int FindAmountOfNotEmptyFiles(FileData* files, const int& from, const int& to) {
    int firstEmpty = to;

    for (int i = 0; i < to && firstEmpty == to; ++i) 
    {
        if (files[i].eof) 
        {
            firstEmpty = i;
        }
    }

    return files[0].eof ? -1 : firstEmpty;
}

bool IsEndOfTempSequences(FileData* files, const int& from, const int& to) {
    bool isEnd = true;

    for (size_t i = from; i < to && isEnd; ++i)
    {
        isEnd = files[i].eos;
    }

    return isEnd;
}

FileData& FindMinFileInSequences(FileData* files, const int& from, const int& to) {
    FileData* fd = &files[from];
    int i = from + 1;

    while (fd->eos) 
    {
        fd = &files[i++];
    }

    while (i < to) 
    {
        if (!files[i].eos && files[i].elem.GetNumber() < fd->elem.GetNumber()) 
        {
            fd = &files[i];
        }

        i++;
    }

    return *fd;
}
//--------------------------------------SORT----------------------------------------------------------------------//
void Distribution(FileData& mainFile, FileData* files, const int& amountOfFiles) {
    OpenForBinaryReading(mainFile);

    for (size_t i = 0; i < amountOfFiles; ++i)
    {
        OpenForBinaryWriting(files[i]);
    }

    while (!mainFile.eof) 
    {
        for (size_t i = 0; i < amountOfFiles && !mainFile.eof; ++i) 
        {
            copyRun(mainFile, files[i]);
            mainFile.eos = mainFile.eof;
        }
    }

    for (size_t i = 0; i < amountOfFiles; ++i)
    {
        CloseOut(files[i]);
    }

    CloseIn(mainFile);
}

bool Merge(FileData* files, const int& amountOfFiles, FileData& mainFile) {
    bool endOfSort = false;

    for (int i = 0; i < amountOfFiles; ++i)
    {
        OpenForBinaryReading(files[i]);
    }

    int amountOfNotEmptyFiles = FindAmountOfNotEmptyFiles(files, 0, amountOfFiles);
    if (amountOfNotEmptyFiles != 1 && amountOfNotEmptyFiles !=-1)
    {
        OpenForBinaryWriting(mainFile);
        while (amountOfNotEmptyFiles != -1) 
        {
            while (!IsEndOfTempSequences(files, 0, amountOfNotEmptyFiles)) 
            {
                CopyElement(FindMinFileInSequences(files, 0, amountOfNotEmptyFiles), mainFile);
            }

            for (int i = 0; i < amountOfNotEmptyFiles; ++i) 
            {
                files[i].eos = files[i].eof;
            }

            amountOfNotEmptyFiles = FindAmountOfNotEmptyFiles(files, 0, amountOfNotEmptyFiles);
        }
        CloseOut(mainFile);
    }
    else
    {
        endOfSort = true;
    }

    for (size_t i = 0; i < amountOfFiles; ++i)
    {
        CloseIn(files[i]);
    }

    return endOfSort;
}

void ExternalSort(string source, const int& amountOfActiveFiles) {
    if (amountOfActiveFiles > 1)
    {
        FileData* files = new FileData[amountOfActiveFiles];

        FileData mainFile;
        mainFile.filename = source;

        for (size_t i = 0; i < amountOfActiveFiles; ++i)
        {
            files[i].filename = "files/f" + to_string(i) + ".bin";
        }

        bool end = false;
        cout << endl << "Creating help files..." << endl;
        while (!end) 
        {
            Distribution(mainFile, files, amountOfActiveFiles);

            end = Merge(files, amountOfActiveFiles, mainFile);
        }
        cout << endl << "Deleting help files..." << endl;
        for (int i = 0; i < amountOfActiveFiles; i++) {
            remove(("files/f" + to_string(i) + ".bin").c_str());
        }

        delete[] files;
    }
    else
    {
        std::cout << endl << "it doesn't work like that :)" << endl;
    }
}
//------------------------------------PRINT-FILE------------------------------------------------------------------//
void BINARYprint(const string& filename, int from = 1, int count = -1)
{
    ifstream file(filename, ifstream::binary);
    file.seekg(0, file.beg);
    --from;
    if (file.peek() != EOF && from != 0)
    {
        do
        {
            char symbol = ' ';
            file.read(&symbol, sizeof(char));
            while (symbol >= '0' && symbol <= '9' || symbol == '-')
                file.read(&symbol, sizeof(char));
            --from;
        } while (from != 0 && file.peek() != EOF);
    }

    if (file.peek() != EOF)
    {
        const int step = ((count == -1) ? 0 : -1);
        do
        {
            char symbol = ' ';
            string myverygoodpair = "";
            file.read(&symbol, sizeof(char));
            while (symbol >= '0' && symbol <= '9' || symbol == '-')
            {
                myverygoodpair += symbol;
                file.read(&symbol, sizeof(char));
            }
            myverygoodpair += ' ';
            myverygoodpair += symbol;
            cout << myverygoodpair << '\n';
            count += step;
        } while ((count > 0 || count == -1) && file.peek() != EOF);
    }
    else
    {
        cout << "\nFile is empty or unreadable.\n";
    }
    file.close();
}
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------MAIN------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//
int main() {
    string source = "files\\testBinaryFile.bin";

    ifstream fileIn(source);

    if (!fileIn) 
    {
        cout << endl << "No file!" << endl;
    }
    else 
    {
        int amountOfActiveFiles;// = 3;
        cout << endl << "Enter amount of active files: " << endl;
        std::cin >> amountOfActiveFiles;

        cout << endl << "Before sorting:" << endl;
        BINARYprint(source, 1);

        cout << endl << "Start sorting" << endl;
        ExternalSort(source, amountOfActiveFiles);
        cout << endl << "Ready" << endl;

        cout << endl << "After sorting:" << endl;
        BINARYprint(source, 1);

        cout << endl << "Done. Press any button..." << endl;
    }

    std::cin.ignore();
    std::cin.get();
    return 0;
}
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------END-OF-MAIN-------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//