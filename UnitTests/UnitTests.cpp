// UnitTests.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <ctime>
#include "AESEncryptor.h"
#include "ColorSpaceConverter.h"
#include "EmbeddingModule.h"
#include "MultiplexerLibrary.h"
#include "SoundGenerateLibrary.h"
#include "SourceFileLoad.h"

using namespace std;

void TestAudioGeneration()
{
    FILE* f;
    cout << "Test SoundGenerateLibrary\n\n";

    BYTE header[44] =
    {
        0x52, 0x49, 0x46, 0x46, 0x24, 0x00, 0xFF, 0x7F, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
        0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x44, 0xAC, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00,
        0x01, 0x00, 0x08, 0x00, 0x64, 0x61, 0x74, 0x61, 0x00, 0xC3, 0x0C, 0x00
    };

    cout << "Generate wave with frequncy = 400" << endl;
    BYTE* WAVE = GenerateSound(0, 400);
    f = fopen("C:/TestAudio/w400.wav", "wb");
    try
    {
        fwrite(header, sizeof(BYTE), 44, f);
        fwrite(WAVE, sizeof(BYTE), 44100, f);
    }
    catch (exception)
    {
        cout << "Generation failed!\n";
        fclose(f);
        return;
    }

    fclose(f);
    cout << "Success!\n\n";

    cout << "Generate wave with frequncy = 1000" << endl;
    WAVE = GenerateSound(0, 1000);
    f = fopen("C:/TestAudio/w1000.wav", "wb");
    try
    {
        fwrite(header, sizeof(BYTE), 44, f);
        fwrite(WAVE, sizeof(BYTE), 44100, f);
    }
    catch (exception)
    {
        cout << "Generation failed!\n";
        fclose(f);
        return;
    }

    fclose(f);
    cout << "Success!\n\n";

    cout << "Generate wave with frequncy = 2000" << endl;
    WAVE = GenerateSound(0, 2000);
    f = fopen("C:/TestAudio/w2000.wav", "wb");
    try
    {
        fwrite(header, sizeof(BYTE), 44, f);
        fwrite(WAVE, sizeof(BYTE), 44100, f);
    }
    catch (exception)
    {
        cout << "Generation failed!\n";
        fclose(f);
        return;
    }

    fclose(f);
    cout << "Success!\n\n";

    cout << "Generate noise" << endl;
    WAVE = GenerateSound(5, 400);
    f = fopen("C:/TestAudio/noise.wav", "wb");
    try
    {
        fwrite(header, sizeof(BYTE), 44, f);
        fwrite(WAVE, sizeof(BYTE), 44100, f);
    }
    catch (exception)
    {
        cout << "Generation failed!\n";
        fclose(f);
        return;
    }

    fclose(f);
    cout << "Success!\n\n";
}

int TestMultiplexer()
{
    FILE* f;
    //cout << "Test SourceFileLoad\n\n";
    //cout << "Test ColorSpaceConverter\n\n";
    cout << "Test MultiplexerLibrary\n\n";

    cout << "File path: C:/RowDataCubeColor/\n";
    cout << "Expected Parameters: ";
    cout << "Number of frames: 50\n";
    cout << "Widht = 352\n";
    cout << "Heidht = 288\n";
    cout << "Avi size = 10227932\n";

    BYTE* RGBdata = LoadSourceData("C:/RowDataCubeColor/", 50);
    BYTE* YUYVdata = ConvertColorSpace(RGBdata);

    BYTE* AudioData = new BYTE[88200];
    BYTE* WAVE = GenerateSound(0, 300);
    for (int i = 0; i < 44100; i++)
    {
        AudioData[i] = WAVE[i];
        AudioData[i + 44100] = WAVE[i];
    }

    BYTE* AVI = GetAVIStream(YUYVdata, AudioData);

    UINT32 number = AVI[48] + (AVI[49] << 8) + (AVI[50] << 16) + (AVI[51] << 24);
    UINT32 widht = AVI[64] + (AVI[65] << 8) + (AVI[66] << 16) + (AVI[67] << 24);
    UINT32 heidht = AVI[68] + (AVI[69] << 8) + (AVI[70] << 16) + (AVI[71] << 24);
    int size = AVI[4] + (AVI[5] << 8) + (AVI[6] << 16) + (AVI[7] << 24) + 8;

    cout << "Received Parameters: ";
    cout << "Number of frames: " << number << endl;
    cout << "Widht = " << widht << endl;
    cout << "Heidht = " << heidht << endl;
    cout << "Avi size = " << size << endl;

    bool isSuccess = true;
    if (number != 50) isSuccess = false;
    if (widht != 352) isSuccess = false;
    if (heidht != 288) isSuccess = false;
    if (size != 10227932) isSuccess = false;

    if (isSuccess)
        cout << "Success!\n\n";
    else
    {
        cout << "Failed!\n\n";
        goto NEXTTEST;
    }

    cout << "Writting to file C:/RowDataCubeColor/result.avi" << endl;
    f = fopen("C:/RowDataCubeColor/result.avi", "wb");
    try
    {
        fwrite(AVI, sizeof(BYTE), size, f);
    }
    catch (exception e)
    {
        cout << "Writting to file C:/RowDataCubeColor/result.avi : Failed!\n" << endl;
        goto NEXTTEST;
    }

    cout << "Writting to file C:/RowDataCubeColor/result.avi : Success!\n" << endl;
    fclose(f);

NEXTTEST:
    cout << "File path: C:/RowDataCubeHD/\n";
    cout << "Expected Parameters: ";
    cout << "Number of frames: 25\n";
    cout << "Widht = 1280\n";
    cout << "Heidht = 720\n";
    cout << "Avi size = 46125432\n"; ;

    RGBdata = LoadSourceData("C:/RowDataCubeHD/", 25);
    YUYVdata = ConvertColorSpace(RGBdata);

    AVI = GetAVIStream(YUYVdata, AudioData);

    number = AVI[48] + (AVI[49] << 8) + (AVI[50] << 16) + (AVI[51] << 24);
    widht = AVI[64] + (AVI[65] << 8) + (AVI[66] << 16) + (AVI[67] << 24);
    heidht = AVI[68] + (AVI[69] << 8) + (AVI[70] << 16) + (AVI[71] << 24);
    size = AVI[4] + (AVI[5] << 8) + (AVI[6] << 16) + (AVI[7] << 24) + 8;

    cout << "Received Parameters: ";
    cout << "Number of frames: " << number << endl;
    cout << "Widht = " << widht << endl;
    cout << "Heidht = " << heidht << endl;
    cout << "Avi size = " << size << endl;

    isSuccess = true;
    if (number != 25) isSuccess = false;
    if (widht != 1280) isSuccess = false;
    if (heidht != 720) isSuccess = false;
    if (size != 46125432) isSuccess = false;

    if (isSuccess)
        cout << "Success!\n\n";
    else
    {
        cout << "Failed!\n\n";
        return -1;
    }

    cout << "Writting to file C:/RowDataCubeHD/result.avi" << endl;
    f = fopen("C:/RowDataCubeHD/result.avi", "wb");
    try
    {
        fwrite(AVI, sizeof(BYTE), size, f);
    }
    catch (exception)
    {
        cout << "Writting to file C:/RowDataCubeHD/result.avi : Failed!" << endl;
        return -1;
    }

    cout << "Writting to file C:/RowDataCubeHD/result.avi : Success!" << endl;
    fclose(f);
}

void TestAESEncryptor()
{
    cout << hex;
    BYTE* key = new BYTE[16];
    BYTE* OpenText = new BYTE[32];
    BYTE* ClosedText;
    BYTE* DecryptText;
    UINT32 sz = 32;
    cout << "Generating open text: \n";
    for (int i = 0; i < 32; i++)
    {
        OpenText[i] = (BYTE)rand();
        cout << (int)OpenText[i] << " ";
    }
    cout << "\nGenerating key: \n";
    for (int i = 0; i < 16; i++)
    {
        key[i] = (BYTE)rand();
        cout << (int)key[i] << " ";
    }
    cout << "\nEncryption...\n";
    ClosedText = AESEncryption(OpenText, key, sz);
    cout << "Encryption result:\n";
    for (int i = 0; i < 32; i++)
    {
        cout << (int)ClosedText[i] << " ";
    }
    cout << "\nDecryption...\n";
    DecryptText = AESDecryption(ClosedText, key, sz);
    cout << "Decryption result:\n";
    for (int i = 0; i < 32; i++)
    {
        cout << (int)DecryptText[i] << " ";
    }
    cout << "\nTest result: ";
    bool isSuccess = true;
    for (int i = 0; i < 32; i++)
    {
        if (OpenText[i] != DecryptText[i]) isSuccess = false;
    }
    if (isSuccess) cout << "Success!\n\n";
    else cout << "Failed!\n\n";
    cout << dec;
}

void TestEmbeddingModule()
{
    BYTE* ContainerData = new BYTE[100];
    BYTE* EmData = new BYTE[3];
    BYTE* Buffer = new BYTE[3];
    BYTE* ReceivedData;
    UINT16 sz = 3;
    cout << hex;
    cout << "Generating container data: \n";
    for (int i = 0; i < 99; i++)
    {
        ContainerData[i] = (BYTE)rand();
        cout << (int)ContainerData[i];
    }
    cout << "\nGenerating embedding data: \n";
    for (int i = 0; i < 3; i++)
    {
        EmData[i] = (BYTE)rand();
        Buffer[i] = EmData[i];
        cout << (int)EmData[i] << " ";
    }
    cout << "\nEmbedding...\n";
    ContainerData = EmbedData(ContainerData, EmData, sz);
    cout << "Embedding result:\n";
    for (int i = 0; i < 99; i++)
    {
        cout << (int)ContainerData[i];
    }
    cout << "\nGetting data...\n";
    ReceivedData = GetData(ContainerData, sz);
    cout << "Getting result:\n";
    bool isSuccess = true;
    if (sz != 3)
    {
        isSuccess = false;
        cout << "Wrong data size\nTest result: Failed!\n\n";
        return;
    }
    for (int i = 0; i < sz; i++)
    {
        cout << (int)ReceivedData[i] << " ";
        if (ReceivedData[i] != Buffer[i]) isSuccess = false;
    }
    cout << "\nTest result: ";
    if (isSuccess)
        cout << "Success!\n\n";
    else
        cout << "Failed!\n\n";
}

int main()
{
    srand(time(0));
    cout << "Test EmbeddingModule\n\n";

    for (int i = 0; i < 10; i++)
    {
        cout << "Test #" << i << endl;
        TestEmbeddingModule();
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
