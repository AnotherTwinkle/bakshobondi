#ifndef DISK_H
#define DISK_H

#include "kernel/util.h"
#include "kernel/io.h"

#define ATA_DATA   0x1F0
#define ATA_STATUS 0x1F7
#define ATA_COMMAND 0x1F7
#define ATA_SECTOR_COUNT 0x1F2
#define ATA_LBA_LOW 0x1F3
#define ATA_LBA_MID 0x1F4
#define ATA_LBA_HIGH 0x1F5
#define ATA_DRIVE_HEAD 0x1F6

void ata_read_sector(u32 lba, u16* buffer, u8 sectors) {
    outportb(ATA_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    outportb(ATA_SECTOR_COUNT, sectors);
    outportb(ATA_LBA_LOW, lba & 0xFF);
    outportb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outportb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);
    outportb(ATA_COMMAND, 0x20); // Read sectors command

    for (u8 s = 0; s < sectors; s++) {
        // Wait for DRQ
        while (!(inportb(ATA_STATUS) & 0x08)) {}
        for (int i = 0; i < 256; i++)
            buffer[i + s*256] = inportw(ATA_DATA);
    }
}

void load_bytes_to_buffer(char* dest, u32 sector_start, u32 size_bytes) {
    u32 sectors = (size_bytes + 511) / 512;
    ata_read_sector(sector_start, (u16*)dest, sectors);
}


#endif // DISK_H
