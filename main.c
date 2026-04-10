#include<stdio.h>
#include<string.h>
#include<stdbool.h>


//CONSTANTS

#define MAX 50
#define FILENAME "bank_data.txt"

// DATA STRUCTURS


typedef struct {
    int id;        /* Unique account ID, starts at 1001 */
    char name[50]; /* Account holder full name */
    float balance; /* Current balance in DZD */
    int pin;       /* 4-digit PIN */
    int active;    /* 1 = open, 0 = closed */
} Account;

Account accounts[MAX];
int count = 0;   /* Number of accounts currently stored */
int nextID = 1001;

// FUNCTION DECLARATIONS 

void loadData();
void saveData();
void createAccount();
int findAccount(int id);
int verifyPIN(int idx);
void deposit(int idx, float amount);
void withdraw(int idx, float amount);
void transfer(int from, int to, float amount);
void closeAccount(int idx);
void printStatement(int idx);
void listAllAccounts();
void bankSummary();



//  COLORE ( ANSI ESCAPE CODES )

#define RED     "\033[31m"
#define GREEN   "\033[32m"  // Healthy balance (>=1000 DZD)
#define YELLOW  "\033[33m"  // Low balance (< 1000 DZD)
#define RESET   "\033[0m"   // Zero balance


// MAIN 

int main () {

    loadData(); // at first it must show No existing data found. Starting with an empty bank.
 
    int choice =0; // for choosing from 0 to 8 choices
    int id,idx,from,to ;
    float amount;
    int oldbalance;
   

        
   

    do
    {
        printf("============================================\n");
        printf("      NATIONAL BANK OF ALGERIA-- MENU\n");
        printf("============================================\n");
    
        printf("1. Create new account\n");
        printf("2. Deposit funds\n");
        printf("3. Withdraw funds\n");
        printf("4. Transfer between accounts\n");
        printf("5. View account statement\n");
        printf("6. List all accounts (sorted by balance)\n");
        printf("7. Bank summary\n");
        printf("8. Close account\n");
        printf("0. Save and exit\n");
        printf("============================================\n");
        printf("\n - Enter your choice: ");scanf("%d",&choice);

        switch (choice)
        {
      
        case 1:         // Create account 
           createAccount();
           saveData();   // saving data after  option risk of data loss when program crash
           break;
        case 2:         // Deposit  
            printf(" - Enter account ID: ");
            scanf("%d",&id);
            idx=findAccount(id);
            if (idx == -1)
            {
                printf(" - Account not found or inactive!\n");
                break;
            }

            if (verifyPIN(idx) == 0) break;

            printf(" - Enter amount to deposit: ");
            scanf("%f",&amount);
            oldbalance = accounts[idx].balance;
            deposit(idx,amount);
            if (oldbalance < accounts[idx].balance)
            {
                printf(" - Successfully deposited %.2f DZD\n",amount);   
                printf(" - Your Balance is %.2f DZD\n",accounts[idx].balance); 
            }
            saveData();   // saving data after option risk of data loss when program crash
            break;

        case 3:        // Withdraw
            printf(" - Enter account ID: ");
            scanf("%d",&id);
            idx=findAccount(id);
            if (idx == -1)
            {
                printf(" - Account not found or inactive!\n");
                break;
            }

            if (verifyPIN(idx) == 0) break;

            printf(" - Enter amount to withdraw: ");
            scanf("%f",&amount);
            oldbalance = accounts[idx].balance;
            withdraw(idx,amount);

            if (oldbalance > accounts[idx].balance)
            {
               printf(" - Successfully withdrew %.2f DZD\n",amount);
               printf(" - NEW balance : %.2f DZD\n ",accounts[idx].balance); 
            }
            saveData();   // saving data after option risk of data loss when program crash
            break;

        case 4:         // Tranfer
            printf(" - Enter source account ID: ");
            scanf("%d",&id);
            from = findAccount(id);
            if (from == -1)  { printf(" - Source account not found or inactive!\n");
                break; }
            if (verifyPIN(from) == 0) break;

            printf(" - Enter destination account ID: ");
            scanf("%d",&id);
            to = findAccount(id);
            if (to == -1)  { printf(" - Destination account not found or inactive!\n");
                break; }
          //  if (verifyPIN(to) == 0) break;  NO need to verifyPIN in real life only verify scource account

            printf(" - Enter amount to transfer: ");
            scanf("%f",&amount);
            transfer(from,to,amount);
            saveData();   // saving data after  option risk of data loss when program crash
            break;


        case 5:         // View statement 
          printf(" - Enter account ID: ");
          scanf("%d", &id);
          idx = findAccount(id);
          if (idx == -1) {
          printf(" - Account not found or inactive!\n");
          break;}
          if (verifyPIN(idx) == 1)
          {
            printStatement(idx);
          }
            break;


        case 6:       // listAllAccounts();
           listAllAccounts();

            break;

        case 7:       //bankSummary();
           bankSummary();

            break;

        case 8:        //Close account 
           printf(" - Enter account ID: ");
           scanf("%d", &id);
           idx = findAccount(id);
           if (idx == -1) {
           printf(" - Account not found or inactive!\n");
           break;    }
           if (verifyPIN(idx) == 0) break; 
           closeAccount(idx);
           break;


        case 0:
            saveData();
            printf("\n - DATA saved.\nThank you for using THE NATIONAL BANK OF ALGERIA!\n");
            break;   
        default:
            printf("\n - Invalid choice! Please select 0-8\n");
            break;
        }
    } while (choice != 0);
    



    return 0;
}

// lOAD THE DATE FROM FILE

void loadData () {
   
 FILE *pfile= fopen(FILENAME,"r");//r for read

    
        if (pfile == NULL)
         {
        printf(" - No existing data found. Starting with an empty bank.\n");
        count = 0;
        nextID = 1001;
        return;
        }

        count =0;
        while (count<MAX)
        {
            // Usind | as separator so names with spaces are allowed

            if ( fscanf(pfile, "%d|%49[^|]|%f|%d|%d\n",     /* used %49 to let space for \0 and [^|] to read name with space */
                &accounts[count].id,
                accounts[count].name, // this's array so no need to &
                &accounts[count].balance,
                &accounts[count].pin,
                &accounts[count].active) != 5)
            {
                break; // end of file
            }

           // Validate PIN

            if (accounts[count].pin < 0 || accounts[count].pin > 9999) {
               printf(" - Invalid PIN (MUST BE BETWEEN 0000 AND 9999) for account %d. Setting to '0000' .\n", accounts[count].id);
               accounts[count].pin=0000;
              }

            // Validate active
            if (accounts[count].active != 0 && accounts[count].active != 1) {
               printf(" - Invalid active for account %d. Setting to closed.\n", accounts[count].id);
              accounts[count].active = 0; //close it automatically
             }

             count++; // counting number of accounts
        }

    
     fclose(pfile);
     printf(" - Loaded %d account(s) successfully.\n",count);  // to see if the load works or not and the number of accs

    if (count>0)
    {
        nextID = accounts[count - 1].id + 1;
    }
    
    


}

// SAVE DATA IN FILE

void saveData(){

    FILE *pFile= fopen(FILENAME,"w");// w for write

    if (pFile== NULL)
    {
        printf(" - Error: Could not create/open bank_data.txt for saving!\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(pFile,"%d|%s|%.2f|%d|%d\n",
           accounts[i].id,
           accounts[i].name, 
           accounts[i].balance,
           accounts[i].pin,
           accounts[i].active);
    }
    
    
    
    
    //fprintf(pFile,);// fprintf means file printf. special for files

    
    fclose(pFile);// when u open a file be sure to CLOSE IT

}

// TO CREATE A NEW ACCOUNT

void createAccount(){


    //check if Bank is full

    if (count == MAX)
    {
        printf(" - Error: Bank is full! Maximum %d accounts allowed.\n",MAX);
        return;
    }

    printf("=== Create New Account ===\n");
    
   
    //FULL NAME 

    printf(" - Enter your full name : ");

   // getchar();   // CLEAR LEFTOVER NEWLINE FROM PREVIOUS SCANF   BUT NEXT SOLUTION IS BETTER THATS WHY I SWITHED TO IT.
   // clear input buffer safely

   int c;
   while ((c = getchar()) != '\n'  &&  c != EOF)  // EOF : end of file ,
   {  // empty loop to clear buffer 
    }
   
    fgets(accounts[count].name,50,stdin); 
    //Remove newline from fgets
    accounts[count].name[strcspn(accounts[count].name, "\n")] = '\0';
  
    // CHECK FOR INITIAL DEPOSIT

    do
    {
        printf(" - Enter initial deposit (MUST BE >=1000 DZD) : ");
        scanf("%f",&accounts[count].balance);

        if (accounts[count].balance < 1000)
        {
            printf(" - Initial deposit must be at least 1000 DZD!\n");
        }
        
    } while (accounts[count].balance<1000);
 
    // CHECK FOR 4-DIGIT PIN

    do {
      printf(" - Enter 4-digit PIN(0000-9999): ");
      scanf("%d", &accounts[count].pin);

     if (accounts[count].pin<1000 || accounts[count].pin>9999)
     {
         printf(" - Invalid PIN! Must be between 1000 and 9999.\n");
     }
     

   } while (accounts[count].pin < 0 || accounts[count].pin > 9999);
   
   // FINALIZE ACCOUNT 

   accounts[count].id = nextID; // unique ID
   accounts[count].active = 1; // acc is open

   nextID++; // prepare ID for next acc
   count++; //counting acc

   // NOW displaying the informations 
  
    printf("\n  - Account created successfully!\n");
    printf(" - Account ID : %d\n", accounts[count-1].id);
    printf(" - Name       : %s\n", accounts[count-1].name);
    printf(" - Balance    : %.2f DZD\n", accounts[count-1].balance);
    printf(" - PIN        : %04d\n", accounts[count-1].pin);

    
      
}


// NOT SURE OF THESE NEED TO CHECK LATER  

// FINDING ACCOUNT BY ID

int findAccount(int id){

    for ( int i = 0; i < count; i++)
    {
        if ((accounts[i].id == id)&&(accounts[i].active==1))
        {
           
            return i;//found!!
        } 
    }

    
        return -1;  // not found
    


}

// SECUTRETY FOR PIN

int verifyPIN(int idx){

    int temppin;

    for (int try=1 ; try <=3; try++)
    {
        printf(" - Enter your PIN (attempt %d/3 !): ",try);
        scanf("%d",&temppin);
        
        if (temppin == accounts[idx].pin)
        {
           return 1;  // success
        }
        
        printf(" - Wrong PIN!\n");
    }

    printf(" - Access denied after 3 failed attempts.\n");
    return 0; // failure

}

//DEPOSING MONEY 

void deposit(int idx, float amount){

 
    if (amount<=0)
    {
        printf(" - Invalid amount! (MUST BE > 0 DZD)\n");
        return;
        
    }
   
    accounts[idx].balance = accounts[idx].balance + amount;

  //  printf("Successfully deposited %.2f DZD\n",amount);    use the in the menu
 //   printf("Your Balance is %.2f DZD\n",accounts[idx].balance);    
    
 // No success message here → we will print it in main() menu

}

// WITHDRAWINF MONEY

void withdraw(int idx, float amount){


    if (amount <= 0)
    {
        printf(" - Invalid amount! Withdrawal must be > 0 DZD\n");
        return;
    }
    if (amount > accounts[idx].balance)
    {
        printf(" - Insufficient funds! Your balance is: %.2f DZD\n", accounts[idx].balance);
        return;
    }
    
    accounts[idx].balance = accounts[idx].balance - amount;

   // printf("Successfully withdrew %.2f DZD\n",amount);
  //  printf("NEW balance : %.2f DZD\n ",accounts[idx].balance);  use them in the menu

  // No success message here → we will print it in main() menu
}


// TRANSFER FROM ACC TO ANOTHER 

void transfer(int from, int to, float amount) {

    if (amount <= 0)
    {
        printf(" - Invalid amount! Transfer must be > 0 DZD.\n");
        return;
    }
    // check for same account 
    if (from == to)
    {
        printf(" - Cannot transfer to the same account!.\n");
        return;
    }
    
    if (accounts[from].balance < amount)
    {
        printf(" - Insufficient funds in source account! Current balance: %.2f DZD\n",accounts[from].balance);
        return;
    }

    withdraw(from,amount);
    deposit(to,amount);

    printf(" - Transfer of %.2f DZD successful!\n",amount);
    printf(" - From Account ID %d To Account ID %d\n ",accounts[from].id,accounts[to].id);

}

// CLOSING ACC

void closeAccount(int idx){

    if (accounts[idx].active== 0)
    {
        printf("\n - Your account is already closed.\n");
        return;
    }
    if (accounts[idx].balance == 0)
    {
        accounts[idx].active = 0;
        printf(" - Account ID %d has been closed successfully.\n",accounts[idx].id);
    }
    else
    {
        printf(" - Cannot close account! You must withdraw all money first.\n");
        printf(" - Current balance: %.2f DZD\n",accounts[idx].balance);
    }
     
}

// PRINTING ACC DETAILS

void printStatement(int idx){

    /**
   * Prints all details of an account with color-coded balance status.
   * - RED    : balance == 0
   * - YELLOW : balance < 1000 (but > 0)
   * - GREEN  : balance >= 1000
    */

    printf("\n=== Account Statemet ===\n");
    printf(" - Account ID : %d\n", accounts[idx].id);
    printf(" - Name       : %s\n", accounts[idx].name);

    // Color coded balance 

    if (accounts[idx].balance == 0)
    {
      printf(" - Balance    : " RED "%.2f" RESET " DZD [ZERO BALCANCE]\n", accounts[idx].balance);  
    }
    else if (accounts[idx].balance < 1000)
    {
      printf(" - Balance    : " YELLOW "%.2f" RESET " DZD [LOW BALANCE]\n", accounts[idx].balance);  
    }
    else
    {
      printf(" - Balance    : " GREEN "%.2f" RESET " DZD [HEALTHY]\n", accounts[idx].balance);  
    } 
    printf(" - PIN          : %04d\n", accounts[idx].pin);
    printf("==========================\n");

}

// TABLE OF ALL ACTIVE ACC 

void listAllAccounts() {

    if (count == 0)
    {
        printf(" - No accounts found.\n");
        return;
    }

   // printf("\n=== List All Accounts ===\n");

   int indices[MAX];
   int n=0;

   // collect active acc indices
   for (int i = 0; i < count; i++)
   {
      if (accounts[i].active==1)
      {
        indices[n++]=i;
      } 
   }
   if (n==0)
   {
     printf(" - No active accounts.\n");
     return;
   }

   // sort indices by balance (descending)

   for (int i = 0; i < n-1; i++)
   {
       for (int j = i+1 ; j < n; j++)
       {
          if (accounts[indices[i]].balance < accounts[indices[j]].balance)
          {
              int temp = indices[i];
              indices[i]=indices[j];
              indices[j]=temp;
          }
          
       }
       
   }
   
   // print formatted table

   printf("\n========================================================\n");
   printf(" %-5s %-20s %-12s %-6s %-8s\n", "ID" , "Name" ,"Balance" , "PIN" , "Status"); //  -5 ,-20 -12 .. to have same space like  a table 
   printf("-------------------------------------------------------\n");

   for (int i = 0; i < n; i++)
   {
     int idx=indices[i];
     
    // Add color to balance 
    const char *color;

    if (accounts[idx].balance == 0)
    {
        color = RED;

    }else if (accounts[idx].balance < 1000)
    {
        color = YELLOW;
    }else
    {
        color = GREEN;
    }
    
    



     printf(" %-5d %-20s %s%-12.2f%s %s   %-8s\n",
      accounts[idx].id,
      accounts[idx].name,
     color, accounts[idx].balance, RESET,
       "****",    /* accounts[idx].pin, hidding PIN is better*/
      "Active");
   }
   printf("\n========================================================\n");
   
   
  
}

// SUMMERY OF BANK 

void bankSummary() {
    /**
 * Displays a complete bank summary:
 * - Total active accounts
 * - Total deposits held
 * - Average balance
 * - Highest and lowest balance
 */
  
  if (count == 0)
  {
    printf("\n=== Bank Summary ===\n");
    printf(" - No accounts in the bank yet.\n");
    return;
  }

  int activeCount = 0;
  float total = 0.0;
  float highest = 0.0;  
  float lowest = 0.0;
  bool first = true; // t fill highest and lowest with first acc

  for (int i = 0; i < count; i++)
  {
    if (accounts[i].active == 1)
    {
         activeCount++;
         total += accounts[i].balance;

         // Update highest and lowest

         if (first)
         {
            highest = accounts[i].balance;
            lowest=accounts[i].balance;
            first=false;
         }else
         {
           if ( accounts[i].balance > highest )
           {
            highest = accounts[i].balance;
           }
           if ( accounts[i].balance < lowest )
           {
             lowest = accounts[i].balance;
            } 
         }
    }   
  }

  if (activeCount == 0)
  {
    printf("\n=== Bank Summary ===\n");
    printf(" - No active accounts.\n");
    return;
  }

  float average = total / activeCount;

  // display 

  printf("\n=== NATIONAL BANK OF ALGERIA - SUMMARY ===\n");
  printf(" - Total Active Accounts   : %d\n",activeCount);
  printf(" - Total Deposits Held     : %.2f DZD\n",total);
  printf(" - Average Balance         : %.2f DZD\n",average);
  printf(" - Highest Balance         : %.2f DZD\n",highest);
  printf(" - Lowest Balance          : %.2f DZD\n",lowest);
  printf("============================================\n");
  
  


    
    
}