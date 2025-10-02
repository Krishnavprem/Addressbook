#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
    FILE *fp = fopen("contact.csv", "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }

    // Save total count
    if (fprintf(fp, "%d\n", addressBook->contactCount) < 0) {
        printf("Error writing to file\n");
        fclose(fp);
        return;
    }

    // Save each contact
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (fprintf(fp, "%s,%s,%s\n",
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email) < 0) {
            printf("Error writing contact %d\n", i + 1);
            break;
        }
    }
    fclose(fp);
}

void loadContactsFromFile(AddressBook *addressBook) {
    FILE *fp = fopen("contact.csv", "r");
    if (fp == NULL) {
        printf("No saved contacts found. Using dummy contacts only.\n");
        return;
    }

    int fileCount;
    if (fscanf(fp, "%d\n", &fileCount) != 1) {
    printf("Contacts file is empty or corrupted. Starting fresh.\n");
    //addressBook->contactCount = 0;
    fclose(fp);
    return;
}
    addressBook->contactCount = 0;

    // Append file contacts after existing dummy contacts
    for (int i = 0; i < fileCount; i++) { 
        if (fscanf(fp, "%[^,],%[^,],%[^\n]\n",
                   addressBook->contacts[addressBook->contactCount].name,
                   addressBook->contacts[addressBook->contactCount].phone,
                   addressBook->contacts[addressBook->contactCount].email) != 3) {
            printf("Error reading contact %d from file\n", i + 1);
            break;
        }
        addressBook->contactCount++;
    }
    fclose(fp);
}