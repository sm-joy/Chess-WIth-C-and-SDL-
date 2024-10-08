/*
  ZINI: A Minimalistic INI Parser for C
  Author: Zihad Joy

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the author be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not
     be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/



#ifndef ZINI_PARSER_H
#define ZINI_PARSER_H

#include <stdbool.h>
#include <stdio.h>


#ifndef MAX_SECTION_LENGTH
    #define MAX_SECTION_LENGTH 128
#endif // MAX_SECTION_LENGTH

#ifndef MAX_KEY_LENGTH
    #define MAX_KEY_LENGTH 128
#endif // MAX_KEY_LENGTH

#ifndef MAX_VALUE_LENGTH
    #define MAX_VALUE_LENGTH 128
#endif // MAX_VALUE_LENGTH

#define MAX_LINE_LENGTH (MAX_KEY_LENGTH + MAX_VALUE_LENGTH + 2)

#ifndef MAX_DOUBLE_PRECISION
    #define MAX_DOUBLE_PRECISION 2
#elif MAX_DOUBLE_PRECISION > 15
    #undef MAX_DOUBLE_PRECISION
    #define MAX_DOUBLE_PRECISION 15
#endif // MAX_DOUBLE_PRECISION

#ifndef MAX_FLOAT_PRECISION
    #define MAX_FLOAT_PRECISION 2
#elif MAX_FLOAT_PRECISION > 7
    #undef MAX_FLOAT_PRECISION
    #define MAX_FLOAT_PRECISION 7
#endif // MAX_FLOAT_PRECISION



typedef enum {
    ZINI_SUCCESS,
    ZINI_KEY_NOT_FOUND,
    ZINI_SECTION_NOT_FOUND,
    ZINI_MEMORY_ERROR,
    ZINI_INVALID_INPUT,
    ZINI_FILE_ERROR
} ZINI_Status; // i'll add it later

typedef enum {
    ZINI_STR,
    ZINI_INT,
    ZINI_LINT,
    ZINI_LLINT,
    ZINI_UINT,
    ZINI_FLOAT,
    ZINI_DOUBLE,
    ZINI_BOOL
} ZINI_DType;


/**
 * Represents a key-value pair in an INI file.
 */
typedef struct INIKeyValuePair {
    char key[MAX_KEY_LENGTH];    /**< Key of the pair */
    char value[MAX_VALUE_LENGTH]; /**< Value of the pair */
} Pair;

/**
 * Represents a section in an INI file.
 */
typedef struct INISection {
    char section[MAX_SECTION_LENGTH];   /**< Name of the section */
    Pair* pairs;                        /**< Array of key-value pairs in the section */
    size_t pairCount;                      /**< Number of key-value pairs in the section */
} Section;

/**
 * Represents an INI file, including all sections and their key-value pairs.
 */
typedef struct {
    Section* sections;    /**< Array of sections in the INI file */
    size_t sectionCount;     /**< Number of sections in the INI file */
} INIFILE;



/**
 * Initializes an INI file structure.
 * @param iniFile Pointer to the INIFILE structure to be initialized.
 */
void ZINI_Init(INIFILE *iniFile);

/**
 * Opens and reads an INI file, populating the INIFILE structure.
 * @param iniFile Pointer to the INIFILE structure to be populated.
 * @param filename Path to the INI file to be opened.
 * @return True if the file was successfully opened and read, false otherwise.
 */
bool ZINI_Open(INIFILE* iniFile, const char* filename);

/**
 * Saves the current state of the INIFILE structure to an INI file.
 * @param iniFile Pointer to the INIFILE structure to be saved.
 * @param filename Path to the INI file where the data will be saved.
 * @return True if the file was successfully saved, false otherwise.
 */
bool ZINI_Save(INIFILE* iniFile, const char* filename);

/**
 * Adds a new section to the INIFILE structure.
 * @param iniFile Pointer to the INIFILE structure to be modified.
 * @param section Name of the section to be added.
 * @return Pointer to the newly added Section structure, or NULL if an error occurred.
 */
Section* ZINI_AddSection(INIFILE* iniFile, const char* section);

/**
 * Adds a new key-value pair to a section.
 * @param section Pointer to the Section structure to be modified.
 * @param key Key of the pair to be added.
 * @param value Value of the pair to be added.
 * @return Pointer to the newly added Pair structure, or NULL if an error occurred.
 */
Pair* ZINI_AddPair(Section* section, const char* key, const char* value);

Pair* ZINI_AddPairVT(Section* section, const char* key, void* value, ZINI_DType type);

Pair* ZINI_AddPairEx(INIFILE* iniFile, const char* section, const char* key, const char* value);

Pair* ZINI_AddPairVTEx(INIFILE* iniFile, const char* section, const char* key, void* value, ZINI_DType type);

/**
 * Finds a section in the INIFILE structure.
 * @param iniFile Pointer to the INIFILE structure to be searched.
 * @param section Name of the section to find.
 * @return Pointer to the Section structure if found, NULL otherwise.
 */
Section* ZINI_FindSection(INIFILE* iniFile, const char* section);

/**
 * Finds the value associated with a key in a section.
 * @param section Pointer to the Section structure to be searched.
 * @param key Key whose value is to be found.
 * @return Value associated with the key if found, NULL otherwise.
 */
const char* ZINI_GetValue(Section* section, const char* key);

/**
 * Finds the value associated with a key in any section of the INIFILE structure.
 * @param iniFile Pointer to the INIFILE structure to be searched.
 * @param key Key whose value is to be found.
 * @return Value associated with the key if found, NULL otherwise.
 */
const char* ZINI_GetValueEx(INIFILE* iniFile, const char* section, const char* key); // will add a const char* section (for narrowing down the search)

/**
 * Cleans up and frees memory used by the INIFILE structure.
 * @param iniFile Pointer to the INIFILE structure to be cleaned.
 */
void ZINI_Clean(INIFILE* iniFile);

/**
 * Removes a key-value pair from a specified section.
 *
 * @param section Pointer to the `Section` structure from which the key-value pair should be removed.
 * @param key The key of the key-value pair to be removed.
 *
 * This function searches for the key in the section's key-value pairs and removes it if found.
 * Memory management for the removed key-value pair is handled within the function.
 */
void ZINI_RemovePair(Section* section, const char* key);


/**
 * Removes a key-value pair from a specified section in the INI file.
 *
 * @param iniFile Pointer to the `INIFILE` structure containing the section.
 * @param section The name of the section from which the key-value pair should be removed.
 * @param key The key of the key-value pair to be removed.
 *
 * This function locates the specified section within the INI file and then removes the key-value pair
 * from that section. Both the section and the key are required to be valid.
 */
void ZINI_RemovePairEx(INIFILE* iniFile, const char* section, const char* key);


/**
 * Updates the value associated with a specified key in a section.
 *
 * @param section Pointer to the `Section` structure where the key's value will be updated.
 * @param key The key whose value should be updated.
 * @param newValue The new value to be associated with the key.
 *
 * This function searches for the key within the section's key-value pairs and updates its value if the
 * key is found. If the key does not exist, no changes are made.
 */
void ZINI_SetValue(Section* section, const char* key, const char* newValue);


/**
 * Updates the value of a specified key in a section of the INI file.
 *
 * @param iniFile Pointer to the `INIFILE` structure containing the section.
 * @param section The name of the section where the key's value will be updated.
 * @param key The key whose value should be updated.
 * @param newValue The new value to be associated with the key.
 *
 * This function locates the specified section within the INI file and updates the key's value within
 * that section. Both the section and the key are required to be valid. If the section or key does not exist,
 * no changes are made.
 */
void ZINI_SetValueEx(INIFILE* iniFile, const char* section, const char* key, const char* newValue);


/**
 * Removes a section from the INI file.
 *
 * @param iniFile Pointer to the `INIFILE` structure from which the section should be removed.
 * @param section The name of the section to be removed.
 *
 * This function locates the specified section within the INI file and removes it along with its key-value
 * pairs. Memory allocated for the section and its pairs is freed.
 */
void ZINI_RemoveSection(INIFILE* iniFile, const char* section);


/**
 * Checks if a section exists in the INI file.
 *
 * @param iniFile Pointer to the `INIFILE` structure to be searched.
 * @param section The name of the section to be checked.
 * @return `true` if the section exists, `false` otherwise.
 *
 * This function searches for the specified section in the INI file and returns a boolean indicating its
 * presence.
 */
bool ZINI_SectionExists(INIFILE* iniFile, const char* section);


/**
 * Checks if a key exists in a section.
 *
 * @param section Pointer to the `Section` structure to be searched.
 * @param key The key to be checked.
 * @return `true` if the key exists, `false` otherwise.
 *
 * This function searches for the specified key in the section's key-value pairs and returns a boolean
 * indicating its presence.
 */
bool ZINI_KeyExists(Section* section, const char* key);


/**
 * Prints the INI file content to the specified stream.
 *
 * @param iniFile Pointer to the `INIFILE` structure to be printed.
 * @param stream Pointer to the `FILE` stream where the INI content will be written.
 *
 * This function writes the content of the INI file to the specified stream, formatted according to INI
 * file standards. The function handles sections and key-value pairs, ensuring they are printed correctly.
 */
void ZINI_Print(INIFILE* iniFile, FILE* stream);


#endif // ZINI_PARSER_H