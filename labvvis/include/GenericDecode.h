
/**
  *		
  *		GenericDecode.cis  Copyright(c) 2004 Kagi
  *		All Rights Reserved.
  *
  *		Permission is granted to use this code module for development 
  *		and deployment of a Kagi ACG free of charge as long as Kagi 
  *		sells your products for those application(s) .  
  *		No warranty is made as to the suitability
  *		of GenericDecode.h to your application.
  *
  *		BECAUSE THE MODULE IS LICENSED FREE OF CHARGE, THERE IS NO 
  *		WARRANTY FOR THE MODULE, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
  *		EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR
  *		OTHER PARTIES PROVIDE THE MODULE "AS IS" WITHOUT WARRANTY OF ANY
  *		KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
  *		IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  *		PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
  *		MODULE IS WITH YOU.  SHOULD THE MODULE PROVE DEFECTIVE, YOU ASSUME
  *		THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
  *
  *		If you make any corrections to the basic module or have suggestions
  *			please send them to acg@kagi.com so others may benefit
  *		VERSION HISTORY 
  *     	Alpha   3/1/2004
  *     	1.0     Implementaion of the corrections sugested by Ty.
  *     	
  **/
  
  /**
   * This program implements the algorithm for decoding the registration code
   * generated by the 'Generic Registration Code Algorithm'
   * It denotes how to reverse the calculations you performed on the seed to
   * get the registration code. It performs the following operations to obtain the seed 
   * for registration code.
   * 1. Unformat the registration code.
   * 2. Validate the checkdigit received with the registration code.
   * 3. Convert the registration code to base ten number
   * 4. Convert the base ten number into a set of '2' or '3' digit numbers
   * 5. Conver the number to charcters
   * 6. Unscramble the characters
   * 7. Separate it into the seed ,the constant value , the sequence number and
   *  date from the registration code
   * You require two pieces of data the registration code and the configuration
   * that was used to create the registration code. 
   * To compile 
   */
#ifndef GenericDecode_H
	#define GenericDecode_H

//#define DEBUG


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * This method decodes the registration code into seed, constant
 * sequence number and  the registration date. It returns 1 if 
 * the registration code was successfully decoded.
 */
int decodeGenericRegCode(char* , char*); 
/**
 * This method decodes the registration code into seed, constant
 * sequence number and  the registration date. It returns 1 if 
 * the registration code was successfully decoded.
 */
int decodeGenericRegCode2(char* );
/**
 * This method parses the configuration data and uses
 * it to initialize the variables.The configuration data consist of name 
 * value pairs (eg SUPPLIERID:softedge) separated by the character %.
 * The configuration data consists all the necessary information to
 * decode the registration code.
 */
void initializeConfigData(char* config); 
/** This tores the parsed configuration data. Depending on the value
 * of type , various variables are intialized.
 */
void storeData(int length, char *data,int type);

/**
 * This method unformats the registration code. It removes any sttaic text,
 *  the checkdigit and separator characters '-' from the registraion code
 */
void unformat(void); 
/**
 * This method calculates the checkdigit for the registration code and
 * compares it against the check digit obtained  with the registratin code
 * If the calculated check digit matches the one received then it returns 1.
 * To calculate the checkdigit start at the rightmost character. Multiply the
 * value of that character (so in base 16 char 'A' has value 10) by weight "2".
 * Repeat this for all characters alternating the weight between "1" and "2". 
 * Total up the values so calculated for all characters. Do "sum % base". 
 * Lookup what character the mod value represents in your character set.
 * This is your check digit
 */
int verifyCheckDigit(void); 

/**
 * Returns the digit value for the base character
 * (so in base 16 char 'A' has value 10).This method would return
 * 10 if the charcter is 'A' and base is 16.
 */
int getDigitValue(char);
/**
 * Returns the charcter for the digit character
 * (so in base 16 char 'A' has value 10).This method would return
 * 'A' for the digit value 10 and base is 16.
 */
char getDigit(int); 
/**
 * This method converts the registration code from base xx to base ten
 * This method uses the operations of the BigInt to convert the long
 * registration code to base ten number.
 */
void convertToBaseTen(void); //converts the number to base ten.
/**
 * This method groups the base ten number digits into groups of
 * '2' or '3' depending on how the character to number conversion
 * was represented.
 */
void undoarithmetic(char*); 

/**
 * This method parses the the string containing math operations.
 */
void parseMathOperations(void); 
/** 
  * This method performs the arithmetic operation on the two
  * integers  and returns the result. The result represents a ASCII 
  * character.
  */
int domath(int,int); 

/**
 *This method parses the string containing scramble sequence
 */
void parseScrambleSequence(void); 

/**
 * This method unscrambles the seed into user seed, 
 * constant value, date and sequence.
 */
void unScrambleSeed(void); 

/**
 * This method deallocates the memory allocation
 */
void freememory(void);

/**
 *This method returns the user seed part of the seed
 */
char * getUserSeed(void);
/**
 * This method returns the constant part of the seed
 */
char * getConstant(void); 
/**
 * This method returns the sequence number
 */
char * getSequenceNumber(void);

/**
 * This method returns the date the registration code was generated.
 */
char * getDate(void); 

/**
 *This method does  memory allocation
 */
void * allocateMemory( int size , int dataType);

/**
 * This method stores the debug message
 */
void setDebugMessage( int size, char * );

/**
 *This method returns all the debug output
 *for this program
 */
char * getDebugMessage();

/**
 * This method reset variables to 0  and pionters tonull
 * before a registration code can be decoded
 */
void resetVariables(void);
/**
 *This method sets the user name
 */
 void setUserName( char* name);
 /**
  *This method sets the user email 
  */
 void setUserEmail( char* email);

/**
 * This method creates the user seed of the length 'userseedlength'
 * and type 'seedcombo' as per the configuration for a given 
 * user name/hotsync id and user email.
 */
void createUserSeed(void);


/**
 * Returns the user seed created for a given user name ane email.
 */
 char * getCreatedUserSeed(void);
#endif 
