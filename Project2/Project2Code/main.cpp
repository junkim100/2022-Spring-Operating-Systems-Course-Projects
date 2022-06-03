#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

//Global Variable
bool debug = false;

struct pager
{
    string address;
    unsigned int pageNum; //unsigned int instead of an int because address's value can be above the max for ints
    char rw;
};

//Checks if the page number for pager is present in vec.
//Returns the index in vec if it's present. -1 if it isn't present
int isInDeque(deque<pager> &vec, pager &pge) {
    for(int i = 0; i < vec.size(); i++) {
        if(vec.at(i).pageNum == pge.pageNum)
            return i;
    }
    return -1;
}

/* FIFO Algorithm (Taken from the slides of the student presentation on March 1st)
Three cases
1. Page is not in table and table is full
    -Pop front of vec, push_back page to vec
2. Page is in table
    -Update write for page in table if 'W' encountered.
3. Page is not in table and table is not full
    -Add new page to back of table */
vector<int> fifo(vector<pager> &vec, int numFrames) {
    if(debug)
        printf("Running fifo()\n");
    
    int event = 0, read = 0, write = 0, size = 0, hit = 0;
    vector<int> result;
    deque<pager> table;

    for(pager i : vec) {
        event++;
        int index = isInDeque(table, i);

        if(index == -1 && size >= numFrames) {  //Case 1, page is not in the table, table is full
            if(table.at(0).rw == 'W')
                write++;    //increment write if the to be removed page was "dirty"
            read++;         //increment read either way

            table.pop_front(); //remove the first element
            table.push_back(i); //inserts page i
        }
        else if(index != -1) {  //Case 2, page is in table
            if(table.at(index).rw == 'R' && i.rw == 'W') //If we are writing to the page, update it from read to write
                table.at(index).rw = 'W';
            
            hit++;
        }
        else if(index == -1 && size < numFrames) { //Case 3, page is not in the table, table is not full
            table.push_back(i); //Insert then increment read/size
            read++;
            size++;
        }
    }

    result.push_back(numFrames); result.push_back(event); result.push_back(read); result.push_back(write); result.push_back(hit);
    return result;
}

/* LRU Algorithm (Taken from the slides of the student presentation on March 1st)
Three cases
1. Page is not in table and table is full
    -Pop the first page in the table (the least recently used) and insert the new page to the back
2. Page is in table
    -Update write for page in table if 'W' encountered. Then delete page. Then push page to back of table
3. Page is not in table and table is not full
    -Add new page to the back of the table  */
vector<int> lru(vector<pager> &vec, int numFrames) {
    if(debug)
        printf("Running LRU()\n");
    
    int event = 0, read = 0, write = 0, size = 0, hit = 0;
    vector<int> result;
    deque<pager> table;

    for(pager i : vec) {
        event++;
        int index = isInDeque(table, i);

        if(index == -1 && size >= numFrames) {  //Case 1, page is not in the table, table is full
            if(table.front().rw == 'W')
                write++;    //increment write if the to be removed page was "dirty"
            read++;         //increment read either way

            table.pop_front(); //remove the first element
            table.push_back(i); //inserts page i
        }
        else if(index != -1) {  //Case 2, page is in table
            if(table.at(index).rw == 'R' && i.rw == 'W') //If we are writing to the page, update it from read to write
                table.at(index).rw = 'W';

            table.push_back(table.at(index)); //Reinsert it as the newest
            table.erase(table.begin() + index);//Remove the original (uses an iterator which is why I add index)

            hit++;
        }
        else if(index == -1 && size < numFrames) { //Case 3, page is not in the table, table is not full
            table.push_back(i); //Insert then increment read/size
            read++;
            size++;
        }
    }

    result.push_back(numFrames); result.push_back(event); result.push_back(read); result.push_back(write); result.push_back(hit);
    return result;
}

/* Segmented FIFO Algorithm (Taken from the slides of the student presentation on March 1st)
Five cases
1. Page is in FIFO table
    -Update 'W' bit if necessary
2. Page is in LRU (FIFO is full)
    -Update 'W' bit if necessary, move page to back of FIFO and move front of FIFO to LRU.
3. Page is not in FIFO, FIFO is full, page is not in LRU, and LRU is full
    -Eject oldest LRU page. Move front of FIFO to LRU. Add new page to back of FIFO.
4. Page is not in FIFO and FIFO is not full
    -Add page to back of FIFO, increment FIFO size.
5. Page is not in FIFO, FIFO is full, and LRU is not full
    -Eject front of FIFO. Put ejected page into LRU. Add page to FIFO
*/
vector<int> vms(vector<pager> &vec, int numFrames, int p) {
    int event = 0, read = 0, write = 0, hit = 0;
    vector<int> result;

    deque<pager> lruTable; //Secondary cache
    int lruMaxSize = (numFrames * p) / 100;
    int lruSize = 0, lruIndex = 0;

    deque<pager> fifoTable; //Primary cache
    int fifoMaxSize = numFrames - lruMaxSize;
    int fifoSize = 0, fifoIndex = 0;

    int whichCase = 0; //debug info
    int cases[6] = {0, 0, 0, 0, 0, 0};

    if(debug)
        printf("Running vms()\n");
    
    if(lruMaxSize == 0) { //Do FIFO instead
        if(debug)
            printf("vms()'s LRU table has a max size of 0. Running fifo() insead.\n");
        return fifo(vec, numFrames);
    }
    else if(fifoMaxSize == 0) { //Do LRU instead
        if(debug)
            printf("vms()'s FIFO table has a max size of 0. Running lru() insead.\n");
        return lru(vec, numFrames);
    }

    for(pager i : vec) {
        event++;
        fifoIndex = isInDeque(fifoTable, i);
        lruIndex = isInDeque(lruTable, i);
        whichCase = 0; //debug info
        
        if(fifoIndex != -1) { //Case 1, page i is in FIFO table
            if(fifoTable.at(fifoIndex).rw == 'R' && i.rw == 'W') //If we are writing to page i, update it from read to write
                fifoTable.at(fifoIndex).rw = 'W';
            
            whichCase = 1; //debug info

            hit++;
        }
        else if(lruIndex != -1) { //Case 2, page i is in LRU table
            if(lruTable.at(lruIndex).rw == 'R' && i.rw == 'W') //If we are writing to page i, update it from read to write
                lruTable.at(lruIndex).rw = 'W';

            lruTable.push_back(fifoTable.at(0)); //Move front of FIFO table to LRU table
            fifoTable.pop_front();
            
            fifoTable.push_back(lruTable.at(lruIndex)); //Move page i from LRU table to back of FIFO
            lruTable.erase(lruTable.begin() + lruIndex);
            
            whichCase = 2; //debug info

            hit++;
        }
        else if(lruIndex + fifoIndex == -2 && fifoSize + lruSize == fifoMaxSize + lruMaxSize) { //Case 3, page is not in FIFO or LRU table. Both tables are full
            if(lruTable.front().rw == 'W') //Update write if we are "writing" to disk
                write++;
            read++; //increment read either way
            
            lruTable.pop_front(); //Eject oldest LRU page

            lruTable.push_back(fifoTable.front()); //Move front of FIFO to LRU
            fifoTable.pop_front();

            fifoTable.push_back(i); //Add new page to back of FIFO.

            whichCase = 3; //debug info
        }
        else if(fifoIndex == -1 && fifoSize < fifoMaxSize) { //Case 4, page is not in FIFO and FIFO is not full
            //Add page to back of FIFO, increment FIFO size.
            fifoTable.push_back(i);
            fifoSize++;
            read++;

            whichCase = 4; //debug info
        }
        else if(fifoIndex == -1 && fifoSize == fifoMaxSize && lruSize < lruMaxSize) { //Case 5, page is not in FIFO, FIFO is full, and LRU is not full
            lruTable.push_back(fifoTable.at(0)); //Eject front of FIFO. Put ejected page into LRU.
            fifoTable.pop_front();
            
            fifoTable.push_back(i); //Add page i to FIFO.
            lruSize++;
            read++;

            whichCase = 5; //debug info
        }

        if(debug) {
            printf("Event: %d, Case: %d\n", event, whichCase);
            cases[whichCase]++;
        }
    }

    if(debug) {
        int sum = 0;
        printf("FIFO table max size: %d", fifoMaxSize);
        printf("LRU table max size: %d", lruMaxSize);
        
        printf("Count of each case:\n");
        for(int i = 0; i < 6; i++) {
            printf("Case %d: %d\n", i, cases[i]);
            sum += cases[i];
        }
        printf("Count for all cases: %d\n", sum);
        printf("Note: case 4 and case 5 count should match FIFO table max size and LRU table max size respectively.\nCount for all cases should match Trace Count.\n");
    }

    result.push_back(numFrames); result.push_back(event); result.push_back(read); result.push_back(write); result.push_back(hit);
    return result;
}

/*  argc = size of argv
    argv = array of strings where each string is something that was entered in command line.
    ex:       ./memsim   tracefile       nframes          <lru | fifo | vms>     <debug | quiet>
    argv index:  [0]      [1]             [2]                   [3]                   [4]
    argc = 5
    So tracefile is argv[1]. nframes is argv[2], etc through the end. (if argv[3] is vms, argv[4] is P and debug/quiet is argv[5] */
int main(int argc, char *argv[])
{
    // get debug/quiet (for printing debug purposes)
    string debugOrNot = "";
    string memMethod = "";
    string fileName = "";
    int numFrames = 0;
    int p = 0;
    fstream traceFile;
    vector <pager> vec;
    vector<int> result;

    // Get debug value
    if (argc == 5) // argv[3] is not vms so debug/quiet is argv[4]
        debugOrNot = argv[4];
    else // argv[3] is vms so debug/quiet is argv[5]
        debugOrNot = argv[5];

    if(debugOrNot.compare("debug") == 0) //For any other string just don't print
        debug = true;

    if (debug)
        printf("debug/quiet = %s\n", debugOrNot.c_str());


    // Attempt to open the trace file (should be argv[1])
    fileName = argv[1];
    if (debug)
        printf("File name = %s\n", fileName.c_str());

    traceFile.open(fileName, ios::in);
    if (!traceFile) {
        printf("Error, file: %s not opened.\n", fileName.c_str());
        printf("Terminating program...\n");
        return 0;
    }
    else if (debug)
        printf("File opened successfully.\nReading from trace file: %s\n", fileName.c_str());
    
    // Read from trace file
    string line;
    while (getline(traceFile, line)) {
        pager p;

        p.address = line.substr(0,8);       //Get the address
        p.rw = line[9];                     //Get read/write character
        p.pageNum = stoull(p.address, 0, 16)/4096;   //Get page number

        vec.push_back(p);   //Add to vector
        if(debug)
            cout << p.address << ' ' << p.rw << ' ' << p.pageNum << endl;
    }

    // Get numFrames
    numFrames = atoi(argv[2]);
    if (debug)
        printf("numFrames = %d\n", numFrames);

    // Get memory sim method
    memMethod = argv[3];
    if (debug)
        printf("memory function = %s\n", memMethod.c_str());

    //Run mem sim function depending on method
    if (memMethod.compare("vms") == 0) {
        // Get P if mem sim method is vms
        p = atoi(argv[4]);

        if (debug)
            printf("p = %d\n", p);

        if (p < 0 || p > 100) {
            if(debug)
                printf("Warning, p's value is: %d.\nThis is not between 0 and 100 (inclusive). Setting p to the default value of 50.\n", p);
            p = 50;
        }

        result = vms(vec, numFrames, p);
    }
    else if (memMethod.compare("fifo") == 0)
        result = fifo(vec, numFrames);
    else if (memMethod.compare("lru") == 0)
        result = lru(vec, numFrames);
    else {
        if(debug)
            printf("Warning, memsim function is not vms, fifo, or lru. Defaulting to fifo.");
        result = fifo(vec, numFrames);
    }

    float hitrate = ((float)result.at(4)/result.at(1))*100;

    printf("Total Memory Frames: %d\nEvents in Trace: %d\nTotal Disk Reads: %d\nTotal Disk Writes: %d\nHit rate: %f\n", result.at(0), result.at(1), result.at(2), result.at(3), hitrate);

    return 0;
}
