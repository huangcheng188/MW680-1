/**
 * @file lv_port_fs_templ.c
 *
 */

 /*Copy this file as "lv_port_fs.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
//#include "lv_port_fs_templ.h"
#include "lvgl/lvgl.h"
#include "ff.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/* Create a type to store the required data about your file.
 * If you are using a File System library
 * it already should have a File type.
 * For example FatFS has `FIL`. In this case use `typedef FIL file_t`*/
#if 0 //huangcheng
typedef struct {
    /*Add the data you need to store about a file*/
    uint32_t dummy1;
    uint32_t dummy2;
}file_t;
#endif
typedef FIL file_t;		//add huangcheng
/*Similarly to `file_t` create a type for directory reading too */
#if 0 //huangcheng
typedef struct {
    /*Add the data you need to store about directory reading*/
    uint32_t dummy1;
    uint32_t dummy2;
}dir_t;
#endif
typedef DIR dir_t;		//add huangcheng

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static lv_fs_res_t fs_open (struct _lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close (struct _lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read (struct _lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(struct _lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek (struct _lv_fs_drv_t * drv, void * file_p, uint32_t pos);
static lv_fs_res_t fs_size (struct _lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
static lv_fs_res_t fs_tell (struct _lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);
static lv_fs_res_t fs_remove (struct _lv_fs_drv_t * drv, const char *path);
static lv_fs_res_t fs_trunc (struct _lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_rename (struct _lv_fs_drv_t * drv, const char * oldname, const char * newname);
static lv_fs_res_t fs_free (struct _lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p);
static lv_fs_res_t fs_dir_open (struct _lv_fs_drv_t * drv, void * rddir_p, const char *path);
static lv_fs_res_t fs_dir_read (struct _lv_fs_drv_t * drv, void * rddir_p, char *fn);
static lv_fs_res_t fs_dir_close (struct _lv_fs_drv_t * drv, void * rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_fs_init(void)
{
    /*----------------------------------------------------
     * Initialize your storage device and File System
     * -------------------------------------------------*/
    fs_init();

    /*---------------------------------------------------
     * Register the file system interface  in LittlevGL
     *--------------------------------------------------*/

    /* Add a simple drive to open images */
    lv_fs_drv_t fs_drv;                         /*A driver descriptor*/
    memset(&fs_drv, 0, sizeof(lv_fs_drv_t));    /*Initialization*/

    /*Set up fields...*/
    fs_drv.file_size = sizeof(file_t);
    fs_drv.letter = 'P';
    fs_drv.open_cb= fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;
    fs_drv.free_space_cb= fs_free;
    fs_drv.size_cb= fs_size;
    fs_drv.remove_cb= fs_remove;
    fs_drv.rename_cb= fs_rename;
    fs_drv.trunc_cb= fs_trunc;

    fs_drv.rddir_size = sizeof(dir_t);
    fs_drv.dir_close_cb= fs_dir_close;
    fs_drv.dir_open_cb= fs_dir_open;
    fs_drv.dir_read_cb= fs_dir_read;

    //lv_fs_add_drv(&fs_drv);
    lv_fs_drv_register( &fs_drv);		//huangcheng
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Initialize your Storage device and File system. */
static void fs_init(void)
{
    /*E.g. for FatFS initalize the SD card and FatFS itself*/

    /*You code here*/
}

/**
 * Open a file
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open (struct _lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    if(mode == LV_FS_MODE_WR)
    {
        /*Open a file for write*/

        /* Add your code here*/
    }
    else if(mode == LV_FS_MODE_RD)
    {
        /*Open a file for read*/

        /* Add your code here*/
    }
    else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
    {
        /*Open a file for read and write*/

        /* Add your code here*/
    }

    return res;
#endif
	 return f_open(file_p, path, mode);
}


/**
 * Close an opened file
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close (struct _lv_fs_drv_t * drv, void * file_p)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif 
	return f_close(file_p);
}

/**
 * Read data from an opened file
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read (struct _lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif 
	return f_read(file_p,buf, btr, br);
}

/**
 * Write into a file
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(struct _lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif
	return f_write(file_p,buf,btw,bw);
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek (struct _lv_fs_drv_t * drv, void * file_p, uint32_t pos)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif
	return f_lseek(file_p,pos);

}

/**
 * Give the size of a file bytes
 * @param file_p pointer to a file_t variable
 * @param size pointer to a variable to store the size
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_size (struct _lv_fs_drv_t * drv, void * file_p, uint32_t * size_p)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif
	return f_size((FIL*)file_p);
}
/**
 * Give the position of the read write pointer
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell (struct _lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif
	return f_tell((FIL*)file_p);
}

/**
 * Delete a file
 * @param path path of the file to delete
 * @return  LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_remove (struct _lv_fs_drv_t * drv, const char *path)
{
	printf("\r\n%d %s",__LINE__,__func__);
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Truncate the file size to the current position of the read write pointer
 * @param file_p pointer to an 'ufs_file_t' variable. (opened with lv_fs_open )
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_trunc (struct _lv_fs_drv_t * drv, void * file_p)
{
	printf("\r\n%d %s",__LINE__,__func__);
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Rename a file
 * @param oldname path to the file
 * @param newname path with the new name
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
static lv_fs_res_t fs_rename (struct _lv_fs_drv_t * drv, const char * oldname, const char * newname)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif
	return f_rename(oldname,newname);
}

/**
 * Get the free and total size of a driver in kB
 * @param letter the driver letter
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free size [kB]
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_free (struct _lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p)
{
	printf("\r\n%d %s",__LINE__,__func__);
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * Initialize a 'fs_read_dir_t' variable for directory reading
 * @param rddir_p pointer to a 'fs_read_dir_t' variable
 * @param path path to a directory
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_open (struct _lv_fs_drv_t * drv, void * rddir_p, const char *path)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0 //huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif
	return f_opendir(rddir_p,path);
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param rddir_p pointer to an initialized 'fs_read_dir_t' variable
 * @param fn pointer to a buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read (struct _lv_fs_drv_t * drv, void * rddir_p, char *fn)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0//huangcheng
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif 
	FILINFO info;
	lv_fs_res_t result = f_readdir(rddir_p,&info);
	
	return 	result;
}

/**
 * Close the directory reading
 * @param rddir_p pointer to an initialized 'fs_read_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close (struct _lv_fs_drv_t * drv, void * rddir_p)
{
	printf("\r\n%d %s",__LINE__,__func__);
#if 0
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
#endif
	return f_closedir(rddir_p);
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
