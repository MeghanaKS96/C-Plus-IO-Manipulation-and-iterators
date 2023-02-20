#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

class Transaction
 {
  public:
  std::string acc_nb;
  std::string fname;
  std::string lname;
  double amt;
  std::string merchant;
  std::time_t timestamp;
  Transaction(std::string acc_nb, std::string fname, std::string lname,
    double amt, std::string merchant, std::time_t timestamp)
    : acc_nb(acc_nb), fname(fname), lname(lname), amt(amt),
    merchant(merchant), timestamp(timestamp) {}

  
};



class TransactionList  
{
  public:
  std::vector<Transaction> transactions;
  TransactionList();
  void parseFile(std::string);
  std::vector<Transaction>::iterator begin();
  std::vector<Transaction>::iterator end();
  TransactionList sort();
};




