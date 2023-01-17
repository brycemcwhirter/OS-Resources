/*************************************************
*
* Author: Bryce McWhirte
* Assignment: Program 6 (rpcserver.c) Version 1.1
* Class: Intro to Operating Systems
* 
**************************************************/

#include "rpcserver.h"





static xmlrpc_value * rpc_read(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo){

    /*The File Descriptor*/
    int fd;

    // The Size to read
    int size;

    // Specified Offset
    int offset;

    // The Buffer to store read value
    char buf[MAX_BUFFER];

    // decompose values
    xmlrpc_decompose_value(envP, paramArrayP, "(iii)", &fd, &size, &offset);
    if (envP->fault_occurred){
        return NULL;
    }


    // Go to file offset
    if (lseek(fd, offset, SEEK_SET) < 0) {
        return xmlrpc_build_value(envP, 
                            "{s:i,s:s}", 
                            "size", -errno,
                            "buf", "");
    }


    
    ssize_t readBytes = read(fd, buf, (size_t) size);


    if (readBytes < 0) {
        return xmlrpc_build_value(envP, 
                            "{s:i,s:s}", 
                            "size", -errno,
                            "buf", "");
    }


    char setOfBytes[readBytes + 1];
    strncpy(setOfBytes, buf, (size_t) readBytes);
    setOfBytes[readBytes] = '\0';




    // Return number of read bytes
    return xmlrpc_build_value(envP, 
                            "{s:i,s:s}", 
                            "size", readBytes,
                            "buf", setOfBytes);
}











static xmlrpc_value * rpc_write(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo){

    // The File Descriptor
    int fd;

    // The buffer holding to bytes to write from
    void *buf;

    // The Size of bytes to write
    int size;

    // Specified Offset
    int offset;

    // Decomposing Values
    xmlrpc_decompose_value(envP, paramArrayP, "(isii)", &fd, &buf, &size, &offset);
    if (envP->fault_occurred){
        return NULL;
    }


    // Go to file offset
    if (lseek(fd, offset, SEEK_SET) < 0) {
        return xmlrpc_build_value(envP, "i", -errno);
    }

    // Write bytes
    ssize_t writtenBytes = write(fd, buf, (size_t) size);
    if (writtenBytes < 0) {
        return xmlrpc_build_value(envP, "i", -errno);
    }

    // Returning the set of written bytes
    return xmlrpc_build_value(envP, "i", writtenBytes);
}










static xmlrpc_value * rpc_create(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo){


    // Compute the full path name
    char* path;

    // Any specified flags
    int flags;

    // The Specified mode
    int mode;

    // Decompose the Values
    xmlrpc_decompose_value(envP, paramArrayP, "(sii)", &path, &flags, &mode);
    if (envP->fault_occurred){
        return NULL;
    }


    // Create a File 
    int fd = open(path, flags | O_CREAT, mode);
    if (fd <= 0) {
        return xmlrpc_build_value(envP, "i", -errno);
    }


    // Returning the File Descriptor
    return xmlrpc_build_value(envP, "i", fd);
}
















static xmlrpc_value * rpc_chmod(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo){
    
    // the name of the file
    char *pathName; 

    // the new set of permissions
    int mode;

    // Decomposing the values
    xmlrpc_decompose_value(envP, paramArrayP, "(si)", &pathName, &mode);
    if (envP->fault_occurred){
        return NULL;
    }

    // Calling CHMOD
    int retVal = chmod(pathName, (mode_t) mode);
    if (retVal < 0) {
        return xmlrpc_build_value(envP, "i", -errno);
    }

    return xmlrpc_build_value(envP, "i", retVal);
}











static xmlrpc_value * rpc_release (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo){

    //the file descriptor to be released
    int fd;

    // decomposing values
    xmlrpc_decompose_value(envP, paramArrayP, "(i)", &fd);
    if (envP->fault_occurred){
        return NULL;
    }
    
    // closing fd
    int returnVal = close(fd);
    if(returnVal < 0){
        return xmlrpc_build_value(envP, "i", -errno);
    }


    return xmlrpc_build_value(envP, "i", returnVal);
}




static xmlrpc_value * rpc_open(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo){

    // The path of the file
    char *path;

    // Any specified flags
    int flags;



    // Decompose the Values
    xmlrpc_decompose_value(envP, paramArrayP, "(si)", &path, &flags);
    if (envP->fault_occurred){
        return NULL;
    }


    // Opening the file
    int fd = open(path, flags);
    if (fd <= 0) {
        return xmlrpc_build_value(envP, "i", -errno);
    }


    return xmlrpc_build_value(envP, "i", fd);
}













static xmlrpc_value * rpc_getAttr (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo){

    // The Specified path
    char *path;

    // Decompose values
    xmlrpc_decompose_value(envP, paramArrayP, "(s)", &path);
    if (envP->fault_occurred){
        return NULL;
    }

    struct stat sb;

    // storing sb
    if(stat(path, &sb) == -1){
        return xmlrpc_build_value(envP, 
                            "{s:i,s:i,s:i,s:i}", 
                            "errno", -errno,
                            "st_mode",sb.st_mode,
                            "st_uid", sb.st_uid,
                            "st_size", sb.st_size);
    }

    return xmlrpc_build_value(envP, 
                            "{s:i,s:i,s:i,s:i}", 
                            "errno", 0,
                            "st_mode",sb.st_mode,
                            "st_uid", sb.st_uid,
                            "st_size", sb.st_size);
}






static xmlrpc_value * rpc_getCwd(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void *const serverInfo, void * const channelInfo){

    // Generate Path & Get Current working directory
    char path[MAX_BUFFER];
    getcwd(path, MAX_BUFFER);

    // Return working directory. 
    return xmlrpc_build_value(envP, "s", path);
}







// Adds all of the methods above to the registry
void addMethodsToRegistry(xmlrpc_env env, xmlrpc_registry * registryP){

struct xmlrpc_method_info3 const chmodInfo = {
        /* .methodName     = */ "rpc.chmod",
        /* .methodFunction = */ &rpc_chmod,
                                NULL,
                                0,
                                NULL,
                                NULL,
};

struct xmlrpc_method_info3 const openInfo = {
/* .methodName     = */ "rpc.open",
/* .methodFunction = */ &rpc_open,
                            NULL,
                                0,
                                NULL,
                                NULL,
};

struct xmlrpc_method_info3 const createInfo = {
    /* .methodName     = */ "rpc.create",
    /* .methodFunction = */ &rpc_create,
                                NULL,
                                0,
                                NULL,
                                NULL,
};

struct xmlrpc_method_info3 const releaseInfo = {
    /* .methodName     = */ "rpc.release",
    /* .methodFunction = */ &rpc_release,
                                NULL,
                                0,
                                NULL,
                                NULL,
};

struct xmlrpc_method_info3 const getAttrInfo = {
    /* .methodName     = */ "rpc.getAttr",
    /* .methodFunction = */ &rpc_getAttr,
                                NULL,
                                0,
                                NULL,
                                NULL,
};

struct xmlrpc_method_info3 const readInfo = {
    /* .methodName     = */ "rpc.read",
    /* .methodFunction = */ &rpc_read,
                                NULL,
                                0,
                                NULL,
                                NULL,
};

struct xmlrpc_method_info3 const writeInfo = {
    /* .methodName     = */ "rpc.write",
    /* .methodFunction = */ &rpc_write,
                                NULL,
                                0,
                                NULL,
                                NULL,
};

struct xmlrpc_method_info3 const getCwdInfo = {
    /* .methodName     = */ "rpc.getCwd",
    /* .methodFunction = */ &rpc_getCwd,
                                NULL,
                                0,
                                NULL,
                                NULL,
};

    xmlrpc_registry_add_method3(&env, registryP, &chmodInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    xmlrpc_registry_add_method3(&env, registryP, &openInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    xmlrpc_registry_add_method3(&env, registryP, &createInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

   

    xmlrpc_registry_add_method3(&env, registryP, &releaseInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    xmlrpc_registry_add_method3(&env, registryP, &getAttrInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    xmlrpc_registry_add_method3(&env, registryP, &readInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    xmlrpc_registry_add_method3(&env, registryP, &writeInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    xmlrpc_registry_add_method3(&env, registryP, &getCwdInfo);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }
}








int main(int argc, char** argv){

    // The Server Parameters
    xmlrpc_server_abyss_parms serverparm;

    // The Set of methods the server provides
    xmlrpc_registry * registryP;

    // The XMLRPC Environment
    xmlrpc_env env;

    // Argument Validation
    if (argc-1 != 1) {
        fprintf(stderr, "Run As ./rpcserver <port>\n");
        exit(1);
    }

    // Initialize The Environment
    xmlrpc_env_init(&env);

    // Initialize Registry
    registryP = xmlrpc_registry_new(&env);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_new() failed.  %s\n", env.fault_string);
        exit(1);
    }


    // Adding all the methods to our registry 
    addMethodsToRegistry(env, registryP);


    
    // Setting Up Server Parameters
    serverparm.config_file_name = NULL;   /* Select the modern normal API */
    serverparm.registryP        = registryP;
    serverparm.port_number      = (unsigned int) strtoul(argv[1], NULL, 10);


    // Run the server
    printf("Running XML-RPC server...\n");


    // Start up the Server
    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));
    if (env.fault_occurred) {
        printf("xmlrpc_server_abyss() failed.  %s\n", env.fault_string);
        exit(1);
    }

    return 0;
}