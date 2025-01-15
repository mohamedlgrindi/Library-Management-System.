#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;

// BOOK CLASS
class Book
{
private:
    string title;
    string author;
    int Isbn;
    bool isAvailable;
    string BorrowedBy;
public:
    int getIsbn(){return Isbn;}
    bool getavailable(){return isAvailable;}
    void setavailable(bool av){isAvailable=av;}
    void setBorrower(string br){BorrowedBy=br;}
    string getBorrower(){ return BorrowedBy;}

    Book(string title,string author,int Isbn, bool isAvailable,string borrower){
       this->title=title;
       this->author=author;
       this->Isbn=Isbn;
       this->isAvailable=isAvailable;
       this->BorrowedBy=borrower;
    }
    ~Book(){}

    void DisplayDetails(){
        cout<<"   The Title is: "<<title<<endl;
        cout<<"   The author is: "<<author<<endl;
        cout<<"   The Isbn is: "<<Isbn<<endl;
        cout<<"   The Book statu is: ";
        isAvailable?cout<<"available" : cout<<"borrowed"<<endl;
        cout<<"   The Book Borrowed By: "<<BorrowedBy<<endl;
    }
};

// FORWARD DECLARATIONS
class User;
class Teacher;
class Student;

// LIBRARY CLASS
class Library{
private:
    vector<shared_ptr<User>> users;
    vector<Book> books;

public:
    void addBook(string t,string aut,int Isb, bool isAvai,string borrower=" "){
     Book newbook(t,aut, Isb, isAvai,borrower);
     books.emplace_back(newbook);
     cout<<"Book Added Successfully To Library"<<endl;
     cout<<"___________________________________"<<endl;
    }

    vector<Book>& getbooks(){
        return books;
    }

    vector<shared_ptr<User>>& getusers(){
        return users;
    }
 

    void addUser(const shared_ptr<User>&user){
     users.emplace_back(user);
    }
   /* void SetUser(int id, string username, string rol) {
        if (rol == "Student") {
            users.emplace_back(make_shared<Student>(id, username, rol));
        } else if (rol == "Teacher") {
            users.emplace_back(make_shared<Teacher>(id, username, rol));
        } else {
            cout << "Invalid role. User not added." << endl;
        }
    }*/

    void removeBook(int isbn){
        bool exist=false;
        for(int i=0;i<books.size();i++){
            if(books[i].getIsbn()==isbn){
                books.erase(books.begin()+i);
                exist=true;
            }
        }
        if(!exist){cout<<"The Book Does not exist"<<endl;}
    }

    Book* findbook(int isbn){
        bool exist=false;
        for(int i=0;i<books.size();i++){
            if(books[i].getIsbn()==isbn){
                exist=true;
                return &books[i];
            }
        }
        if(!exist){cout<<"The Book Does not exist"<<endl;return nullptr;}
    }

    void trackBorrowedBooks(){
      for(auto &book:getbooks()){
           if(!(book.getavailable())){
             cout<<"The book is borrowed by: "<<book.getBorrower()<<endl;
             book.DisplayDetails();
           }
        }
    }
};

// USER CLASS
class User
{
private:
    string UserName;
    int Id;
    string role;
public:
    User(int id,string user,string rol):Id(id),UserName(user),role(rol) {};

    int getId(){return Id;}
    string getrole(){ return role;}
    string getUsername(){return UserName;}

    void viewBooks(Library& library){
           cout<<"Available Books in the library"<<endl;
           bool  anyavailable=false;

           for(auto &book:library.getbooks()){
              if(book.getavailable()){
                book.DisplayDetails();
                anyavailable=true;
              }
           }
           if(!anyavailable){
            cout<<"THERE IS NO AVAILABLE BOOKS"<<endl;
           }
    }

   virtual void performRolesSpecificAction()=0;
    

    virtual ~User(){}
};

// STUDENT CLASS
class Student: public User{
private:
    vector<Book*> Sborrowed;

public:
    Student(int id, string user, string rol) : User(id, user, rol) {}

    void BorrowBook(Library& library,int isb){
         Book* book=library.findbook(isb);
        
           if(book && book->getavailable()){
            Sborrowed.emplace_back(book);
            book->setavailable(false);
            book->setBorrower(this->getUsername());
            cout<<"____Book borrowed successfully____"<<endl;
            return;
           }
        

    }

    void returnBook(int isb){
      auto it=find_if(Sborrowed.begin(),Sborrowed.end(),[isb](Book* book){return book->getIsbn()==isb;});
      if(it!=Sborrowed.end()){
         (*it)->setavailable(true);
         (*it)->setBorrower("");
         Sborrowed.erase(it);
         cout<<"____Book returned successfully____"<<endl;
         return;
      }
    }

     void BorrowedBooks(){
        for (int i = 0; i <Sborrowed.size() ; i++)
        {
           if(!(Sborrowed.empty())){
            cout<<"______The list of Borrowed books______"<<endl;
            Sborrowed[i]->DisplayDetails();
           }
           else{cout<<"You don't Borrow anything"<<endl;}
        }
        
    }
   
    void performRolesSpecificAction(){
        cout <<"_____________________________"<<endl;
        cout<<"Options for Student:"<<endl;
        cout << "1. View Available Books" << endl;
        cout<< "2.  Borrow books"<<endl;
        cout<< "3.  Return books"<<endl;
        cout<< "4.  List of your borrowed books"<<endl;
        cout <<"_____________________________"<<endl;
    }

};

// TEACHER CLASS
class Teacher:public User{
private:
    vector<Book*> TBorrowed;
    
public:
    Teacher(int id, string user, string rol) : User(id, user, rol) {}

    void performRolesSpecificAction(){
        cout <<"_____________________________"<<endl;
        cout << "Options for Teacher:" << endl;
        cout << "1. View Available Books" << endl;
        cout << "2. Borrow a Book" << endl;
        cout << "3. Return a Book" << endl;
        cout <<"_____________________________"<<endl;
    }

    void BorrowBook(Library& library,int isb){
         Book* book=library.findbook(isb);
        
           if(book && book->getavailable()){
            TBorrowed.emplace_back(book);
            book->setavailable(false);
            book->setBorrower(this->getUsername());
            cout<<"____Book borrowed successfully____"<<endl;
            return;
           }
        

    }

    void returnBook(int isb){
       auto it=find_if(TBorrowed.begin(),TBorrowed.end(),[isb](Book* book){return book->getIsbn()==isb;});
       if(it!=TBorrowed.end()){
        (*it)->setavailable(true);
        (*it)->setBorrower(" ");
        TBorrowed.erase(it);
        return;
       }

       
    }


};

class Librarian:public User{
    public:
    Librarian(int id, string user, string rol) : User(id, user, rol) {}
    
    void addBooks(Library& library,string t,string aut,int Isb, bool isAvai=true,string borrower=" "){
     library.addBook( t, aut, Isb, isAvai, borrower=" ");
    }

    void RemoveBook(Library& library,int isb){
     library.removeBook(isb);
    }
    
    void viewAllBorrowedBooks(Library& library){
     library.trackBorrowedBooks();
    }
  
     void performRolesSpecificAction(){
        cout <<"_____________________________"<<endl;
        cout << "Options for Librarian:" << endl;
        cout << "1. View Available Books" << endl;
        cout << "2. Add a Book" << endl;
        cout << "3. Remove a Book" << endl;
        cout << "4. View All Borrowed Books" << endl;
        cout <<"_____________________________"<<endl;
    }

};

int main(){
  Library library;

  library.addBook("The Great Gatsby", "F. Scott Fitzgerald", 1001,true,"");
  library.addBook("To Kill a Mockingbird", "Harper Lee", 1002,true,"");
  library.addBook("1984", "George Orwell", 1003,true,"");

  auto student=make_shared<Student>(1213,"Mohamed","Student");
  auto teacher=make_shared<Teacher>(4356,"Said","Teacher");
  auto librarian=make_shared<Librarian>(9674,"alice","Liabrarian");

  library.addUser(student);
  library.addUser(teacher);
  library.addUser(librarian);
  //Id
  int Id;
  string username;
  bool login;
  shared_ptr<User>currentuser=NULL;
  cout<<endl;
  cout<<"Enter The Id: "<<endl;
  cin>>Id;
  cin.ignore();
  cout<<"Enter The User Name: "<<endl;
  getline(cin ,username);

  for(auto& user:library.getusers()){
    if(user->getId()==Id && user->getUsername()==username){
        currentuser=user;
        login=true;
    }
  }

  if(!login){
    cout<<"Invalid credentials. Exiting program." <<endl;
  }
  cout<<"The user name is: "<<currentuser->getUsername()<<"Role: "<<currentuser->getrole()<<endl;
   int choise;
 do{
    currentuser->performRolesSpecificAction();
    cout<<"Enter 0 for exit"<<endl;
    cout<<"Enter your choise: "<<endl;
    cin>>choise;

    switch(choise){
      case 1:
       currentuser->viewBooks(library);
      break;
      case 2:
        if( currentuser->getrole()=="Student"){
          int isbn;
          cout<<"Enter The book Isbn: "<<endl;
          cin>>isbn;
          dynamic_pointer_cast<Student>(currentuser)->BorrowBook(library,isbn);
        }
        else if(currentuser->getrole()=="Teacher"){
          int isbn;
          cout<<"Enter The book Isbn: "<<endl;
          cin>>isbn;
          dynamic_pointer_cast<Teacher>(currentuser)->BorrowBook(library,isbn);
        }
        else if(currentuser->getrole()=="Librarian"){
            int isbn;
            string title,author;
            cout<<"Enter the book detaile(title,author,isbn)";
            getline(cin,title);
            getline(cin,author);
            cin>>isbn;
            dynamic_pointer_cast<Librarian>(currentuser)->addBooks(library,title,author,isbn);
        }
      break;

      case 3:
         if(currentuser->getrole()=="Student"){
          int isbn;
          cout<<"Enter The book Isbn: "<<endl;
          cin>>isbn;
          dynamic_pointer_cast<Student>(currentuser)->returnBook(isbn);
         }
         else if(currentuser->getrole()=="Teacher"){
          int isbn;
          cout<<"Enter The book Isbn: "<<endl;
          cin>>isbn;
          dynamic_pointer_cast<Teacher>(currentuser)->returnBook(isbn);
         }
         else if(currentuser->getrole()=="Librarian"){
            int isbn;
            cout<<"Enter the book detaile(isbn)";
            cin>>isbn;
            dynamic_pointer_cast<Librarian>(currentuser)->RemoveBook(library,isbn);
         }
       break;

      case 4:
      if(currentuser->getrole()=="Student"){
          dynamic_pointer_cast<Student>(currentuser)->BorrowedBooks();
         }
         else if(currentuser->getrole()=="Librarian"){
            dynamic_pointer_cast<Librarian>(currentuser)->viewAllBorrowedBooks(library);
          }
           break;
       case 0:
       cout << "Exiting... Goodbye!" << endl;
       break;
       default:
       cout<<"__Invalid input, try agin__"<<endl;
       break;

      }

    }while(choise!=0);

    return 0;
}
