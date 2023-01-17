#define FUSE_USE_VERSION 26


#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>


#define CACHING 1
#define NAME "XML-RPC Client"
#define VERSION "1.0"
#define PATH_MAX 500



static char * server; 
static char * port;

static char url[PATH_MAX];
static char fileSystemRoot[PATH_MAX];


/*
 * Compute the byte length of the full path string 
 * from the root (fileSystemRoot + path)
 */
static size_t getFullPathLength(const char *path) {
  return strlen(fileSystemRoot) + strlen(path) + 1;
}




/*
 * Compute the full path from the root (fileSystemRoot + path).
 * We assume the user only wants up to n-1 bytes of the full path.
 */
static char *getFullPath(const char *path, char *fullPath, size_t n) {
  strncpy(fullPath, fileSystemRoot, n);
  strncat(fullPath, path, n);
  return fullPath;
}





/*
* Checks if a fault occured in an XMLRPC Environment
*/
void die_if_fault_occurred (xmlrpc_env *env)
{
    /* Check our error-handling environment for an XML-RPC fault. */
    if (env->fault_occurred) {
        fprintf(stderr, "XML-RPC Fault: %s (%d)\n",
                env->fault_string, env->fault_code);
        exit(1);
    }
}





/*
* Sets up Working Environment by establishing Client URL & file root
*/
void setUpEnvironment(){

  // Creating URL
  strcpy(url, "http://");
  strcat(url, server);
  strcat(url, ":");
  strcat(url, port);
  strcat(url, "/RPC2");


  //Creating XMLRPC Environment
  xmlrpc_env env;
  xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
  xmlrpc_env_init(&env);

  // Getting Result
  xmlrpc_value * result;
  char *buf;
  

  // Grabbing root path of server
  result = xmlrpc_client_call(&env, url, "rpc.getCwd", "()");
  die_if_fault_occurred(&env);

  // Parsing Return Value
  xmlrpc_parse_value(&env, result, "s", &buf);
  die_if_fault_occurred(&env);

  // Copying File into file system root
  strncpy(fileSystemRoot, buf, sizeof fileSystemRoot - 1);
  fileSystemRoot[PATH_MAX - 1] = '\0';


  // Cleaning Environment
  xmlrpc_DECREF(result);
  xmlrpc_env_clean(&env);
}








static int hoofs_setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
  printf("setxattr\n");
  return 0;
}

static int hoofs_chown(const char *path, uid_t uid, gid_t gid) {
  printf("chown\n");
  return 0;
}

static int hoofs_utime(const char *path, struct utimbuf *ubuf) {
  printf("utime\n");
  return 0;
}

static int hoofs_truncate(const char *path, off_t newsize) {
  printf("truncate\n");
  return 0;
}

static int hoofs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
  printf("readdir\n");

  return 0;
}





static int hoofs_chmod(const char *path, mode_t mode) {

  //Creating XMLRPC Environment
  xmlrpc_env env;
  xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
  xmlrpc_env_init(&env);

  // Result & Return Values
  xmlrpc_value * result;
  xmlrpc_int32 createReturnVal;

  // Compute the full path name
  size_t pathLen = getFullPathLength(path);
  char fullPath[pathLen];
  getFullPath(path, fullPath, pathLen);

  // Calling chmod
  result = xmlrpc_client_call(&env, url, "rpc.chmod", "(si)", fullPath, (xmlrpc_int32) mode);
  die_if_fault_occurred(&env);

  // Parsing Return Value
  xmlrpc_parse_value(&env, result, "i", &createReturnVal);
  die_if_fault_occurred(&env);

  // Cleaning Environment
  xmlrpc_DECREF(result);
  xmlrpc_env_clean(&env);


  /** // TODO Caching
  * If the file exist, update file mode in cache with new mode
  */ 


  // Returning Value
  int returnValue = (int) createReturnVal;
  return returnValue;
}





static int hoofs_getattr(const char *path, struct stat *stbuf) {

    /** // TODO Caching
    * If the file exist, return the file attributes
    */ 

    //XMLRPC Environment
    xmlrpc_env env;
    xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
    xmlrpc_env_init(&env);

    // Value Returned from GetAttr
    xmlrpc_value * getAttrValue;

    // Compute the full path name
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);

    // Calling Method
    getAttrValue = xmlrpc_client_call(&env, url, "rpc.getAttr", "(s)", fullPath);
    die_if_fault_occurred(&env);

    // Stat Buffer
    struct stat sb;

    // errno value returned
    int errnoVal; 

    // Transfering into stack
    xmlrpc_decompose_value(&env, getAttrValue,"{s:i,s:i,s:i,s:i,*}", 
                            "errno", &errnoVal,
                            "st_mode",&sb.st_mode,
                            "st_uid", &sb.st_uid,
                            "st_size", &sb.st_size);
    die_if_fault_occurred(&env);

    // If error occured return error val
    if(errnoVal < 0){
      return errnoVal;
    }

    // Saving Stat Values
    stbuf->st_mode=sb.st_mode;
    stbuf->st_uid=sb.st_uid;
    stbuf->st_size=sb.st_size;

    
   
    // Cleaning Environmens
    xmlrpc_DECREF(getAttrValue);
    xmlrpc_env_clean(&env);

    /** // TODO Caching
    * Add File Attributes to Cache
    */ 


    return 0;
}






static int hoofs_open(const char *path, struct fuse_file_info *fi) {

  //XMLRPC Environment
  xmlrpc_env env;
  xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
  xmlrpc_env_init(&env);


  // Return Values
  xmlrpc_value * result;
  xmlrpc_int32 createReturnVal;

  // Compute the full path name
  size_t pathLen = getFullPathLength(path);
  char fullPath[pathLen];
  getFullPath(path, fullPath, pathLen);


  // Calling Open
  result = xmlrpc_client_call(&env, url, "rpc.open", "(si)", fullPath, (xmlrpc_int32) fi->flags);
  die_if_fault_occurred(&env);

  // Parsing Result
  xmlrpc_parse_value(&env, result, "i", &createReturnVal);
  die_if_fault_occurred(&env);


  // Returning Errno on Error
  int fd = (int) createReturnVal; 
  if(fd < 0){
    return -fd;
  }

  // Cleaning Environment
  xmlrpc_DECREF(result);
  xmlrpc_env_clean(&env);

  // Saving File Descriptor
  fi->fh = (uint64_t) createReturnVal;

  /** // TODO Caching
    * Add File Descriptor to Cache
    */ 

  return 0;
}







static int hoofs_release(const char *path, struct fuse_file_info *fi) {

  // Creating XMLRPC Environment
  xmlrpc_env env;
  xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
  xmlrpc_env_init(&env);
  

  // Result & Return Values
  xmlrpc_value * result;
  xmlrpc_int32 releaseVal;


  // Calling Release
  result = xmlrpc_client_call(&env, url, "rpc.release", "(i)", (xmlrpc_int64) fi->fh);
  die_if_fault_occurred(&env);

  // Parsing Return Value
  xmlrpc_parse_value(&env, result, "i", &releaseVal);
  die_if_fault_occurred(&env);
  
  // Cleaning Environment
  xmlrpc_DECREF(result);
  xmlrpc_env_clean(&env);

  /** // TODO Caching
    * Release File from Cache
    */ 

  return (int) releaseVal;
}







static int hoofs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {


  //XMLRPC Environment
  xmlrpc_env env;
  xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
  xmlrpc_env_init(&env);

  // Result & Return Values
  xmlrpc_value * result;
  xmlrpc_int64 createReturnVal;

  // Compute the full path name
  size_t pathLen = getFullPathLength(path);
  char fullPath[pathLen];
  getFullPath(path, fullPath, pathLen);

  // Calling Create
  result = xmlrpc_client_call(&env, url, "rpc.create", "(sii)", fullPath, (xmlrpc_int32) fi->flags, (xmlrpc_int32) mode);
  die_if_fault_occurred(&env);

  // Parse return value
  xmlrpc_parse_value(&env, result, "i", &createReturnVal);
  die_if_fault_occurred(&env);

  xmlrpc_DECREF(result);
  xmlrpc_env_clean(&env);

  // Returning Errno on Error
  int fd = (int) createReturnVal;
  if(fd < 0){
    return fd;
  }
  
  // Saving File Descriptor
  fi->fh = (uint64_t) fd;

  /** // TODO Caching
    * Add FD to Cache
    */ 
  
  return 0;

}









static int hoofs_unlink(const char *path) {

  //XMLRPC Environment
  xmlrpc_env env;
  xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
  xmlrpc_env_init(&env);


  // Result & Return Values
  xmlrpc_value * result;
  xmlrpc_int32 unlinkVal;


  // Compute the full path name
  size_t pathLen = getFullPathLength(path);
  char fullPath[pathLen];
  getFullPath(path, fullPath, pathLen);
  

  // Calling Unlink
  result = xmlrpc_client_call(&env, url, "rpc.unlink", "(s)", fullPath);
  die_if_fault_occurred(&env);

  // Parsing Return Value
  xmlrpc_parse_value(&env, result, "i", &unlinkVal);
  die_if_fault_occurred(&env);

  // Cleaning Environment
  xmlrpc_DECREF(result);
  xmlrpc_env_clean(&env);

  /** // TODO Caching
    * Remove any file with same name from cache
    */ 

  return (int) unlinkVal;
}





static int hoofs_rmdir(const char *path) {
  /** // TODO Caching
    * Remove any file with same name from cache
    */ 


  return hoofs_unlink(path);
}





static int hoofs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

  //XMLRPC Environment
  xmlrpc_env env;
  xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
  xmlrpc_env_init(&env);
  

  // Generating Result & return value
  xmlrpc_value * readResult;
  int readReturnValue;
  void *readBuf;

  // Calling Method
  readResult = xmlrpc_client_call(&env, url, "rpc.read", "(iii)", (xmlrpc_int64) fi->fh, (xmlrpc_int32) size, (xmlrpc_int32) offset);
  die_if_fault_occurred(&env);

  // Parsing Read Results
  xmlrpc_decompose_value(&env, readResult,"{s:i,s:s,*}",
                            "size", &readReturnValue,
                            "buf", &readBuf);
  die_if_fault_occurred(&env);


  // Copying Read Bytes into buffer
  strncpy(buf, readBuf, (size_t) readReturnValue);


  // Cleaning Environment
  xmlrpc_DECREF(readResult);
  xmlrpc_env_clean(&env);

  return readReturnValue;
}






static int hoofs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

  //XMLRPC Environment
  xmlrpc_env env;
  xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
  xmlrpc_env_init(&env);

  // Setting up result & return value
  xmlrpc_value * result;
  xmlrpc_int32 writeVal;


  // Calling Write
  result = xmlrpc_client_call(&env, url, "rpc.write", "(isii)", (xmlrpc_int64) fi->fh, buf, (xmlrpc_int32) size, (xmlrpc_int32) offset);
  die_if_fault_occurred(&env);

  // Parsing 
  xmlrpc_parse_value(&env, result, "i", &writeVal);
  die_if_fault_occurred(&env);

  // Cleaning Environment 
  xmlrpc_DECREF(result);
  xmlrpc_env_clean(&env);

  /** // TODO Caching
    * Update size of file with same name 
    */ 

  // Return Result
  return (int) writeVal;
}




static struct fuse_operations hoofs_oper = {
  .getattr = hoofs_getattr,
  .setxattr = hoofs_setxattr,
  .chmod = hoofs_chmod,
  .chown = hoofs_chown,
  .utime = hoofs_utime,
  .truncate = hoofs_truncate,
  .readdir = hoofs_readdir,
  .open = hoofs_open,
  .release = hoofs_release,
  .create = hoofs_create,
  .unlink = hoofs_unlink,
  .rmdir = hoofs_rmdir,
  .read = hoofs_read,
  .write = hoofs_write,
};





static int myfs_opt_proc(void *data, const char *arg, int key, struct fuse_args *outargs) {
  if (key == FUSE_OPT_KEY_NONOPT && (server == NULL || port == NULL)) {
    if (server == NULL) {
        server = strdup(arg);
    }
    else if (port == NULL) {
        port = strdup(arg);
    }
    return 0;
  }
  return 1;
}



int main(int argc, char** argv){

    if(argc-1 < 3){
        fprintf(stderr, "Usage: ./hoofsrpc <server name/ip> <port> <fuse parameters> ..\n");
        exit(1);
    }    
    
    
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    
    fuse_opt_parse(&args, NULL, NULL, myfs_opt_proc);

    setUpEnvironment();

    int r = fuse_main(args.argc, args.argv, &hoofs_oper, NULL);

    return r;
}
