#ifndef LOAD_SHLIB_H
#define LOAD_SHLIB_H


#include <stdint.h>
#include "config_paramdesc.h"

typedef struct {
   char *fname;
   int (*fptr)(void);
}loader_shlibfunc_t;

typedef struct {
   char               *name;
   char               *shlib_version;    // 
   char               *shlib_buildversion;
   char               *thisshlib_path;
   uint32_t           numfunc;
   loader_shlibfunc_t *funcarray;
   uint32_t           len_funcarray;
}loader_shlibdesc_t;

typedef struct {
   char               *mainexec_buildversion;
   char               *shlibpath;
   uint32_t           maxshlibs;
   uint32_t           numshlibs;
   loader_shlibdesc_t *shlibs;
}loader_data_t;

/* function type of functions which may be implemented by a module */
/* 1: init function, called when loading, if found in the shared lib */
typedef int(*initfunc_t)(void *);

/* 2: version checking function, called when loading, if it returns -1, trigger main exec abort  */
typedef int(*checkverfunc_t)(char * mainexec_version, char ** shlib_version);
/* 3: get function array function, called when loading when a module doesn't provide */
/* the function array when calling load_module_shlib (farray param NULL)             */
typedef int(*getfarrayfunc_t)(loader_shlibfunc_t **funcarray);

// #ifdef LOAD_MODULE_SHLIB_MAIN
#define LOADER_CONFIG_PREFIX  "loader"
#define DEFAULT_PATH      ""
#define DEFAULT_MAXSHLIBS 10
loader_data_t loader_data;

/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*                                       LOADER parameters                                                                                                  */
/*   optname               helpstr   paramflags           XXXptr	                           defXXXval	              type       numelt   check func*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define LOADER_PARAMS_DESC { \
{"shlibpath",                NULL,    PARAMFLAG_NOFREE, strptr:(char **)&(loader_data.shlibpath), defstrval:DEFAULT_PATH,      TYPE_STRING, 0,    NULL},\
{"maxshlibs",                NULL,    0,                uptr:&(loader_data.maxshlibs),            defintval:DEFAULT_MAXSHLIBS, TYPE_UINT32, 0,   NULL}\
}

/*-------------------------------------------------------------------------------------------------------------*/
#else  /* LOAD_MODULE_SHLIB_MAIN */

extern int load_module_version_shlib(char *modname, char *version, loader_shlibfunc_t *farray, int numf, void *initfunc_arg);
extern void * get_shlibmodule_fptr(char *modname, char *fname);
extern loader_data_t loader_data;
#endif /* LOAD_MODULE_SHLIB_MAIN */
#define load_module_shlib(M, F, N, I) load_module_version_shlib(M, NULL, F, N, I)
void loader_reset();
// #endif

#ifndef _LOAD_MODULE_SHLIB_H_
#define _LOAD_MODULE_SHLIB_H_
/* checkedparam_t is possibly used in paramdef_t for specific parameter value validation */
#define CONFIG_MAX_NUMCHECKVAL            20
typedef struct paramdef paramdef_t;
typedef union checkedparam {
  struct  {
    int  (*f1)(paramdef_t *param);   /* check an integer against a list of authorized values */
    int okintval[CONFIG_MAX_NUMCHECKVAL];                        /* integer array, store possible values  */
    int num_okintval;                                            /* number of valid values in the checkingval array */
  } s1;
  struct  {
    int  (*f1a)(paramdef_t *param);   /* check an integer against a list of authorized values and set param value */
    /* to the corresponding item in setintval array (mainly for RRC params)     */
    int okintval[CONFIG_MAX_NUMCHECKVAL];                        /* integer array, store possible values in config file */
    int setintval[CONFIG_MAX_NUMCHECKVAL];                        /* integer array, values set in the paramdef structure */
    int num_okintval;                                            /* number of valid values in the checkingval array */
  } s1a;
  struct {
    int  (*f2)(paramdef_t *param);  /* check an integer against an authorized range, defined by its min and max value */
    int okintrange[CONFIG_MAX_NUMCHECKVAL];  /* integer array, store  min and max values  */

  } s2;
  struct {
    int  (*f3)(paramdef_t *param); /* check a string against a list of authorized values */
    char *okstrval[CONFIG_MAX_NUMCHECKVAL];                      /* string array, store possible values  */
    int  num_okstrval;                                           /* number of valid values in the checkingval array */
  } s3;
  struct {
    int  (*f3a)(paramdef_t *param); /* check a string against a list of authorized values and set param value */
    /* to the corresponding item in setintval array (mainly for RRC params) */
    char *okstrval[CONFIG_MAX_NUMCHECKVAL];                      /* string array, store possible values  */
    int  setintval[CONFIG_MAX_NUMCHECKVAL];                      /* integer array, values set in the paramdef structure */
    int  num_okstrval;                                           /* number of valid values in the checkingval array */
  } s3a;
  struct {
    int  (*f4)(paramdef_t *param); /* generic check function, no arguments but the param description */

  } s4;
  struct {
    void (*checkfunc)(void) ;
  } s5;
} checkedparam_t;

#endif