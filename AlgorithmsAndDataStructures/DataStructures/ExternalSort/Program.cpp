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
using std::stoi;
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
    ~FileData() {};
};

//-----------------------------------FOR-ELEMENTS-----------------------------------------------------------------//
void ReadNext(FileData& file);

void CopyElement(FileData& file_in, FileData& file_out);

void copyRun(FileData& file_in, FileData& file_out);
//----------------------------------OPEN-CLOSE--------------------------------------------------------------------//
void OpenForBinaryReading(FileData& file);
void OpenForBinaryWriting(FileData& file);

void CloseIn(FileData& file);
void CloseOut(FileData& file);
//--------------------------------FOR-FILEDATA--------------------------------------------------------------------//
int FindAmountOfNotEmptyFiles(FileData* files, const int& from, const int& to);

bool IsEndOfTempSequences(FileData* files, const int& from, const int& to);

FileData& FindMinFileInSequences(FileData* files, const int& from, const int& to);
//--------------------------------------SORT----------------------------------------------------------------------//
void Distribution(FileData& main_file, FileData* files, const int& amount_of_files);

bool Merge(FileData* files, const int& amount_of_files, FileData& main_file);

void ExternalSort(string source, const int& amount_of_active_files);
//------------------------------------PRINT-FILE------------------------------------------------------------------//
void BINARYprint(const string& filename, int from = 1, int count = -1);
//-------------------------------------BACKUP---------------------------------------------------------------------//
void BackUp(const string& source_filename, const string& bakup_filename);
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------MAIN--------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//
int main() {
    string overwrite_from = "files\\backup.txt";
    string source = "files\\SourceBinaryFile.bin";

    ifstream file_in(source);

    if (!file_in)
    {
        cout << endl << "No file!" << endl;
    }
    else
    {
        char choice;
        cout << endl << "Want to overwrite from backup.txt before sorting?" << endl << "[Y] - for \"yes\"" << endl << "[N] - for \"no\"" << endl;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            BackUp(source, overwrite_from);
        }
        int amount_of_active_files;
        cout << endl << "Enter amount of active files: " << endl;
        cin >> amount_of_active_files;

        cout << endl << "Before sorting:" << endl;
        BINARYprint(source, 100, 10);

        cout << endl << "Start sorting" << endl;
        ExternalSort(source, amount_of_active_files);
        cout << endl << "Ready" << endl;

        cout << endl << "After sorting:" << endl;
        BINARYprint(source, 10000, 10);

        cout << endl << "Done. Press any button..." << endl;
    }

    cin.ignore();
    cin.get();
    return 0;
}
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------END-OF-MAIN-------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//

//-----------------------------------FOR-ELEMENTS-----------------------------------------------------------------//
void ReadNext(FileData& file) {
    int prev = file.elem.GetNumber();

    if (file.infile.peek() != EOF)
    {
        char symbol = ' ';
        string Number = "";
        file.infile.read(&symbol, sizeof(char));
        while (symbol >= '0' && symbol <= '9' || symbol == '-')
        {
            Number += symbol;
            file.infile.read(&symbol, sizeof(char));
        }
        file.elem.SetSymbol(symbol);
        file.elem.SetNumber(stoi(Number));

        if (prev > file.elem.GetNumber())
        {
            file.eos = true;
        }
    }
    else
    {
        file.eof = true;
        file.eos = true;
    }
}

void CopyElement(FileData& file_in, FileData& file_out) {
    string Number = to_string(file_in.elem.GetNumber());
    size_t length = Number.length();
    for (size_t i = 0; i < length; ++i)
    {
        file_out.outfile.write(&Number[i], sizeof(char));
    }
    char symbol = file_in.elem.GetSymbol();
    file_out.outfile.write(&symbol, sizeof(char));
    ReadNext(file_in);
}

void copyRun(FileData& file_in, FileData& file_out)
{
    while (!file_in.eos)
    {
        CopyElement(file_in, file_out);
    }
}
//----------------------------------OPEN-CLOSE--------------------------------------------------------------------//
void OpenForBinaryReading(FileData& file) {
    file.infile.open(file.filename, ifstream::binary);
    file.infile.seekg(0, file.infile.beg);

    if (file.infile.peek() != EOF)
    {
        char symbol = ' ';
        string Number = "";
        file.infile.read(&symbol, sizeof(char));
        while (symbol >= '0' && symbol <= '9' || symbol == '-')
        {
            Number += symbol;
            file.infile.read(&symbol, sizeof(char));
        }
        file.elem.SetSymbol(symbol);
        file.elem.SetNumber(stoi(Number));
    }
    else
    {
        file.eof = true;
        file.eos = true;
    }
}

void OpenForBinaryWriting(FileData& file) {
    file.outfile.open(file.filename, ofstream::binary);
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

    for (size_t i = 0; i < to && firstEmpty == to; ++i)
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

        ++i;
    }

    return *fd;
}
//--------------------------------------SORT----------------------------------------------------------------------//
void Distribution(FileData& main_file, FileData* files, const int& amount_of_files) {
    OpenForBinaryReading(main_file);

    for (size_t i = 0; i < amount_of_files; ++i)
    {
        OpenForBinaryWriting(files[i]);
    }

    while (!main_file.eof)
    {
        for (size_t i = 0; i < amount_of_files && !main_file.eof; ++i)
        {
            copyRun(main_file, files[i]);
            main_file.eos = main_file.eof;
        }
    }

    for (size_t i = 0; i < amount_of_files; ++i)
    {
        CloseOut(files[i]);
    }

    CloseIn(main_file);
}

bool Merge(FileData* files, const int& amount_of_files, FileData& main_file) {
    bool endOfSort = false;

    for (size_t i = 0; i < amount_of_files; ++i)
    {
        OpenForBinaryReading(files[i]);
    }

    int amountOfNotEmptyFiles = FindAmountOfNotEmptyFiles(files, 0, amount_of_files);
    if (amountOfNotEmptyFiles != 1 && amountOfNotEmptyFiles != -1)
    {
        OpenForBinaryWriting(main_file);
        while (amountOfNotEmptyFiles != -1)
        {
            while (!IsEndOfTempSequences(files, 0, amountOfNotEmptyFiles))
            {
                CopyElement(FindMinFileInSequences(files, 0, amountOfNotEmptyFiles), main_file);
            }

            for (int i = 0; i < amountOfNotEmptyFiles; ++i)
            {
                files[i].eos = files[i].eof;
            }

            amountOfNotEmptyFiles = FindAmountOfNotEmptyFiles(files, 0, amountOfNotEmptyFiles);
        }
        CloseOut(main_file);
    }
    else
    {
        endOfSort = true;
    }

    for (size_t i = 0; i < amount_of_files; ++i)
    {
        CloseIn(files[i]);
    }

    return endOfSort;
}

void ExternalSort(string source_filename, const int& amount_of_active_files) {
    if (amount_of_active_files > 1)
    {
        FileData* files = new FileData[amount_of_active_files];

        FileData main_file;
        main_file.filename = source_filename;

        for (size_t i = 0; i < amount_of_active_files; ++i)
        {
            files[i].filename = "files/f" + to_string(i + 1) + ".bin";
        }

        bool end = false;
        cout << endl << "Creating help files..." << endl;
        while (!end)
        {
            Distribution(main_file, files, amount_of_active_files);

            end = Merge(files, amount_of_active_files, main_file);
        }
        cout << endl << "Deleting help files..." << endl;
        for (size_t i = 0; i < amount_of_active_files; ++i) {
            remove(("files/f" + to_string(i + 1) + ".bin").c_str());
        }

        delete[] files;
    }
    else
    {
        cout << endl << "it doesn't work like that :)" << endl;
    }
}
//------------------------------------PRINT-FILE------------------------------------------------------------------//
void BINARYprint(const string& filename, int from, int count)
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
        cout << "\nFile is empty or unreadable in the area.\n";
    }
    file.close();
}
//-------------------------------------BACKUP---------------------------------------------------------------------------//
void BackUp(const string& source_filename, const string& bakup_filename)
{
    ifstream backup_file(bakup_filename, ifstream::binary);
    ofstream source_file(source_filename, ofstream::binary);
    backup_file.seekg(0, backup_file.beg);

    if (backup_file.peek() != EOF)
    {
        do
        {
            char symbol = ' ';
            backup_file.read(&symbol, sizeof(char));
            while (symbol >= '0' && symbol <= '9' || symbol == '-')
            {
                source_file.write(&symbol, sizeof(char));
                backup_file.read(&symbol, sizeof(char));
            }
            source_file.write(&symbol, sizeof(char));
        } while (backup_file.peek() != EOF);
    }

    source_file.close();
    backup_file.close();
}
//----------------------------------------------------------------------------------------------------------------//