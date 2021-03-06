#ifndef _HISTORY_TRANSACTION_CPP_
#define _HISTORY_TRANSACTION_CPP_

#include <cassert>
#include <iomanip>
#include <iostream>

//Transaction Class Constructor which initialises some of the values and sets the object pointers to their appropriate defintion.
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,
      unsigned int month_date,  unsigned year_date,
      bool buy_sell_trans,  unsigned int number_shares,
            double trans_amount ){
                this->symbol = ticker_symbol;
                this->day = day_date;
                this->month = month_date;
                this->year = year_date;
    
                if(buy_sell_trans == true){
                    this->trans_type = "Buy";
                    }
                else{
                    this->trans_type = "Sell";
                    }
                this->shares = number_shares;
                
                this->amount = trans_amount;
                
                this->trans_id = assigned_trans_id;
                ++assigned_trans_id;

                // These private members have to be populated.
                this->acb = 0.0;
                this->acb_per_share = 0.0;
                this->share_balance = 0.0;
                this->cgl = 0.0;

                this->p_next = nullptr;;
}


//Transaction Class Destructor.
Transaction::~Transaction(){
}

//Overloaded the "<" operator to help with the sort function which would allow the dates to be sorted in ascending order.
bool Transaction::operator<( Transaction const &other ){
    if (year < other.year){
        return true;
    }
    if ((year == other.year) && (month < other.month)){
        return true;
    }
    if ((year == other.year) && (month == other.month) && (day < other.day)){
        return true;
    }
    else {
        return false;
    }
    
}
// GIVEN
// Member functions to get values (getters)
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values (setters)
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}

//History Class Constructor
History::History(){
    this->p_head = nullptr;
}

//History Class Destructor
History::~History(){
    Transaction *p_temp = nullptr;
    while(p_head != nullptr){
        p_temp = p_head;
        p_head = p_head->get_next();
        
        delete p_temp;
        p_temp = nullptr;
    }
}

//Read function reads all the transactions from the "Transactions_History.txt" file
void History::read_history(){
    ece150::open_file();
    while (ece150::next_trans_entry()){
        Transaction *trans = new Transaction(
                    ece150::get_trans_symbol(),
                    ece150::get_trans_day(),
                    ece150::get_trans_month(),
                    ece150::get_trans_year(),
                    ece150::get_trans_type(),
                    ece150::get_trans_shares(),
                    ece150::get_trans_amount());
        this->insert(trans);
    }
    ece150::close_file();
}
    

//Insert function inserts the new transaction in the appropriate place in the Transaction History List.
void History::insert(Transaction *p_new_trans){
   
    if (p_head == nullptr){
    p_head=p_new_trans; //empty case
    }
    else{
    Transaction *p_temp{p_head}; //general case
    
    while(p_temp->get_next() != nullptr){
        p_temp = p_temp->get_next();
    }
    p_temp->set_next(p_new_trans);
    }
    
}

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//Sorting Algorithm which sorts the transactions in Ascending Order by Date.
void History::sort_by_date(){
   Transaction * p_sort{nullptr};
   Transaction * p_temp{nullptr};
   
   if((p_head -> get_next() == nullptr) || p_head == nullptr){
       return;
   }
   
   p_sort = p_head;
   p_head = p_head -> get_next();
   p_sort -> set_next(nullptr);
   
   while(p_head != nullptr){
       
       p_temp = p_head;
       p_head = p_head->get_next();
       p_temp->set_next(nullptr);
       
       if (p_sort == nullptr) {
           p_sort = p_temp;
           
       }
       else {
           
           if (*p_temp < *p_sort) {
               p_temp->set_next(p_sort);
               p_sort = p_temp;
           }
           
           else {
               
               Transaction *temp = p_sort;
               
               while ((temp->get_next() != nullptr) && !(*p_temp < *(temp->get_next()))) {
                            temp = temp ->get_next();
               }
               
               p_temp->set_next(temp ->get_next());
               temp ->set_next(p_temp);
           }
       }
   }
   p_head = p_sort;
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//Function updates all the header values as seen in the Output Print File. It updates based on whether the file was a "Buy" or "Sell".
void History::update_acb_cgl(){
    Transaction *p_temp = p_head;
    double acb = 0.0;
    double acb_per_share = 0.0;
    double share_balance = 0.0;
    double cgl = 0.0;
    
    while(p_temp != nullptr){
            if (p_temp->get_trans_type() == true){
                acb = acb + p_temp->get_amount();
                share_balance = share_balance + p_temp->get_shares();
                acb_per_share = acb/ share_balance;


                p_temp->set_acb(acb);
                p_temp->set_cgl(0.00);
                p_temp->set_acb_per_share(acb_per_share);
                p_temp->set_share_balance(share_balance);
            }
            else{
                acb = acb- p_temp->get_amount();
                share_balance= share_balance - p_temp->get_shares();
                acb_per_share = acb_per_share;
                cgl = p_temp->get_amount()- p_temp-> get_shares() * acb_per_share;
                acb = acb + cgl;

                p_temp->set_acb(acb);
                p_temp->set_cgl(cgl);
                p_temp->set_acb_per_share(acb_per_share);
                p_temp->set_share_balance(share_balance);
            }
            p_temp = p_temp -> get_next();
        }
    }


// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
//Calculates the Capital Gain/Loss for a chosen year by the user.
double History::compute_cgl(unsigned int year){
    Transaction * p_temp = p_head;
    double cgl_ = 0.0;
    
    while(p_temp != nullptr){
        if((p_temp->get_year() == year) && (p_temp->get_trans_type() == false)){
            cgl_ += p_temp->get_cgl();
        }
        
        p_temp = p_temp->get_next();
    }
    
    return cgl_;
}




// print() Print the transaction history.
//TASK 9
//Prints the transaction history in the format as seen in the Print Format Output Sample.
void History::print(){
    std::cout<<" ========== BEGIN TRANSACTION HISTORY ============ "<<std::endl;
    Transaction * p_temp{p_head};
    
    while (p_temp != nullptr){
        p_temp->print();
        p_temp = p_temp->get_next();
    }
    
    std::cout<<" ========== END TRANSACTION HISTORY ============ "<<std::endl;
    
}


// GIVEN
// get_p_head(): Full access to the linked list.
Transaction *History::get_p_head() { return p_head; }

