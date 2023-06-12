#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


using namespace std;

bool DEBUG = false;

struct fileInfo{
    //inital page info
    int pagesize,  numframes,  numpages, numbackingblocks,cmdindex=0;
    string indexAlg="ERR";
    //Counters
    int wdisk = 0;
    int rdisk = 0;
    int ref=0;
    int mapped=0;
    int miss=0;
    int taken =0;
    //Frame Table Itorator
    int frame_itr = 0;

    //return frame_iter at calltime the moves the iter for FIFO Reclaim alg
    int FIFO_ptr(){
        if (frame_itr<numframes){
            
            int r = frame_itr;
            frame_itr++;
            return r;

        }else{
            frame_itr = 1;
            return 0;
        }
        
    }
    

    
    void print(){
        cout<<"Page size: "<< pagesize<<"\n";
        cout<<"Num frames: "<< numframes<<"\n";
        cout<<"Num pages: "<< numpages<<"\n";
        cout<<"Num backing blocks: "<<numbackingblocks<<"\n";
        cout<<"Reclaim algorithm: "<<indexAlg<<"\n";
       
    }

    void print_counter(){
        cout<<"Pages referenced: "<< ref << endl;
        cout<<"Pages mapped: "<< mapped << endl;
        cout<<"Page misses: "<< miss << endl;
        cout<<"Frames taken: "<< taken << endl;
        cout<<"Frames written to disk: "<< wdisk << endl;
        cout<<"Frames recovered from disk: "<< rdisk << endl;
    }

    int pageIndex(int address){
        return address/pagesize;
    }

};

struct Page{

    string type = "UNMAPPED";
    int frameNum = -1;
    int onDisk = 0;
    //return the index of the old fame befor overwrite
    int onWrite(int frame_ptr){
        int old_fnum = frameNum;
        frameNum = frame_ptr;
        type = "MAPPED";
        return old_fnum;
    }
    //I know its the same as onWrite, left it this way in-case that changes in development.
    int onRead(int frame_ptr){
        int old_fnum = frameNum;
        frameNum = frame_ptr;
        type = "MAPPED";
        return old_fnum;
    }

    void Take(bool wasDirty){
        type = "TAKEN";
        frameNum = -1;
        if (wasDirty){
            onDisk = 1;
        }

    }

    void println(int index){
        if(index<10){
            cout<<"    ";
        }else{
            cout<<"   ";
        }
        if (type == "UNMAPPED"){
            cout<<index<<" type:"<<type<<endl;
        }else{ 
        cout<<index<<" type:"<<type<<" framenum:"<<frameNum<<" ondisk:"<<onDisk<<endl;
        }
    }
};

struct Frame{
    int inUse=0;
    int dirty=0;
    int firstUse=0;
    int lastUse=0;

    bool onRead(int count,bool mapped){
        if (!mapped){
            firstUse = count+1;
        }
        lastUse = count+1;
        inUse = 1;
        if(dirty == 1){
            return true;
        }else return false;
    }

    bool onWrite(int count,bool mapped){
        if (!mapped){
            firstUse = count+1;
        }
        int r = dirty;
        lastUse = count+1;
        inUse = 1;
        dirty = 1;
        if (r == 1){
            return true;
        }else return false;
    }

    void println(int index){
        if(inUse==0){
            cout<<"    "<<index<<" inuse:"<<inUse<<endl;
        }else{
            cout<<"    "<<index<<" inuse:"<<inUse<<" dirty:"<<dirty<<" first_use:"<<firstUse<<" last_use:"<<lastUse<<endl;
        }
    }
};

void print_vm(vector<Frame> frame_table,vector<Page> page_table,fileInfo file_perams){
    
    cout<<"Page Table\n";
    for(int i=0; i < file_perams.numpages; ++i){
        page_table[i].println(i);
    }
    
    cout<<"Frame Table\n";
    for(int i=0; i < file_perams.numframes; ++i){
        frame_table[i].println(i);
    }

    file_perams.print_counter();

}

int run(fileInfo file_perams,vector<string> instr){

    vector<Frame> frame_table;
    vector<Page> page_table;
    Page page;
    Frame frame;
    

    //init page table as unmapped
    for(int i=0; i < file_perams.numpages; ++i){
        page_table.push_back(page);  
    }

    //init frame table
    for(int i=0; i < file_perams.numframes; ++i){
        frame_table.push_back(frame);
    }

    //walking list of cmds ex.(print, r address , w address)
    for(int i = 1; i < instr.size(); ++i){

        int count = file_perams.ref;
        string cmd;
        int address;
        
        istringstream parse_cmd(instr[i]);

        parse_cmd >> cmd;

        

        if (DEBUG && cmd != "print"|| !DEBUG && cmd == "print"){
            print_vm(frame_table,page_table,file_perams);
        }
        if(DEBUG)cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=\ncommand: " << cmd << endl;
        if(cmd == "r"){
            file_perams.ref = count+1;
            parse_cmd >> address;
            int index = file_perams.pageIndex(address);
            if(DEBUG)cout << "address: " << address <<"  index: "<< index << "  pagesize: "<< file_perams.pagesize << endl;

            if (page_table[index].type == "MAPPED"){
                frame_table[page_table[index].frameNum].onRead(count,true);
            }else{
                

                if(page_table[index].type == "UNMAPPED"){ 
                    file_perams.mapped++;
                }
                file_perams.miss++;
                int fptr=0;
                
                if(file_perams.indexAlg == "FIFO"){
                    fptr= file_perams.FIFO_ptr();
                }

                if(file_perams.indexAlg == "LRU"){
                    int time = -1;
                    for(int j = 0; j < file_perams.numframes; j++){
                        
                        if(frame_table[j].inUse == 0){ 
                            fptr = j;
                            break;
                        }
                        int diff = file_perams.ref-frame_table[j].lastUse;
                        if(diff > time){
                            time = diff;
                            fptr = j; 
                        }
                    }
                }
                if(page_table[index].onDisk == 1){
                    file_perams.rdisk++;
                    //frame_table[fptr].dirty = 0;

                }
               

                if(DEBUG)cout<<"Frame ptr: "<< fptr<<endl;
                int old_fptr = page_table[index].onRead(fptr);
                bool is_dirty = frame_table[fptr].onRead(count,false);
                if(old_fptr == -1 && page_table[index].onDisk == 1 && is_dirty){
                    file_perams.wdisk++;
                    frame_table[fptr].dirty = 0;
                }
                for(int j = 0; j < file_perams.numpages; j++){
                    if(j!=index && page_table[j].frameNum == fptr){
                        page_table[j].Take(is_dirty);
                        file_perams.taken++;
                    } 
                }
            }

        } else if(cmd == "w"){
            file_perams.ref = count+1;
            parse_cmd >> address;
            int index = file_perams.pageIndex(address);
            if(DEBUG)cout << "address: " << address <<"  index: "<< index <<"  pagesize: "<<file_perams.pagesize<<endl;
            if (page_table[index].type == "MAPPED"){
                bool old_dirt = frame_table[page_table[index].frameNum].onWrite(count,true);
               //did help might hurt 
                if (old_dirt){
                    file_perams.wdisk++;
                    page_table[index].onDisk = 1;
                }
            }else{
                
                

                if(page_table[index].type == "UNMAPPED"){ 
                    file_perams.mapped++;
                }
                file_perams.miss++;
                int fptr=0;
                
                if(file_perams.indexAlg == "FIFO"){
                    fptr= file_perams.FIFO_ptr();
                }

                if(file_perams.indexAlg == "LRU"){
                    int time = -1;
                    for(int j = 0; j < file_perams.numframes; j++){
                        
                        if(frame_table[j].inUse == 0){ 
                            fptr = j;
                            break;
                        }
                        int diff = file_perams.ref-frame_table[j].lastUse;
                        if(diff > time){
                            time = diff;
                            fptr = j; 
                        }
                    }
                }
                
                if(DEBUG)cout<<"Frame ptr: "<< fptr<<endl;
                int old_fptr = page_table[index].onWrite(fptr);
                bool is_dirty =frame_table[fptr].onWrite(count,false);
                if(is_dirty){
                    file_perams.wdisk++;


                }
                for(int j = 0; j < file_perams.numpages; j++){
                    if(j!=index && page_table[j].frameNum == fptr){
                        page_table[j].Take(is_dirty);
                        file_perams.taken++; 
                    } 
                }
            }
        } else if(cmd != "print"){
            cout << "Command not recocnized: "<< cmd << endl;
        }
    }
     
    print_vm(frame_table,page_table,file_perams);
    

    return 0;

}




int main (int argc, char *argv[]) {

    if (argc > 4){
        cout<< "To many args!\n";
        return -1;
    }else if (argc < 2){
        cout << "missing args!\n";
        return -1;
    }

    bool backstore = false;
    int argOfset=0;
    vector<string> fileout;
    string filename;
    fileInfo fileinit;

    //intilization//////////////////////////////////////////
    string arg = argv[1];
    if(arg == "-w"){
        argOfset = 2;
        backstore = true;
    }else if(arg == "-d"){
        argOfset = 2;
        DEBUG = true;

    }else{
        //cout <<"-w/-d not found: "<<argv[1]<<endl;
        argOfset = 1;
    }

    fileinit.indexAlg = argv[argOfset];
    filename= argv[(argOfset+1)];
    
    ifstream file(filename);
    if(!file.is_open()){
        cout<<"file failed to open:"<<filename<<endl;
        return -1;
    }

    string line;
    while(getline(file,line)){
        if(line[0]!='#'){ 
            fileout.push_back(line);
            if(DEBUG){cout<<"read line from file: "<<line<<"\n"; }   //debug
        }
    }

    istringstream parseperam(fileout[0]);

    parseperam >> fileinit.pagesize;
    parseperam >> fileinit.numframes;
    parseperam >> fileinit.numpages;
    parseperam >> fileinit.numbackingblocks;

    fileinit.print();
    //just call it run to reduce repeat code 
    run(fileinit,fileout);


}