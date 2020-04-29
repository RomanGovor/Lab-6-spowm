#include <stdio.h>
#include <iostream>
#include <string.h>
#include <conio.h>

using namespace std;

#define All_memory 32768  

int index;
int memorySize;
void* indexAmount[64];

char Array_total_memory[All_memory];                          // ����� ������ ������
char* Block_buffer[BUFSIZ];
char* activeBuffer[BUFSIZ];

unsigned int LastBlockBuffer;
unsigned int Buffer_counter_of_blocks = 1;
unsigned int Active_blocks_amount = 0;                        // ����� �������� ������
unsigned int Size_of_block[BUFSIZ];
unsigned int blockBufferSize[BUFSIZ];
unsigned long Available_memory = All_memory;                  // ��������� ������ 

int getInt(int limit);
void Menu();
void ShowInformation();
void Initialization();
int _free(void* memoryBlock);
void* _malloc(unsigned long size);
void* _realloc(void* memory_block, int size);               // 

int main()
{
    setlocale(LC_ALL, "Russian");
    char command;
    Initialization();
    while (true)
    {
        Menu();
        command = _getch();

        switch (command) 
        {
        case '1':
        {
            system("cls");  
            cout << "������� ������ � ��������� [1..64]: ";
            int index  = getInt(64);                       // ������ ����� ��� ��������� ������

            if (indexAmount[index] == NULL)                 // ���� ���� �� �����
            {
                cout << "������� ������ ���������� ������: ";
                int memorySize = getInt(All_memory);       // ������ ���������� ������

                indexAmount[index] = _malloc(memorySize);
                if (indexAmount[index] == NULL)
                {
                    cout << endl << "������: ������������ ������" << endl;
                    rewind(stdin);
                }
                else
                {
                    cout << "-----------------------------------" << endl;
                    cout << "������: " << index << endl;
                    cout << "������� ������: " << Active_blocks_amount << endl;
                    cout << "�������� ������: " << memorySize << endl;
                }
            }
            else
            {
                cout << "������: ������ ���� ��� ������������" << endl;
                system("pause");
                break;
            }
            break;
        }
        case '2':
        {
            system("cls");
            cout << "������� ������ � ��������� [1..64]: ";
            int index = getInt(64);                       // ������ ����� ��� ������������ ������
           
            if (_free(indexAmount[index]) == -1)           // ���� ���� �� ������
            {
                cout << endl <<"������: ���� �� ������" << endl;
                cout << "������� ������: " << Active_blocks_amount << endl;
                break;
            }
            cout << "������ �����������" << endl;
            cout << "������� ������: " << Active_blocks_amount << endl;
            break;
        }
        case '3':
        {
            system("cls");
            cout << "������� ������ � ��������� [1..64]: ";
            int index = getInt(64);                       // ������ ����� ��� ������������ ������

            if (indexAmount[index] == NULL)
            {
                cout << "������: ���� �� ������" << endl;
                break;
            }

            cout << "������� ������ ���������� ������: ";
            int memorySize = getInt(All_memory);       // ������ ���������� ������

            indexAmount[index] = _realloc(indexAmount[index], memorySize); // ������������� ������ 
            cout << "������� ������: " << Active_blocks_amount << endl;
            cout << "������ ��������������" << endl;
            break;
        }
        case '4':
        {
            ShowInformation();
            break;
        }
        case '5':
        {
            return 0;
        }
        default:
        {
            system("cls");
            break;
        }
        }
    }
}

int getInt(int limit)                                      // �������� �� ��������� �������
{
    int q;
    while (!(cin >> q) || q < 1 || q >limit ){
        rewind(stdin); 
        cout << "������������ ��������. �������� ������� �������� " << endl;      
    } 
    return q;
}

void Menu()                                                // �������� ����
{
    cout << "-----------------------------------" << endl;
    cout << "�������� �������:" << endl << endl;
    cout << "1. �������� ������ (malloc)" << endl;
    cout << "2. ���������� ������ (free)" << endl;
    cout << "3. �������������� ������ (realloc)" << endl;
    cout << "4. �������� ���������� � ������" << endl;
    cout << "5. ����� �� ���������" << endl;
    cout << "-----------------------------------" << endl;
}

void ShowInformation()                                     // ���������� � ������� ������
{
    system("cls");
    cout << "���������� � ������" << endl;
    cout << "-----------------------------------" << endl;
    cout << "����� ��������� ������:" << endl;
    cout << Available_memory << endl;
    cout << "���������� ������� ������:" << endl;
    cout << Active_blocks_amount << endl;
}

void Initialization()                                       // ��������� �������� ������
{
    Block_buffer[0] = Array_total_memory;
    blockBufferSize[0] = Available_memory;                  // ������������ ����������� ��������� ������
}

void* _malloc(unsigned long size)                           // ��������� ������
{
    char* tempBuffer = NULL;
    unsigned long int endBlock;

    if (size > Available_memory)                            // ���� ������ ���������� ������ ������ ����������
    {
        cout << endl << "������: �������� ������ ��������� ������" << endl;
        rewind(stdin);
        return tempBuffer;
    }

    for (int i = 0; i < Buffer_counter_of_blocks; i++)      
    {
        if (size <= blockBufferSize[i])                     // ���� � ����� ������ ���������� ������ ������
        {
            tempBuffer = Block_buffer[i];                  // ������ �������� ����� ������
            endBlock = i;                                 // ����������� ������� �����
            break;
        }
    }

    if (!tempBuffer)                                       // ���� ���� ������ ����
        return NULL;

    activeBuffer[Active_blocks_amount] = tempBuffer;      // ������ �������� ������ � ����� ������
    Size_of_block[Active_blocks_amount] = size;             // ������ ���������� ������
    Active_blocks_amount++;
    LastBlockBuffer++;                                    // ����� ����������� �����
    Block_buffer[endBlock] = (char*)(endBlock + size + 1);// ������ ������
    Block_buffer[endBlock] = Block_buffer[endBlock] - size;
    Available_memory -= size;                                // �������� ��������� ������ 

    return tempBuffer;
}

int _free(void* Deleting_memory)                            // ������������ ������
{
    char* tempBuffer = NULL;
    unsigned int endBlock;

    for (int i = 0; i < LastBlockBuffer; i++)             // ���������� ���������� ������
    {
        if (Deleting_memory == activeBuffer[i])            // ���� � �������� ����� �������� ���������� ������
        {
            tempBuffer = activeBuffer[i];                 // ������ ������
            endBlock = i;
            break;
        }
    }

    if (!tempBuffer)
        return -1;

    activeBuffer[endBlock] = 0;                          // ��������� ������
    Active_blocks_amount--;                                 // ��������� ����� ���������� ������
    Block_buffer[Buffer_counter_of_blocks] = (char*)Deleting_memory;// ����������� ��������� ������������� ������
    blockBufferSize[Buffer_counter_of_blocks] = Size_of_block[endBlock];
    Buffer_counter_of_blocks++;                             // ����������������� ����� ������ ��������� ������ 
    Available_memory += Size_of_block[endBlock];          // ����������� ������ � ����������� ����� 

    return 0;
}

void* _realloc(void* index, int size)                      // ������������� ������ 
{
    char* tempBuffer = 0;
    int i, endBlock;

    for (i = 0; i < LastBlockBuffer; i++)                  // ���������� ���������� ������
    {
        if (index == activeBuffer[i])                      // ���� � �������� ����� �������� ���������� ������
        {
            if (activeBuffer[i] == NULL)                   // ���� �� ������� ������ �� ��������, �� ��������� ������ 
                return _malloc(size);
            else
            {
                tempBuffer = activeBuffer[i];             // ������ �������� �������� ������ � ������ �����
                endBlock = i;
                break;
            }
        }
    }

    if (!tempBuffer)
        return NULL;

    _free(index);
    return _malloc(size);
}