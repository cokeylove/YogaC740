#ifndef OEM_EDP_H
#define OEM_EDP_H

extern void Init_PS8625(void);

typedef struct PSXSmbusStruct
{
    BYTE  smbaddr;
    BYTE  smboffset;
	BYTE  smbvalue;
} sPSXSmbusStruct;

#define PS8625P0	0x10
#define	PS8625P1	0x12
#define PS8625P2	0x14
#define PS8625P3	0x16
#define	PS8625P4	0x18
#define	PS8625P5	0x1A
#define	PS8625P6	0x1C
#define PS8625P7	0x1E

#endif