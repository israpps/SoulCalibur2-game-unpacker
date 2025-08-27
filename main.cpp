#include <iostream>
#include <cstdint>
#include <string.h>
#include <stdio.h>

#define eprintf(fmt, arg...) fprintf(stderr, fmt, ##arg) // error printf

#define BLOB_SIZE 1024 * 418
#define TARGET_SIZE 854928
#define SOURCE_FILE argv[1]          //"../PS2AC05"
#define DESTINATION_FILE argv[2]     //"../PS2AC05.rle"
#define DESTINATION_RLE_FILE argv[3] //"../PS2AC05.decompressed"

#define byte uint8_t
#define uint uint32_t
#define ulong uint64_t
#define ushort uint16_t

uint16_t CONCAT11(uint8_t x, uint8_t y)
{
    return (((uint16_t)x) << 8) | (uint8_t)y;
}

uint32_t CONCAT21(uint16_t x, uint8_t y)
{
    return ((uint32_t)x << 8) | (uint8_t)y;
}

uint32_t CONCAT31(uint32_t x, uint8_t y)
{
    return (x << 8) | (uint8_t)y;
}

char version[] = "VersionA";

uint SoulCaliburDecrypt(byte *out_data, byte *in_data, int data_length)
{
    byte bVar1;
    ushort uVar2;
    uint uVar3;
    uint uVar4;
    ulong uVar5;
    ulong uVar6;
    ushort *puVar7;
    int iVar8;
    ulong uVar9;
    int iVar10;
    byte *pbVar11;
    ulong uVar12;
    uint uVar13;

    iVar8 = data_length + -0xb;
    uVar5 = 0xebd7; // 0xebd7;
    uVar13 = (uint) * (ushort *)(in_data + iVar8);
    uVar12 = 0xa21f; // 0xa21f;
    uVar9 = uVar5 ^ (long)(int)((uint)(*(ushort *)(in_data + iVar8) >> 3) | uVar13 << 0xd) & 0xffffU;
    pbVar11 = in_data;
    if (iVar8 < 1)
    {
        uVar5 = 0xeb7d; // 0xebd7;
        bVar1 = *in_data;
    }
    else
    {
        uVar2 = 0xd; // 0xd;
        iVar10 = iVar8;
        do
        {
            uVar6 = *pbVar11 ^ uVar9;
            uVar9 = (long)(int)(((uint)uVar9 >> 1 | ((uint)uVar9 & 1) << 0xf) * 5 + 1) & 0xffff;
            pbVar11 = pbVar11 + 1;
            uVar6 = (uVar6 & 0xff) * (ulong)uVar2;
            iVar10 = iVar10 + -1;
            uVar12 = (long)(int)((int)uVar12 + ((uint)uVar6 & 0xffff)) & 0xffff;
        } while (0 < iVar10);
        bVar1 = *pbVar11;
    }
    uVar5 = ((ulong)(ushort)(CONCAT11(pbVar11[1], bVar1) >> 3) |
             ((ulong)CONCAT11(pbVar11[1], bVar1) & 7) << 0xd) ^
            uVar5;
    pbVar11 = pbVar11 + 2;
    if (uVar5 == uVar12)
    {
        puVar7 = (ushort *)version;
        do
        {
            uVar2 = *puVar7;
            uVar4 = *pbVar11 ^ uVar13;
            puVar7 = (ushort *)((intptr_t)puVar7 + 1);
            pbVar11 = pbVar11 + 1;
            uVar13 = (uVar13 >> 5 | (uVar13 & 0x1f) << 0xb) * 5 + 1 & 0xffff;
            if ((uVar4 & 0xff) != (uint)(byte)uVar2)
                goto abort;
        } while (*(byte *)puVar7 != 0);
        for (; 0 < iVar8; iVar8 = iVar8 + -1)
        {
            *out_data = *in_data ^ (byte)uVar5;
            uVar5 = (long)(int)(((uint)uVar5 >> 1 | ((uint)uVar5 & 1) << 0xf) * 5 + 1) & 0xffff;
            in_data = in_data + 1;
            out_data = out_data + 1;
        }
        uVar3 = 0;
    }
    else
    {
    abort:
        uVar3 = 1;
    }
    return uVar3;
}

int rle_decompress(byte *input, byte *output)
{
    uint uVar1;
    byte *pbVar2;
    byte *pbVar3;
    uint uVar4;
    byte *pbVar5;
    byte *pbVar6;
    uint uVar7;
    int iVar8;
    byte control_byte;

    control_byte = *input;
    pbVar2 = input + 1;
    pbVar3 = output;
    while (uVar7 = (uint)control_byte, uVar7 != 0)
    {
        pbVar5 = pbVar2 + 1;
        if (uVar7 < 2)
        {
            control_byte = *pbVar2;
            pbVar2 = pbVar5;
        }
        else
        {
            do
            {
                uVar1 = uVar7 & 1;
                uVar7 = (int)uVar7 >> 1;
                if (uVar1 == 0)
                {
                    iVar8 = 0;
                    uVar1 = (uint)*pbVar2 * 0x100 + (uint)*pbVar5;
                    uVar4 = uVar1 & 0x7ff;
                    if (uVar4 == 0)
                    {
                        uVar4 = 0x800;
                    }
                    pbVar6 = pbVar3 - uVar4;
                    uVar1 = (int)uVar1 >> 0xb & 0x1f;
                    if (uVar1 == 0)
                    {
                        uVar1 = 0x20;
                    }
                    pbVar2 = pbVar5 + 1;
                    if (uVar1 == 0)
                    {
                        pbVar5 = pbVar5 + 2;
                    }
                    else
                    {
                        pbVar5 = pbVar5 + 2;
                        do
                        {
                            control_byte = *pbVar6;
                            iVar8 = iVar8 + 1;
                            pbVar6 = pbVar6 + 1;
                            *pbVar3 = control_byte;
                            pbVar3 = pbVar3 + 1;
                        } while (iVar8 < (int)uVar1);
                    }
                }
                else
                {
                    *pbVar3 = *pbVar2;
                    pbVar3 = pbVar3 + 1;
                    pbVar2 = pbVar5;
                    pbVar5 = pbVar5 + 1;
                }
            } while (1 < uVar7);
            control_byte = *pbVar2;
            pbVar2 = pbVar5;
        }
    }
    return (intptr_t)pbVar3 - (intptr_t)output;
}

int verbose = 0;
int main(int argc, char **argv)
{
    printf("Sys246 SoulCalibur2 game unpacker\nBy El_isra and DiscoStarslayer\n");
    if (argc < 4)
    {
        eprintf("missing args...\nusage: %s <INPUT> <OUTPUT1> <OUTPUT2>\n"
                "\tINPUT:   The game's PS2AC05 file\n"
                "\tOUTPUT1: decrypted file\n"
                "\tOUTPUT2: decrypted and decompressed file\n",
                argv[0]);
        return 1;
    }
    FILE *f = fopen(SOURCE_FILE, "rb");

    if (f == NULL)
    {
        eprintf("Error opening file!\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    void *source = malloc(size * sizeof(char));
    void *dest = malloc(size * sizeof(char));
    void *rle_dest = malloc(size * sizeof(char) * 8);
    if (source == NULL || dest == NULL || rle_dest == NULL)
    {
        eprintf("Failed to allocate buffers\n");
        return 1;
    }
    size_t result = fread(source, sizeof(char), size, f);
    fclose(f);

    printf("Readed %llu bytes from input\n", result);

    uint32_t decrypt_result = SoulCaliburDecrypt((uint8_t *)dest, (uint8_t *)source, size);
    if (decrypt_result != 0)
    {
        eprintf("Decryption failed!\n");
        return 1;
    }
    printf("Decryption success...\n", size);

    f = fopen(DESTINATION_FILE, "wb");
    fwrite(dest, sizeof(char), size, f);
    fclose(f);

    size_t decompressed_size = rle_decompress((uint8_t *)dest + 4, (uint8_t *)rle_dest);

    printf("Decompressed %llu bytes\n", decompressed_size);

    f = fopen(DESTINATION_RLE_FILE, "wb");
    size_t ww = fwrite(rle_dest, sizeof(char), decompressed_size, f);
    fclose(f);

    free(source);
    free(dest);
    free(rle_dest);

    return 0;
}
