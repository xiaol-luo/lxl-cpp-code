//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSGPublic.h
//  Version     :   1.0
//  Creater     :   Freeway Chen
//  Date        :   2004-4-19 20:44:49
//  Comment     :   Kingsoft Game Public header file
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KGPUBLIC_H
#define _KGPUBLIC_H    1

#include <assert.h>

#ifndef ASSERT
#define ASSERT assert
#endif

#ifdef WIN32
#define KG_FUNCTION __FUNCTION__
#else
#define KG_FUNCTION __PRETTY_FUNCTION__
#endif

#ifdef _MSC_VER
    #define TEMP_DISABLE_WARNING(warningCode, expression)   \
        __pragma(warning(push))                             \
        __pragma(warning(disable:warningCode))              \
        expression                                          \
        __pragma(warning(pop))
#else
    #define TEMP_DISABLE_WARNING(warningCode, expression)   \
        expression
#endif 

// _MSC_VER warning C4127: conditional expression is constant
// use this macro instead "WHILE_FALSE_NO_WARNING", you can enjoy the level 4 warning ^_^
#define WHILE_FALSE_NO_WARNING  \
    TEMP_DISABLE_WARNING(4127, while (false))

#include <stdio.h>

#define KGLOG_EXPECT_TRUE(Condition) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
            printf ("KGLOG_EXPECT_TRUE(%s) at line %d in %s\n", \
			#Condition, __LINE__, KG_FUNCTION);					\
			goto exit0;         \
        }                       \
    } WHILE_FALSE_NO_WARNING
	
#define KG_EXPECT_TRUE(Condition) \
    do  \
    {   \
        if (!(Condition))   \
            goto exit0;     \
    } WHILE_FALSE_NO_WARNING

#define KG_PROCESS_SUCCESS(Condition) \
    do  \
    {   \
        if (Condition)      \
            goto exit1;     \
    } WHILE_FALSE_NO_WARNING
	
#define KG_ASSERT_EXIT(Condition) \
    do  \
    {   \
        assert(Condition);  \
        if (!(Condition))   \
            goto exit0;     \
    } WHILE_FALSE_NO_WARNING


#define KG_DELETE_ARRAY(pArray)     \
    do  \
    {   \
        if (pArray)                 \
        {                           \
            delete [](pArray);      \
            (pArray) = NULL;        \
        }                           \
    } WHILE_FALSE_NO_WARNING


#define KG_DELETE(p)    \
    do  \
    {   \
        if (p)              \
        {                   \
            delete (p);     \
            (p) = NULL;     \
        }                   \
    } WHILE_FALSE_NO_WARNING

#endif // _KGPUBLIC_H
