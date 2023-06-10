#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>



using namespace std;

#define ITYPE_IS(MODE,TYPE) ((MODE & IFMT) == (TYPE))


int debug = 0;
int debug_file = 0;
int sifter_debug = 0;



struct filter{
    string f_type="";
    string f_arg = "";
    void mk(string filter, string arg){
        f_type = filter;
        f_arg = arg;
    }
};

struct file{
    string path;
    string name;
    int size;
    string t = "u";
    unsigned nlink;


    void mk(string p, string n,int s, string ty){
        path = p;
        name = n;
        size = s;
        t = ty;
    }

    bool is_larger(int n){
        return size>n;
    }

    bool size_eq(int n){
        if(sifter_debug) cout<<n<< "=="<<size<<endl;
        return size==n;
    }

    bool is_smaller(int n){
        return size<n;
    }

    void adoption(string child){
        path = path +"/"+ child;
    }

    bool comp_blocks(int n_blocks){
        int BLOCKSIZE = 512;
        if(size%BLOCKSIZE == 0){
            return size/BLOCKSIZE == n_blocks;

        } else{
            return ((size/BLOCKSIZE)+1 == n_blocks); 
        }
    }
    bool comp_blocks_lesser(int n_blocks){
        int BLOCKSIZE = 512;
        if(size%BLOCKSIZE == 0){
            return size/BLOCKSIZE < n_blocks;

        } else{
            return ((size/BLOCKSIZE)+1 < n_blocks); 
        }
    }

    bool comp_blocks_greater(int n_blocks){
        int BLOCKSIZE = 512;
        if(size%BLOCKSIZE == 0){
            return size/BLOCKSIZE > n_blocks;

        } else{
            return ((size/BLOCKSIZE)+1 > n_blocks); 
        }
    }



};

const int MAXFILTER = 8;

bool filter_arg_check(string filter_tk, string arg);
void print_dir(vector<file> arry);
int dir_maker(DIR *dir,string path);
int fir_sifter(filter filters[MAXFILTER],int n_filters);



vector<file> file_vec;



int main(int argc,char *argv[])
{
  if (argc < 2) {
        cerr << "Not enough arguments\n";
        exit(-1);
    }
    if (argc > 8){
        cout<< "Too many args";
        exit(-1);
    }

    int f_count = 0;
    DIR *dir;
    filter filterAr[MAXFILTER];
    string path = argv[1];
    
    
// Creat a DIR from argv[1] 
    if ((dir = opendir(argv[1])) == NULL) {
        perror(argv[1]);
        exit(-1);
    }

//Loads filter, args into filter struct
    if(argc > 2){      
        for (int i =2; i<argc;i=i+2){

            string arg ="";
            
            {if(argv[i+1]!=nullptr)
                arg = argv[i+1];
            }
            string filter_tk = argv[i];
            
            if (debug) cout<<"Filter <arg>: " << filter_tk <<": "<< arg<< endl;
            
            if(filter_arg_check(filter_tk,arg)){
                filterAr[f_count].mk(filter_tk,arg);
                f_count++;
            }else{
                exit(-1);
            }
        }
    } 

    if(debug){
        for(int i =0; i < f_count; i++ ){
            cout << "Filer name:"<<filterAr[i].f_type<<"  Filer arg:"<< filterAr[i].f_arg<<endl;
        }
    }
/*Now we have:
    Gloable: file_vec
    && local:
    sting path
    array of filters
    DIR dir at root
*/
    //pushing root to file struct Vec
    file root;
    root.mk(path,".",0,"d");
    file_vec.push_back(root);
    if(debug) cout<<"Root dir :Push-> '.' at" <<path<<endl;

    //Filling in file Vec with all directory and the children
    dir_maker(dir,path);

    fir_sifter(filterAr,f_count);
    
    print_dir(file_vec);

    if (closedir(dir) == -1) {
        perror("closedir");
        exit(-2);
    }

    exit(0);
}


bool filter_arg_check(string filter_tk, string arg){
     if (debug) cout<< "argument found: "<< arg<< endl;
    if (filter_tk == "-name"){
        if(arg == "-type" || arg == "-size" || arg == ""){
           
            cout<<"-name requires argument"<<endl;
            return false;
        }
        return true;
    }else if(filter_tk == "-type") {
        if (arg != "f" && arg != "d" && arg != "l"){
            cout<<"-type requires argument"<<endl;
            return false;
        }
        return true;
    }else if(filter_tk == "-size"){
        if(arg == "-type" || arg == "-name" || arg == ""){
            cout<<"-size requires argument"<<endl;
            return false;
        }
        return true;
    
    }else{
        cout<<"Unknown filter: "<< filter_tk <<endl;
        return false;
    }
}



void print_dir(vector<file> arry){

    for (long unsigned int i=0; i<arry.size();i++){
        if(debug_file){
            cout<< "File["<<i<<"]\n";
            cout<< arry[i].name<<" | "<<  arry[i].size<<" | "<< arry[i].t<<" | "<< arry[i].nlink<<endl;
        }
        cout<< arry[i].path<<endl;
    } 

}

//recusive call that loads all subdir into global Vec of files structs
int dir_maker(DIR *dir,string path){
        struct dirent *dirInfo;
        

       
        while((dirInfo = readdir(dir)) != NULL) {
            struct stat filestat;
            string file_name = dirInfo->d_name;
            string type = "u";
            string full_path = path +"/"+ file_name;

            int ret = lstat(full_path.c_str(),&filestat);
            if(ret != 0){
                if(debug_file) cout<< "lstat called on: "<< full_path<<endl;
                int try2 = stat(full_path.c_str(),&filestat);
                if (try2 != 0){
                cout<<"\nopps stat failure\n"<<endl;
                return(-1);
                }
            }
            int ftype = filestat.st_mode & S_IFMT;

            

            
            if(debug){
                printf("File: %s  <%s>\n",
	            file_name.c_str(),
	            (ftype == S_IFIFO)?"fifo":
	            (ftype == S_IFCHR)?"character special":
	            (ftype == S_IFDIR)?"directory":
	            (ftype == S_IFBLK)?"block special":
	            (ftype == S_IFREG)?"regular file":
	            (ftype == S_IFLNK)?"symbolic link":
	            (ftype == S_IFSOCK)?"socket":
	             "UNKNOWN/OTHER");
            }

            //size of file in bytes
            int size = filestat.st_size;
            //files types that we are filtering for
            
            switch (ftype){
                case S_IFDIR:
                    type = 'd';
                    break;
                case S_IFREG:
                    type = 'f';
                    break;
                case S_IFLNK:
                    type = 'l';
                    break;
                default:
                    break;
            }


            if (path == "."){
                file tmp;
                tmp.mk(path,".",size,type);
                file_vec.push_back(tmp);
                if(debug) cout<<"root of curent path in dir_maker :Push-> '.' at" <<path<<endl;
                tmp.nlink = filestat.st_nlink;
            }    
            if(file_name != ".."&& file_name != "."){
                file tmp;

                tmp.mk(path,file_name,size,type);
                tmp.adoption(file_name);
                file_vec.push_back(tmp);

                if(debug) cout << "Pushed-> " <<tmp.name << "\n----> Next dir is "<<tmp.path<<endl;
                
                DIR *subdir = opendir(tmp.path.c_str());
                if(subdir != NULL) dir_maker(subdir,tmp.path);
            }
        

    }
    return 0;
}

int fir_sifter(filter filters[MAXFILTER],int n_filters){
    
    for (int i = 0; i < n_filters; i++){
        if(sifter_debug){ 
            cout<<"filter: "<< filters[i].f_type<<endl;
            cout<<"filter Arg: "<< filters[i].f_arg<<endl;
        }

        char tk = filters[i].f_type[1];
        vector<file> tmp;
        switch (tk){
            case 'n':{
                
                for (long unsigned int j=0; j < file_vec.size();j++){
                    if (sifter_debug) cout<< "Compare arg with name: "<< file_vec[j].name<<endl;
                    if(filters[i].f_arg == file_vec[j].name){
                        tmp.push_back(file_vec[j]);
                    }
                }
                file_vec = tmp;
                break;
            }
            case 't':{
                for (long unsigned int j=0; j < file_vec.size();j++){
                    if (sifter_debug) cout<< "Compare arg with type: "<< file_vec[j].t<<endl;
                    if(filters[i].f_arg == file_vec[j].t ){
                        
                            tmp.push_back(file_vec[j]);
                    
                    }
                }
                file_vec=tmp;

                break;
            }    
            case 's':{
                char operand = filters[i].f_arg[0];
                if(sifter_debug) cout<< "Operand: "<< operand<< endl;
                char unit = filters[i].f_arg.back();
                if(sifter_debug) cout<< "Units: "<< unit << endl;

                for (long unsigned int j=0; j < file_vec.size();j++){
                    if (sifter_debug) cout<< "Compare size in bytes:" << file_vec[j].size<<endl;
                    string num;
                    switch (operand)
                    {
                    case '+':{
                        
                        if(unit == 'c'){
                            num = filters[i].f_arg.substr(1,filters[i].f_arg.size()-1);
                            int n = stoi(num);
                            if(file_vec[j].is_larger(n)){
                                tmp.push_back(file_vec[j]);
                            }
                        
                        }else{
                            num = filters[i].f_arg.substr(1,filters[i].f_arg.size());
                            int n = stoi(num);
                            if(file_vec[j].comp_blocks_greater(n)){
                                tmp.push_back(file_vec[j]);
                            }
                        }
                        break;
                    }
                    case '-':{
        
                        if(unit == 'c'){
                            num = filters[i].f_arg.substr(1,filters[i].f_arg.size()-1);
                            int n = stoi(num);
                            if(file_vec[j].is_smaller(n)){
                                tmp.push_back(file_vec[j]);
                            }
                            
                        }else{
                            num = filters[i].f_arg.substr(1,filters[i].f_arg.size());
                            int n = stoi(num);
                            if(file_vec[j].comp_blocks_lesser(n)){
                                tmp.push_back(file_vec[j]);
                            }
                        }
                        break;
                    }    
                    default:{
                    if(unit == 'c'){
                            num = filters[i].f_arg.substr(0,filters[i].f_arg.size()-1);
                            int n = stoi(num);
                            if(file_vec[j].size_eq(n)){
                                tmp.push_back(file_vec[j]);
                            }
                            
                        }else{
                            num = filters[i].f_arg.substr(0,filters[i].f_arg.size());
                            int n = stoi(num);
                            if(file_vec[j].comp_blocks(n)){
                                tmp.push_back(file_vec[j]);
                            }
                        }

                        break;
                    }

                    }
                
                
                }
                file_vec = tmp;
                break;
            }    
        
            default:{
                cout<<"\n!!Error!!\ntype invald found char token: "<< tk<< endl;
                break;
            }
        }
        


    }
    

    
    return 0;
}