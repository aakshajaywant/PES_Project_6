/******************************************************************************
 *  					PES PROJECT 5
 *   AAKSHA JAYWANT (AAJA1276) & RUCHA BORWANKAR (RUBO1268)
 * 				Cross Platform IDE: MCUXpresso IDE v11
 * 					Cross-Compiler: ARM GCC
 * 						cir_buffer.c
 ********************************************************************************/
/**********************************REFERENCE*********************************
 https://stackoverflow.com/questions/827691/how-do-you-implement-a-circular-buffer-in-c
 http://www.equestionanswers.com/c/c-circular-buffer.php
*****************************************************************************/
#include "cir_buffer.h"

ring_status r_status;
uint8_t count_add_item = 1;
/***************************BUFFER INITIALIZE FUNCTION**********************
 In this function we take pointer and capacity of the buffer in the function and
  check for errors if pointer is null or capacity is 0 and if not then
  we allocate memory for buffer and initialize all the variables of the structure.
*****************************************************************************/

ring_status buff_initialize(ring_buffer *p, uint8_t capacity)
{
	//log_messages(mode,buffinitialize);

	if(p == NULL || capacity <= 0)
	{
		return buffer_init_not_done;

	}
	else
	{
		p->buffer = (uint16_t*) malloc(sizeof(uint8_t)*capacity);
		p->head = 0;
		p->tail = 0;
		p->capacity = capacity;
		p->count = 0;
		p->head_count=0;
		p->tail_count=0;
		//PRINTF("\n\r Head position in initial stage is at %d",p->head);
		return buffer_init_done;
		if(p == NULL)
		{
			return buffer_init_not_done;
		}

	}
}
/***************************BUFFER CHECK FULL FUNCTION**********************
 In this function we take pointer in the function and
  check for errors such as if pointer is null then buffer pointer is valid,
  if count=capacity,then returns the status of buffer_full and
  if count=0 then we say buffer is empty else it returns status
  as buffer not full
*****************************************************************************/
ring_status buff_check_full(ring_buffer *p)
{
//	log_messages(mode,buffcheck_full);
	if(p == NULL)
	{
		buff_ptr_valid(p);
	}
	else if(p->count == p->capacity)
	{
		return buffer_full;
	}
	else if (p->count == 0)
	{
		return buffer_empty;
	}

		return buffer_not_full;

}
/******************BUFFER CHECK EMPTY FUNCTION**********************
  In this function we take pointer in the function and
  check for errors such as if pointer is null then buffer pointer is valid,
  if count=capacity,then returns the status of buffer_full and
  if count=0 then we say buffer is empty else it returns status
  as buffer not full
*****************************************************************************/
ring_status buff_check_empty(ring_buffer *p)
{
//	log_messages(mode,buffcheck_empty);
	if(p == NULL)
		{
		buff_ptr_valid(p);
		}
	else if(p->count == 0)
		{
			return buffer_empty;
		}
	else if(p->count == p->capacity)
		{
			return buffer_full;
		}

			return buffer_not_empty;
}

/*****************BUFFER ADD ITEM FUNCTION**********************************************************
  In this function we take pointer and the character to be stored
  in the function and check for conditions before storing the character such as:
  1-if head =capacity-1 and if buffer is empty or buffer is not full then it wraps around the buffer
  2-if buffer is full,then returns the status of buffer_full and item is not added to buffer
  3-if buffer is empty or not full then it increments the head for next round of addition
***************************************************************************************************/
ring_status buff_add_item(ring_buffer *p,uint16_t item)
{
//	log_messages(mode,buffadd_item);
			if((p->head == (p->capacity - 1)) && ((buff_check_full(p) == buffer_empty) || (buff_check_full(p) == buffer_not_full)))
			{

				*(p->buffer + p->head) = item;
//				PRINTF("\n\r Wrap around p->buffer[p->head]: %c",*(p->buffer + p->head));
//				PRINTF("\n\r Wrap around address of p->head  %p",(p->buffer + p->head));
			//	if(mode==test || mode==debug){
				PRINTF("\n Wrap around occurs");
				//}
				p->head = 0;
				p->tail = 0;
			//	p->count++;
				p->count = 0;
				p->head_count++;
				return wrap_around;
			}
//			else if(buff_check_full(p) == buffer_full)
//			{
//				p->head = 0;
//
//		//		init_LED();
//		//		error_led();
//			//	if(mode==test || mode==debug)
//				//{
//				PRINTF("\n Item not added to buffer full");
//				//}
//
//				return item_not_added_in_buff;
//			}

			else if((buff_check_full(p) == buffer_empty) || (buff_check_full(p) == buffer_not_full))
			{
				*(p->buffer + p->head) = item;
				//PRINTF("**************************************************************");
				PRINTF("\n\r p->buffer[p->head] character: %c",*(p->buffer + p->head));
				PRINTF("\n\r address of p->head  %p",(p->buffer + p->head));
				p->head++;
				p->count++;
				p->head_count++;
				//PRINTF("\n\r head position incremented is now %d",p->head);
				//PRINTF("\n\r head count is %d",p->count);
				//PRINTF("\n\r4TH");
				PRINTF("COUNT OF ADD ITEM IS %d",count_add_item);
				count_add_item++;
				return item_added_in_buff;
			}

return 0;
}
/**************************************BUFFER REMOVE ITEM FUNCTION*************************************
  In this function we take pointer in the function and check for conditions before storing the character such as:
  1-if head =capacity-1 and if buffer is empty or buffer is not full then it wraps around the buffer
  2-if buffer is full,then returns the status of buffer_full and item is not added to buffer
  3-if buffer is empty or not full then it increments the tail count for next round of addition
******************************************************************************************************/

ring_status buff_remove_item(ring_buffer *p)
{
//	log_messages(mode,buffremove_item);
if(p->head_count > p->tail_count)
{
	uint8_t read;

		if((p->tail == (p->capacity - 1)) && ((buff_check_empty(p) == buffer_full) || (buff_check_empty(p) == buffer_not_empty)))
		  	{
					read = *(p->buffer + p->tail);
					//PRINTF("\n\r Wrap around p->buffer[p->tail]: %c",read);
					//PRINTF("\n\r Wrap around address of p->tail  %p",(p->buffer + p->tail));
			//		if(mode==test || mode==debug)
				//	{
						PRINTF("\n Wrap around occurs");
					//}
					p->tail = 0;
					p->tail_count++;
					p->count--;
					return wrap_around;
		  	}

		else if(buff_check_empty(p) == buffer_empty)
		{
			PRINTF("ITEM not removed because full");
			return oldest_item_not_removed;
		}
		else if((buff_check_empty(p) == buffer_full) || (buff_check_empty(p) == buffer_not_empty))
			{
				read = *(p->buffer + p->tail);
//				PRINTF("\n\r p->buffer[p->tail] character: %c",read);
//				PRINTF("\n\r address of p->tail  %p",(p->buffer + p->tail));
		//		if(mode==test || mode==debug)
		//		{
					PRINTF("\n Buffer removed \n");
		//		}
				(p->tail)++;
				p->count--;
				p->tail_count++;
			//	PRINTF("\n\rtail position incremented is now %d",p->tail);
			//	PRINTF("\n\rcount  now is %d",p->count);
				return oldest_item_removed;
			}
}
else
{
	PRINTF("\n********* wait************ \n");
	//PRINTF("waitttttttttttttt");
}
return 0;
}

/**************************************BUFFER DESTROY*************************************
  In this function we take pointer in the function and destroy the created buffer:
  1-Check if the pointer is null and free buffer
******************************************************************************************************/



ring_status buff_destroy(ring_buffer *p)
{
//	log_messages(mode,buffdestroy);
		if(p == NULL)
		{
		buff_ptr_valid(p);
		}
		//else
		//{
		free(p->buffer);
		//free(buffer);
		return buffer_destroyed;
		//}

}

/**************************************BUFFER REMOVE ITEM FUNCTION*************************************
  In this function we take pointer in the function and check if pointer is valid:
  1-if pointer is null, return status is FAIL
  2-else return status is SUCCESS
******************************************************************************************************/



ring_status buff_ptr_valid(ring_buffer *p)
{
//	log_messages(mode,buffptr_valid);
	if(p == NULL)
		{
			return FAIL;
		}
	else
	{
		return SUCCESS;
	}
}


/**************************************BUFFER REMOVE ITEM FUNCTION*************************************
  In this function we take pointer in the function and reallocate more memory on the heap
  1-takes input of the previous malloc pointer
  2-allocates more memory in the same address or at new address on the heap; if memory is reallocated
  then it return status as memory reallocated
  3-if new memory is not reallocated then it returns a status as memory not reallocated
******************************************************************************************************/


ring_status buff_resize(ring_buffer *p)
{
	//log_messages(mode,buffresize);
	if(p->count == p->capacity)
	{
		//putstr("\n \r *****************************************************");
		PRINTF("\n \r Buffer is resized");

		p->buffer_new = (uint16_t*) realloc(p->buffer, sizeof(char)*2*(p->capacity));

		return memory_reallocated;
	}

	else
	{
		return memory_not_reallocated;
	}
}
