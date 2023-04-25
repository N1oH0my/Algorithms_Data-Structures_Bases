//------------------------------------INCLUDE---------------------------------------------------------------------//
#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
//-------------------------------------USING----------------------------------------------------------------------//
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ios;
using std::fstream;
using std::to_string;
//------------------------------------STRUCTURES------------------------------------------------------------------//
struct TableOfIndexes
{
    int size;

    int min_file_table_index = 0;
    int amountOfNotEmptyFiles = 0;
    int amountOfNotEosFiles = 0;
    int* table;
    TableOfIndexes(const int& _size = 0)
    {
        table = new int[_size];
        for (int i = 0; i < _size; ++i)
            table[i] = i;
        size = _size;
    }
};
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

    //---------------Set/Get-----------------//
    void SetNumber(const int& _number) { number = _number; };
    void SetSymbol(const char& _symbol) { symbol = _symbol; };
    int GetNumber() { return number; };
    char GetSymbol() { return symbol; };
    //---------------------------------------//
    void RemakeMyVeryGoodPair(const int& _number, const char& _symbol) { number = _number; symbol = _symbol; };
    void RemakeMyVeryGoodPair(const MyVeryGoodPair& A) { number = A.number; symbol = A.symbol; };
    //---------------------------------------//
};

class FileData {
private:
    string filename;
    fstream file;
    bool eof = false;
    bool eos = false;
    MyVeryGoodPair elem;
public:
    FileData() {};
    ~FileData() {};
    //---------------Set/Get-----------------//
    void SetFileName(const string& _filename) { filename = _filename; }
    void SetEof(const bool& _eof) { eof = _eof; }
    void SetEos(const bool& _eos) { eos = _eos; }

    void SetPair(MyVeryGoodPair& pair) { elem.RemakeMyVeryGoodPair(pair); }
    void SetPair(const int& _number, const char& _symbol) { elem.RemakeMyVeryGoodPair(_number, _symbol); }

    string GetFilename() { return filename; }
    bool GetEof() { return eof; }
    bool GetEos() { return eos; }
    MyVeryGoodPair& GetPair() { return elem; }
    //---------------------------------------//

    //-------------File-Functions------------//
    void FileClose() { file.close(); }
    void InFileOpen() { file.open(filename, ios::binary | ios::in); }
    void OutFileOpen() { file.open(filename, ios::binary | ios::out); }

    bool ReadElem() { return (bool)file.read((char*)&elem, sizeof(MyVeryGoodPair)); }
    bool WriteElem(const MyVeryGoodPair& pair) { return (bool)file.write((char*)&pair, sizeof(MyVeryGoodPair)); }

    void SeekgStart() { file.seekg(0, file.beg); }
    void SeekgEnd() { file.seekg(0, file.end); }

    auto InFilePeek() { return file.peek(); }
    bool IsOpen() { return file.is_open(); }
    //---------------------------------------//
};

//------------------------------------PRINT-FILE------------------------------------------------------------------//
void BINARYprint(const string& filename, int from = 1, int count = -1);
//-----------------------------------FOR-ELEMENTS-----------------------------------------------------------------//
void ReadNext(FileData* file);

void CopyElement(FileData* file_in, FileData* file_out, TableOfIndexes& T);

void copyRun(FileData* file_in, FileData* file_out, TableOfIndexes& T);
//----------------------------------OPEN-CLOSE--------------------------------------------------------------------//
void OpenForBinaryReading(FileData* file);
void OpenForBinaryWriting(FileData* file);

void CloseIn(FileData* file);
void CloseOut(FileData* file);
//--------------------------------FOR-FILEDATA--------------------------------------------------------------------//
void FindMinFileInSequences(FileData** files, const int& from, const int& to, TableOfIndexes& T);
//--------------------------------------SORT----------------------------------------------------------------------//
void Distribution(FileData* main_file, FileData** files, const int& amount_of_files, TableOfIndexes& T);

void Merge(FileData** files, TableOfIndexes& T, FileData* main_file);

void ExternalSort(string source, const int& amount_of_active_files);
//-------------------------------------BACKUP---------------------------------------------------------------------//
void GenerateBackUp(const string& bakup_filename, const int& N);
void BackUp(const string& source_filename, const string& bakup_filename);
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------MAIN--------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//
int main() {
    srand(time(NULL));
    string overwrite_from = "files\\backup.txt";
    string source = "files\\SourceBinaryFile.bin";

    fstream file_in(source);

    if (!file_in)
    {
        cout << endl << "No file!" << endl;
    }
    else
    {
        //---------------------------------------------------------------------//
        char choice;
        cout << endl << "Want to regenerate backup.txt?" << endl << "[Y] - for \"yes\"" << endl << "[N] - for \"no\"" << endl;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            GenerateBackUp(overwrite_from, 9);
            //BINARYprint(overwrite_from);
        }
        //---------------------------------------------------------------------//
        cout << endl << "Want to overwrite from backup.txt before sorting?" << endl << "[Y] - for \"yes\"" << endl << "[N] - for \"no\"" << endl;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
            BackUp(source, overwrite_from);
        //---------------------------------------------------------------------//

        int amount_of_active_files;
        cout << endl << "Enter amount of active files: " << endl;
        cin >> amount_of_active_files;
        //---------------------------------------------------------------------//

        cout << endl << "Before sorting:" << endl;
        BINARYprint(source);

        cout << endl << "Start sorting" << endl;
        ExternalSort(source, amount_of_active_files);
        cout << endl << "Ready" << endl;

        //---------------------------------------------------------------------//
        cout << endl << "After sorting:" << endl;
        BINARYprint(source);

        cout << endl << "Done. Press any button..." << endl;
        //---------------------------------------------------------------------//
    }

    cin.ignore();
    cin.get();
    return 0;
}
//----------------------------------------------------------------------------------------------------------------//
//----------------------------------------END-OF-MAIN-------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------//

//-----------------------------------FOR-ELEMENTS-----------------------------------------------------------------//
void ReadNext(FileData* file) {
    int prev = file->GetPair().GetNumber();
    MyVeryGoodPair pair;
    if (file->InFilePeek() != EOF)
    {
        file->ReadElem();

        if (prev > file->GetPair().GetNumber())
            file->SetEos(true);
    }
    else
    {
        file->SetEof(true);
        file->SetEos(true);
    }
}

void CopyElement(FileData* file_in, FileData* file_out, TableOfIndexes& T) {
    file_out->WriteElem(file_in->GetPair());

    ReadNext(file_in);

    if (file_in->GetEos())
    {
        if (T.amountOfNotEosFiles - 1 != T.min_file_table_index)
            std::swap(T.table[T.amountOfNotEosFiles - 1], T.table[T.min_file_table_index]);
        --T.amountOfNotEosFiles;
    }
    if (file_in->GetEof())
    {
        if (T.amountOfNotEosFiles != T.amountOfNotEmptyFiles - 1)
            std::swap(T.table[T.amountOfNotEosFiles], T.table[T.amountOfNotEmptyFiles - 1]);
        --T.amountOfNotEmptyFiles;
    }
}

void copyRun(FileData* file_in, FileData* file_out, TableOfIndexes& T)
{
    while (!file_in->GetEos())
    {
        file_out->WriteElem(file_in->GetPair());
        ReadNext(file_in);
    }
    T.amountOfNotEmptyFiles += 1;
}
//----------------------------------OPEN-CLOSE--------------------------------------------------------------------//
void OpenForBinaryReading(FileData* file) {
    file->InFileOpen();
    file->SeekgStart();
    MyVeryGoodPair pair;

    if (file->InFilePeek() != EOF)
    {
        file->ReadElem();
    }
    else
    {
        file->SetEof(true);
        file->SetEos(true);
    }
}

void OpenForBinaryWriting(FileData* file) {
    file->OutFileOpen();
}
void CloseIn(FileData* file) {
    file->SetEof(false);
    file->SetEos(false);
    file->FileClose();
}
void CloseOut(FileData* file) {
    file->SetEof(false);
    file->SetEos(false);
    file->FileClose();
}
//--------------------------------FOR-FILEDATA--------------------------------------------------------------------//

void FindMinFileInSequences(FileData** files, const int& from, const int& to, TableOfIndexes& T) {
    FileData* fd = files[T.table[from]];
    int i = from + 1;
    T.min_file_table_index = from;

    while (i < to)
    {
        if (files[T.table[i]]->GetPair().GetNumber() < fd->GetPair().GetNumber())
        {
            fd = files[T.table[i]];
            T.min_file_table_index = i;
        }
        ++i;
    }
}
//--------------------------------------SORT----------------------------------------------------------------------//
void Distribution(FileData* main_file, FileData** files, const int& amount_of_files, TableOfIndexes& T) {
    OpenForBinaryReading(main_file);
    for (size_t i = 0; i < amount_of_files; ++i)
        OpenForBinaryWriting(files[T.table[i]]);

    while (!main_file->GetEof())
    {
        for (size_t i = 0; i < amount_of_files && !main_file->GetEof(); ++i)
        {
            copyRun(main_file, files[T.table[i]], T);
            main_file->SetEos(main_file->GetEof());
        }
    }

    for (size_t i = 0; i < amount_of_files; ++i)
        CloseOut(files[T.table[i]]);
    CloseIn(main_file);
}

void Merge(FileData** files, TableOfIndexes& T, FileData* main_file) {
    for (size_t i = 0; i < T.size; ++i)
        OpenForBinaryReading(files[i]);
    OpenForBinaryWriting(main_file);

    while (T.amountOfNotEmptyFiles != 0)
    {
        T.amountOfNotEosFiles = T.amountOfNotEmptyFiles;
        while (T.amountOfNotEosFiles != 0)
        {
            FindMinFileInSequences(files, 0, T.amountOfNotEosFiles, T);
            CopyElement(files[T.table[T.min_file_table_index]], main_file, T);
        }

        for (int i = 0; i < T.amountOfNotEmptyFiles; ++i)
            files[T.table[i]]->SetEos(files[T.table[i]]->GetEof());
    }

    CloseOut(main_file);
    for (size_t i = 0; i < T.size; ++i)
        CloseIn(files[i]);
}

void ExternalSort(string source_filename, const int& amount_of_active_files) {
    if (amount_of_active_files > 1)
    {
        FileData** files = new FileData * [amount_of_active_files];
        for (size_t i = 0; i < amount_of_active_files; ++i)
            files[i] = new FileData;

        FileData* main_file = new FileData;
        main_file->SetFileName(source_filename);

        for (size_t i = 0; i < amount_of_active_files; ++i)
            files[i]->SetFileName("files/f" + to_string(i + 1) + ".bin");

        bool end = false;
        TableOfIndexes T(amount_of_active_files);
        cout << endl << "Creating help files..." << endl;
        while (!end)
        {
            Distribution(main_file, files, amount_of_active_files, T);
            /*for (size_t i = 0; i < amount_of_active_files; ++i)
            {
                string filenamee = files[i]->GetFilename();
                cout << filenamee << endl;
                BINARYprint(filenamee);
            }*/
            if (T.amountOfNotEmptyFiles <= amount_of_active_files)
            {
                end = true;
                if (T.amountOfNotEmptyFiles != 0 && T.amountOfNotEmptyFiles != 1)
                    Merge(files, T, main_file);
            }
            else
            {
                T.amountOfNotEmptyFiles = amount_of_active_files;
                Merge(files, T, main_file);
            }
        }
        cout << endl << "Deleting help files..." << endl;
        for (size_t i = 0; i < amount_of_active_files; ++i)
            remove(("files/f" + to_string(i + 1) + ".bin").c_str());

        for (size_t i = 0; i < amount_of_active_files; ++i)
            delete files[i];
        delete[] files;
        delete main_file;
    }
    else
    {
        cout << endl << "it doesn't work like that :)" << endl;
    }
}
//------------------------------------PRINT-FILE------------------------------------------------------------------//
void BINARYprint(const string& filename, int from, int count)
{
    fstream file(filename, ios::binary | ios::in);
    file.seekg(0, file.beg);
    MyVeryGoodPair pair;
    --from;
    if (file.peek() != EOF && from != 0)
    {
        do
        {
            file.read((char*)&pair, sizeof(MyVeryGoodPair));
            --from;
        } while (from != 0 && file.peek() != EOF);
    }

    if (file.peek() != EOF)
    {
        const int step = ((count == -1) ? 0 : -1);
        do
        {
            file.read((char*)&pair, sizeof(MyVeryGoodPair));
            cout << pair.GetNumber() << ' ' << pair.GetSymbol() << endl;
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
void GenerateBackUp(const string& bakup_filename, const int& N)
{
    int A = -999;
    int B = 999;
    fstream file(bakup_filename, ios::binary | ios::out);

    for (size_t i = 0; i < N; ++i)
    {
        /*pair.number = A + rand() % ((B + 1) - A);
        pair.symbol = 'a' + rand() % 26;*/
        MyVeryGoodPair pair(A + rand() % ((B + 1) - A), 'a' + rand() % 26);
        file.write((char*)(&pair), sizeof(pair));
    }
    file.close();
}
void BackUp(const string& source_filename, const string& bakup_filename)
{
    fstream backup_file(bakup_filename, ios::binary | ios::in);
    fstream source_file(source_filename, ios::binary | ios::out);
    backup_file.seekg(0, backup_file.beg);
    MyVeryGoodPair pair;
    if (backup_file.peek() != EOF)
    {
        do
        {
            backup_file.read((char*)&pair, sizeof(MyVeryGoodPair));
            source_file.write((char*)&pair, sizeof(pair));
        } while (backup_file.peek() != EOF);
    }

    source_file.close();
    backup_file.close();
}
//----------------------------------------------------------------------------------------------------------------//