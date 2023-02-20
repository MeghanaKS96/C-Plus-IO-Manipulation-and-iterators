#include "transaction.h"
#include <iostream>
#include<fstream>
#include <sstream>
#include <stdlib.h>
#include<locale>
#include <iomanip>
#include<stdexcept>
#include<exception>


class TransactionRecComp 
{
    public:
    bool operator()(Transaction t1, Transaction t2)
    {
        return (t1.acc_nb > t2.acc_nb) ;
    }
};

TransactionList::TransactionList()
{

}

void parseRecord(std::string line,TransactionList &tl)
{
    std::string acc_nb;
    std::string fname;
    std::string lname;
    std::string amt;
    std::string merchant;
    std::string date;
    std::string time;
    std::stringstream ss(line);
    getline(ss,acc_nb,'|');
    getline(ss,fname,'|');
    getline(ss,lname,'|');
    getline(ss,amt,'|');
    getline(ss,merchant,'|');
    getline(ss,date,'|');
    getline(ss,time,'|');

    struct tm when;
    std::string hh;
    std::string mm;
    std::string sec;
    std::stringstream ti(time);
    getline(ti,hh,':');
    getline(ti,mm,':');
    getline(ti,sec,':');
    when.tm_hour = std::stoi(hh);
    when.tm_min = std::stoi(mm);
    when.tm_sec = std::stoi(sec);
    std::string year;
    std::string month;
    std::string day;
    std::stringstream dd(date);
    getline(dd,day,'.');
    getline(dd,month,'.');
    getline(dd,year,'.');
    when.tm_year = std::stoi(year)+2000 - 1900;
    when.tm_mon = std::stoi(month) - 1; //0-based
    when.tm_mday = std::stoi(day);
    time_t converted;
    converted = mktime(&when);
    Transaction t(acc_nb,fname,lname,std::strtod(amt.c_str(),NULL),merchant,converted);
    tl.transactions.push_back(t);   
}

void TransactionList::parseFile(std::string filename)
{
    try
    {
       std::ifstream myFile;
        myFile.open(filename);
    if(myFile.fail())
    {
        throw std::runtime_error("File does not exist/open due to exception");
    }
    while(myFile.good())
    {
        std::string line;
        getline(myFile,line,'\n');
        parseRecord(line,*this);
    }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }    
    
}

std::vector<Transaction>::iterator TransactionList::begin()
{
    return transactions.begin();
}

std::vector<Transaction>::iterator TransactionList::end()
{
    return transactions.end();
}

TransactionList TransactionList::sort()
{
  std::sort(begin(),end(),TransactionRecComp());
  return *this;
}

std::ostream &operator<<(std::ostream &os,const Transaction& t)
{
    
    std::locale dt("de_DE.utf-8");
    os <<std::right<<std::setw(15)<< "Account Nb:" <<std::right<< std::setw(50)<<t.acc_nb << std::endl;
    os <<std::right<<std::setw(15)<< "First Name:" <<std::right<< std::setw(50)<< t.fname<< std::endl;
    os <<std::right<<std::setw(15)<< "Last Name:" << std::right<<std::setw(50)<< t.lname<< std::endl;
    std::cout.imbue(dt);
    os <<std::right<<std::setw(15)<< "Amount:" << std::right<<std::setw(50)<<std::showbase<< std::put_money(t.amt*100,true)<< std::endl;
    os <<std::right<<std::setw(15)<< "Merchant:" <<std::right<< std::setw(50)<< t.merchant<< std::endl;
    os <<std::right<<std::setw(15)<< "Date and Time:" <<std::right<< std::setw(50)<< std::asctime(std::localtime(&t.timestamp))<< std::endl;

    os << std::endl;
    return os;
}

int main()
{
    TransactionList tl;
    tl.parseFile("transactions.txt");
    TransactionList sorted = tl.sort();
    for(Transaction t : sorted.transactions)
    {
        std::cout<<t;
    }
    return 0;
}
