#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

//Matrix for plane seats
int plane_seats[2][50];
//Variable for busy waiting process
int turn = 0;
//For checking how many empty seats left in future
int emptySeats = 100;
//For having a loop in threads
bool filling_seat = true;

//Respected thread function
//I want to make the function more modular to be used by both agent
//I will check the loop by having a boolean variable
//If there is a reservation then decrement an empty seat
//By the agent id parameter I will decide which agent will do the related instructions
void *ReservationSys(void *param)
{
  int * agentptr = (int*) param;
  if(*agentptr == 1)
  {
    while(filling_seat == true)
    {

      //To generate random number
      int random_integer = (rand() % 100) + 1;
      //In future I will convert the number to a row and column integers
      while(turn != 0);
      cout << "Agency 1 Entered Critical Region" << endl;
      //Checking for first row
      if(random_integer <= 50)
      {
        if(plane_seats[0][random_integer - 1] == 0)
        {
          plane_seats[0][random_integer - 1] = 1;
          cout << "Seat Number " << random_integer << " is reserved by Agency 1" << endl;
          emptySeats--;

        }
      }
      //Checking for second row
      else
      {
        if(plane_seats[1][random_integer - 51] == 0)
        {
          plane_seats[1][random_integer - 51] = 1;
          cout << "Seat Number " << random_integer << " is reserved by Agency 1" << endl;
          emptySeats-- ;

        }
      }
      //I add this part to be very similar to the sample output in guideline
      // Since we have two threads and a main thread I inhibit the unwanted scheduling
      // output which includes more than 1 empty search output
      // even though we have only 1 due to busy waiting in the sample
      // I am also checking in main thread but I declined any empty search output after
      // no empty seat due to concurrency of threads
      if(emptySeats != 0 && filling_seat == true)
      {
        cout << "Agency 1 Exit Critical Region" << endl << endl;
      }
      else if(emptySeats == 0 && filling_seat == true)
      {
        cout << "Agency 1 Exit Critical Region" << endl << endl;
        filling_seat = false;
      }
      else
      {
        cout << "Agency 1 Exit Critical Region" << endl << "No Seats Left"<< endl << endl;
      }
      turn = 1;
    }
  }
  else
  {
    while(filling_seat == true)
    {

      //To generate random number
      int random_integer = (rand() % 100) + 1;
      while(turn != 1);
      cout << "Agency 2 Entered Critical Region" << endl;
      //Checking for first row
      if(random_integer <= 50)
      {
        if(plane_seats[0][random_integer - 1] == 0)
        {
          plane_seats[0][random_integer - 1] = 2;
          cout << "Seat Number " << random_integer << " is reserved by Agency 2" << endl;
          emptySeats-- ;
        }
      }
      //Checking for second row
      else
      {
        if(plane_seats[1][random_integer - 51] == 0)
        {
          plane_seats[1][random_integer - 51] = 2;
          cout << "Seat Number " << random_integer << " is reserved by Agency 2" << endl;
          emptySeats-- ;
        }
      }
      if(emptySeats != 0 && filling_seat == true)
      {
        cout << "Agency 2 Exit Critical Region" << endl << endl;
      }
      else if(emptySeats == 0 && filling_seat == true)
      {
        cout << "Agency 2 Exit Critical Region" << endl << endl;
        filling_seat = false;
      }
      else
      {
        cout << "Agency 2 Exit Critical Region" << endl << "No Seats Left" << endl << endl;
      }
      turn = 0;
    }
  }
}

int main()
{
//To generate different random numbers as stated in the guideline
  srand(time(NULL));
  bool matrix_checker = true;

//Child threads that I will use in this homework
  pthread_t TravelAgency1, TravelAgency2;
  // ID number for each agent
  int agent1 = 1;
  int agent2 = 2;
//To fill global array that I assigned with 0 as an initial state as stated in the guideline
  for(int i = 0; i <=1; i++)
  {
    for(int j = 0; j < 50; j++)
    {
      plane_seats[i][j] = 0;
    }
  }
  // To create child threads I will use the creation function
  pthread_create(&TravelAgency1, NULL, &ReservationSys, (void*) &agent1);
  pthread_create(&TravelAgency2, NULL, &ReservationSys, (void*) &agent2);

  //To check the number of empty Seats
  while(matrix_checker == true)
  {
    if(emptySeats == 0)
    {
      filling_seat = false;
      matrix_checker = false;
    }
    else
    {}
  }

  //To join threads
  pthread_join(TravelAgency1, NULL);
  pthread_join(TravelAgency2, NULL);

  //To print the last version of the matrix output that is wanted
  cout << "Plane is full:\n";
  for(int i = 0; i <=1; i++)
  {
    for(int j = 0; j < 50; j++)
    {
        cout << plane_seats[i][j];
    }
    cout << endl;
  }

  return 0;
}
