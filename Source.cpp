#include <stdio.h>
#include <iostream>
#include <string.h>
#include <conio.h>

using namespace std;

#define All_memory 32768  

int index;
int memorySize;
void* indexAmount[64];

char Array_total_memory[All_memory];                          // Общий массив памяти
char* Block_buffer[BUFSIZ];
char* activeBuffer[BUFSIZ];

unsigned int LastBlockBuffer;
unsigned int Buffer_counter_of_blocks = 1;
unsigned int Active_blocks_amount = 0;                        // Число активных блоков
unsigned int Size_of_block[BUFSIZ];
unsigned int blockBufferSize[BUFSIZ];
unsigned long Available_memory = All_memory;                  // Доступная память 

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
            cout << "Введите индекс в диапазоне [1..64]: ";
            int index  = getInt(64);                       // Индекс блока для выделения памяти

            if (indexAmount[index] == NULL)                 // Если блок не занят
            {
                cout << "Введите размер выделяемой памяти: ";
                int memorySize = getInt(All_memory);       // Размер выделяемой памяти

                indexAmount[index] = _malloc(memorySize);
                if (indexAmount[index] == NULL)
                {
                    cout << endl << "Ошибка: недостаточно памяти" << endl;
                    rewind(stdin);
                }
                else
                {
                    cout << "-----------------------------------" << endl;
                    cout << "Индекс: " << index << endl;
                    cout << "Занятых блоков: " << Active_blocks_amount << endl;
                    cout << "Выделено памяти: " << memorySize << endl;
                }
            }
            else
            {
                cout << "Ошибка: данный блок уже используется" << endl;
                system("pause");
                break;
            }
            break;
        }
        case '2':
        {
            system("cls");
            cout << "Введите индекс в диапазоне [1..64]: ";
            int index = getInt(64);                       // Индекс блока для освобождения памяти
           
            if (_free(indexAmount[index]) == -1)           // Если блок не найден
            {
                cout << endl <<"Ошибка: блок не найден" << endl;
                cout << "Занятых блоков: " << Active_blocks_amount << endl;
                break;
            }
            cout << "Память освобождена" << endl;
            cout << "Занятых блоков: " << Active_blocks_amount << endl;
            break;
        }
        case '3':
        {
            system("cls");
            cout << "Введите индекс в диапазоне [1..64]: ";
            int index = getInt(64);                       // Индекс блока для освобождения памяти

            if (indexAmount[index] == NULL)
            {
                cout << "Ошибка: блок не найден" << endl;
                break;
            }

            cout << "Введите размер выделяемой памяти: ";
            int memorySize = getInt(All_memory);       // Размер выделяемой памяти

            indexAmount[index] = _realloc(indexAmount[index], memorySize); // Перевыделение памяти 
            cout << "Занятых блоков: " << Active_blocks_amount << endl;
            cout << "Память переопределена" << endl;
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

int getInt(int limit)                                      // Проверка на введенное значние
{
    int q;
    while (!(cin >> q) || q < 1 || q >limit ){
        rewind(stdin); 
        cout << "Некорректное значение. Повторно введите значения " << endl;      
    } 
    return q;
}

void Menu()                                                // Основное меню
{
    cout << "-----------------------------------" << endl;
    cout << "Выберите команду:" << endl << endl;
    cout << "1. Выделить память (malloc)" << endl;
    cout << "2. Освободить память (free)" << endl;
    cout << "3. Переопределить память (realloc)" << endl;
    cout << "4. Показать информацию о памяти" << endl;
    cout << "5. Выход из программы" << endl;
    cout << "-----------------------------------" << endl;
}

void ShowInformation()                                     // Информация о занятых блоках
{
    system("cls");
    cout << "Информация о памяти" << endl;
    cout << "-----------------------------------" << endl;
    cout << "Объем свободной памяти:" << endl;
    cout << Available_memory << endl;
    cout << "Количество занятых блоков:" << endl;
    cout << Active_blocks_amount << endl;
}

void Initialization()                                       // Начальные значения данных
{
    Block_buffer[0] = Array_total_memory;
    blockBufferSize[0] = Available_memory;                  // Записывается максимально доступная память
}

void* _malloc(unsigned long size)                           // Выделение памяти
{
    char* tempBuffer = NULL;
    unsigned long int endBlock;

    if (size > Available_memory)                            // Если размер выделяемой памяти больше доступного
    {
        cout << endl << "Ошибка: превышен размер свободной памяти" << endl;
        rewind(stdin);
        return tempBuffer;
    }

    for (int i = 0; i < Buffer_counter_of_blocks; i++)      
    {
        if (size <= blockBufferSize[i])                     // Если в блоке памяти выделенная память больше
        {
            tempBuffer = Block_buffer[i];                  // Запись текущего блока памяти
            endBlock = i;                                 // Запоминание индекса блока
            break;
        }
    }

    if (!tempBuffer)                                       // Если блок памяти пуст
        return NULL;

    activeBuffer[Active_blocks_amount] = tempBuffer;      // Запись активной памяти в конец буфера
    Size_of_block[Active_blocks_amount] = size;             // Запись выделяемой памяти
    Active_blocks_amount++;
    LastBlockBuffer++;                                    // Номер записанного блока
    Block_buffer[endBlock] = (char*)(endBlock + size + 1);// Запись памяти
    Block_buffer[endBlock] = Block_buffer[endBlock] - size;
    Available_memory -= size;                                // Пересчет доступной памяти 

    return tempBuffer;
}

int _free(void* Deleting_memory)                            // Освобождение памяти
{
    char* tempBuffer = NULL;
    unsigned int endBlock;

    for (int i = 0; i < LastBlockBuffer; i++)             // Количество записанных блоков
    {
        if (Deleting_memory == activeBuffer[i])            // Если в активном блоке записана выделенная память
        {
            tempBuffer = activeBuffer[i];                 // Запись памяти
            endBlock = i;
            break;
        }
    }

    if (!tempBuffer)
        return -1;

    activeBuffer[endBlock] = 0;                          // Обнуление бафера
    Active_blocks_amount--;                                 // Декремент числа записанных блоков
    Block_buffer[Buffer_counter_of_blocks] = (char*)Deleting_memory;// Запоминание последней освобожденной памяти
    blockBufferSize[Buffer_counter_of_blocks] = Size_of_block[endBlock];
    Buffer_counter_of_blocks++;                             // Инкрементирование числа блоков последней памяти 
    Available_memory += Size_of_block[endBlock];          // Возвращение памяти с занимаемого блока 

    return 0;
}

void* _realloc(void* index, int size)                      // Перевыделение памяти 
{
    char* tempBuffer = 0;
    int i, endBlock;

    for (i = 0; i < LastBlockBuffer; i++)                  // Количество записанных блоков
    {
        if (index == activeBuffer[i])                      // Если в активном блоке записана выделенная память
        {
            if (activeBuffer[i] == NULL)                   // Если по индексу ничего не записано, то выделение памяти 
                return _malloc(size);
            else
            {
                tempBuffer = activeBuffer[i];             // Запись текущего значения памяти в нужном блоке
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