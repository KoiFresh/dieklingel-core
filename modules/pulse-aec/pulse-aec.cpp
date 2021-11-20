// compile with "g++ -o pulse-aec pulse-aec.cpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define WERTE 900
#define RANGE_MIN_MUTE 126.0
#define RANGE_MAX_MUTE 128.0
#define RANGE_MIN_LOUD 126.0
#define RANGE_MAX_LOUD 128.0

int c[WERTE] = {0};
int count = 0;
int sum = 0;
float sum_old = 0;
int zero_count = 0;

int pitch(int first, int second) {
    return std::abs(first - second);
}

int main(void) {
    /*for(int i = 0; i < WERTE; i++) 
    {
        c[i] = 128;
    } */
    system("killall pulse2");
    while(1)
    {
	    if(count < WERTE)
	    {
            c[count] = getchar() - 128;
            count++;	
	        if(count == WERTE)
	        { 
	            count = 0;
	        }
	    }
        sum = 0;
        for(int i = 0; i < WERTE; i++)
        {
            sum += std::abs(c[i]);
        }
        sum = sum / WERTE;

        //std::cout << sum << std::endl;
        
        if( (sum_old == 0 ) && (sum > 0)  ) 
        {
            // positive flanke
            std::cout << "+" << sum << "-" << sum_old << std::endl;
            system("pactl -- set-source-volume 1 10%");
            zero_count = 0;
        }else if( (sum_old > 0) && (sum == 0) ) 
        {
            // negative flanke
            std::cout << "-" << sum << "-" << sum_old << std::endl;
            system("pactl -- set-source-volume 1 30%");
            zero_count = 0;
        } else if( (sum_old == 0) && (sum == 0) ) {
            if(zero_count == 10000) {
                system("pactl -- set-source-volume 1 60%");
                zero_count++;
            }else if(zero_count == 20000) {
                system("pactl -- set-source-volume 1 100%");
                zero_count++;
            }else if(zero_count < 20000){
                zero_count++;
            }    
        }

        

       /* if(((sum_old <= RANGE_MAX_MUTE) && (sum_old >= RANGE_MIN_MUTE)) && ((m_sum > RANGE_MAX_MUTE) || (m_sum < RANGE_MIN_MUTE)))
        {
          std::cout << "30%" << sum << std::endl;
          //system("pactl -- set-source-volume 1 10%");
        }
        else if (((sum_old > RANGE_MAX_LOUD) || (sum_old < RANGE_MIN_LOUD)) && ((m_sum <= RANGE_MAX_LOUD) && ( m_sum >= RANGE_MIN_LOUD )))
        {
          std::cout << "100%" << sum << std::endl;
          //system("pactl -- set-source-volume 1 100%");
        } 
        */
        sum_old = sum;
    }
    return(0);
}
