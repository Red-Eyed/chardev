#ifndef CHARDEV_LOG_H
#define CHARDEV_LOG_H

#include <linux/kernel.h>

#define LOG_LEVEL_NONE  0
#define LOG_LEVEL_CRIT  1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_DEBUG 3

#define LOG_LEVEL LOG_LEVEL_DEBUG

#define GET_FILE_LINE_FUNC_ARG __FILE__, __LINE__, __FUNCTION__

#define PRINT_DUMP_STACK\
    do{\
    printk("\n\nDump stack begin:\n\n");\
    dump_stack();\
    printk("\n\nDump stack end:\n\n");\
    }while(0);

#if LOG_LEVEL >= LOG_LEVEL_CRIT
#define CHARDEV_LOG_CRIT(format, args...)\
    printk(KERN_CRIT "File:(%s), Line:(%u), Function:(%s)\n\tMessage: "format"\n", GET_FILE_LINE_FUNC_ARG, args);
#else
#define CHARDEV_LOG_CRIT(format, args...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define CHARDEV_LOG_INFO(format, args...)\
    printk(KERN_INFO "File:(%s), Line:(%u), Function:(%s)\n\tMessage: "format"\n", GET_FILE_LINE_FUNC_ARG, args);
#else
#define CHARDEV_LOG_INFO(format, args...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define CHARDEV_LOG_DEBUG(format, args...)\
    printk(KERN_DEBUG "File:(%s), Line:(%u), Function:(%s)\n\tMessage: "format"\n", GET_FILE_LINE_FUNC_ARG, args);
#else
#define CHARDEV_LOG_DEBUG(format, args...)
#endif

#endif // CHARDEV_LOG_H
