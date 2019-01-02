#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ftw.h>
/*#include<stdlib.h>
#include<fcntl.h>
#include<time.h>
#include<queue>*/
using namespace std;

void pwd();
void list();
void cd();
void makedir();
void removedir();
void rname();
void find();
void findfile(char *buf, char *dirname, char *filename);

int main()
{
	cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
	cout<<">>>>>>>>>Welcome to Linjunhao CMD<<<<<<<<<"<<endl;
	cout<<"***You can use the commands as follows:***"<<endl;
	cout<<"1. pwd "<<endl;
	cout<<"2. list <dirname>"<<endl;
	cout<<"3. cd <dirname or path> "<<endl;
	cout<<"4. mkdir <dirname> "<<endl;
	cout<<"5. rmdir <dirname> "<<endl;
	cout<<"6. rename <old filename> <new filename> "<<endl;
	cout<<"7. find <dir> -name <filename> "<<endl;
	cout<<"8. exit "<<endl;
	cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
 
	string str;
	while(str != "exit") {
		cout<<"[linjunhao@]$";
		cin>>str;
		if(str == "exit") {
			continue;
		}     
		if(str == "pwd") {
			pwd();
			continue;
		}
		if(str == "list") {
			list();
			continue;
		}
		if(str == "cd") {
			cd();
			continue;
		}
		if(str == "mkdir") {
			makedir();
			continue;
		}
		if(str == "rmdir") {
			removedir();
			continue;
		}
		if(str == "rename") {
			rname();
			continue;
		}
		if(str == "find") {
			find();
			continue;
		}
		cout << str << ": command not found" << endl;
	}
	return 0;
}

/**
*输出当前所在目录的路径名
**/
void pwd()
{
	char buf[100];
	getcwd(buf,sizeof(buf));
	cout << buf << endl;
}

/**
*1.查看当前目录下的所有目录及文件
*2.查看当前目录下的子目录的所有目录及文件，如果是文件则打印提示信息
**/
void list()
{
	DIR * dir;
	struct dirent * dp;
	int count = 0;
	char * dirname = new char[50];
	cin >> dirname;
	dir = opendir(dirname);
	if(dir == NULL) {
		cout << "Can not open directory" << endl;
	} else {
		while((dp = readdir(dir)) != NULL) {
			if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {	
			} else { 
				cout << dp->d_name << " ";
			}
			count++;
			if(count % 6 == 0) cout << endl;
		}
	}
	closedir(dir);
	cout << endl;
}

/**
*改变当前工作目录
**/
void cd()
{
	char dirname[50];
	cin >> dirname;
	if(chdir(dirname) == -1) {
		cout << "change directory failed!!!" << endl;
	} else {
		cout << "change directory successful" << endl;
	}
}

/**
*新建一个目录
**/
void makedir()
{
	char dirname[30];
	cin >> dirname;
	if(mkdir(dirname, S_IRWXU) == 0) {//00700权限，代表该文件所有者拥有读，写和执行操作的权限
		cout << dirname << ": create successful" << endl;
	} else {
		cout << dirname << ": create failed" << endl;
	}
}

/**
*删除空目录，非空目录会删除失败
**/
void removedir()
{
	char dirname[30];
	cin >> dirname;
	if (rmdir(dirname) == 0) {
		cout << dirname << ": delete successful" << endl;
	} else {
		cout << dirname << ": delete failed" << endl;
	}
}

/**
*重命名一个文件或目录
**/
void rname()
{
	char oldname[30], newname[30];
	cin >> oldname >> newname;
	if (rename(oldname, newname) ==0) {
		cout << oldname << " suceess change to " << newname << endl;
	} else {
		cout << oldname << " failed change to " << newname << endl;
	}
}

/**
*在指定的目录及其子目录中查找指定的文件，并输出查找到的文件的绝对路径
**/
void find()
{
	char dirname[30], filename[30];
	string str;
	cin >> dirname;
	cin >> str >> filename;
	if ("-name" != str) {
		cout << str << ": command not found" << endl;
		return;
	}
	char buf[100];
	getcwd(buf,sizeof(buf));
	if (strcmp(dirname, ".") != 0) {
		strcat(buf, "/");
		strcat(buf, dirname);
	}
	findfile(buf, dirname, filename);
}

/**
*查找
**/
void findfile(char *buf, char *dirname, char *filename) {
	DIR * dir;
	struct dirent * dp;
	struct stat st;
	char path[100];
	strcpy(path, buf);
	if(0 > stat(buf, &st)) {		//获取目录失败直接返回
		return ;
	}
	if(S_ISREG(st.st_mode)) {		//普通文件则比较
		if (strcmp(dirname, filename) == 0) {
			cout << buf << endl;
		}

	} else {
		
		if (S_ISDIR(st.st_mode)) {	//目录文件，递归查找
			dir = opendir(buf);
			if(dir == NULL) {
				cout << "can not open document" << endl;
				return;
			}
			while((dp = readdir(dir)) != NULL){
				if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) { //剔除“.”和“..”
					continue;
				}
				strcpy(buf, path);
				strcat(buf, "/");
				strcat(buf, dp->d_name); //保存路径
				findfile(buf, dp->d_name, filename); //递归
			}
			closedir(dir);
		}
	}
}


