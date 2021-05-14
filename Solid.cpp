#include<iostream>
#include<cmath>
#include<FL/Fl.H>
#include<FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Timer.H>
#include <FL/Fl_Button.H>
#include <bits/stdc++.h>
#include<random>
using namespace std ;

double time_step = 0.1 ;

bool trigger = false ;

class Shape : public Fl_Widget{
	protected:
	double x, y, w, h;
	public:
		Shape (int x, int y, int w, int h) : Fl_Widget (x,y,w,h){
			this->x=x; this->y=y;
			this->w=w; this->h=h;
		}
		virtual void wipe()=0;
		virtual void refresh(double dx, double dy)=0;
};

class Circle : public Shape{
	private:
	
	public:
	double x,y,r ;
	double v_x , v_y ;
	double a_x , a_y ;
	Circle(int x , int y , double r ) : Shape(x,y,2*ceil(r),2*ceil(r)){
		this->x = x ;
		this->y = y ;
		this->r = r ;
		v_x = 0;
		v_y=0;
		a_x=0;
		a_y=0;		
	}
	virtual void draw(){
		fl_color(fl_rgb_color(0,0,99));
		fl_pie(x-r,y-r,2*r,2*r,0,360) ;
	}
	virtual void wipe(){		
		fl_color(fl_rgb_color(193,193,193));
		fl_pie(x-r,y-r,2*r,2*r,0,360) ;		
	}
	virtual void refresh( double dx , double dy ){
		wipe();
		x += dx ;
		y += dy ;
		fl_color(fl_rgb_color(0,0,99));
		fl_pie(x-r,y-r,2*r,2*r,0,360) ;
	}
	virtual void refresh(){	
		wipe(); 
		v_x += a_x*time_step ;
		v_y += a_y*time_step ;
		x += v_x*time_step ;
		y += v_y*time_step;
		a_x=0;
		a_y=0;
		 
		fl_color(fl_rgb_color(0,0,99));
		fl_pie(x-r,y-r,2*r,2*r,0,360) ;
	}
	
	
};

typedef std::pair<Circle* , Circle*> link ;

class Wrapper{
	private:
		int num_circles ;
		int num_links ;
		int d_num_links ;
		Circle* c[100] ;
		link* nhbrs;
		link* d_nhbrs ;
		double spring_const  ;	
		double natural_length ;	
		double d_spring_const  ; 
		double d_natural_length ;
		double eta ;
	public:
		Wrapper( int n_c , Circle* circles[], int n_l , link link_list[] , int d_n_l , link d_link_list[] , double sc  , double nat_len ,double d_sc, double d_nat_len , double eta1){
			num_circles = n_c ;
			for(int i = 0 ; i < num_circles ; i++){
				c[i] = circles[i] ;
			}
			num_links = n_l ;
			nhbrs = new link[num_links] ;
			for(int i = 0 ; i < num_links ; i++ ){
				nhbrs[i].first = link_list[i].first ;
				nhbrs[i].second = link_list[i].second ;
			}
			d_num_links = d_n_l ;
			d_nhbrs = new link[d_num_links];
			for(int i = 0 ; i < d_num_links ; i++ ){
				d_nhbrs[i].first = d_link_list[i].first ;
				d_nhbrs[i].second = d_link_list[i].second ;
			}
			spring_const = sc;
			natural_length = nat_len ;
			eta = eta1;	
			d_spring_const = d_sc;
			d_natural_length = d_nat_len ;		
		}
		
		void refreshAll(){
			
			if(trigger){
				
				c[45]->refresh(45,45);
				trigger = false ;
			}
			
			for(int i=0 ; i<num_links ; i++ ){
			
				double ext = sqrt((nhbrs[i].second->x	- nhbrs[i].first->x )*(nhbrs[i].second->x	- nhbrs[i].first->x ) + (nhbrs[i].second->y	- nhbrs[i].first->y )*(nhbrs[i].second->y	- nhbrs[i].first->y )) - natural_length;
				double x_proj = (nhbrs[i].second->x	- nhbrs[i].first->x )/sqrt((nhbrs[i].second->x	- nhbrs[i].first->x )*(nhbrs[i].second->x	- nhbrs[i].first->x ) + (nhbrs[i].second->y	- nhbrs[i].first->y )*(nhbrs[i].second->y	- nhbrs[i].first->y ));
				double y_proj = (nhbrs[i].second->y	- nhbrs[i].first->y )/sqrt((nhbrs[i].second->x	- nhbrs[i].first->x )*(nhbrs[i].second->x	- nhbrs[i].first->x ) + (nhbrs[i].second->y	- nhbrs[i].first->y )*(nhbrs[i].second->y	- nhbrs[i].first->y )); 
				nhbrs[i].first->a_x += spring_const*x_proj*ext;
				nhbrs[i].first->a_y += spring_const*y_proj*ext;
				nhbrs[i].second->a_x += -spring_const*x_proj*ext;
				nhbrs[i].second->a_y += -spring_const*y_proj*ext;
			}
			
			for(int i=0 ; i <d_num_links ; i++ ){
				
				double ext = sqrt((d_nhbrs[i].second->x	- d_nhbrs[i].first->x )*(d_nhbrs[i].second->x	- d_nhbrs[i].first->x ) + (d_nhbrs[i].second->y	- d_nhbrs[i].first->y )*(d_nhbrs[i].second->y	- d_nhbrs[i].first->y )) - d_natural_length;
				double x_proj = (d_nhbrs[i].second->x	- d_nhbrs[i].first->x )/sqrt((d_nhbrs[i].second->x	- d_nhbrs[i].first->x )*(d_nhbrs[i].second->x	- d_nhbrs[i].first->x ) + (d_nhbrs[i].second->y	- d_nhbrs[i].first->y )*(d_nhbrs[i].second->y	- d_nhbrs[i].first->y ));
				double y_proj = (d_nhbrs[i].second->y	- d_nhbrs[i].first->y )/sqrt((d_nhbrs[i].second->x	- d_nhbrs[i].first->x )*(d_nhbrs[i].second->x	- d_nhbrs[i].first->x ) + (d_nhbrs[i].second->y	- d_nhbrs[i].first->y )*(d_nhbrs[i].second->y	- d_nhbrs[i].first->y )); 
				d_nhbrs[i].first->a_x += d_spring_const*x_proj*ext;
				d_nhbrs[i].first->a_y += d_spring_const*y_proj*ext;
				d_nhbrs[i].second->a_x += -d_spring_const*x_proj*ext;
				d_nhbrs[i].second->a_y += -d_spring_const*y_proj*ext;
				
			}	
			
			for(int i=0 ; i<num_circles ; i++){
				c[i]->refresh() ;
			}
		
		}
		
			

};

void callBack( void* target){
	
	if(true){
		Wrapper* w = (Wrapper*)target ;		
		w->refreshAll() ;
		Fl::repeat_timeout(time_step, callBack , target )	;
	}
}

class MyTimer{
	private:
		bool run ;
	public:
		MyTimer(){
			run = false ;
		}
		void start(void* target){
			run = true ;
			Fl::repeat_timeout(time_step, callBack , target )	;
		}
		void stop(){
			run = false ;
		}

};

int main(){
	Fl_Window* window = new Fl_Window(1000,1000,"H2O") ;
	Circle* c[100] ;
	link link_list[5050] ;
	link d_link_list[5050] ;
	int top = 0 ;
	int d_top = 0;
	for(int i=0 ; i < 100; i ++){
		c[i] = new Circle(50+100*(i%10),50+100*(i/10),10);
	}
	for(int i=0 ; i< 10 ; i++ ){
		for(int j=0 ; j<10 ; j++){
			if( i<9 ){
				link_list[top].first = c[10*j+i];
				link_list[top].second = c[10*j+i+1];
				top++ ;
			}
			if( j<9 ){
				link_list[top].first = c[10*j+i];
				link_list[top].second = c[10*j+i+10];
				top++ ;
			} 
			if(i<9 && j<9 ){
				d_link_list[d_top].first = c[10*j+i];
				d_link_list[d_top].second = c[10*j+i+10+1];
				d_top++ ;	
			}
			if( j>0 && i<9 ){
				d_link_list[d_top].first = c[10*j+i];
				d_link_list[d_top].second = c[10*j+i-10+1];
				d_top++ ;
			} 
		}
	}
	
	Wrapper* wrapper = new Wrapper(100,c,top,link_list,d_top,d_link_list,0.1,100,0.07,100*sqrt(2),0.01);
	trigger = true;
	MyTimer* t = new MyTimer;
	t->start(wrapper);
	
	window->end();
	window->show();
	Fl::run();

}
