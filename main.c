/*-------------------------------- INCLUDE LIBRARY ---------------------------------------*/
#include <stdio.h>              // ไลบรารีสำหรับการใช้งานฟังก์ชันพื้นฐาน เช่น printf(), scanf()
#include <stdlib.h>             // ไลบรารีที่ใช้สำหรับการใช้งานฟังก์ชันระบบ เช่น exit(), malloc(), free()
#include <time.h>               // ไลบรารีที่ใช้สำหรับการจัดการเวลา เช่นการสุ่มด้วย time()
#include <windows.h>            // ไลบรารีที่ใช้สำหรับฟังก์ชันเฉพาะของ Windows เช่น Sleep(), system()
/*----------------------------------------------------------------------------------------*/

// สัญลักษณ์บนไพ่
#define heart 3                         // ใช้เพื่อแสดงสัญลักษณ์รูปหัวใจ
#define diamond 4                       // ใช้เพื่อแสดงสัญลักษณ์รูปเพชร
#define club 5                          // ใช้เพื่อแสดงสัญลักษณ์ดอกจิก
#define spade 6                         // ใช้เพื่อแสดงสัญลักษณ์ดอกโพดำ

// ANSI escape codes สำหรับสี
char *black = "\033[1;30m";  // สีดำ
char *red = "\033[1;31m";    // สีแดง
char *yellow = "\033[1;33m"; // สีเหลือง
char *cyan = "\033[1;36m";   // สีฟ้า
char *green = "\033[1;32m";  // สีเขียว
char *blue = "\033[1;34m";   // สีน้ำเงิน
char *magenta = "\033[1;35m";// สีม่วงแดง
char *reset = "\033[0m";     // รีเซ็ตสีกลับสู่ปกติ
char *bold = "\033[1m";     // bold

// โครงสร้างข้อมูลไพ่แต่ละใบ (Card)
typedef struct card 
{
    int face;                           // เก็บหน้าไพ่ เช่น A K Q J 10 9 8 7 6 5 4 3 2
    int suit;                           // เก็บประเภทของสัญลักษณ์ เช่น หัวใจ เพชร ดอกจิก โพดำ
} Card;

// โครงสร้างข้อมูลของไพ่ในมือ (Hand)
typedef struct hand 
{
    Card cards[3];      // ในหนึ่งมือสามารถมีไพ่ได้สูงสุด 3 ใบ
    int cardCount;      // จำนวนไพ่ในมือนั้นๆ
} Hand;

// โครงสร้างข้อมูลสำรับไพ่ (Deck)
typedef struct deck 
{
    Card dealCards[52];  // สำรับไพ่จะมีไพ่ทั้งหมด 52 ใบ
    int numCardsDealt;  // จำนวนไพ่ที่แจกไปแล้ว
} Deck;

Hand playerHand, dealerHand;    // ประกาศตัวแปรมือของผู้เล่นและดีลเลอร์
Deck deck;                      // ประกาศสำรับไพ่

// ตัวแปรเก็บค่าเงินของผู้เล่น
int playerMoney = 10000;        // เริ่มต้นผู้เล่นมีเงิน 10,000
int betAmount;                  // ตัวแปรสำหรับเก็บจำนวนเงินเดิมพัน


Card dealCard();            // ฟังก์ชันแจกไพ่ (สุ่ม)

void loadingBar();          // ฟังก์ชันแสดงแถบโหลด (loading)
void menu();                // ฟังก์ชันแสดงเมนูหลัก
int confirm_exit();         // ฟังก์ชันยืนยันการออกจากเกม
void how_to_play();         // ฟังก์ชันอธิบายวิธีการเล่น

void initializeDeck();      // ฟังก์ชันเตรียมสำรับไพ่
// ฟังก์ชันสับไพ่
void shuffleDeck();         // สับไพ่ให้สุ่มแบบสุ่มทุกครั้ง
void placeBet();            // ฟังก์ชันสำหรับวางเดิมพัน

void generateHands();       // ฟังก์ชันแจกไพ่ให้ผู้เล่นและดีลเลอร์
void printHands();          // ฟังก์ชันแสดงไพ่ในมือของผู้เล่นและดีลเลอร์
void printCardsInLine(Card cards[], int cardCount); // ฟังก์ชันแสดงไพ่แต่ละใบ
int calculateHandValue(Hand hand);      // ฟังก์ชันคำนวณแต้มในมือของผู้เล่นหรือดีลเลอร์
void analyzeHands();        // ฟังก์ชันวิเคราะห์ไพ่ในมือและตัดสินผล
void playerTurn();          // ฟังก์ชันสำหรับตาของผู้เล่นในการจั่วหรือหยุด
void dealerTurn();          // ฟังก์ชันสำหรับตาของดีลเลอร์ในการจั่วหรือหยุด
void play();                // ฟังก์ชันหลักที่ใช้ในการเล่นเกม
void askOver();             // ฟังก์ชันสอบถามผู้เล่นว่าต้องการเล่นต่อหรือไม่
void checkMoney();          // ฟังก์ชันตรวจสอบเงินของผู้เล่น (ว่าหมดหรือยัง)


int main()
{
    int option; // ประกาศตัวแปร 'option' เพื่อเก็บตัวเลือกของผู้ใช้
    loadingBar(); // เรียกใช้ฟังก์ชัน loadingBar() เพื่อแสดงแถบการโหลด

    while (1) { // ลูป while ที่ทำงานไปเรื่อย ๆ (infinite loop) จนกว่าจะมีการสั่งหยุด
        menu(); // เรียกใช้ฟังก์ชัน menu() เพื่อแสดงเมนูให้ผู้ใช้เลือก
        
        
        if (scanf("%d", &option) != 1) { // รับค่าอินพุตจากผู้ใช้ และตรวจสอบว่าผู้ใช้ป้อนค่าที่เป็นตัวเลขหรือไม่
            printf("%sInvalid input.%s Please enter a number between 0-2%s", red, yellow, reset); // แสดงข้อความแจ้งว่าค่าที่ป้อนไม่ถูกต้อง
            for(int i = 0; i < 3; i++) {
                printf("%s.%s ", yellow, reset);
                Sleep(1000);
            }
            while (getchar() != '\n'); // ล้างบัฟเฟอร์อินพุต เพื่อเตรียมพร้อมสำหรับการป้อนข้อมูลใหม่
            system("cls");  
            continue; // ข้ามไปยังรอบถัดไปของลูป while โดยไม่ทำงานต่อ
        }

        switch (option) { // ตรวจสอบค่าของ 'option' เพื่อเลือกคำสั่งที่ต้องการดำเนินการ
            case 1:
                play(); // เรียกใช้ฟังก์ชัน play() เมื่อผู้ใช้เลือกตัวเลือก 1
                break;
            case 2:
                how_to_play(); // เรียกใช้ฟังก์ชัน how_to_play() เมื่อผู้ใช้เลือกตัวเลือก 2
                break;
            case 0:
                if (confirm_exit()) { // เรียกใช้ฟังก์ชัน confirm_exit() เพื่อตรวจสอบว่าผู้ใช้ต้องการออกจากโปรแกรมหรือไม่
                    printf("%sExiting program%s", green, reset); // แสดงข้อความแจ้งว่ากำลังออกจากโปรแกรม
                    for(int i = 0; i < 3; i++) {
                    printf("%s. %s", green, reset);
                    Sleep(1000);
                }
                    system("cls"); // ล้างหน้าจอคอนโซล (ทำงานได้เฉพาะบน Windows)
                    return 0;  // ออกจากโปรแกรมโดยส่งคืนค่า 0
                }
                break;
            default:
                printf("%sPlease choose an option between 0-2%s", yellow, reset); // แจ้งให้ผู้ใช้เลือกตัวเลือกที่ถูกต้องระหว่าง 0-2
                for(int i = 0; i < 3; i++) {
                    printf("%s. %s", yellow, reset);
                    Sleep(1000);
                }
                system("cls");
                break;
        }
    }
    
}

int confirm_exit() {
    char choice;    // ตัวแปรสำหรับเก็บคำตอบจากผู้ใช้
    while (1) {     // ลูปทำงานเรื่อย ๆ จนกว่าจะมีการคืนค่า (return)
        system("cls");  // ล้างหน้าจอ
        printf("\n%sAre you sure you want to exit? %s(%sY%s/%sN%s): ", bold, reset, green, reset, red, reset);   // แสดงข้อความถามยืนยันผู้ใช้
        scanf(" %c", &choice);  // รับตัวอักษรจากผู้ใช้ (Y/N)

        if (choice == 'Y' || choice == 'y') { // ถ้าผู้ใช้เลือก 'Y' หรือ 'y' ให้คืนค่า 1 (แสดงว่าต้องการออก)
            return 1;  
        } else if (choice == 'N' || choice == 'n') { // ถ้าผู้ใช้เลือก 'N' หรือ 'n' ให้คืนค่า 0 (แสดงว่าไม่ต้องการออก) และล้างหน้าจอ
            system("cls");
            return 0;  
        } else {     // ถ้าผู้ใช้กรอกตัวอักษรอื่น ๆ ที่ไม่ใช่ 'Y' หรือ 'N'
            printf("Please choose 'Y' or 'N' only "); // แจ้งให้ผู้ใช้เลือกใหม่
            for(int i = 0; i < 3; i++) { // วนลูปพิมพ์จุดเพิ่ม 3 จุดพร้อมหน่วงเวลา
                printf(". ");
                Sleep(1000); // หน่วงเวลา 1 วินาทีในแต่ละครั้ง
            }
        }
    }
}

void loadingBar() 
{
    // ล้างหน้าจอ
    system("cls");
    // ตั้งสีตัวอักษรในคอนโซลเป็นสีเขียว
    system("color 0A");
    // กำหนด ANSI escape codes สำหรับสีรุ้ง (แดง, เหลือง, เขียว, ฟ้า, น้ำเงิน, ม่วง)
    char *colors[] = {
        "\033[0;31m", // สีแดง
        "\033[0;33m", // สีเหลือง
        "\033[0;32m", // สีเขียว
        "\033[0;36m", // สีฟ้า
        "\033[0;34m", // สีน้ำเงิน
        "\033[0;35m", // สีม่วง
    };

    // กำหนดตัวแปร a และ b เป็นตัวอักษร ASCII
    char a = 177, b = 219;

    printf("\n\n\n\n");     // เพิ่มบรรทัดว่าง 4 บรรทัดเพื่อเลื่อนข้อความลงมา
    printf("\n\n\n\n\t\t\t\t\t\t\t\t\t"); // เพิ่มบรรทัดว่างอีก 4 บรรทัดเพื่อเลื่อน "Loading..." ไปด้านขวาของหน้าจอ
    printf("Loading...\n\n"); // โชว์ "Loading..."
    printf("\t\t\t\t\t\t\t\t"); // เพิ่มช่องว่างเพื่อจัดให้บาร์โหลดอยู่ตรงกลาง

    // วนลูปพิมพ์ตัวอักษร a ซึ่งเป็นตัวอักษรแบบบาร์ที่ยังไม่ถูกเติมเต็ม
    for (int i = 0; i < 26; i++) 
        printf("%c", a); 

    // ย้ายตำแหน่งเคอร์เซอร์กลับไปจุดเริ่มต้นของบาร์โหลด (ตำแหน่งซ้ายสุด)
    printf("\r"); 
    printf("\t\t\t\t\t\t\t\t"); 

    // วนลูปพิมพ์ตัวอักษร b ซึ่งเป็นบาร์ที่ถูกเติมเต็มแล้ว พร้อมเปลี่ยนสีเป็นสีรุ้ง
    for (int i = 0; i < 26; i++) { 
        printf("%s%c", colors[i % 6], b); // เปลี่ยนสีสลับไปเรื่อยๆ ในแต่ละตำแหน่งของบาร์
        Sleep(50); // หน่วงเวลา 0.05 วินาทีต่อการพิมพ์แต่ละตัว
    }

    // รีเซ็ตสีตัวอักษรกลับไปเป็นสีขาว
    printf("\033[0m");

    // หน่วงเวลารอ 1 วินาที
    Sleep(1000);
    // ล้างหน้าจออีกครั้งหลังจากบาร์โหลดเสร็จสมบูรณ์
    system("cls");
}

void menu() 
{

    // แสดงชื่อเกมด้วยสัญลักษณ์ไพ่ (โพดำ, โพแดง, ดอกจิก, ข้าวหลามตัด) และข้อความ "POK KA DENG CARD GAME"
    printf("\n\n\t\t\t\t\t\t\t%s%c %c %c %c %sPOK KA DENG CARD GAME%s %s%c %c %c %c%s\n", 
            red, spade, heart, club, diamond, cyan, reset, red, spade, heart, club, diamond, reset);
    // แสดงเส้นขอบสีเหลืองสำหรับแยกส่วนหัว
    printf("\t\t\t\t\t\t%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%s\n", yellow, reset);
    
    // แสดงหัวข้อหลัก "MAIN MENU" พร้อมสัญลักษณ์โพดำประกอบ
    printf("\t\t\t\t%s\t\t\t     %c %c %c %c  %sMAIN MENU%s  %c %c %c %c%s\n", yellow, spade, spade, spade, spade, cyan, yellow, spade, spade, spade, spade,reset);
    // แสดงเส้นขอบสีเหลืองอีกครั้งเพื่อแยกหัวข้อกับเนื้อหา
    printf("\t\t\t\t\t\t%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%s\n", yellow, reset);

    // แสดงตัวเลือกเมนูที่ 1 คือเริ่มเกม (GAME START) โดยใช้สัญลักษณ์โพแดงและตัวอักษรสีเขียว
    printf("\t\t\t\t\t\t\t%s%c  %s[1] GAME START%s\n", blue, heart, green, reset);
    // แสดงตัวเลือกเมนูที่ 2 คือวิธีการเล่น (HOW TO PLAY) โดยใช้สัญลักษณ์โพดำและตัวอักษรสีน้ำเงิน
    printf("\t\t\t\t\t\t\t%s%c  %s[2] HOW TO PLAY%s\n", magenta, spade, blue, reset);
    // แสดงตัวเลือกเมนูที่ 0 คือออกจากเกม (EXIT) โดยใช้สัญลักษณ์ดอกจิกและตัวอักษรสีแดง
    printf("\t\t\t\t\t\t\t%s%c  %s[0] EXIT%s\n", yellow, club, red, reset);

    // แสดงเส้นขอบสีเหลืองเป็นเส้นปิดท้าย
    printf("\t\t\t\t\t\t%s~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~%s\n", yellow, reset);
    // แสดงข้อความให้ผู้ใช้กรอกตัวเลือก โดยใช้สีฟ้า
    printf("\t\t\t\t\t\t  %sEnter your choice: %s", cyan, reset);
}

void how_to_play()
{
    char choice;
    // ล้างหน้าจอและแสดงคำอธิบายกติกาพร้อมกรอบตกแต่ง
    system("cls");

    // สร้างกรอบด้านบน
    printf("\t\t\t\t\t\t\t%s*********************************************************%s\n", yellow, reset);

    // แสดงข้อความ "HOW TO PLAY" พร้อมสัญลักษณ์ไพ่ในกรอบ
    printf("\t\t\t\t\t\t\t%s*%s            %c %c %c %c %s%sHOW TO PLAY%s %c %c %c %c                %s*%s\n", yellow, reset, heart, diamond, club, spade, bold, magenta, reset, heart, diamond, club, spade, yellow, reset);

    // อธิบายกติกาของเกม Pok Ka Deng ว่ามีผู้เล่นสองคนและรายละเอียดการเล่น
    printf("\t\t\t\t\t\t\t%s*                                                       *%s\n", yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   This game has two players: the 'Dealer' and         %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   the 'Player' (You).                                 %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   Before the game starts, the player needs to         %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   place a bet.                                        %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   After betting, the dealer will give two cards       %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   to you and two cards to themselves.                 %s*%s\n", yellow, reset, yellow, reset);

    // อธิบายการนับคะแนนและการจั่วไพ่เพิ่มเติม
    printf("\t\t\t\t\t\t\t%s*                                                       *%s\n", yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   %sIf%s the total value of your cards is 8 or 9, you     %s*%s\n", yellow, reset, green, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   win, as long as the dealer's total is lower.        %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   For face cards (K, Q, J), the value is 0. For       %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   Ace (A), the value is 1.                            %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*                                                       *%s\n", yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   %sIf%s your total card value is 4 or less, an           %s*%s\n", yellow, reset, green, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   additional card will be drawn automatically.        %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   If your total is between 5 and 7, you can choose    %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s   to draw or stay. The choice is yours.               %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*                                                       *%s\n", yellow, reset);

    // อธิบายกติกาพิเศษ (พิเศษ: ถ้าได้ไพ่แบบเรียงหรือเลขซ้ำ)
    printf("\t\t\t\t\t\t\t%s*%s  %sSpecial rules%s:                                       %s*%s\n", yellow, reset, red, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*  %s- Same suit: bet is doubled (*2).                    %s*%s\n", yellow, green, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*  %s- Same number: bet is doubled (*2).                  %s*%s\n", yellow, green, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*  %s- Straight (1, 2, 3): bet is tripled (*3).           %s*%s\n", yellow, green, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*  %s- Three same numbers: bet is multiplied by 5 (*5).   %s*%s\n", yellow, green, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*  %s- Straight + matching suits: bet is multiplied by 5  %s*%s\n", yellow, green, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*                                                       *%s\n", yellow, reset);

    // แสดงข้อความบอกว่าเกมจะจบเมื่อผู้เล่นเลือกออกหรือเงินหมด
    printf("\t\t\t\t\t\t\t%s*%s  The game will end when you either exit or            %s*%s\n", yellow, reset, yellow, reset);
    printf("\t\t\t\t\t\t\t%s*%s  lose all your money.                                 %s*%s\n", yellow, reset, yellow, reset);

    // ปิดกรอบข้อความ
    printf("\t\t\t\t\t\t\t%s*********************************************************%s\n", yellow, reset);

    // วนลูปให้ผู้ใช้เลือกว่าจะกลับไปที่เมนูหลักหรือออกจากเกม
    do {
        printf("\n%sDo you want to go back to the Main Menu or Exit the game?%s\n", yellow, reset);
        printf(" Enter%s 'M' %sfor %sMain Menu%s or %s'E'%s to %sExit%s: ", red, reset, red, reset, red, reset, red, reset);
        scanf(" %c", &choice);

        // ถ้าผู้ใช้เลือก 'M' หรือ 'm' ให้ย้อนกลับไปที่เมนูหลัก
        if (choice == 'M' || choice == 'm') {
            printf("\nReturning to the Main Menu");
            for(int i = 0; i < 3; i++) {
                printf(". ");
                Sleep(1000);
            }
            system("cls");
            return;         // กลับไปแสดงเมนูหลักโดยให้ฟังก์ชัน `how_to_play()` จบลง
        } else if (choice == 'E' || choice == 'e') // ถ้าผู้ใช้เลือก 'E' หรือ 'e' ให้จบเกม
        {
            printf("\nExiting the game...\n");
            exit(0); // ออกจากเกม
        }
         else // ถ้าผู้ใช้ใส่ตัวเลือกไม่ถูกต้อง
         {
            printf("\nInvalid choice. Please enter 'M' for Main Menu or 'E' to Exit.");
            for(int i = 0; i < 3; i++) {
                printf(". ");
                Sleep(1000);
            }
        }

    } while (choice != 'M' && choice != 'm' && choice != 'E' && choice != 'e');
}

// ดึงไพ่หนึ่งใบจากสำรับที่สับแล้ว โดยเพิ่มจำนวนไพ่ที่แจกไปแล้ว  
Card dealCard() 
{
    // คืนค่าไพ่ใบต่อไปจากสำรับ และเพิ่มค่าตัวนับ numCardsDealt เพื่อให้แจกใบถัดไปในการเรียกครั้งต่อไป
    return deck.dealCards[deck.numCardsDealt++];
}

// ฟังก์ชันนี้สร้างสำรับไพ่ที่มีทั้งหมด 52 ใบ และรีเซ็ตตัวนับการแจกไพ่
void initializeDeck()
{
    // รีเซ็ตจำนวนไพ่ที่แจกไปแล้วเป็น 0
    deck.numCardsDealt = 0;

    // วนลูปเพื่อสร้างสำรับไพ่ โดยมี 4 ชุด (ดอก) และ 13 หน้าไพ่ในแต่ละชุด
    for (int suit = 0; suit < 4; ++suit) 
    {
        for (int face = 0; face < 13; ++face) 
        {
            // ตั้งค่าหน้าไพ่ (face) และดอกไพ่ (suit) สำหรับแต่ละใบ
            deck.dealCards[suit * 13 + face].face = face;          
            deck.dealCards[suit * 13 + face].suit = suit;
        }
    }
}

// ฟังก์ชันนี้สับไพ่โดยการสลับตำแหน่งของไพ่แบบสุ่ม
void shuffleDeck() 
{
    // วนลูปผ่านไพ่ทั้งหมดในสำรับ
    for (int i = 0; i < 52; ++i) 
    {
        // สุ่มหาตำแหน่งที่ต้องการสลับไพ่ด้วย
        int j = rand() % 52;
        // เก็บไพ่ใบที่ i ไว้ในตัวแปรชั่วคราว
        Card temp = deck.dealCards[i];
        // สลับไพ่ใบที่ i กับใบที่ตำแหน่งสุ่ม j
        deck.dealCards[i] = deck.dealCards[j];     
        // นำไพ่ใบที่ i ไปไว้ที่ตำแหน่ง j               
        deck.dealCards[j] = temp;
    }
}

void placeBet() 
{
    int validInput; // ประกาศตัวแปร 'validInput' เพื่อเก็บผลลัพธ์จากการรับอินพุต
    int USD = 36;   // กำหนดตัวแปร 'USD' เป็นค่า ASCII ของสัญลักษณ์ '$' ซึ่งเท่ากับ 36
    system("cls"); // ล้างหน้าจอคอนโซล
    // แสดงข้อความแสดงยอดเงินปัจจุบันของผู้เล่น โดยใช้สีที่กำหนดจากตัวแปร 'yellow', 'green', และ 'reset'
    printf("%sYour current is%s %d %s%c%s\n",yellow, green, playerMoney, yellow, USD, reset); 
    do {
        printf("%sEnter bet amount%s: ", yellow, reset); // แสดงข้อความให้ผู้ใช้ป้อนจำนวนเงินเดิมพัน โดยใช้สีที่กำหนด
        validInput = scanf("%d", &betAmount); // รับค่าที่ผู้ใช้ป้อน และเก็บไว้ในตัวแปร 'betAmount'

        if (validInput != 1) { // ตรวจสอบว่าผู้ใช้ป้อนค่าที่ไม่ใช่ตัวเลข
            printf("Invalid input. Please enter a number.\n"); // แจ้งเตือนให้ผู้ใช้ป้อนตัวเลข
            while (getchar() != '\n'); // ล้างบัฟเฟอร์อินพุต
            continue; // ข้ามไปยังรอบถัดไปของลูป do-while
        }

        if (betAmount <= 0) { // ตรวจสอบว่าจำนวนเงินเดิมพันน้อยกว่าหรือเท่ากับ 0
        // แสดงข้อความแจ้งว่าจำนวนเงินเดิมพันต้องมากกว่า 0 โดยใช้สี 'yellow', 'red', และ 'reset'
            printf("%sBet amount must be greater than%s 0.%s\n", yellow, red, reset);
        } 
        // ตรวจสอบว่าจำนวนเงินเดิมพันมากกว่ายอดเงินที่ผู้เล่นมี
        else if (betAmount > playerMoney) {
            // แสดงข้อความแจ้งว่าจำนวนเงินเดิมพันเกินยอดเงินที่มี
            printf("%sBet amount exceeds available money.%s\n", yellow, reset);
        } 
        else {
            // แสดงข้อความยืนยันว่าการเดิมพันได้รับการยอมรับ โดยใช้สี 'green', 'yellow', และ 'reset'
            printf("%sBet accepted%s: %s%d%s\n",green, reset, yellow, betAmount, reset);
            break; // ออกจากลูป do-while
        }
    } while (1); // ลูปจะทำงานต่อไปเรื่อย ๆ จนกว่าจะมีคำสั่ง break ออกจากลูป
    playerMoney = playerMoney - betAmount; // หักจำนวนเงินเดิมพันออกจากยอดเงินของผู้เล่น
}

// ฟังก์ชันนี้แจกไพ่ให้กับผู้เล่นและดีลเลอร์คนละ 2 ใบ
void generateHands() 
{
    playerHand.cardCount = 0;   // ตั้งค่าเริ่มต้นจำนวนไพ่ในมือผู้เล่นเป็น 0
    dealerHand.cardCount = 0;   // ตั้งค่าเริ่มต้นจำนวนไพ่ในมือดีลเลอร์เป็น 0

    // แจกไพ่ 2 ใบให้ดีลเลอร์ โดยใช้ฟังก์ชัน dealCard() แจกทีละใบ
    dealerHand.cards[dealerHand.cardCount++] = dealCard();
    dealerHand.cards[dealerHand.cardCount++] = dealCard();
    // แจกไพ่ 2 ใบให้ผู้เล่น
    playerHand.cards[playerHand.cardCount++] = dealCard();
    playerHand.cards[playerHand.cardCount++] = dealCard();

}

// ฟังก์ชันนี้แสดงไพ่ในมือของผู้เล่นและดีลเลอร์
void printHands() 
{
    // แสดงข้อความบอกมือไพ่ของดีลเลอร์ พร้อมสีแดง (red)
    printf("%sDealer's Hand%s:\n", red, reset);
    // วนลูปเพื่อให้เกิดความล่าช้า 1 วินาทีเพื่อสร้างบรรยากาศการแจกไพ่
    for (int i = 0; i < dealerHand.cardCount; ++i) 
    {
        Sleep(1000); // หน่วงเวลา 1 วินาที
    }
    printf("\n");
    // เรียกใช้ฟังก์ชัน printCardsInLine() เพื่อแสดงไพ่ของดีลเลอร์
    printCardsInLine(dealerHand.cards, dealerHand.cardCount);
    printf("\n");

    // แสดงข้อความบอกมือไพ่ของผู้เล่น พร้อมสีม่วง (magenta)
    printf("%sPlayer's Hand%s:\n", magenta, reset);
    // วนลูปเพื่อให้เกิดความล่าช้า 1 วินาทีอีกครั้งสำหรับผู้เล่น
    for (int i = 0; i < playerHand.cardCount; ++i) 
    {
        Sleep(1000);
    }
    printf("\n");
    // เรียกใช้ฟังก์ชัน printCardsInLine() เพื่อแสดงไพ่ของผู้เล่น
    printCardsInLine(playerHand.cards, playerHand.cardCount);
    printf("\n");
}

// ฟังก์ชันนี้แสดงไพ่หลายใบในแนวเดียวกัน
void printCardsInLine(Card cards[], int cardCount)
{
    // ชื่อหน้าไพ่ (A, 2, 3, ... K)
    char *faces[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    char suits[] = {heart, diamond, club, spade}; // สัญลักษณ์ดอกไพ่ (หัวใจ, ข้าวหลามตัด, ดอกจิก, โพธิ์ดำ)

    // พิมพ์เส้นขอบบนของไพ่ทั้งหมด
    for (int i = 0; i < cardCount; ++i)
        printf("%s-------%s  ", yellow, reset);
    printf("\n");

    // พิมพ์สัญลักษณ์ดอกไพ่ด้านซ้ายบนของไพ่แต่ละใบ
    for (int i = 0; i < cardCount; ++i)
        if (cards[i].suit == 0 || cards[i].suit == 1)
        {
            printf("%s|%s%c    %s|%s  ", yellow, red, suits[cards[i].suit], yellow, reset); // สีแดงสำหรับ heart และ diamond
        }
        else if (cards[i].suit == 2 || cards[i].suit == 3)
        {
            printf("%s|%s%c    %s|%s  ", yellow, black, suits[cards[i].suit], yellow, reset); // สีดำสำหรับ club และ spade
        }
    printf("\n");

    // พิมพ์ชื่อหน้าไพ่ตรงกลางของไพ่แต่ละใบ
    for (int i = 0; i < cardCount; ++i)
        printf("%s|%s  %s  %s|%s  ", yellow, reset, faces[cards[i].face], yellow, reset);
    printf("\n");

    // พิมพ์สัญลักษณ์ดอกไพ่ด้านขวาล่างของไพ่แต่ละใบ
    for (int i = 0; i < cardCount; ++i)
        if (cards[i].suit == 0 || cards[i].suit == 1)
        {
            printf("%s|    %s%c%s|%s  ", yellow, red, suits[cards[i].suit], yellow, reset); // สีแดงสำหรับ heart และ diamond
        }
        else if (cards[i].suit == 2 || cards[i].suit == 3)
        {
            printf("%s|    %s%c%s|%s  ", yellow, black, suits[cards[i].suit], yellow, reset); // สีดำสำหรับ club และ spade
        }
    printf("\n");

    // พิมพ์เส้นขอบล่างของไพ่ทั้งหมด
    for (int i = 0; i < cardCount; ++i)
        printf("%s-------%s  ", yellow, reset);
    printf("\n");
}

int calculateHandValue(Hand hand) 
{
    int value = 0;
    for (int i = 0; i < hand.cardCount; ++i) // วนลูปเพื่อคำนวณค่าของไพ่ในมือทีละใบ
    {
        int faceValue = hand.cards[i].face + 1; // กำหนดค่าให้ faceValue โดยเพิ่ม 1 เนื่องจากในโค้ดดั้งเดิมค่าของไพ่เริ่มต้นที่ 0
        if (faceValue > 10) faceValue = 10; // หากค่าไพ่เกิน 10 (คือ J, Q, K) ให้ปรับค่าเป็น 10 เนื่องจากไพ่เหล่านี้มีค่าเท่ากับ 10
        value += faceValue; // รวมค่าของไพ่ในแต่ละใบเข้ากับค่า value ที่จะใช้คำนวณทั้งหมด
    }
    return value % 10; // ผลรวมของค่าของไพ่ให้ใช้เฉพาะตัวเลขหลักสุดท้าย (เช่น หากผลรวมคือ 15 จะได้ค่าเป็น 5)
}

void playerTurn() 
{
    int playerHandValue = calculateHandValue(playerHand); // คำนวณค่ามือของผู้เล่น
    int dealerHandValue = calculateHandValue(dealerHand); // คำนวณค่ามือของดีลเลอร์

    // ตรวจสอบว่ามือของดีลเลอร์ไม่ใช่ 9 หรือ 8
    if( !(dealerHandValue == 9 || dealerHandValue == 8) )
    {
        // ถ้าค่ามือของผู้เล่นอยู่ระหว่าง 5 ถึง 7
        if (playerHandValue >= 5 && playerHandValue <= 7) 
        {
            // ลูปแบบไม่มีที่สิ้นสุดจนกว่าจะมีคำสั่ง break
            for(;;)
            {
                char choice; // ประกาศตัวแปรสำหรับเก็บตัวเลือกของผู้เล่น
                printf("\n%sYour hand value is %s%d%s. Do you want to (%sH%s)it or (%sS%s)tay?: ", yellow, red, playerHandValue, reset, blue, reset, red , reset); // แสดงข้อความถามผู้เล่นว่าต้องการจั่วไพ่หรืออยู่
                scanf(" %c", &choice); // รับอินพุตจากผู้เล่น 
                if (choice == 'h' || choice == 'H')  // ถ้าผู้เล่นเลือก 'h' หรือ 'H' 
                {
                    playerHand.cards[playerHand.cardCount++] = dealCard(); // จั่วไพ่เพิ่มและเพิ่มลงในมือของผู้เล่น
                    printf("%sYou drew a card.%s\n", yellow, reset); // แสดงข้อความว่าผู้เล่นจั่วไพ่แล้ว
                    printHands(); // แสดงไพ่ของผู้เล่นและดีลเลอร์
                    break; // ออกจากลูป
                }
                if (choice == 's' || choice == 'S') // ถ้าผู้เล่นเลือก 's' หรือ 'S'
                {
                    printf("Stay.\n"); // แสดงข้อความว่าอยู่ (ไม่จั่วไพ่)
                    break; // ออกจากลูป
                }
                else
                {
                    printf("H or S only "); // ถ้าอินพุตไม่ใช่ 'h', 'H', 's', หรือ 'S'
                    for(int i = 0; i < 3; i++)  // แจ้งเตือนให้ผู้ใช้เลือกเฉพาะ H หรือ S
                    {
                        printf(". "); 
                        Sleep(1000); // รอ 1 วินาที
                    }   
                    continue; // ทำซ้ำลูปใหม่
                }
            }
        } 
        else if (playerHandValue < 5)  // ถ้าค่ามือของผู้เล่นน้อยกว่า 5
        {
            // แสดงข้อความว่าค่ามือน้อยกว่า 5 และจะจั่วไพ่ให้
            printf("%sYour hand value is less than %s5%s. Drawing a card for you.%s\n", yellow, red, yellow, reset);
            playerHand.cards[playerHand.cardCount++] = dealCard(); // จั่วไพ่เพิ่ม
            printHands(); // แสดงไพ่ของผู้เล่นและดีลเลอร์
        }
        
    }
}

void dealerTurn() 
{
    int dealerHandValue = calculateHandValue(dealerHand); // คำนวณค่ามือของดีลเลอร์
    int playerHandValue = calculateHandValue(playerHand); // คำนวณค่ามือของผู้เล่น

    if( !(playerHandValue == 9 || playerHandValue == 8) ) // ตรวจสอบว่ามือของผู้เล่นไม่ใช่ 9 หรือ 8
    {    
        if (dealerHandValue <= 5)  // ถ้าค่ามือของดีลเลอร์น้อยกว่าหรือเท่ากับ 5
        {
            // แสดงข้อความว่าดีลเลอร์จะจั่วไพ่
            printf("%sDealer's hand value is less than %s5%s. Dealer draws a card.%s\n", yellow, red, yellow, reset);
            dealerHand.cards[dealerHand.cardCount++] = dealCard(); // ดีลเลอร์จั่วไพ่เพิ่ม
            printHands(); // แสดงไพ่ของผู้เล่นและดีลเลอร์ 
        }
    }
}

void checkMoney()
{
    if(playerMoney <= 0) // ถ้าเงินของผู้เล่นน้อยกว่าหรือเท่ากับ 0
    {
        printf("%sGame Over!%s\n", red, reset); // แสดงข้อความว่าเกมจบ
        exit(0); // ออกจากโปรแกรม
    } 
}

void askOver()
{
    char answer; // ประกาศตัวแปร 'answer' เพื่อเก็บคำตอบของผู้เล่น
    do {
        printf("%sWould you like to play one more time?%s\n", yellow, reset); // ถามว่าผู้เล่นต้องการเล่นอีกครั้งหรือไม่
        printf("(y)es / (n)o: ");
        scanf(" %c", &answer); // รับคำตอบจากผู้เล่น

        // ถ้าผู้เล่นเลือก 'y' หรือ 'Y'
        if(answer == 'y' || answer == 'Y') {
            play(); // เรียกใช้ฟังก์ชัน play() เพื่อเริ่มเกมใหม่
        } 
        // ถ้าผู้เล่นเลือก 'n' หรือ 'N'
        else if(answer == 'n' || answer == 'N') {
            // แสดงข้อความขอบคุณ
            printf("%sThank you for playing! Exiting now%s ",green, reset);
            for(int i = 0; i < 5; i++) 
            {
                printf("%s%c%s ", red, heart, reset);
                Sleep(1000); // รอ 1 วินาที
            } 
            system("cls"); // ล้างหน้าจอคอนโซล
            exit(0); // ออกจากโปรแกรม
        } 
        // ถ้าผู้เล่นป้อนคำตอบที่ไม่ใช่ 'y', 'Y', 'n', หรือ 'N'
        else {
            // แสดงข้อความแจ้งเตือนให้ป้อน 'y' หรือ 'n' เท่านั้น
            printf("Please enter '%sY%s' or '%sN%s' only ", red, reset, red, reset);
            for(int i = 0; i < 3; i++) {
                printf(". ");
                Sleep(1000); // รอ 1 วินาที
            }
            printf("\n");
            system("cls"); // ล้างหน้าจอคอนโซล
        }

    } while(answer != 'n' && answer != 'N'); // ลูปทำงานต่อไปจนกว่าผู้เล่นจะเลือก 'n' หรือ 'N'
}

void analyzeHands() {
    // คำนวณค่าของไพ่สองใบแรกในมือของผู้เล่นและดีลเลอร์
    int playerInitialValue = calculateHandValue((Hand){{playerHand.cards[0], playerHand.cards[1]}, 2});
    int dealerInitialValue = calculateHandValue((Hand){{dealerHand.cards[0], dealerHand.cards[1]}, 2});

    // คำนวณค่าของไพ่ทั้งหมดในมือของผู้เล่นและดีลเลอร์
    int playerHandValue = calculateHandValue(playerHand);
    int dealerHandValue = calculateHandValue(dealerHand);

    // แสดงค่ารวมของไพ่ในมือของดีลเลอร์และผู้เล่น
    printf("%sDealer's%s Hand Total Value: %s%d%s\n", red, reset, yellow, dealerHandValue, reset);
    printf("%sPlayer's%s Hand Total Value: %s%d%s\n", magenta, reset, yellow, playerHandValue, reset);

    // ตรวจสอบว่าผู้เล่นมีไพ่ 2 ใบที่มีดอกเดียวกันหรือไม่
    int playerHas2MatchingSuits = (playerHand.cardCount == 2 && playerHand.cards[0].suit == playerHand.cards[1].suit);
    // ตรวจสอบว่าผู้เล่นมีไพ่ 3 ใบที่มีดอกเดียวกันหรือไม่
    int playerHas3MatchingSuits = (playerHand.cardCount == 3 && playerHand.cards[0].suit == playerHand.cards[1].suit && playerHand.cards[1].suit == playerHand.cards[2].suit);
    // ตรวจสอบว่าดีลเลอร์มีไพ่ 2 ใบที่มีดอกเดียวกันหรือไม่
    int dealerHas2MatchingSuits = (dealerHand.cardCount == 2 && dealerHand.cards[0].suit == dealerHand.cards[1].suit);
    // ตรวจสอบว่าดีลเลอร์มีไพ่ 3 ใบที่มีดอกเดียวกันหรือไม่
    int dealerHas3MatchingSuits = (dealerHand.cardCount == 3 && dealerHand.cards[0].suit == dealerHand.cards[1].suit && dealerHand.cards[1].suit == dealerHand.cards[2].suit);

    // ตรวจสอบว่าผู้เล่นมีไพ่ 2 ใบที่มีค่าเท่ากันหรือไม่
    int playerHas2MatchingFaces = (playerHand.cardCount == 2 && playerHand.cards[0].face == playerHand.cards[1].face);
    // ตรวจสอบว่าดีลเลอร์มีไพ่ 2 ใบที่มีค่าเท่ากันหรือไม่
    int dealerHas2MatchingFaces = (dealerHand.cardCount == 2 && dealerHand.cards[0].face == dealerHand.cards[1].face);

    // กรณีที่ผู้เล่นได้ค่า 9 หรือ 8 จากไพ่สองใบแรก (Pok)
    if (playerInitialValue == 9 || playerInitialValue == 8) {
        // ผู้เล่นชนะหากคะแนนมากกว่าดีลเลอร์
        if (playerInitialValue > dealerInitialValue) 
        {
            // ถ้าผู้เล่นมีไพ่ที่ดอกหรือหน้าตรงกัน จะได้เงินเดิมพันเพิ่มเป็น 3 เท่า
            if (playerHas2MatchingSuits || playerHas2MatchingFaces) 
            {
                printf("%sPlayer%s wins with %sPok %s%d %sand %smatching cards!%s\n", magenta, reset, green, yellow, playerInitialValue,  reset, green, reset);
                playerMoney = playerMoney + (betAmount * 3); 
                
            } 
            // ถ้าไม่มีไพ่ตรงกัน จะได้เงินเดิมพันเพิ่มเป็น 2 เท่า
            else 
            {
                printf("%sPlayer%s wins with %sPok %s%d!%s\n", magenta, reset, green, yellow, playerInitialValue, reset);
                playerMoney += betAmount * 2;
                
            }
        }
        // ถ้าคะแนนของดีลเลอร์และผู้เล่นเท่ากัน ถือว่าเสมอ
        else if (dealerInitialValue == playerInitialValue) 
        {
            printf("It's a %stie%s with both having %sPok %s%d!%s\n", green, reset, green, yellow, playerInitialValue, reset);
           
            playerMoney = playerMoney + betAmount; // ผู้เล่นได้เงินเดิมพันคืน
            
        } 
    }

    // กรณีที่ดีลเลอร์ได้ค่า 9 หรือ 8 จากไพ่สองใบแรก (Pok)
    else if (dealerInitialValue == 9 || dealerInitialValue == 8) {
        if (dealerInitialValue > playerInitialValue ) {
            // ถ้าดีลเลอร์มีไพ่ที่ดอกหรือหน้าตรงกัน ผู้เล่นเสียเงินเดิมพันทั้งหมด
            if (dealerHas2MatchingSuits || dealerHas2MatchingFaces) 
            {
                printf("%sDealer%s wins with %sPok %s%d%s and %smatching cards!%s\n", red, reset, green, yellow, dealerInitialValue, reset);
                playerMoney -= betAmount; 
                
            } 
            // ถ้าไม่มีไพ่ตรงกัน ผู้เล่นจะไม่เสียเงินเพิ่มเติม
            else 
            {
                printf("%sDealer%s wins with %sPok %s%d!%s\n", red, reset, green, yellow, dealerInitialValue, reset);
                playerMoney = playerMoney; 
                
            }
        }
    }
    // กรณีที่คะแนนไพ่ของผู้เล่นมากกว่าดีลเลอร์
    else if (playerHandValue > dealerHandValue)
    {
        // ถ้าผู้เล่นมีไพ่ที่ดอกหรือหน้าตรงกัน จะได้เงินเดิมพันเพิ่มเป็น 3 เท่า
        if (playerHas2MatchingSuits || playerHas2MatchingFaces)
        {
            printf("%sPlayer%s wins with %smatching cards!%s\n", magenta, reset, green, reset);
            playerMoney += betAmount * 3;
        }
        // ถ้าผู้เล่นมีไพ่ 3 ใบที่ดอกเดียวกัน จะได้เงินเดิมพันเพิ่มเป็น 4 เท่า
        else if (playerHas3MatchingSuits)
        {
            printf("%sPlayer%s wins with %s3 matching cards!%s\n", magenta, reset, green, reset);
            playerMoney += betAmount * 4;
        }
        // ถ้าไม่มีไพ่ตรงกัน จะได้เงินเดิมพันเพิ่มเป็น 2 เท่า
        else 
        {
            printf("%sPlayer win!%s\n", green, reset);
            playerMoney += betAmount * 2;
        }
    }
    // กรณีที่คะแนนไพ่ของดีลเลอร์มากกว่าผู้เล่น
    else if (dealerHandValue > playerHandValue)
    {
        // ถ้าดีลเลอร์มีไพ่ที่ดอกหรือหน้าตรงกัน ผู้เล่นเสียเงินเดิมพันทั้งหมด
        if (dealerHas2MatchingSuits || dealerHas2MatchingFaces)
        {
            printf("%sDealer%s wins with %smatching cards!%s\n", red, reset, green, reset);
            playerMoney -= betAmount;
        }
        // ถ้าดีลเลอร์มีไพ่ 3 ใบที่ดอกเดียวกัน ผู้เล่นเสียเงินเดิมพัน 2 เท่า
        else if (dealerHas3MatchingSuits)
        {
            printf("%sDealer%s wins with %s3 matching cards!%s\n", red, reset, green, reset);
            playerMoney -= betAmount * 2;
        }
        // ถ้าไม่มีไพ่ตรงกัน ดีลเลอร์ชนะ แต่ไม่เพิ่มเงินเดิมพันที่เสีย
        else
        {
            printf("%sDealer win!%s\n", red, reset);
        }
    }
    // กรณีคะแนนไพ่เท่ากัน
    else
    {
        printf("%sTie!%s\n", green, reset);
        playerMoney += betAmount; // เสมอคืนเงินเดิมพันให้ผู้เล่น
    }

    // แสดงเงินที่เหลือของผู้เล่น
    printf("%sPlayer Money: %s%d%s\n", yellow, green, playerMoney, reset);
    checkMoney(); // ตรวจสอบเงินของผู้เล่นว่ามีเพียงพอที่จะเล่นต่อหรือไม่
    askOver(); // สอบถามผู้เล่นว่าต้องการเล่นต่อหรือไม่
}

void play()
{
    // กำหนดค่าเริ่มต้นให้กับตัวสร้างตัวเลขสุ่ม (random number generator) โดยใช้เวลาปัจจุบันเพื่อให้การสุ่มแตกต่างกันทุกครั้งที่เรียกใช้โปรแกรม
    srand(time(NULL));

    initializeDeck(); // เรียกใช้ฟังก์ชันเพื่อสร้างและกำหนดไพ่ในสำรับ
    shuffleDeck(); // สับไพ่ในสำรับให้มีลำดับแบบสุ่ม

    placeBet(); // เรียกใช้ฟังก์ชันเพื่อให้ผู้เล่นวางเงินเดิมพัน

    generateHands(); // แจกไพ่ให้กับผู้เล่นและดีลเลอร์
    printHands(); // แสดงไพ่ที่แจกให้ผู้เล่นและดีลเลอร์

    playerTurn(); // เรียกใช้ฟังก์ชันเพื่อดำเนินการรอบของผู้เล่น
    dealerTurn(); // เรียกใช้ฟังก์ชันเพื่อดำเนินการรอบของดีลเลอร์

    analyzeHands(); // วิเคราะห์และตรวจสอบผลลัพธ์ของไพ่ในมือของผู้เล่นและดีลเลอร์
}