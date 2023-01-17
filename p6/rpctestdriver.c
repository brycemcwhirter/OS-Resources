/*************************************************
*
* Author: Bryce McWhirte
* Assignment: Program 6 (rpctestdriver.c) 
* Class: Intro to Operating Systems
* 
**************************************************/

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


#include <xmlrpc-c/client.h>

#define NAME "XML-RPC C TestDriver"
#define VERSION "1.0"


void printMode(mode_t st_mode){
    printf("Mode: ");
    printf( (S_ISDIR(st_mode)) ? "d" : "-");
    printf( (st_mode & S_IRUSR) ? "r" : "-");
    printf( (st_mode & S_IWUSR) ? "w" : "-");
    printf( (st_mode & S_IXUSR) ? "x" : "-");
    printf( (st_mode & S_IRGRP) ? "r" : "-");
    printf( (st_mode & S_IWGRP) ? "w" : "-");
    printf( (st_mode & S_IXGRP) ? "x" : "-");
    printf( (st_mode & S_IROTH) ? "r" : "-");
    printf( (st_mode & S_IWOTH) ? "w" : "-");
    printf( (st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}





void die_if_fault_occurred (xmlrpc_env *env)
{
    /* Check our error-handling environment for an XML-RPC fault. */
    if (env->fault_occurred) {
        fprintf(stderr, "XML-RPC Fault: %s (%d)\n",
                env->fault_string, env->fault_code);
        exit(1);
    }
}





int main(int argc, char **argv){

    // Argument Validation
    if(argc -1 != 2){
        fprintf(stderr, "Run as ./rpctestdriver <server name/ip> <port>\n");
        exit(1);
    }
    char * serverNameIP = argv[1];
    char * port = argv[2];

    // The URL of the server
    char url[50];

    // Generating URL
    strcpy(url, "http://");
    strcat(url, serverNameIP);
    strcat(url, ":");
    strcat(url, port);
    strcat(url, "/RPC2");

    // XMLRPC Environment
    xmlrpc_env env;

    // The Result Returned 
    xmlrpc_value * result;
    
    // Return Value for Create
    xmlrpc_int32 createReturnVal;

    
    // Setting up Client Environment
    xmlrpc_client_init(XMLRPC_CLIENT_NO_FLAGS, "XML-RPC Client", "1.0");
    xmlrpc_env_init(&env);

    // Create file “f1” with O_RDWR + O_TRUNC + permissions 640 and FD=FDa
    result = xmlrpc_client_call(&env, url, "rpc.create", "(sii)", "f1", (xmlrpc_int32) O_RDWR | O_TRUNC, 416);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int FDa = (int) createReturnVal;
    if(FDa < 0){
        fprintf(stderr, "%s\n", strerror(-FDa));
        exit(1);
    }
    





    // Write “go” to FDa - Verify 2 bytes written
    result = xmlrpc_client_call(&env, url, "rpc.write", "(isii)", FDa, "go", (xmlrpc_int32) 2,(xmlrpc_int32) 0);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int writeVal = (int) createReturnVal;
    if(writeVal != 2){
        fprintf(stderr, "%s\n", strerror(-writeVal));
        exit(1);
    }




    // Write “team” to FDa - Verify 4 bytes written
    result = xmlrpc_client_call(&env, url, "rpc.write", "(isii)", FDa, "team", (xmlrpc_int32) 4,(xmlrpc_int32) 2);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int writeVal2 = (int) createReturnVal;
    if(writeVal2 != 4){
        fprintf(stderr, "%s\n", strerror(-writeVal2));
        exit(1);
    }



    // Release FDa - Verify no error
    result = xmlrpc_client_call(&env, url, "rpc.release", "(i)", (xmlrpc_int32) FDa);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    if(0 != (int) createReturnVal){
        fprintf(stderr, "Release Error: %s\n", strerror(- (int) createReturnVal));
        exit(1);
    }

    // Release FDa - Verify specific error value
    result = xmlrpc_client_call(&env, url, "rpc.release", "(i)", (xmlrpc_int32) FDa);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int fdaRetVal = (int) createReturnVal;
    if(0 > fdaRetVal){
        printf("Second Release of FDa Message: %s\n", strerror(-fdaRetVal));
    }


    // Create file “f2” with O_RDWR + O_TRUNC + permissions 600 and FD=FDb
    result = xmlrpc_client_call(&env, url, "rpc.create", "(sii)", "f2", (xmlrpc_int32) O_RDWR | O_TRUNC, 384);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int FDb = (int) createReturnVal;
    if(FDb < 0){
        fprintf(stderr, "Error on creating f2: %s\n", strerror(-FDb));
        exit(1);
    }



    // Write “bear” to FDb - Verify 4 bytes written
    result = xmlrpc_client_call(&env, url, "rpc.write", "(isii)", FDb, "bear", (xmlrpc_int32) 4,(xmlrpc_int32) 0);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int writeVal3 = (int) createReturnVal;
    if(writeVal3 != 4){
        fprintf(stderr, "Error on Write: %s\n", strerror(-writeVal3));
        exit(1);
    }


    // Open file “f1” with O_RDONLY and FD=FDc
    result = xmlrpc_client_call(&env, url, "rpc.open", "(si)", "f1", (xmlrpc_int32) O_RDONLY);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int FDc = (int) createReturnVal;
    if(FDc < 0){
        fprintf(stderr, "Error on Create: %s\n", strerror(-FDc));
        exit(1);
    }


    // Read 3 bytes from FDc - Verify 3 bytes read consisting of “got”
    int size;
    char *buf;
    result = xmlrpc_client_call(&env, url, "rpc.read", "(iii)", (xmlrpc_int32) FDc, (xmlrpc_int32) 3, (xmlrpc_int32) 0);
    die_if_fault_occurred(&env);
    xmlrpc_decompose_value(&env, result,"{s:i,s:s,*}",
                            "size", &size,
                            "buf", &buf);
    die_if_fault_occurred(&env);
    if(size < 0){
        fprintf(stderr, "Error On Read: %s\n", strerror(-size));
    }
    else{
        printf("Read from FDc: %s\n", buf);
    }

    
    // Read 5 bytes from FDc - Verify 3 bytes read consisting of “eam”
    int size2;
    char *buf2;
    result = xmlrpc_client_call(&env, url, "rpc.read", "(iii)", (xmlrpc_int32) FDc, (xmlrpc_int32) 5, (xmlrpc_int32) 3);
    die_if_fault_occurred(&env);
    xmlrpc_decompose_value(&env, result,"{s:i,s:s,*}",
                            "size", &size2,
                            "buf", &buf2);
    die_if_fault_occurred(&env);
    if(size < 0){
        fprintf(stderr, "Error On Read: %s\n", strerror(-size));
    }
    else{
        printf("Read from FDc %s\n", buf2);
    }

    // Release FDc
    result = xmlrpc_client_call(&env, url, "rpc.release", "(i)", (xmlrpc_int32) FDc);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    if(0 != (int) createReturnVal){
        fprintf(stderr, "Error On Release: %s\n", strerror(-(int) createReturnVal));
        exit(1);
    }

    

    // Get attributes of “f1” and list mode (rwxrwxrwx), userid, and file size
    xmlrpc_value * getAttrValue;
    getAttrValue = xmlrpc_client_call(&env, url, "rpc.getAttr", "(s)", "f1");
    die_if_fault_occurred(&env);
    struct stat sb;
    int error;
    xmlrpc_decompose_value(&env, getAttrValue,"{s:i,s:i,s:i,s:i,*}", 
                            "errno", &error,
                            "st_mode",&sb.st_mode,
                            "st_uid", &sb.st_uid,
                            "st_size", &sb.st_size);
    die_if_fault_occurred(&env);

    if(error < 0){
        fprintf(stderr, "%s\n", strerror(-error));
        exit(1);
    }
    else{
        printf("f1 ");
        printMode(sb.st_mode);
        printf("User ID: %d ", sb.st_uid);
        printf("Size : %zu\n", sb.st_size);
    }

    




    // Chmod 777 for “f1”
    result = xmlrpc_client_call(&env, url, "rpc.chmod", "(si)", "f1", (xmlrpc_int32) 511);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    if(0 != (int) createReturnVal){
        fprintf(stderr, "Error on chmod: %s\n", strerror(- (int) createReturnVal));
        exit(1);
    }

    // Get attributes of “f1” and list mode (rwxrwxrwx), userid, and file size
    xmlrpc_value * getAttrValue2;
    getAttrValue2 = xmlrpc_client_call(&env, url, "rpc.getAttr", "(s)", "f1");
    die_if_fault_occurred(&env);
    struct stat sb2;
    int error2;
    xmlrpc_decompose_value(&env, getAttrValue2,"{s:i,s:i,s:i,s:i,*}", 
                            "errno", &error2,
                            "st_mode",&sb2.st_mode,
                            "st_uid", &sb2.st_uid,
                            "st_size", &sb2.st_size);
    die_if_fault_occurred(&env);

   if(error2 < 0){
        fprintf(stderr, "%s\n", strerror(-error));
        exit(1);
    }
    else{
        printf("f1 ");
        printMode(sb2.st_mode);
        printf("User ID: %d ", sb2.st_uid);
        printf("Size : %zu\n", sb2.st_size);
    }

    // Open file “fx” with O_RDONLY and FD=FDd - Verify specific error value
    result = xmlrpc_client_call(&env, url, "rpc.open", "(si)", "fx", (xmlrpc_int32) O_RDONLY);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int FDd = (int) createReturnVal;
    if(FDd > 0){
        fprintf(stderr, "Error: Opening fx should fail");
        exit(1);
    }
    else{
        printf("Opening fx error message: %s\n", strerror(-FDd));
    }

    // Open file “fy” with O_RDONLY and FD=FDe - Verify specific error value
    result = xmlrpc_client_call(&env, url, "rpc.open", "(si)", "fy", (xmlrpc_int32) O_RDONLY);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int FDe = (int) createReturnVal;
    if(FDe > 0){
        fprintf(stderr, "Error: Opening fy should fail");
        exit(1);
    }
     else{
        printf("Opening fy error message: %s\n", strerror(-FDe));
    }

    // Open file “f1” with O_RDONLY and FD=FDf
    result = xmlrpc_client_call(&env, url, "rpc.open", "(si)", "f1", (xmlrpc_int32) O_RDONLY);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int FDf = (int) createReturnVal;
    if(FDf < 0){
        fprintf(stderr, "Error on Open: %s\n", strerror(-FDf));
        exit(1);
    }


    // Write “bad” to FDf - Verify specific error value
    result = xmlrpc_client_call(&env, url, "rpc.write", "(isii)", FDf, "bad", (xmlrpc_int32) 3,(xmlrpc_int32) 0);
    die_if_fault_occurred(&env);
    xmlrpc_parse_value(&env, result, "i", &createReturnVal);
    die_if_fault_occurred(&env);
    int bad = (int) createReturnVal;
    if(bad != 3 || bad < 0){
        printf("Writing to f1 error message: %s\n", strerror(-bad));
    }


    // Dispose Result Value
    xmlrpc_DECREF(result);

    // Shutdown Client & Environment
    xmlrpc_env_clean(&env);
    xmlrpc_client_cleanup();

    return 0;

}
