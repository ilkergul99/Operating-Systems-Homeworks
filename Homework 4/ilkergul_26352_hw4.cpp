#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int main()
{

  ifstream countFile("loremipsum.txt");
  if(countFile.is_open())
  {
    int count = 0;
    char ch;
    while(countFile.get(ch))
    {
      if(ch == 'a')
      {
        count++;
      }

    }
    cout << "The occurrence of character ’a’ in the file is " << count << endl;
  }
  countFile.close();
  return 0;
}
