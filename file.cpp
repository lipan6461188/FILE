#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

/*
 
 LeetekiMacBook-Pro:Desktop lee$ ls -lR testDir
 total 0
 drwxr-xr-x  4 lee  staff  136 12 24 21:58 Dir1
 drwxr-xr-x  3 lee  staff  102 12 24 21:58 Dir2
 drwxr-xr-x  2 lee  staff   68 12 24 21:58 Dir3
 -rw-r--r--  1 lee  staff    0 12 24 21:58 file1
 -rw-r--r--  1 lee  staff    0 12 24 21:58 file2
 -rw-r--r--  1 lee  staff    0 12 24 21:58 file3
 
 testDir/Dir1:
 total 0
 -rw-r--r--  1 lee  staff  0 12 24 21:58 file1.1
 -rw-r--r--  1 lee  staff  0 12 24 21:58 file1.2
 
 testDir/Dir2:
 total 0
 -rw-r--r--  1 lee  staff  0 12 24 21:58 file2.1
 
 testDir/Dir3:
 
 
 */

//三个字符串拼接
char *join(char *string1, char *string2, char *string3)
{
    char *result = (char*)malloc(strlen(string1) + strlen(string2) + strlen(string3) + 1);
    strcpy(result, string1);
    strcat(result, string2);
    strcat(result, string3);
    
    return result;
}

//文件权限信息
char *permission(struct stat entry)
{
    char *result = (char*)malloc(10 * sizeof(char));
    
    //文件类型
    //if ( (entry.st_mode & S_IFREG) == S_IFREG) {
        result[0] = '_';
    //}
    if ( (entry.st_mode & S_IFLNK) == S_IFLNK) {
        result[0] = 's';
    }
    if ( (entry.st_mode & S_IFBLK) == S_IFBLK) {
        result[0] = 'b';
    }
    if ( (entry.st_mode & S_IFDIR) == S_IFDIR) {
        result[0] = 'd';
    }
    
    //文件所有者的权限
    if( (entry.st_mode & S_IRUSR) == S_IRUSR )
        result[1] = 'r';
    else
        result[1] = '_';
    
    if( (entry.st_mode & S_IWUSR) == S_IWUSR )
        result[2] = 'w';
    else
        result[2] = '_';
    
    if( (entry.st_mode & S_IXUSR) == S_IXUSR )
        result[3] = 'x';
    else
        result[3] = '_';
    
    //文件用户组的权限
    if( (entry.st_mode & S_IRGRP) == S_IRGRP )
        result[4] = 'r';
    else
        result[4] = '_';
    
    if( (entry.st_mode & S_IWGRP) == S_IWGRP )
        result[5] = 'w';
    else
        result[5] = '_';
    
    if( (entry.st_mode & S_IXGRP) == S_IXGRP )
        result[6] = 'x';
    else
        result[6] = '_';
    
    //其他用户的权限
    if( (entry.st_mode & S_IROTH) == S_IROTH )
        result[7] = 'r';
    else
        result[7] = '_';
    
    if( (entry.st_mode & S_IWOTH) == S_IWOTH )
        result[8] = 'w';
    else
        result[8] = '_';
    
    if( (entry.st_mode & S_IXOTH) == S_IXOTH )
        result[9] = 'x';
    else
        result[9] = '_';
    
    return result;
}

//返回空格
char *blank(int i){
    char *result = (char*)malloc(4*i*sizeof(char));
    
    int f = 0;
    for (;f<4*i;f+=4) {
        result[f] = ' ';
        result[f+1] = ' ';
        result[f+2] = ' ';
        result[f+3] = ' ';
    }
    
    return result;
}


void printdir(char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    
    if((dp = opendir(dir)) == NULL){
        perror("open directory meets error\n");
        exit(0);
    }
    
    chdir(dir);
    
    while((entry = readdir(dp))){
        
        char *newDir = join(dir, "/", entry->d_name);
        lstat(newDir,&statbuf);
        
        
        if (S_ISDIR(statbuf.st_mode)) {
            
            if ( !strncmp(entry->d_name,".",strlen(entry->d_name)) || !strncmp(entry->d_name,"..",strlen(entry->d_name)) )
                continue;
            long year = statbuf.st_ctime / 31536000 + 1969 ;
            printf("%s%s %d %d %lld %ld %s \n",blank(depth),permission(statbuf),statbuf.st_uid,statbuf.st_gid,statbuf.st_size,year,entry->d_name);
            //   printf("sss%ssss",blank(depth));
            printdir(newDir, depth+1);
            
        }else{
            //非常粗略的年份
            long year = statbuf.st_ctime / 31536000 + 1969 ;
            //依次打印出权限、UID、GID、size、修改年份、文件名
            printf("%s%s %d %d %lld %ld %s \n",blank(depth),permission(statbuf),statbuf.st_uid,statbuf.st_gid,statbuf.st_size,year,entry->d_name);
        }
        free(newDir);
        
        if(depth == 0) printf("\n");
    }
    
    chdir("..");
    closedir(dp);
}

//由当前目录输出
int main(int argc, const char * argv[])
{
    char *dir = (char*)malloc(1000*sizeof(char));
    getwd(dir);
    printdir(dir,0);
    free(dir);
    return 0;
}













