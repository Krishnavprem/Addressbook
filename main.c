/*
NAME : KRISHNA V PREM
BATCH & REG.NO: 25021A_013
PROJECT TITLE : ADDRESS BOOK
DESCRIPTION : The Address Book project is a console-based C application that allows users to manage personal contacts efficiently.
             The program supports creation, listing, searching, editing, and deletion of contacts. 
             It persists contact information using a CSV file so that data is retained between program runs.
DATE : 23-09-2025 
*/
#include <stdio.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

int main() {
    int choice;
    char c;
    int sortChoice = 0;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book
    loadContactsFromFile(&addressBook);

    do {
        printf("\nAddress Book Menu:\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save contacts\n");		
        printf("7. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d%c", &choice, &c) != 2 || c != '\n') {
            printf("Please enter a valid choice\n");
            while (getchar() != '\n'); 
            continue;
        }
        
        switch (choice) {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:          
                listContacts(&addressBook, sortChoice);
                break;
            case 6:
                printf("Saving and Exiting...\n");
                saveContactsToFile(&addressBook);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
    
       return 0;
}
