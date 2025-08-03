#include <stdio.h>

char main_bytes[] = {
0x54, 0x8a, 0xe0, 0x84, 0xe5, 0x23, 0x8e, 0xeb, 0xe1, 0xab, 0x15, 0x17, 0x19, 0x1b, 0x34, 0xdb, 0xa2, 0xcf, 0x29, 0x4f, 0x09, 0xa3, 0x29, 0x27,
0xd9, 0x95, 0xc8, 0xc8, 0xc6, 0xb8, 0xf9, 0x2f, 0xc4, 0x83, 0x31, 0x4d, 0xca, 0xa7, 0x41, 0x25, 0x51, 0xbb, 0xb0, 0xaa, 0xa6, 0xa4, 0x37, 0x5f,
0x0b, 0x62, 0x6b, 0x03, 0x6f, 0x07, 0xe1, 0xa3, 0xf0, 0xf0, 0xee, 0x90, 0xd1, 0x07, 0x9c, 0xdb, 0x69, 0x15, 0xa2, 0xcf, 0x29, 0x4d, 0x29, 0xc3,
0xea, 0xd2, 0xce, 0xcc, 0xdd, 0x85, 0xc7, 0xc4, 0xc2, 0xbc, 0xad, 0x4f, 0x2d, 0x07, 0xc1, 0x4f, 0x45, 0xa7, 0xf4, 0xae, 0xaa, 0xa8, 0xda, 0x9f,
0x4d, 0xdc, 0x8d, 0x67, 0x6b, 0x03, 0x6f, 0x06, 0x63, 0x09, 0xe5, 0xdb, 0xec, 0xec, 0xea, 0x94, 0xdd, 0x0b, 0x94, 0x5a, 0xd5, 0xa0, 0x58, 0xd3,
0xd6, 0x5e, 0x27, 0xac, 0xdd, 0x3f, 0x5f, 0x36, 0xd1, 0xb5, 0xc0, 0xc0, 0xbe, 0xab, 0x3c, 0xb9, 0xb6, 0xb4, 0x2b, 0x88, 0x14, 0x8b, 0x57, 0x57,
0xda, 0xb7, 0x51, 0x37, 0x76, 0x77, 0x01, 0x03, 0xed, 0x81, 0xf4, 0xf4, 0xf2, 0x8c, 0xd5, 0x03, 0x73, 0x9e, 0x5c, 0xc1, 0xda, 0x5a, 0xfd, 0x5c,
0x25, 0x27, 0x28, 0xa8, 0xc1, 0x2b, 0x5b, 0x23, 0xb8, 0x72, 0xe1, 0x6b, 0xc2, 0x4a, 0xb5, 0xab, 0x32, 0xba, 0xb6, 0xb4, 0xce, 0x8b, 0x41, 0xd6,
0x95, 0x22, 0x49, 0xd8, 0xb1, 0x53, 0x09, 0x11, 0x89, 0x07, 0x0d, 0xef, 0x3a, 0xf6, 0xf2, 0xf0, 0x92, 0xd7, 0x05, 0xff, 0x32, 0xe5, 0xe2, 0xe0,
0xa2, 0xcf, 0x29, 0xd8, 0x5c, 0xdf, 0xc5, 0xff, 0xcd, 0xcc, 0xca, 0xb4, 0xfd, 0x2b, 0x85, 0x3f, 0x41, 0x43, 0x45, 0x8e, 0x8a, 0xdb, 0xdd, 0xdf,
0xc1, 0xc3,
0x00, // extra null byte, just to mark the end
};

int main() {
    int count = 1;
    for (int i = 0; main_bytes[i] != '\0'; i++) {
        if (count > 100) {
            count = 1;
        }
        putchar(count ^ main_bytes[i]);
        count = count + 2;
    }
}

/*
Ghidra output:

                         main                                      XREF[6]:   entry:08048477(*), 
                                                                               FUN_08048510:08048516(*), 
                                                                               FUN_08048510:08048547(R), 
                                                                               mprotect_and_xor_on_main:080
                                                                               mprotect_and_xor_on_main:080
                                                                               mprotect_and_xor_on_main:080
      0804860d 54            undefin   54h
                         DAT_0804860e                              XREF[2]:   FUN_08048510:0804856b(*), 
                                                                               mprotect_and_xor_on_main:080
      0804860e 8a            ??        8Ah
      0804860f e0            ??        E0h
      08048610 84            ??        84h
      08048611 e5            ??        E5h
      08048612 23            ??        23h    #
      08048613 8e            ??        8Eh
      08048614 eb            ??        EBh
      08048615 e1            ??        E1h
      08048616 ab            ??        ABh
      08048617 15            ??        15h
      08048618 17            ??        17h
      08048619 19            ??        19h
      0804861a 1b            ??        1Bh
      0804861b 34            ??        34h    4
      0804861c db            ??        DBh
      0804861d a2            ??        A2h
      0804861e cf            ??        CFh
      0804861f 29            ??        29h    )
      08048620 4f            ??        4Fh    O
      08048621 09            ??        09h
      08048622 a3            ??        A3h
      08048623 29            ??        29h    )
      08048624 27            ??        27h    '
      08048625 d9            ??        D9h
      08048626 95            ??        95h
      08048627 c8            ??        C8h
      08048628 c8            ??        C8h
      08048629 c6            ??        C6h
      0804862a b8            ??        B8h
      0804862b f9            ??        F9h
      0804862c 2f            ??        2Fh    /
      0804862d c4            ??        C4h
      0804862e 83            ??        83h
      0804862f 31            ??        31h    1
      08048630 4d            ??        4Dh    M
      08048631 ca            ??        CAh
      08048632 a7            ??        A7h
      08048633 41            ??        41h    A
      08048634 25            ??        25h    %
      08048635 51            ??        51h    Q
      08048636 bb            ??        BBh
      08048637 b0            ??        B0h
      08048638 aa            ??        AAh
      08048639 a6            ??        A6h
      0804863a a4            ??        A4h
      0804863b 37            ??        37h    7
      0804863c 5f            ??        5Fh    _
      0804863d 0b            ??        0Bh
      0804863e 62            ??        62h    b
      0804863f 6b            ??        6Bh    k
      08048640 03            ??        03h
      08048641 6f            ??        6Fh    o
      08048642 07            ??        07h
      08048643 e1            ??        E1h
      08048644 a3            ??        A3h
      08048645 f0            ??        F0h
      08048646 f0            ??        F0h
      08048647 ee            ??        EEh
      08048648 90            ??        90h
      08048649 d1            ??        D1h
      0804864a 07            ??        07h
      0804864b 9c            ??        9Ch
      0804864c db            ??        DBh
      0804864d 69            ??        69h    i
      0804864e 15            ??        15h
      0804864f a2            ??        A2h
      08048650 cf            ??        CFh
      08048651 29            ??        29h    )
      08048652 4d            ??        4Dh    M
      08048653 29            ??        29h    )
      08048654 c3            ??        C3h
      08048655 ea            ??        EAh
      08048656 d2            ??        D2h
      08048657 ce            ??        CEh
      08048658 cc            ??        CCh
      08048659 dd            ??        DDh
      0804865a 85            ??        85h
      0804865b c7            ??        C7h
      0804865c c4            ??        C4h
      0804865d c2            ??        C2h
      0804865e bc            ??        BCh
      0804865f ad            ??        ADh
      08048660 4f            ??        4Fh    O
      08048661 2d            ??        2Dh    -
      08048662 07            ??        07h
      08048663 c1            ??        C1h
      08048664 4f            ??        4Fh    O
      08048665 45            ??        45h    E
      08048666 a7            ??        A7h
      08048667 f4            ??        F4h
      08048668 ae            ??        AEh
      08048669 aa            ??        AAh
      0804866a a8            ??        A8h
      0804866b da            ??        DAh
      0804866c 9f            ??        9Fh
      0804866d 4d            ??        4Dh    M
      0804866e dc            ??        DCh
      0804866f 8d            ??        8Dh
      08048670 67            ??        67h    g
      08048671 6b            ??        6Bh    k
      08048672 03            ??        03h
      08048673 6f            ??        6Fh    o
      08048674 06            ??        06h
      08048675 63            ??        63h    c
      08048676 09            ??        09h
      08048677 e5            ??        E5h
      08048678 db            ??        DBh
      08048679 ec            ??        ECh
      0804867a ec            ??        ECh
      0804867b ea            ??        EAh
      0804867c 94            ??        94h
      0804867d dd            ??        DDh
      0804867e 0b            ??        0Bh
      0804867f 94            ??        94h
      08048680 5a            ??        5Ah    Z
      08048681 d5            ??        D5h
      08048682 a0            ??        A0h
      08048683 58            ??        58h    X
      08048684 d3            ??        D3h
      08048685 d6            ??        D6h
      08048686 5e            ??        5Eh    ^
      08048687 27            ??        27h    '
      08048688 ac            ??        ACh
      08048689 dd            ??        DDh
      0804868a 3f            ??        3Fh    ?
      0804868b 5f            ??        5Fh    _
      0804868c 36            ??        36h    6
      0804868d d1            ??        D1h
      0804868e b5            ??        B5h
      0804868f c0            ??        C0h
      08048690 c0            ??        C0h
      08048691 be            ??        BEh
      08048692 ab            ??        ABh
      08048693 3c            ??        3Ch    <
      08048694 b9            ??        B9h
      08048695 b6            ??        B6h
      08048696 b4            ??        B4h
      08048697 2b            ??        2Bh    +
      08048698 88            ??        88h
      08048699 14            ??        14h
      0804869a 8b            ??        8Bh
      0804869b 57            ??        57h    W
      0804869c 57            ??        57h    W
      0804869d da            ??        DAh
      0804869e b7            ??        B7h
      0804869f 51            ??        51h    Q
      080486a0 37            ??        37h    7
      080486a1 76            ??        76h    v
      080486a2 77            ??        77h    w
      080486a3 01            ??        01h
      080486a4 03            ??        03h
      080486a5 ed            ??        EDh
      080486a6 81            ??        81h
      080486a7 f4            ??        F4h
      080486a8 f4            ??        F4h
      080486a9 f2            ??        F2h
      080486aa 8c            ??        8Ch
      080486ab d5            ??        D5h
      080486ac 03            ??        03h
      080486ad 73            ??        73h    s
      080486ae 9e            ??        9Eh
      080486af 5c            ??        5Ch    \
      080486b0 c1            ??        C1h
      080486b1 da            ??        DAh
      080486b2 5a            ??        5Ah    Z
      080486b3 fd            ??        FDh
      080486b4 5c            ??        5Ch    \
      080486b5 25            ??        25h    %
      080486b6 27            ??        27h    '
      080486b7 28            ??        28h    (
      080486b8 a8            ??        A8h
      080486b9 c1            ??        C1h
      080486ba 2b            ??        2Bh    +
      080486bb 5b            ??        5Bh    [
      080486bc 23            ??        23h    #
      080486bd b8            ??        B8h
      080486be 72            ??        72h    r
      080486bf e1            ??        E1h
      080486c0 6b            ??        6Bh    k
      080486c1 c2            ??        C2h
      080486c2 4a            ??        4Ah    J
      080486c3 b5            ??        B5h
      080486c4 ab            ??        ABh
      080486c5 32            ??        32h    2
      080486c6 ba            ??        BAh
      080486c7 b6            ??        B6h
      080486c8 b4            ??        B4h
      080486c9 ce            ??        CEh
      080486ca 8b            ??        8Bh
      080486cb 41            ??        41h    A
      080486cc d6            ??        D6h
      080486cd 95            ??        95h
      080486ce 22            ??        22h    "
      080486cf 49            ??        49h    I
      080486d0 d8            ??        D8h
      080486d1 b1            ??        B1h
      080486d2 53            ??        53h    S
      080486d3 09            ??        09h
      080486d4 11            ??        11h
      080486d5 89            ??        89h
      080486d6 07            ??        07h
      080486d7 0d            ??        0Dh
      080486d8 ef            ??        EFh
      080486d9 3a            ??        3Ah    :
      080486da f6            ??        F6h
      080486db f2            ??        F2h
      080486dc f0            ??        F0h
      080486dd 92            ??        92h
      080486de d7            ??        D7h
      080486df 05            ??        05h
      080486e0 ff            ??        FFh
      080486e1 32            ??        32h    2
      080486e2 e5            ??        E5h
      080486e3 e2            ??        E2h
      080486e4 e0            ??        E0h
      080486e5 a2            ??        A2h
      080486e6 cf            ??        CFh
      080486e7 29            ??        29h    )
      080486e8 d8            ??        D8h
      080486e9 5c            ??        5Ch    \
      080486ea df            ??        DFh
      080486eb c5            ??        C5h
      080486ec ff            ??        FFh
      080486ed cd            ??        CDh
      080486ee cc            ??        CCh
      080486ef ca            ??        CAh
      080486f0 b4            ??        B4h
      080486f1 fd            ??        FDh
      080486f2 2b            ??        2Bh    +
      080486f3 85            ??        85h
      080486f4 3f            ??        3Fh    ?
      080486f5 41            ??        41h    A
      080486f6 43            ??        43h    C
      080486f7 45            ??        45h    E
      080486f8 8e            ??        8Eh
      080486f9 8a            ??        8Ah
      080486fa db            ??        DBh
      080486fb dd            ??        DDh
      080486fc df            ??        DFh
      080486fd c1            ??        C1h
      080486fe c3            ??        C3h
*/
