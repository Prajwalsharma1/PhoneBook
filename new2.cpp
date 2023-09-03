#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;
# define long long long

class TrieNode{
    public:
    unordered_set<string>*Data;
    bool isEnd;
    TrieNode**Children = new TrieNode*[100]();
    TrieNode(){isEnd=false;Data=NULL;}
    ~TrieNode(){
        for(int i=0;i<98;i++){
            delete Children[i];
        }
        delete []Children;
    }
};

class Trie{
    TrieNode *Root = NULL;

    // constructor >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    public: Trie(){
        Root = new TrieNode(); 
    }

    // Inserting a element >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: void insert(string &name,int i, TrieNode* root,string &number){
        if(i==name.size()){
            root->isEnd=true;
            if(number!=""){
                if(root->Data==NULL){
                    root->Data=new unordered_set<string>();
                }
                root->Data->insert(number);
            }
            return ;
        }

        TrieNode* &child = root->Children[name[i]-30];
        if(child==NULL){
            child = new TrieNode();
        }
        insert(name,i+1,child ,number); 
    }
 
    public: void insert(string &name,string &number){
        insert(name,0,Root,number);
    }

    // Inserting a set of Numbers  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: void insertNumbers(string &name,int i, TrieNode* root,unordered_set<string>* number){
        if(i==name.size()){
            root->isEnd=true;
            root->Data=number;
            return ;
        }

        TrieNode* &child = root->Children[name[i]-30];
        if(child==NULL){
            child = new TrieNode();
        }
        insertNumbers(name,i+1,child ,number); 
    }

    public: void insertNumbers(string &name,unordered_set<string>*number){
        insertNumbers(name,0,Root,number);
    }
    
    /// searching >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: bool isThere(string &s,int i,TrieNode*root){
        if(i==s.size()){
            return root->isEnd;
        }

        TrieNode* &child = root->Children[s[i]-30];
        if(child==NULL){
            return false;
        }   
        else{
            return isThere(s,i+1,child); 
        }
    }
    
    public: bool isThere(string &s ){
        return isThere(s,0,Root);
    }

    // getNumbers >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: unordered_set<string>* getNumber(string &s,int i,TrieNode*root){
        if(i==s.size()){
            if(root->isEnd){
                return root->Data;
            }
            else return NULL;
        }

        TrieNode* &child = root->Children[s[i]-30];
        if(child==NULL){
            return NULL;
        }   
        else{
            return getNumber(s,i+1,child); 
        }
    }

    public: unordered_set<string>* getNumber(string &name ){
        return getNumber(name,0,Root);
    }

    // delete >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    private: unordered_set<string>* remove(string &name,int i ,TrieNode*root){
        // cout<<i<<endl;
        if(i==name.size()){
            if(root->isEnd){
                root->isEnd=false;
                return root->Data;
            }
            return NULL;
        }
        TrieNode* &child = root->Children[name[i]-30];
        if(child==NULL){
            return NULL;
        }   
        else{
            unordered_set<string>* numbers = remove(name,i+1,child);
            if(!(child->isEnd)){ // checking if we need to remove the element child or not ../. to optimize space.
                bool eraseIt = true;
                for(int i=0;i<100;i++){
                    if(child->Children[i]!=NULL){
                        eraseIt=false;
                        break;
                    }
                }
                if(eraseIt){
                    delete child;
                    child=NULL;
                }
            } 
            return numbers;
        }
    }
    
    public: unordered_set<string>* remove(string &name){
        return remove(name,0,Root);
    }
    
    // Remove Number >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    private: void removeNumber(string &name,int i ,TrieNode*root,string& number){
        // cout<<i<<endl;
        if(i==name.size()){
            if(root->Data!=NULL && root->Data->count(number)){
                root->Data->erase(number);
                if(root->Data->size()==0){
                    root->isEnd=false;
                }
            }
            return ;
        }
        TrieNode* &child = root->Children[name[i]-30];
        if(child==NULL){
            return ;
        }   
        else{
            removeNumber(name,i+1,child,number);
            if(!(child->isEnd)){ // checking if we need to remove the element child or not ../. to optimize space.
                bool eraseIt = true;
                for(int i=0;i<100;i++){
                    if(child->Children[i]!=NULL){
                        eraseIt=false;
                        break;
                    }
                }
                if(eraseIt){
                    delete child;
                    child=NULL;
                }
            } 
            return ;
        }
    }
    
    public: void removeNumber(string &name,string number){
        removeNumber(name,0,Root,number);
    }

    // Prefiexes.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: TrieNode*getEnd(string &prefix, int i,TrieNode*root){
        if(i==prefix.size()){
            return root;
        }
        TrieNode* &child = root->Children[prefix[i]-30];
        if(child==NULL){
            return NULL;
        }   
        else{
            return getEnd(prefix,i+1,child); 
        }
    }
    
    private: void getPrefixes(TrieNode* root ,string prefix,vector<pair<string,unordered_set<string>*>> *ans){
        if(root->isEnd){
            ans->push_back({prefix,root->Data});
        }
        for(int i=0;i<100;i++){
            if(root->Children[i]){
                char a = 30+i;
                getPrefixes(root->Children[i],prefix+a,ans);
            }
        }
    }

    // possible Option..
    public: vector<pair< string ,unordered_set<string>*>>* prefixes(string &prefix){
        TrieNode*End = getEnd(prefix,0,Root);
        if(!End)return NULL;

        vector<pair<string,unordered_set<string>*>>*ans= new vector<pair<string,unordered_set<string>*>>();
        getPrefixes(End,prefix,ans);
        return ans ;
    }
    
    // destructor >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    ~Trie(){
        delete Root;
    }

};

class DigitNode{
    public:
    string Data;
    bool isEnd;
    DigitNode**Children = new DigitNode*[10]();
    DigitNode(){isEnd=false; Data="";}
    ~DigitNode(){
        for(int i=0;i<10;i++){
            delete Children[i];
        }
        delete []Children;
    }
};

class Number{
    DigitNode *Root = NULL;
    public:
    // constructor >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    Number(){
        Root = new DigitNode(); 
    }

    // Inserting a element >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private:void insert(string &number,int i, DigitNode* root , string &name){
        if(i==number.size()){
            root->isEnd=true;
            root->Data=name;
            return ;
        }

        DigitNode* &child = root->Children[number[i]-'0'];
        if(child==NULL){
            child = new DigitNode();
        }
        insert(number,i+1,child,name); 
    }

    public: void insert(string &number,string &name){
        insert(number,0,Root,name);
    }
    
    /// searching >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: bool isThere(string &number,int i,DigitNode*root){
        if(i==number.size()){
            return root->isEnd;
        }

        DigitNode* &child = root->Children[number[i]-'0'];
        if(child==NULL){
            return false;
        }   
        else{
            return isThere(number,i+1,child); 
        }
    }
    
    public: bool isThere(string &number){
        return isThere(number,0,Root);
    }

    // get name>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: string getName(string &number,int i,DigitNode*root){
        if(i==number.size()){
            if(root->isEnd){
                return root->Data;
            }   else return "";
        }

        DigitNode* &child = root->Children[number[i]-'0'];
        if(child==NULL){
            return "";
        }   
        else{
            return getName(number,i+1,child); 
        }
    }
   
    public: string getName(string &number ){
        return getName(number,0,Root);
    }


    // delete >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: string remove(string &number,int i ,DigitNode*root){
        // cout<<i<<endl;
        if(i==number.size()){
            if(root->isEnd){
                root->isEnd=false;
                return root->Data;
            }   
            else return "";
        }
        DigitNode* &child = root->Children[number[i]-'0'];
        if(child==NULL){
            return "";
        }   
        else{
            string name = remove(number,i+1,child);
            if(!(child->isEnd)){ // checking if we need to remove the element child or not ../. to optimize space.
                bool eraseIt = true;
                for(int i=0;i<10;i++){
                    if(child->Children[i]!=NULL){
                        eraseIt=false;
                        break;
                    }
                }
                if(eraseIt){
                    delete child;
                    child=NULL;
                }
            } 
            return name;
        }
    }
    
    public: string remove(string &number){
        return remove(number,0,Root);
    }

    // Prefiexes.along with name :>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    private: DigitNode*getEnd(string &prefix, int i ,DigitNode*root){
        if(i == prefix.size()){
            return root;
        }
        DigitNode* child = root->Children[prefix[i]-'0'];
        if(child==NULL){
            return NULL;
        }   
        else{
            return getEnd(prefix,i+1,child); 
        }
    }

    // get remaining answer in vector...
    private: void getPrefix(DigitNode*root ,string prefix,vector<pair<string,string>> *ans){
        if(root->isEnd){
            ans->push_back({prefix,root->Data});
        }
        for(int i=0;i<10;i++){
            if(root->Children[i]){
                char a = '0'+i;
                getPrefix(root->Children[i],prefix+a,ans);
            }
        }
    }

    // possible Option..
    public: vector<pair<string,string>>* prefixes(string &prefix){
        DigitNode *End = getEnd(prefix,0,Root);
        if(End==NULL)return NULL;
    
        vector<pair<string,string>>*ans= new vector<pair<string,string>>();
    
        getPrefix(End,prefix,ans);
        return ans ;
    }
    
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    private: void updateName(string & number ,int i ,DigitNode*root,string&name){
        if(i==number.size()){
            if(root->isEnd){
                root->Data=name;
            }
            return;
        }

        DigitNode* &child = root->Children[number[i]-'0'];
        if(child==NULL){
            return;
        }   
        else{
            updateName(number,i+1,child,name); 
        }
    }
 
    public: void updateName(string &number ,string &name){
        updateName(number ,0,Root ,name);
    }
    
    // destructor >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    ~Number(){
        delete Root;
    }
};

class PhoneBook{
    Trie* NameDirectory; // this will contain contact name with numbers  .
    Number* PhoneDirectory; // this will contain numbers with name ..

    private : bool isNumber(string s){
        int n=s.size();
        if(n==0)return false;
        int i=0;
        while(i<n){
            if(s[i]<'0' || s[i]>'9') break;
            i++;
        }
        return i==n;
    }
    public:
    PhoneBook(){
        NameDirectory = new Trie();
        PhoneDirectory = new Number();
    }

    void insert(string name ,string number){
        if(isNumber(number)){
            NameDirectory->insert(name,number);
            PhoneDirectory->insert(number,name);
            cout<<"Done"<<endl;
        }  else{
            cout<<"Process Terminated : Error With The Number !! "<<endl;
        }
    }

    void removeContact(string name){
        if(NameDirectory->isThere(name)){
            unordered_set<string>* numbers = NameDirectory->remove(name);
            for(string number : *numbers)
            PhoneDirectory->remove(number);
            cout<<"Done"<<endl;
        }
        else{
            cout<<"Contact Name Not Found !!"<<endl;
        }
    }

    void removeNumber(string number){
        if(isNumber(number)){
            if(PhoneDirectory->isThere(number)){
                string name = PhoneDirectory->remove(number);
                NameDirectory->removeNumber(name,number);
                cout<<"Done"<<endl;
            }   
            else{
                cout<<"Contact Number not found !!"<<endl;
            }
        }   
        else{
            cout<<"Process Terminated : Error With The Number !! "<<endl;
        }
    }

    void updateName(string oldName ,string freshName){
        if(NameDirectory->isThere(oldName) && !(NameDirectory->isThere(freshName))){
            unordered_set<string>*numbers = NameDirectory->remove(oldName);
            NameDirectory->insertNumbers(freshName,numbers);

            for(string number : *numbers){
                PhoneDirectory->updateName(number, freshName);
            }
            cout<<"Done"<<endl;
        }   else{
            cout<<"Error!! with the Names "<<endl;
        }
    }

    void updateNumber(string old ,string fresh){
        if(isNumber(old) && isNumber(fresh)){
            if(PhoneDirectory->isThere(old) && !(PhoneDirectory->isThere(fresh))){
                string name= PhoneDirectory->remove(old);
                PhoneDirectory->insert(fresh,name);
                NameDirectory->removeNumber(name,old);       
                NameDirectory->insert(name,fresh);  
                     
                cout<<"Done"<<endl;
            }   
            else{
                cout<<"Error!! with the Numbers "<<endl;
            }
        }    
        else {
            cout<<"Process Terminated : Error With The Number !! "<<endl;
        }
    }

    void getNumbers(string name){
        if(NameDirectory->isThere(name)){
            unordered_set<string>* numbers = NameDirectory->getNumber(name); 
            if(numbers!=NULL){
                for(auto number : *numbers){
                    cout<<number<<' ';
                }   cout<<endl;
            }
        }   
        else{
            cout<<"Contact Name not Found !!"<<endl;
        }
    }

    void getName(string number){
        if(isNumber(number)){
            string name = PhoneDirectory->getName(number);
            if(name.size()){
                cout<<name<<endl;
            }   else{
                cout<<"Contact Number Not Found !!"<<endl;
            }
        }
        else {
            cout<<"Process Terminated : Error With The Number !! "<<endl;
        }
    }

    void getPrefixes_by_Name(string prefix){
        vector<pair<string,unordered_set<string>*>>* prefixes = NameDirectory->prefixes(prefix);
        if(prefixes){
            for(auto x:*prefixes){
                cout<<x.first<<" : ";
                if(x.second!=NULL){
                    for(auto number : *x.second){
                        cout<<number<<' ';
                    }
                }  
                cout<<endl;
            }
        }   
    }
    
    void getPrefixes_by_Number(string prefix){
        if(prefix.size()==0 || isNumber(prefix)){
            vector<pair<string,string>> * prefixes = PhoneDirectory->prefixes(prefix);

            if(prefixes){
                for(auto x:*prefixes){
                    cout<<x.first<<" : "<<x.second<<endl;
                }
            }
        }
        else{
            cout<<"Process Terminated : Error With The Number !! "<<endl;
        }
    }
    
    void  searchName(string name){
        cout<< ( NameDirectory->isThere(name)? "Found !!":"Not Found !!" )<<endl;
    }
    
    void searchNumber(string number){
        if(isNumber(number)){
            cout<< ( PhoneDirectory->isThere(number)? "Found !!":"Not Found !!" )<<endl;
        }   
        else {
            cout<<"Process Terminated : Error With The Number !! "<<endl;
        }
    }

    ~PhoneBook(){
        delete NameDirectory;
        delete PhoneDirectory;
    }
};

class Menu{
    private : PhoneBook *phone;
    
    public: Menu(){
        phone=new PhoneBook;
    }

    public : void printMain(){
        string a;
        int b=0;
        while(true){
            cout<<"# Main Menu :- "<<endl;
            cout<<"  1: Insert"<<endl;
            cout<<"  2: Get"<<endl;
            cout<<"  3: Search"<<endl;
            cout<<"  4: Update"<<endl;
            cout<<"  5: Prefixes"<<endl;
            cout<<"  6: Remove"<<endl;
            cout<<"  7: Exit"<<endl;
            cout<<"Enter the Choice : "<<endl;

            cin>>a;
            cout<<"=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="<<endl<<endl;

            if(a.size()==0 || a.size()>1) cout<<"Syntax Error !! "<<endl;
            else {
                b=a[0]-'0';
                if(b<1 || b>7) cout<<"Value Out of Range Error !! "<<endl;
                else if(b==1)insert();
                else if(b==2)get();
                else if(b==3)search();
                else if(b==4)update();
                else if(b==5)prefixes();
                else if(b==6)remove();
                else {
                    cout<<"=================< Phone Boook Cloaed >================"<<endl<<endl;
                    break;
                }
            }
            cout<<endl;
        }
    }

    private : void insert(){
        string a;
        int b;
        while(true){
            cout<<"@ Insert in PhoneBook :"<<endl;
            cout<<"  1: Insert Name & Number"<<endl;
            cout<<"  2: Exit"<<endl;
            cout<<"Enter the Choice : "<<endl;

            cin>>a;
            cout<<"__________________________________________"<<endl;

            if(a.size()==0 || a.size()>1) cout<<"Syntax Error !1"<<endl;
            else {
                b=a[0]-'0';
                if(b<1 || b>2) cout<<"Value Out of Range Error !!"<<endl;
                else if(b==1){
                    cout<<"Enter the Name : "<<endl;
                    string name;
                    cin >>name;
                    cout<<"Enter the Number : "<<endl;
                    string number;
                    cin >>number;
                    cout<<"Status : ";
                    phone->insert(name , number);
                }
                else{
                    cout<<"---------------------------------------------------------"<<endl;
                    break;
                } 
            }
            cout<<"__________________________________________"<<endl;
            cout<<endl;
        }
    }
    
    private : void get(){
        string a;
        int b=0;
        while(true){
            cout<<"@ Get Info. from PhoneBook : "<<endl;
            cout<<"  1: Get Name associated to the Number"<<endl;
            cout<<"  2: Get all Numbers of under the Name"<<endl;
            cout<<"  3: Exit"<<endl;
            cout<<"Enter the Choice : "<<endl;
            
            cin>>a;
            cout<<"__________________________________________"<<endl;

            if(a.size()==0 || a.size()>1) cout<<"Syntax Error !!"<<endl;
            else {
                b=a[0]-'0';
                if(b<1 || b>3) cout<<"Value Out of Range Error !!"<<endl;
                else if(b==1){
                    cout<<"Enter the Number : "<<endl;
                    string number;
                    cin >>number;
                    cout<<"Status : ";
                    phone->getName(number);
                }   
                else if(b==2){
                    cout<<"Enter the Name : "<<endl;
                    string name;
                    cin >>name;
                    cout<<"Status : ";
                    phone->getNumbers(name);
                }   
                else {
                    cout<<"---------------------------------------------------------"<<endl;
                    break;
                }
            }
            cout<<"__________________________________________"<<endl;
            cout<<endl;
        }
    }

    private : void search(){
        string a;
        int b=0;
        while(true){
            cout<<"@ Search in PhoneBook :"<<endl;
            cout<<"  1: Search by Number "<<endl;
            cout<<"  2: Search by Name "<<endl;
            cout<<"  3: Exit"<<endl;
            cout<<"Enter the Choice : "<<endl;

            cin>>a;
            cout<<"__________________________________________"<<endl;

            if(a.size()==0 || a.size()>1) cout<<"Syntax Error !!"<<endl;
            else {
                b=a[0]-'0';
                if(b<1 || b>3) cout<<"Value Out of Range Error !!"<<endl;
                else if(b==1){
                    cout<<"Enter the Number : "<<endl;
                    string  number;
                    cin >>number;
                    cout<<"Status : ";
                    phone->searchNumber(number);
                }   
                else if(b==2){
                    cout<<"Enter the Name : "<<endl;
                    string name;
                    cin >>name;
                    cout<<"Status : ";
                    phone->searchName(name);
                }
                else {
                    cout<<"---------------------------------------------------------"<<endl;
                    break;
                }
            }
            cout<<"__________________________________________"<<endl;
            cout<<endl;
        }
    }
    
    private : void prefixes(){
        string a;
        int b=0;
        while(true){
            cout<<"@ Get Prefixes from PhoneBook : "<<endl;
            cout<<"  1: Get Prefixes by Number"<<endl;
            cout<<"  2: Get Prefixes by Name"<<endl;
            cout<<"  3: Exit"<<endl;
            cout<<"Enter the Choice : "<<endl;

            cin>>a;
            cout<<"__________________________________________"<<endl;

            if(a.size()==0 || a.size()>1) cout<<"Syntax Error !!"<<endl;
            else {
                b=a[0]-'0';
                if(b<1 || b>3) cout<<"Value Out of Range Error !!"<<endl;
                else if(b==1){
                    cout<<"Enter the prefix Number : "<<endl;
                    string prefix_num;
                    cin >>prefix_num;
                    cout<<"Status : "<<endl;
                    phone->getPrefixes_by_Number(prefix_num);
                }   
                else if(b==2){
                    cout<<"Enter the prefix Name : "<<endl;
                    string prefix_name;
                    cin >>prefix_name;
                    cout<<"Status : "<<endl;
                    phone->getPrefixes_by_Name(prefix_name);
                }
                else {
                    cout<<"---------------------------------------------------------"<<endl;
                    break;
                }
            }
            cout<<"__________________________________________"<<endl;
            cout<<endl;
        }
    }
   
    private : void remove(){
        int b=0;
        string a;
        while(true){
            cout<<"@ Remove info. from PhoneBook : "<<endl;
            cout<<"  1: Remove Number"<<endl;
            cout<<"  2: Remove Name"<<endl;
            cout<<"  3: Exit"<<endl;
            cout<<"Enter the Choice : "<<endl;

            cin>>a;
            cout<<"__________________________________________"<<endl;

            if(a.size()==0 || a.size()>1) cout<<"Syntax Error !!"<<endl;
            else {
                b=a[0]-'0';
                if(b<1 || b>3) cout<<"Value Out of Range Error !!"<<endl;
                else if(b==1){
                    cout<<"Enter the Number : "<<endl;
                    string number;
                    cin >>number;
                    cout<<"Status : ";
                    phone->removeNumber(number);
                }   
                else if(b==2){
                    cout<<"Enter the Name : "<<endl;
                    string name;
                    cin >>name;
                    cout<<"Status : ";
                    phone->removeContact(name);
                }
                else {
                    cout<<"---------------------------------------------------------"<<endl;
                    break;
                }
            }
            cout<<"__________________________________________"<<endl;
            cout<<endl;
        }
    }
    
    private : void update(){
        string a;
        int b=0;
        while(true){
            cout<<"@ Update in PhoneBook :"<<endl;
            cout<<"  1: Update Number "<<endl;
            cout<<"  2: Update Name "<<endl;
            cout<<"  3: Exit"<<endl;
            cout<<"Enter the Choice : "<<endl;
            cin>>a;
            cout<<"__________________________________________"<<endl;

            if(a.size()==0 || a.size()>1) cout<<"Syntax Error !!"<<endl;
            else {
                b=a[0]-'0';
                if(b<1 || b>3) cout<<"Value Out of Range Erro !!"<<endl;
                else if(b==1){
                    cout<<"Enter the old Number : "<<endl;
                    string old_num;
                    cin >>old_num;
                    cout<<"Enter the new Number : "<<endl;
                    string new_num;
                    cin >>new_num;
                    cout<<"Status : ";
                    phone->updateNumber(old_num,new_num);
                }   
                else if(b==2){
                    cout<<"Enter the old Name : "<<endl;
                    string old_name;
                    cin >>old_name;
                    cout<<"Enter the new Name : "<<endl;
                    string new_name;
                    cin >>new_name;
                    cout<<"Status : ";
                    phone->updateName(old_name,new_name);
                }
                else {
                    cout<<"---------------------------------------------------------"<<endl;
                    break;
                }
            }
            cout<<"__________________________________________"<<endl;
            cout<<endl;
        }
    }
    public: ~Menu(){
        delete phone;
    }
};

int main(){
    Menu menu;
    menu.printMain();
    return 0;
}

// int main(){
//     // Testing .
//     PhoneBook phone;

//     cout<<"--------------------------------Insert Function--------------------------------------"<<endl;
//     phone.insert("Prajwal",90956);
//     phone.insert("Prajwal",99283);
//     phone.insert("Prajwal",90181);
//     phone.insert("Prajwal",89126);
//     phone.insert("Prajwal",18522);

//     phone.insert("Prabhu",90836);
//     phone.insert("Prabhu",91787);
//     phone.insert("Pravarsh",91713);
//     phone.insert("Pranjal",81032);
//     phone.insert("Prakash",32727);
//     phone.insert("Pranav",5522);1

//     cout<<"--------------------------------Get Function--------------------------------------"<<endl;
//     phone.getNumbers("Prajwal");
//     phone.getName(90956);


//     cout<<"--------------------------------Search Function--------------------------------------"<<endl;
//     cout<< ( phone.searchName("prajwal")? "Found !!":"Not Found !!" )<<endl;
//     cout<< ( phone.searchName("Pranav")? "Found !!":"Not Found !!" )<<endl;
//     cout<< ( phone.searchNumber(90916)? "Found !!":"Not Found !!" )<<endl;
//     cout<< ( phone.searchNumber(18522)? "Found !!":"Not Found !!" )<<endl;

//     cout<<"--------------------------------Update Function--------------------------------------"<<endl;
//     phone.updateName("Prajwal","Praju");
//     phone.getNumbers("Prajwal");
//     phone.getNumbers("Praju");
//     phone.getName(18522);

//     cout<<endl;
    
//     phone.updateNumber(18522 , 18888);
//     phone.getName(18522);
//     phone.getName(18888);
//     phone.getNumbers("Praju");
    
//     cout<<"--------------------------------Prefixes Function--------------------------------------"<<endl;
//     phone.getPrefixes_by_Name("Pra");
//     phone.getPrefixes_by_Number(9);

//     cout<<"--------------------------------Remove Functions--------------------------------------"<<endl;
//     phone.removeNumber(9095);
    
//     cout<<endl;
    
//     phone.removeNumber(91713);
//     cout<< (phone.searchNumber(91713)? "Found !!":"Not Found !!" )<<endl;
//     phone.getNumbers("Pravarsh");  // will give contact name not found 

//     cout<<endl;

//     phone.removeContact("Pravarsh");
    
//     cout<<endl;
    
//     phone.getNumbers("Pranav");
//     phone.removeContact("Pranav");
//     phone.getNumbers("Pranav");
//     phone.getName(5522);
// }

                                                                               
