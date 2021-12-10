/************************************************************************************/
/* Author 	: Abdallah Issa															*/
/* Date   	: 9 Dec 2021															*/
/* Version	: V1.0																	*/
/************************************************************************************/


#include "FLASH_PAGE.h"
#include "string.h"
#include "stdio.h"


// STM32F103 have 128 PAGES (Page 0 to Page 127) of 1 KB each. This makes up 128 KB Flash Memory
// FLASH_PAGE_SIZE should be able to get the size of the Page according to the controller
/*
static uint32_t GetPage(uint32_t Address)
{
  for (int indx=0; indx<128; indx++)
  {
	  if((Address < (0x08000000 + (FLASH_PAGE_SIZE *(indx+1))) ) && (Address >= (0x08000000 + FLASH_PAGE_SIZE*indx)))
	  {
		  return (0x08000000 + FLASH_PAGE_SIZE*indx);
	  }
  }

  return 0;
}
*/

uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *Data_32)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;
	int sofar=0;

	int numberofwords = (strlen(Data_32)/4) + ((strlen(Data_32)%4)!=0);
	// Unlock the Flash to enable the flash control register access
	HAL_FLASH_Unlock();

	/*


	// Erase the user Flash area

	uint32_t StartPage = GetPage(StartPageAddress);
	uint32_t EndPageAdress = StartPageAddress + numberofwords*4;
	uint32_t EndPage = GetPage(EndPageAdress);

	*/

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = StartPageAddress;
    EraseInitStruct.NbPages     = (numberofwords/FLASH_PAGE_SIZE) +1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
    	/*Error occurred while page erase.*/
    	return HAL_FLASH_GetError ();
    }

    // Program the user Flash area word by word
    while (sofar<numberofwords)
    {
    	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, Data_32[sofar]) == HAL_OK)
    	{
    		StartPageAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
    		sofar++;
    	}
    	else
    	{
    		//Error occurred while writing data in Flash memory
    		return HAL_FLASH_GetError ();
    	}
    }

    /* Lock the Flash to disable the flash control register access (recommended
	  to protect the FLASH memory against possible unwanted operation)*/
    HAL_FLASH_Lock();

    return 0;
}


void Flash_Read_Data (uint32_t StartPageAddress, __IO uint32_t* Data_32, uint32_t length)
{
	int count = 0;
	while (1)
	{
		*Data_32 = *(__IO uint32_t *)StartPageAddress;
		if(count == length)
		{
			*Data_32 = '\0';
			break;
		}
		StartPageAddress += 4;
		Data_32++;
		count ++;
	}
}





