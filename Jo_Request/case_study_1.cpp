#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to reverse a string
void reverseString(char* str) {
    // count the lenght of the string
    int len = strlen(str);
    // loop for reversing by swapping first with last character
    // until the string is reversed
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

// Function to inverse capitalize a string
void inverseCapitalize(char* str) {
    // using loop to check if the character is lower or upper and inverse its capitalization 
    for (int i = 0; str[i] != '\0'; i++) {
        if (islower(str[i])) {
            str[i] = toupper(str[i]);
        } else if (isupper(str[i])) {
            str[i] = tolower(str[i]);
        }
    }
}

int main() {
    char input[101]; // Maximum length of the string is 100 + 1 for null terminator

    printf("Enter a string: ");
    scanf("%100s", input); // Use %100s to avoid overflow

    // Reverse the string
    reverseString(input);

    // Inverse capitalize the characters
    inverseCapitalize(input);

    // Output the final result
    printf("Converted string: %s\n", input);

    return 0;
}
