//Vikram Tharakan 5/9/11
//poly2.cxx

#include "poly2.h"
#include <algorithm>
#include <cmath>
#include <climits>
#include <cassert>
#include <cstdlib>

using namespace std;
namespace main_savitch_5
{
  // CONSTRUCORS AND DESTRUCTOR
  polynomial::polynomial(double c, unsigned int exponent){
    head_ptr = new polynode;
    if(c>0){
      current_degree = exponent;
    }
    else{
      current_degree = 0;
    }

    
    if (c == 0) {
      tail_ptr = head_ptr;
      head_ptr -> set_exponent(0);
      head_ptr -> set_coef(0);
    }

    if(exponent ==0 && c!=0){
      tail_ptr = head_ptr;
      head_ptr -> set_coef(c);
      head_ptr -> set_exponent(0);
    }

    recent_ptr = head_ptr;
    if(c>0){
      for(int i =0; i < exponent; i++){
	polynode *p1 = new polynode;
	recent_ptr->set_fore(p1);
	p1->set_back(recent_ptr);
	p1 -> set_exponent(i+1);
	if((i+1) != exponent){
	  p1-> set_coef(0);
	}
	else{
	  p1-> set_coef(c);
	  tail_ptr = p1;
	}
	recent_ptr = p1;
      }
    }      
  }
  
  polynomial::polynomial(const polynomial& source){
    head_ptr = new polynode;
    recent_ptr = head_ptr;
    if(source.head_ptr == NULL){
      tail_ptr = head_ptr;
      head_ptr -> set_coef(0);
      head_ptr -> set_exponent(0);
    }
    else{
      head_ptr ->set_coef(source.head_ptr->coef());
      head_ptr ->set_exponent(0);
      //Check if there is a source.head_ptr->fore()?? It doesn't matter for the test doe
      for(polynode *temp = source.head_ptr->fore(); temp!=NULL; temp = temp->fore()){
	polynode *creater = new polynode;
	recent_ptr ->set_fore(creater);
	creater ->set_back(recent_ptr);
	creater -> set_coef(temp->coef());
	creater -> set_exponent(temp->exponent());
	recent_ptr = creater;
	}
      tail_ptr = recent_ptr; 
    }
    current_degree = source.current_degree;
  }
  
  polynomial::~polynomial( ){
    while(head_ptr != NULL){
      polynode *remove_ptr;
      remove_ptr = head_ptr;
      head_ptr = head_ptr->fore();
      delete remove_ptr;
    }
  }

  // MODIFICATION MEMBER FUNCTIONS
  polynomial& polynomial::operator =(const polynomial& source){
    polynode *new_head;
    if(this == &source)
      return *this;
    
    if(current_degree != source.current_degree){
      new_head = new polynode;
      //clears previous polynomial (code is same as destructor)
      while(head_ptr != NULL){
	polynode *remove_ptr;
	remove_ptr = head_ptr;
	head_ptr = head_ptr->fore();
	delete remove_ptr;
      }
      head_ptr = new_head;
    }

    //Sets the polynomial = to the source polynomial (code the same as copy constructor)
    recent_ptr = head_ptr;
    if(source.head_ptr == NULL){
      tail_ptr = head_ptr;
      head_ptr -> set_coef(0);
      head_ptr -> set_exponent(0);
    }
    else{
      head_ptr ->set_coef(source.head_ptr->coef());
      head_ptr ->set_exponent(0);
      //Check if there is a source.head_ptr->fore()?? It doesn't matter for the test case
      for(polynode *temp = source.head_ptr->fore(); temp!=NULL; temp = temp->fore()){
	polynode *creater = new polynode;
	recent_ptr ->set_fore(creater);
	creater ->set_back(recent_ptr);
	creater -> set_coef(temp->coef());
	creater -> set_exponent(temp->exponent());
	recent_ptr = creater;
      }
      tail_ptr = recent_ptr;
    }
    current_degree = source.current_degree;
    return *this;
    
  }



  
  void polynomial::add_to_coef(double amount, unsigned int exponent){    
    if(exponent ==0){
      double x = head_ptr->coef();
      head_ptr -> set_coef(amount + x);
    }

    
    if(exponent <= current_degree){
      polynode *p1 = head_ptr;
      double new_c;
      for(int i =0; i < exponent; i++){
        p1=p1->fore();
        if((i+1) == exponent){
          double x = p1->coef();
          new_c = x + amount;
          p1-> set_coef(new_c);
	  //Important for rounding down
	  if(new_c == 0){
            polynode *p2 = head_ptr;
            while(p2->exponent() != exponent){
              if(p2->coef() != 0)
                current_degree = p2->exponent();
	      p2 = p2->fore();
            }
          }

        }
      }
    }

 if(exponent > current_degree && amount>0){
      recent_ptr = tail_ptr;
      for(int i = current_degree; i < exponent; i++){
        polynode *p1 = new polynode;
        recent_ptr->set_fore(p1);
	p1->set_back(recent_ptr);
        p1 -> set_exponent(i+1);
        if((i+1) != exponent){
          p1-> set_coef(0);
        }
        else{
          p1-> set_coef(amount);
          tail_ptr = p1;
        }
        recent_ptr = p1;
      }
      current_degree = exponent;
    }

  }

  
  void polynomial::assign_coef(double coefficient, unsigned int exponent){
    if(exponent ==0){
      head_ptr -> set_coef(coefficient);
    }

    if(exponent <= current_degree){
      polynode *p1 = head_ptr;
      for(int i =0; i < exponent; i++){
	p1=p1->fore();
	if((i+1) == exponent){
	  p1-> set_coef(coefficient);
	  //Important for rounding down
	  if(coefficient == 0 && exponent==current_degree){
	    polynode *p2 = head_ptr;
	    while(p2->exponent() != exponent){
	      if(p2->coef() != 0){
		current_degree = p2->exponent();
		tail_ptr = p2;
	      }
	      p2 = p2->fore();
	    }
	  }
	}
	  
      }
    }
     
    if(exponent > current_degree && coefficient!=0){
      recent_ptr = tail_ptr;
      for(int i = current_degree; i < exponent; i++){
	polynode *p1 = new polynode;
	recent_ptr->set_fore(p1);
	p1->set_back(recent_ptr);
	p1 -> set_exponent(i+1);
	if((i+1) != exponent){
	  p1-> set_coef(0);
	}
	else{
	  p1-> set_coef(coefficient);
	  tail_ptr = p1;
	}
	recent_ptr = p1;
      }
      current_degree = exponent;
    }

  }


  
  void polynomial::clear( ){
    polynode *p1 = head_ptr;
    p1->set_coef(0);
    for(int i =0; i < current_degree; i++){
      p1=p1->fore();
      if((i+1) == current_degree){
	p1-> set_coef(0);
      }
    }
    current_degree = 0;
    tail_ptr = head_ptr;
  }


  
  // CONSTANT MEMBER FUNCTIONS
  double polynomial:: coefficient(unsigned int exponent) const{
    if(exponent <= current_degree){
      polynode *temp = head_ptr;
      for(int i = 0; i<exponent; i++){
	temp =	temp->fore();
      }
      return temp -> coef();
    }
    else{
      return 0;
    }
  }
  
  polynomial polynomial::derivative( ) const{
    polynomial p1;
    polynode *recent_ptr2 = p1.head_ptr;
    if(head_ptr == NULL || head_ptr-> fore() == NULL){
      return p1;
    }
    else{
      p1.head_ptr -> set_coef(head_ptr->fore()->coef());      
      for(polynode *temp = head_ptr->fore()->fore(); temp!=NULL; temp = temp->fore()){
	polynode *creater = new polynode;
	recent_ptr2 ->set_fore(creater);
	creater ->set_back(recent_ptr2);
	creater -> set_coef(temp->coef()*temp->exponent());
	creater -> set_exponent(temp->exponent()-1);
	recent_ptr2 = creater;
      }
    }
    p1.current_degree = current_degree-1;
    return p1;
  }

  
  double polynomial::eval(double x) const{
    double total = 0;
    polynode *temp = head_ptr;
    for(int i = 0; i<current_degree; i++){
      total += pow(x,i)*(temp->coef());
      temp = temp->fore();
    }
    total += pow(x,current_degree)*(temp->coef());
    return total;
   
  }
  void polynomial::find_root(
		 double& answer,
		 bool& success,
		 unsigned int& iterations,
		 double guess,
		 unsigned int maximum_iterations,
		 double epsilon
		 )
    const{
    assert(epsilon>0);
    polynomial p = derivative();
    answer = guess;
    double f = eval(answer);
    double f_prime = p.eval(answer);
    iterations = 0;
    while(iterations<maximum_iterations && abs(f)>epsilon && abs(f_prime)>epsilon){
      iterations += 1;
      answer = answer - (f/f_prime);
      f = eval(answer);
      f_prime = p.eval(answer);
    }
    if(abs(f)<epsilon){
      success = true;
    }
    else{
      success = false;
    }

  }

  
  unsigned int polynomial::next_term(unsigned int e) const{
    if(e>=current_degree)
      return 0;
    else{
      recent_ptr = head_ptr;
      for(int i = 0; i<current_degree; i++){
	if(recent_ptr->coef() != 0 && recent_ptr->exponent()>e){
	  return recent_ptr->exponent();
	}
	recent_ptr = recent_ptr -> fore();
      }
    }
  }

  
  unsigned int polynomial::previous_term(unsigned int e) const{
    if(e == 0){
      return UINT_MAX;
    }

    //The loops are trying to take care of the case that the answer is TESTSIZE
    recent_ptr = head_ptr;
    if(e>current_degree){
      e = current_degree;            
      for(int i = 0; i<e; i++){
	recent_ptr = recent_ptr->fore();
      }
      if(recent_ptr->coef()!=0){       
	return recent_ptr->exponent();
      }
    }
    
    //for when e is normally less than current_degree
    recent_ptr = head_ptr;
    polynode *temp;
    int k = 0;
    for(int i = 0; i<e; i++){
      if(recent_ptr->coef() !=0){
	temp = recent_ptr;
	k+=1;
      }
      recent_ptr = recent_ptr->fore();
    }
    if(k==0)
      return UINT_MAX;
    return temp->exponent();

  }

  
  // NON-MEMBER BINARY OPERATORS
  polynomial operator +(const polynomial& p1, const polynomial& p2){
    polynomial p;
    double holder;

    if(p1.degree()<p2.degree()){
      for(unsigned int i = 0; i<(p1.degree()+1); i++){
	holder = p1.coefficient(i) + p2.coefficient(i);
	p.assign_coef(holder,i);
      }
      for(unsigned int j = (p1.degree()+1); j<(p2.degree()+1); j++){
	holder = p2.coefficient(j);
	p.assign_coef(holder,j);
      }
    }
    else{
      for(unsigned int i = 0; i<(p2.degree()+1); i++){
	holder = p1.coefficient(i) + p2.coefficient(i);
	p.assign_coef(holder,i);
      }
      for(unsigned int j = (p2.degree()+1); j<(p1.degree()+1); j++){
	holder = p1.coefficient(j);
	p.assign_coef(holder,j);
      }
    }
    return p;
  }

  
  polynomial operator -(const polynomial& p1, const polynomial& p2){
    polynomial p;
    double holder;

    if(p1.degree()<p2.degree()){
      for(unsigned int i = 0; i<(p1.degree()+1); i++){
	holder = p1.coefficient(i) - p2.coefficient(i);
	p.assign_coef(holder,i);
      }
      for(unsigned int j = (p1.degree()+1); j<(p2.degree()+1); j++){
	holder = -p2.coefficient(j);
	p.assign_coef(holder,j);
      }
    }
    else{
      for(unsigned int i = 0; i<(p2.degree()+1); i++){
	holder = p1.coefficient(i) - p2.coefficient(i);
	p.assign_coef(holder,i);
      }
      for(unsigned int j = (p2.degree()+1); j<(p1.degree()+1); j++){
	holder = p1.coefficient(j);
	p.assign_coef(holder,j);
      }
    }
    return p;
    
  }

  
  polynomial operator *(const polynomial& p1, const polynomial& p2){
    int limit = (p1.degree()+1)*(p2.degree()+1);
    polynomial p(0,limit);
    for(unsigned int i = 0; i<=p1.degree(); i++){
      for(unsigned int j = 0; j<=p2.degree(); j++){
	if(p1.coefficient(i) != 0 && p2.coefficient(j) != 0){
	  double holder = p1.coefficient(i)*p2.coefficient(j);
	  p.add_to_coef(holder,(i+j));
	}
      }
    }
    return p;
  }

  // NON-MEMBER OUTPUT FUNCTION
  std::ostream& operator << (std::ostream& out, const polynomial& p){
    if (p.degree( ) == 0 && p.coefficient(0)==0)
      {
	out << 0;
	return out;
      }

    out << p.coefficient(p.degree());
    if(p.degree()>1)
      out << "x^" << p.degree();
    else if(p.degree() ==1)
      out << "x";
    for (int i = p.degree( )-1; i >= 0; i--)
      {
	if ( p.coefficient( i ) < 0.0 )
	  out << " - " << std::abs(p.coefficient( i ));
	else if (p.coefficient(i) > 0)
	  out << " + " << p.coefficient(i);

	if(p.coefficient(i) !=0){
	  if(i>1)
	    out << "x^" << i;
	  else if(i ==1)
	    out << "x";
	}
      }
    out << std::endl;
    return out;
    


  }


  
  // PRIVATE MEMBER FUNCTION (to aid the other functions)
  void polynomial::set_recent(unsigned int exponent) const{
    if(exponent == 0){
      recent_ptr = head_ptr;
    }
    else if(exponent >= current_degree){
      recent_ptr = tail_ptr;
    }
    else if(exponent!=0 && exponent < recent_ptr->exponent() && exponent < current_degree){
      //Iterates backward until the pointer points to the desired node
      polynode *temp_ptr = tail_ptr;
      while(temp_ptr->exponent() != exponent){
	temp_ptr->back();
      }
      recent_ptr = temp_ptr;
    }
    else if(exponent!=0 && exponent > recent_ptr->exponent() && exponent < current_degree){
      //Iterates forward until the pointer points to the desired node
      polynode *temp_ptr = recent_ptr;
      while(temp_ptr->exponent() != exponent){
	temp_ptr->fore();
      }
      recent_ptr = temp_ptr;
    }
  }


  
}
  
