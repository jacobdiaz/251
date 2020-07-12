
#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H

#include <vector>
#include <iostream>

using std::vector;



class GridWorld{
private:
    class person{
    public:
        int id;
        int row;
        int col;
        person* next;
        person* prev;
        person(int id, int row, int col); //constructor
    };

    class district{
        person* head, * tail;
        int len; //length of the list
    public:
        district(); //constructor
        person* push_last(int id, int row, int col); //push a person to the end of the district (=new person)
        person* pop_first(); //getting the first person (= oldest person)
        void removePerson(person* personToRemove); //removes a person by it's person class
        bool isEmpty(); //returns if the district is empty
        int size(); //returns length of the list (= total persons)
        //void print();
        std::vector<int>* people(); //getting all the persons in the district
        ~district(); //destructor
    } retire_q; //this queue is used for tracking the latest available id (after death) to insert. If person id die in the order (5,1,7). Then the next id will be 5 for birth.

    unsigned nrows; //total rows of grid
    unsigned ncols; //total cols of grid
    int count_pop; //counting the total population
    std::vector<std::vector<district*> > grid; //2d vector of district. (0,0) is district, (0,1) is district so on.....
    std::vector<person*> populationInfo; //tracks the id vs person info. If a person with id 5, resides in (0,3) district at pointer=0x3234 then, populationInfo[5] = 0x3234.
                                        
public:
    GridWorld(unsigned nrows, unsigned ncols);
    bool birth(int row, int col, int &id);
    bool death(int id);
    bool move(int personID, int targetRow, int targetCol);
    int population()const;
    int population(int row, int col)const;
    bool whereis(int personID, int &row, int &col)const;
    std::vector<int> * members(int row, int col)const;
    int num_rows()const;
    int num_cols()const;
    ~GridWorld();
};

GridWorld::person::person(int id, int row, int col) {
    this->id = id;
    this->row = row;
    this->col = col; //here all of them are inilization
    this->next = NULL;
    this->prev = NULL;
}

GridWorld::district::district() {
    this->head = NULL;
    this->tail = NULL;
    this->len = 0;
}

GridWorld::person* GridWorld::district::push_last(int id, int row, int col) {
    if (head == NULL) { //if there is no person, then the new push is the first person (= head)
        head = new person(id, row, col);
        tail = head;
        len++;
        return head;
    }
    else {
        person* temp = new person(id, row, col);
        temp->prev = tail; //updating prev of new node
        tail->next = temp; //updating next of current last

        tail = temp; //new node is the tail node
        len++;
        return tail;
    }

}

GridWorld::person* GridWorld::district::pop_first() {
    if (head == NULL) {
        return NULL;
    }
    else {
        //push last visualization
        //     head:0x4              0x8
        // NULL < 5 > 0x8  ...  0x4 < 9 > 0x9 ... more
        //       0x4               head:0x8
        // NULL < 5 > 0x8  ...  0x4 < 9 > 0x9 ... more
        //    delete:0x4             0x8
        // NULL < 5 > NULL ...  0x4 < 9 > 0x9 ... more
        //                           0x8
        //      deleted    ...  0x4 < 9 > 0x9 ... more
        //                           0x8
        //                 ... NULL < 9 > 0x9 ... more
        person* oldestPerson = head;

        head = head->next;//set next of the head as the new head
        if (head == NULL) {
            //delete tail;
            tail = NULL;
        }
        else {
           // delete head->prev; //delete head
            head->prev = NULL;
        }
        len--;
        return oldestPerson;
    }
}

void GridWorld::district::removePerson(person* personToRemove) { //removes a current person by it's pointer. the pointer was saved in populationInfo
    if (personToRemove != NULL) {
        person* prevPerson = personToRemove->prev;
        person* nextPerson = personToRemove->next;
        if (prevPerson == NULL) { //no previous person (= head)
            head = nextPerson; //update head
        }
        else {
            prevPerson->next = nextPerson; //updating prev's next with the next of the current node (=skips itself)
        }
        if (nextPerson == NULL) { //no next person (= tail)
            tail = prevPerson; //update tail
        }
        else {
            nextPerson->prev = prevPerson;//updating next's prev with the prev of the current node (=skips itself)
        }
        //delete personToRemove; //delete the node
        len--;
    }
}

bool GridWorld::district::isEmpty() {
    return head == NULL;
}

int GridWorld::district::size() {
    return len;
}
/*
void GridWorld::district::print(){
    person* temp=head;
    std::cout<<"Element: ";
    while(temp){
        std::cout<<temp->id<<" ";
        temp=temp->next;
    }
    std::cout<<"\n";
}
*/
std::vector<int>* GridWorld::district::people() {
    std::vector<int>* peopleList = new std::vector<int>;
    person* temp = head; //scan from head to tail (oldest -> newest)
    while (temp) { //traversing all the person. terminates when temp==NULL
        peopleList->push_back(temp->id); //push each id into the vector
        temp = temp->next; //updating the temp with next person
    }
    return peopleList;
}

GridWorld::district::~district() { //automatically called when the program ends
    person* temp = head;
    while (temp) {
        delete temp->prev; //deleting the previous person pinter one by one. cannot delete the current as it is needed for next person.
        temp = temp->next;
    }
    delete temp; //deleting last one, when temp==NULL (loop terminates), then temp->prev==tail which isn't deleted yet.
}

GridWorld::GridWorld(unsigned nrows, unsigned ncols) {
    this->nrows = nrows;
    this->ncols = ncols;
    grid.assign(nrows, std::vector<district*>(ncols, NULL)); //initializing the districts with NULL in the grid according to the size
    count_pop = 0;
}



bool GridWorld::birth(int row, int col, int& id)
{
    if (row >= nrows || col >= ncols)
        return false;
    if (grid[row][col] == NULL) //if district is empty (=never used before)
        grid[row][col] = new district; //initialize with empty district
    if (retire_q.isEmpty()) { //if there is any dead person's id (which should be assigned with new person)
        //no dead person's id
        id = populationInfo.size(); //getting as well as setting the id for the new person
        person* newPerson = grid[row][col]->push_last(id, row, col); //add him to the desired district and get the memory location
        populationInfo.push_back(newPerson); //add the person's memory location to the vector where his id acts as the index

    }
    else {
        //there is dead person's id to assign
        person* oldestRetiredPerson = retire_q.pop_first(); //get the oldest dead person
        int oldestRetiredPersonId = oldestRetiredPerson->id; //get his id
        person* newPerson = grid[row][col]->push_last(oldestRetiredPersonId, row, col); //assign new person with that old id
        populationInfo[oldestRetiredPersonId] = newPerson; //updating the old id's memory location with the new one
        id = oldestRetiredPersonId;//setting the reference
    }
    count_pop++;
    return true;
}

bool GridWorld::death(int id) {
    if (id >= populationInfo.size()) //id not valid
        return false;
    person* personToDeath = populationInfo[id]; //directly getting the person's memory location, by his id from the populationInfo vector O(1)
    if (personToDeath == NULL) //the id is valid but the person of this id already dead
        return false;

    //id is valid and person is alive
    int row = personToDeath->row;  //getting his district info
    int col = personToDeath->col; //getting his district info

    retire_q.push_last(personToDeath->id, personToDeath->row, personToDeath->col); //add the id to the dead people's queue
    grid[row][col]->removePerson(personToDeath); //removing the person from the district
    count_pop--;
    //delete populationInfo[id]; //deleting the the memory address
    populationInfo[id] = NULL; //setting the memory address of the dead person's id to NULL
    return true;

}

bool GridWorld::move(int personID, int targetRow, int targetCol) {
    if (targetRow >= nrows || targetCol >= ncols)
        return false;
    if (personID >= populationInfo.size())
        return false;
    person* personToMove = populationInfo[personID]; //directly getting the person's memory location, by his id from the populationInfo vector O(1)
    if (personToMove == NULL)
        return false;

    int row = personToMove->row;
    int col = personToMove->col;
    grid[row][col]->removePerson(personToMove); //removing the person from the old district
    if (grid[targetRow][targetCol] == NULL) //if target district is empty (=never used before)
        grid[targetRow][targetCol] = new district;//initialize with empty district
    person* movedPerson = grid[targetRow][targetCol]->push_last(personID, targetRow, targetCol); //assign new person to the target district
    populationInfo[personID] = movedPerson; //updating the new memory location of the person
    return true;
}

int GridWorld::population()const {
    return count_pop;
}

int GridWorld::population(int row, int col)const {
    if (row >= nrows || col >= ncols || grid[row][col] == NULL)
        return 0;
    return grid[row][col]->size();
}

bool GridWorld::whereis(int personID, int& row, int& col)const {
    if (personID >= populationInfo.size())
        return false;
    person* personToGet = populationInfo[personID]; //directly getting the person's memory location, by his id from the populationInfo vector O(1)
    if (personToGet == NULL)
        return false;
    row = personToGet->row; //setting the retrieved district info to the passed reference
    col = personToGet->col; //setting the retrieved district info to the passed reference
    return true;
}
std::vector<int>* GridWorld::members(int row, int col)const {
    if (row >= nrows || col >= ncols || grid[row][col] == NULL)
        return new std::vector<int>; //returning an empty vector (size=0)

    return grid[row][col]->people();
}


int GridWorld::num_rows()const {
    return nrows;
}

int GridWorld::num_cols()const {
    return ncols;
}

GridWorld::~GridWorld() {//automatically called when the program ends
    for (int i = 0; i < nrows; i++)
        for (int j = 0; j < ncols; j++)
            delete grid[i][j]; //deleting all the district's memory address from the grid
}

#endif
