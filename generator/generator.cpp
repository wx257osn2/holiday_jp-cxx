#include<iostream>
#include<string>

int main(){
  std::string str;
  for(std::getline(std::cin, str); not std::cin.eof(); std::getline(std::cin, str))
    std::cout << '"' << str << "\\n\"\n";
}
