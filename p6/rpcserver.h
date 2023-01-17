/*************************************************
*
* Author: Bryce McWhirte
* Assignment: Program 6 (rpcserver.h) Version 1.1
* Class: Intro to Operating Systems
* 
**************************************************/

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <utime.h>



#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>



#include "include/xmlrpc-c/config.h"  /* information about this build environment */

#define MAX_BUFFER 4096


/**
 * Reads up to the specified number of bytes into the given buffer, starting
 * 
 * @param envP - The Given XMLRPC Environment
 * @param paramArrayP - The Array of Parameters
 *  "iii" - fd(i), size(i), offset(i)
 * @param serverInfo - Server Information
 * @param channelInfo - Channel Information
 * 
 * @returns xmlrpc_value - "is" - number of bytes read or the negative
 *   value of errno if an error occured(i), 
 * the set of bytes read or an empty string if -errno returned (s)
 *  */
static xmlrpc_value * rpc_read(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo);








/**
 * Writes up to a specified number of bytes into a file descriptor
 * 
 * @param envP - The Given XMLRPC Environment
 * @param paramArrayP - The Array of Parameters
 *  "isii" - fd(i), buf(s), size(i), offset(i)
 * @param serverInfo - Server Information
 * @param channelInfo - Channel Information
 * 
 * @returns xmlrpc_value - "i" - number of bytes written or the negative
 *   value of errno if an error occured(i)
 * 
 */
static xmlrpc_value * rpc_write(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo);








/**
 * Creates a new file with the given path & mode
 * 
 * @param envP - The Given XMLRPC Environment
 * @param paramArrayP - The Array of Parameters
 *  "sii" - path(s), flags(i), mode(i)
 * @param serverInfo - Server Information
 * @param channelInfo - Channel Information
 * 
 * @returns xmlrpc_value - "i" - the new file descriptor or the negative value
 *   of errno if an error occured(i)
 * 
 */
static xmlrpc_value * rpc_create(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo);








/**
 * @param envP - The Given XMLRPC Environment
 * @param paramArrayP - The Array of Parameters
 *  "si" - pathName(s), mode(i)
 * @param serverInfo - Server Information
 * @param channelInfo - Channel Information
 * 
 * @returns xmlrpc_value - "i" - the value returned when chmod
 *   was called or the negative value of errno if an error occured(i)
 * 
 */
static xmlrpc_value * rpc_chmod(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo);





/**
 * @param envP - The Given XMLRPC Environment
 * @param paramArrayP - The Array of Parameters
 *  "i" - fd(i)
 * @param serverInfo - Server Information
 * @param channelInfo - Channel Information
 * 
 * @returns xmlrpc_value - "i" - the value returned when close
 *   was called or the negative value of errno if an error occured(i)
 * 
 */
static xmlrpc_value * rpc_release (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo);



/**
 * @param envP - The Given XMLRPC Environment
 * @param paramArrayP - The Array of Parameters
 *  "sii" - path(s), flags(i)
 * @param serverInfo - Server Information
 * @param channelInfo - Channel Information
 * 
 * @returns xmlrpc_value - "i" - the value returned when open
 *   was called or the negative value of errno if an error occured(i)
 * 
 */
static xmlrpc_value * rpc_open(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo);





/**
 * @param envP - The Given XMLRPC Environment
 * @param paramArrayP - The Array of Parameters
 *  "sS" - path(s), struct stat buffer(S)
 * @param serverInfo - Server Information
 * @param channelInfo - Channel Information
 * 
 * @returns xmlrpc_value - "S" - the struct stat related to the file, or a empty struct stat if
 * an error occured (S)
 */
static xmlrpc_value * rpc_getAttr (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo);





/**
* @param envP - The Given XMLRPC Environment
 * @param paramArrayP - The Array of Parameters
 * @param serverInfo - Server Information
 * @param channelInfo - Channel Information
 * 
 * @returns xmlrpc_value - "s" - The path of the current working directory
*/
static xmlrpc_value * rpc_getCwd(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo);

