#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<string.h>

#define width 115
#define height 45
#define NUM_STOCKS 10
#define MIN_VALUE 100 // INR
#define MAX_VALUE 10000 // INR
#define FLUCTUATION_MIN -10
#define FLUCTUATION_MAX 10


FILE * fptr;
FILE *fptr2;
char fname[50];

long Acc_No;
long passcode;
long balance;

typedef struct {
    long account_number;
    long passcode;
    double balance;
} AccountInfo;

typedef struct {
    char name[20];
    double value;
    double growth_rate;
    int quantity_owned;
} Stock;

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);//predefined function in conio.h for windows system but for unix based compilers have to define
}
void delay(int microseconds) {
    usleep(microseconds);
}
void tempelate(){
    system("cls");
gotoxy(20,1);
printf("%c",201);
int i;
for( i=0; i<width-2; i++){
    printf("%c", 205);
}
printf("%c", 187);
printf("\n");
gotoxy(20,2);
for( i=0; i<height-2; i++){
    gotoxy(20,i+2);
    printf("%c", 186);
}
gotoxy(20+width-1, 2);
for( i=0; i<height-2; i++){
gotoxy(20+width-1, 2+i);
printf("%c", 186);
}
gotoxy(20, height);
printf("%c",200);
for( i=0; i<width-2; i++){
printf("%c", 205);
}
printf("%c",188);
gotoxy(62,3);
printf("IIT Jodhpur Banking systum");
}
//void encryptDecrypt(char *data) {
  //  char key = ENCRYPTION_KEY;
   // size_t len = strlen(data);
    //int i;
   // for (i = 0; i < len; i++) {
    //    data[i] ^= key;
  //  }
//}
long generateRandomNumber(int numDigits) {
    long minRange = 1;
    long maxRange = 1;
    int i;
    for ( i = 1; i < numDigits; i++) {
        minRange *= 10;
        maxRange = maxRange * 10 + 9;
    }

    srand(time(NULL));

    long randomNumber = minRange + rand() % (maxRange - minRange + 1);

    return randomNumber;
}

void initialize_stocks(Stock stocks[], int num_stocks) {
    int i;
    for (i = 0; i < num_stocks; ++i) {
        sprintf(stocks[i].name, "Stock%d", i + 1);
        stocks[i].value = (double)(rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE);
        stocks[i].growth_rate = 0.0;
        stocks[i].quantity_owned = 0;
    }
}

double random_fluctuation() {
    return (rand() % (FLUCTUATION_MAX - FLUCTUATION_MIN + 1) + FLUCTUATION_MIN) / 100.0;
}

void simulate_stock_market(Stock stocks[], int num_stocks) {
    tempelate();
    int i;
    for (i = 0; i < num_stocks; ++i) {
        double previous_value = stocks[i].value;
        double fluctuation = random_fluctuation();
        stocks[i].growth_rate = fluctuation * 100.0;
        stocks[i].value *= (1 + fluctuation);

        double percentage_change = ((stocks[i].value - previous_value) / previous_value) * 100;
        gotoxy(40, 12 + i);
        printf("%s: INR %.2f  Quantity Owned: %d  Change: %.2f%%\n", stocks[i].name, stocks[i].value, stocks[i].quantity_owned, percentage_change);
    }
}

void buy_stocks(Stock stocks[], int num_stocks, double *balance) {
    tempelate();
    int stock_index;
    int quantity;
    gotoxy(40, 10);
    printf("\tEnter the index of the stock you want to buy (1-%d): ", num_stocks);
    scanf("%d", &stock_index);
    gotoxy(40, 11);
    printf("\tEnter the quantity you want to buy: ");
    scanf("%d", &quantity);

    if (stock_index < 1 || stock_index > num_stocks) {
        gotoxy(40, 14);
        printf("\tInvalid stock index.\n");
        return;
    }

    stock_index--;
    if (stocks[stock_index].value * quantity <= *balance) {
        stocks[stock_index].quantity_owned += quantity;
        *balance -= stocks[stock_index].value * quantity;
        gotoxy(40, 15);
        printf("\tSuccessfully purchased %d shares of %s.\n", quantity, stocks[stock_index].name);
    } else {
        gotoxy(40, 15);
        printf("\tInsufficient funds to purchase.\n");
    }
}

void sell_stocks(Stock stocks[], int num_stocks, double *balance) {
    tempelate();
    int stock_index;
    int quantity;
    gotoxy(40, 10);
    printf("\tEnter the index of the stock you want to sell (1-%d): ", num_stocks);
    scanf("%d", &stock_index);
    gotoxy(40, 11);
    printf("\tEnter the quantity you want to sell: ");
    scanf("%d", &quantity);

    if (stock_index < 1 || stock_index > num_stocks) {
        gotoxy(40, 14);
        printf("\tInvalid stock index.\n");
        return;
    }

    stock_index--;
    if (stocks[stock_index].quantity_owned >= quantity) {
        stocks[stock_index].quantity_owned -= quantity;
        *balance += stocks[stock_index].value * quantity;
        gotoxy(40, 15);
        printf("\tSuccessfully sold %d shares of %s.\n", quantity, stocks[stock_index].name);
    } else {
        gotoxy(40, 15);
        printf("\tInsufficient shares to sell.\n");
    }
}

double read_balance(long account_number) {
    FILE *file = fopen("acc_details.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 0.0;
    }

    AccountInfo account;
    while (fscanf(file, "%ld %ld %lf", &account.account_number, &account.passcode, &account.balance) == 3) {
        if (account.account_number == account_number) {
            fclose(file);
            return account.balance;
        }
    }

    fclose(file);
    printf("Account not found.\n");
    return 0.0;
}

void write_balance(long account_number, double balance) {
    FILE *file = fopen("acc_details.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    AccountInfo account;
    while (fscanf(file, "%ld %ld %lf", &account.account_number, &account.passcode, &account.balance) == 3) {
        if (account.account_number == account_number) {
            fprintf(temp, "%ld %ld %.2lf\n", account_number, account.passcode, balance);
        } else {
            fprintf(temp, "%ld %ld %.2lf\n", account.account_number, account.passcode, account.balance);
        }
    }

    fclose(file);
    fclose(temp);

    if (remove("acc_details.txt") != 0) {
        printf("Error deleting the original file.\n");
        return;
    }

    if (rename("temp.txt", "acc_details.txt") != 0) {
        printf("Error renaming the temporary file.\n");
        return;
    }
    gotoxy(40,24);

    printf("Balance updated successfully.\n");
}

void displayInsuranceSchemes() {
    tempelate();
    gotoxy(22, 6);
    printf("Insurance Schemes Offered:");
    gotoxy(22, 8);
    printf("1. Life Insurance");
    gotoxy(22, 9);
    printf("2. Health Insurance");
    gotoxy(22, 10);
    printf("3. Property Insurance");
    gotoxy(22, 11);
    printf("4. Vehicle Insurance");
    gotoxy(22, 12);
    printf("5. Travel Insurance");
        int option;
    gotoxy(22,14);
      printf("Enter your choice: ");
    scanf("%d", &option);

    // Handle user selection
    selectOption(option);
}
void displayLifeInsurance() {
    tempelate();
    gotoxy(22, 6);
    printf("Life Insurance Coverage:");
    gotoxy(22, 8);
    printf("- Financial protection for family members");
    gotoxy(22, 9);
    printf("- Covers funeral expenses");
    gotoxy(22, 10);
    printf("- Pays off debts");
    gotoxy(22,12);
    char ch;
    printf("At mere monthly payments of 123 p.m");
    gotoxy(22,13);
    printf("Would you like to sign up for it(y/n) ");
    long temppass;
    ch=getch();
    if(ch=='y'){
            gotoxy(22,15);
        printf("Confirm passcode- ");
        scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }
        else{
            gotoxy(22,17);
            printf("Re-try entering password ");
    scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }else{
            system("cls");
            gotoxy(55,8);
            printf("Intruder Alert ");
            gotoxy(55,10);
            printf("Logging out...");
            delay(750000);
            return 0;
        }
        }
    }
    else if(ch=='n'){
        gotoxy(22,15);
            printf("Thankyou for enquiring our services  ");
            gotoxy(22,16);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
    }
}

// Function to display what Health Insurance covers
void displayHealthInsurance() {
    tempelate();
    gotoxy(22, 6);
    printf("Health Insurance Coverage:");
    gotoxy(22, 8);
    printf("- Medical expenses for illnesses and injuries");
    gotoxy(22, 9);
    printf("- Hospitalization costs");
    gotoxy(22, 10);
    printf("- Prescription drugs");
     char ch;
    printf("At mere monthly payments of 123 p.m");
    gotoxy(22,13);
    printf("Would you like to sign up for it(y/n) ");
    long temppass;
    ch=getch();
    if(ch=='y'){
            gotoxy(22,15);
        printf("Confirm passcode- ");
        scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }
        else{
            gotoxy(22,17);
            printf("Re-try entering password ");
    scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }else{
            system("cls");
            gotoxy(55,8);
            printf("Intruder Alert ");
            gotoxy(55,10);
            printf("Logging out...");
            delay(750000);
            return 0;
        }
        }
    }
    else if(ch=='n'){
        gotoxy(22,15);
            printf("Thankyou for enquiring our services  ");
            gotoxy(22,16);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
    }
}

// Function to display what Property Insurance covers
void displayPropertyInsurance() {
    tempelate();
    gotoxy(22, 6);
    printf("Property Insurance Coverage:");
    gotoxy(22, 8);
    printf("- Protection for your home or property against damage or theft");
    gotoxy(22, 9);
    printf("- Coverage for personal belongings");
    gotoxy(22, 10);
    printf("- Liability protection");
     char ch;
    printf("At mere monthly payments of 486 p.m");
    gotoxy(22,13);
    printf("Would you like to sign up for it(y/n) ");
    long temppass;
    ch=getch();
    if(ch=='y'){
            gotoxy(22,15);
        printf("Confirm passcode- ");
        scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            balance=balance-486;
            rewind(fptr);
            long temp;
            while(fscanf(fptr, "%ld", &temp)!=EOF || temp!=Acc_No){
                fscanf(fptr, "%ld", &temp);
                fprintf(fptr, "%ld", balance);
            }
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }
        else{
            gotoxy(22,17);
            printf("Re-try entering password ");
    scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }else{
            system("cls");
            gotoxy(55,8);
            printf("Intruder Alert ");
            gotoxy(55,10);
            printf("Logging out...");
            delay(750000);
            return 0;
        }
        }
    }
    else if(ch=='n'){
        gotoxy(22,15);
            printf("Thankyou for enquiring our services  ");
            gotoxy(22,16);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
    }
}
// Function to display what Vehicle Insurance covers
void displayVehicleInsurance() {
    tempelate();
    gotoxy(22, 6);
    printf("Vehicle Insurance Coverage:");
    gotoxy(22, 8);
    printf("- Protection for your vehicle against accidents and theft");
    gotoxy(22, 9);
    printf("- Coverage for repairs or replacement");
    gotoxy(22, 10);
    printf("- Liability protection");
     char ch;
    printf("At mere monthly payments of 209 p.m");
    gotoxy(22,13);
    printf("Would you like to sign up for it(y/n) ");
    long temppass;
    ch=getch();
    if(ch=='y'){
            gotoxy(22,15);
        printf("Confirm passcode- ");
        scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }
        else{
            gotoxy(22,17);
            printf("Re-try entering password ");
    scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }else{
            system("cls");
            gotoxy(55,8);
            printf("Intruder Alert ");
            gotoxy(55,10);
            printf("Logging out...");
            delay(750000);
            return 0;
        }
        }
    }
    else if(ch=='n'){
        gotoxy(22,15);
            printf("Thankyou for enquiring our services  ");
            gotoxy(22,16);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
    }
}

// Function to display what Travel Insurance covers
void displayTravelInsurance() {
    tempelate();
    gotoxy(22, 6);
    printf("Travel Insurance Coverage:");
    gotoxy(22, 8);
    printf("- Trip cancellation and interruption coverage");
    gotoxy(22, 9);
    printf("- Emergency medical and dental coverage");
    gotoxy(22, 10);
    printf("- Lost or delayed baggage coverage");
     char ch;
    printf("At mere monthly payments of 121 p.m");
    gotoxy(22,13);
    printf("Would you like to sign up for it(y/n) ");
    long temppass;
    ch=getch();
    if(ch=='y'){
            gotoxy(22,15);
        printf("Confirm passcode- ");
        scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            balance-=121;
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }
        else{
            gotoxy(22,17);
            printf("Re-try entering password ");
    scanf("%ld", &temppass);
        if(temppass==passcode){
                system("cls");
        tempelate();
        gotoxy(55,7);
            printf("Thankyou for availing our services ");
            balance-=121;
            gotoxy(55,9);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
        }
        else{
            system("cls");
            gotoxy(55,8);
            printf("Intruder Alert ");
            gotoxy(55,10);
            printf("Logging out...");
            delay(750000);
            return 0;
        }
        }
    }
    else if(ch=='n'){
        gotoxy(22,15);
            printf("Thankyou for enquiring our services  ");
            gotoxy(22,16);
            printf("Taking you back to the home page...");
            delay(750000);
            options();
    }

}
// Function to handle selection using switch-case mechanism
void selectOption(int option) {
    switch (option) {
        case 1:
            tempelate();
            gotoxy(22, 6);
            printf("You have selected: Life Insurance");
            displayLifeInsurance();
            break;
        case 2:
            tempelate();
            gotoxy(22, 6);
            printf("You have selected: Health Insurance");
            displayHealthInsurance();
            break;
        case 3:
            tempelate();
            gotoxy(22, 6);
            printf("You have selected: Property Insurance");
            displayPropertyInsurance();
            break;
        case 4:
            tempelate();
            gotoxy(22, 6);
            printf("You have selected: Vehicle Insurance");
            displayVehicleInsurance();
            break;
        case 5:
            tempelate();
            gotoxy(22, 6);
            printf("You have selected: Travel Insurance");
            displayTravelInsurance();
            break;
        default:
            tempelate();
            gotoxy(22, 6);
            printf("Invalid option");
    }
}
long encrypt(long input) {
    // Define a secret key for encryption
    long key = 1234567890;
    // Perform encryption using XOR operation with the key
    return input ^ key;
}

// Function to decrypt a long type encrypted object
long decrypt(long encrypted) {
    // Define the same secret key used for encryption
    long key = 1234567890;
    // Perform decryption using XOR operation with the key
    return encrypted ^ key;
}
