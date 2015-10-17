/******************************************************************************
 Copyright © 1995-2003,2004,2005-2014 Freescale Semiconductor Inc.
 All Rights Reserved
 
 This is proprietary source code of Freescale Semiconductor Inc., and its use
 is subject to the CodeWarrior EULA.  The copyright notice above does not 
 evidence any actual or intended publication of such source code.
  
*******************************************************************************/

/******************************************************************************
 $Date: 2012/11/22 16:28:48 $
 $Id: os_list_.h,v 1.11 2012/11/22 16:28:48 sw Exp $
 $Source: /cvsdata/SmartDSP/source/common/include/os_list_.h,v $
 $Revision: 1.11 $
******************************************************************************/

/******************************************************************************
 
 @File          os_list_.h

 @Description   OS list header file.
                Simple doubly linked list implementation.

 @Cautions      None.
 
*//***************************************************************************/

#ifndef __OS_LIST_H_
#define __OS_LIST_H_


typedef struct list_object_t {
    struct list_object_t *next, *prev;
} list_object_t;

#define LIST_OBJECT_INIT(name) { &(name), &(name) }

#define LIST_OBJECT(name) \
    struct list_object_t name = LIST_OBJECT_INIT(name)

#define INIT_LIST_OBJECT(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
}while (0)

#define LIST_FIRST(name) ((name)->next)

/**************************************************************************//**
 @Function      low_level_list_add

 @Description   Insert a new_entry entry between two known consecutive entries.
                This is only for internal list manipulation where we know
                the prev/next entries already.
 
 @Param         new_entry  - New list object.
 @Param         next - Before new_entry.
 @Param         prev - After new_entry.
 
*//***************************************************************************/

static inline void low_level_list_add(struct list_object_t *new_entry,
                  struct list_object_t *prev,
                  struct list_object_t *next)
{
    next->prev = new_entry;
    new_entry->next = next;
    new_entry->prev = prev;
    prev->next = new_entry;
}

/**************************************************************************//**
 @Function      list_add

 @Description   add a new_entry entry in list_object of list.
 
 @Param         new_entry  - new_entry entry to be added.
 @Param         list_object - list list_object to add it after.
 
*//***************************************************************************/
static inline void list_add(struct list_object_t *new_entry, struct list_object_t *list_object)
{
    low_level_list_add(new_entry, list_object, list_object->next);
}

/**************************************************************************//**
 @Function      list_tail_add

 @Description   add a new_entry entry before list_object.
 
 @Param         new_entry  - new_entry entry to be added.
 @Param         list_object - list list_object to add it after.
 
*//***************************************************************************/
static inline void list_tail_add(struct list_object_t *new_entry, struct list_object_t *list_object)
{
    low_level_list_add(new_entry, list_object->prev, list_object);
}



/**************************************************************************//**
 @Function      low_level_list_delete

 @Description   Delete a list entry by making the prev/next entries point to
                each other.
 
 @Param         prev - Before deleted item.
 @Param         next - After deleted item.
 
*//***************************************************************************/
static inline void low_level_list_delete(struct list_object_t * prev, struct list_object_t * next)
{
    next->prev = prev;
    prev->next = next;
}

/**************************************************************************//**
 @Function      list_delete

 @Description   deletes entry from list.
 
 @Param         prev - Before deleted item.
 @Param         next - After deleted item.
 
*//***************************************************************************/
static inline void list_delete(struct list_object_t *entry)
{
    low_level_list_delete(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}


/**************************************************************************//**
 @Function      list_move

 @Description   delete from one list and add as another's list_object.
 
 @Param         list - the entry to move.
 @Param         list_object - the list_object that will precede our entry.
 
*//***************************************************************************/
static inline void list_move(struct list_object_t *list, struct list_object_t *list_object)
{
        low_level_list_delete(list->prev, list->next);
        list_add(list, list_object);
}

/**************************************************************************//**
 @Function      list_move_tail

 @Description   delete from one list and add as another's tail.
 
 @Param         list - the entry to move.
 @Param         list_object - the list_object that will precede our entry.
 
*//***************************************************************************/
static inline void list_move_tail(struct list_object_t *list,
                  struct list_object_t *list_object)
{
        low_level_list_delete(list->prev, list->next);
        list_tail_add(list, list_object);
}

/**************************************************************************//**
 @Function      list_empty

 @Description   tests whether a list is empty.
 
 @Param         list_object - the list to test.
 
 @Retval        TRUE if empty.

*//***************************************************************************/
static inline bool list_empty(const struct list_object_t *list_object)
{
    return list_object->next == list_object;
}


#define OFFSET_OF_FIELD( type, field )  ((size_t)&(((type*)0)->field))

#define COMPLEX_OF(ptr, type, field) ({         \
        const list_object_t *__mptr = (ptr);    \
        (type *)( (char *)__mptr - OFFSET_OF_FIELD(type,field) );})


#define LIST_COMPONENT(ptr, type, field) \
        COMPLEX_OF(ptr, type, field)


#define LIST_FOR_EACH(pos, list_object, type, field)            \
        for (pos = LIST_COMPONENT((list_object)->next, type, field);      \
             &pos->field != (list_object);                            \
             pos = LIST_COMPONENT(pos->field.next, type, field))

#define LIST_FOR_EACH_REVERSE(pos, list_object, type, field)            \
        for (pos = LIST_COMPONENT((list_object)->prev, type, field);      \
             &pos->field != (list_object);                            \
             pos = LIST_COMPONENT(pos->field.prev, type, field))


#endif // __OS_LIST_H_
