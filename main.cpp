#include "main.h"
#include "Restaurant.cpp"
#include <fstream>

int MAXSIZE;

void simulate(string filename, imp_res* r)
{
    ifstream ss(filename);
    string str, maxsize, name, energy, num;
    while (ss >> str)
    {
        if (str == "MAXSIZE")
        {
            ss >> maxsize;
            MAXSIZE = stoi(maxsize);
        }
        else if (str == "RED") // RED <NAME> <ENERGY>
        {
            ss >> name;
            ss >> energy;
            r->RED(name, stoi(energy));
        }
        else if (str == "BLUE") // BLUE <NUM>
        {
            ss >> num;
            r->BLUE(stoi(num));
        }
        else if (str == "PURPLE") // PURPLE
        {
            r->PURPLE();
        }
        else if (str == "REVERSAL") // REVERSAL
        {
            r->REVERSAL();
        }
        else if (str == "UNLIMITED_VOID") // UNLIMITED_VOID
        {
            r->UNLIMITED_VOID();
        }
        else if (str == "DOMAIN_EXPANSION") // DOMAIN_EXPANSION
        {
            r->DOMAIN_EXPANSION();
        }
        else // LIGHT <NUM>
        {
            ss >> num;
            r->LIGHT(stoi(num));
        }
    }
}

bool check1(int& a, int& b) { return a <= b; }
bool equal(int& a, int& b) { return a == b; }

bool check(int& a) { return a < 0; }
void op(int& a) { cout << a;  }
void swap(int& a, int& b) {
    int temp = a; 
    a = b; 
    b = temp;
}

int main(int argc, char* argv[]) {

    ////if (argc < 2)
    ////	return 1;
    ////  string fileName = argv[1];
    imp_res* r = new imp_res();
    string fileName = "test.txt";
    simulate(fileName, r);
    delete r;
    /*MAXSIZE = 6;
    imp_res* r = new imp_res();
    r->RED("ABC", 1);
    r->RED("AB", 2);
    r->RED("GOJO", -5);
    r->RED("SUKUNA", 6);
    r->RED("KIET", -4);
    r->RED("SIEUNHAN", -1);

    r->RED("LAM", 10);
    r->RED("mn", -8);
    r->RED("hs", -19);
    r->RED("VINH", 20);

    r->RED("NARUTO", 7);
    r->RED("LUFFY", -2);
    r->LIGHT(1);
    r->REVERSAL(); 
    cout << "......." << endl;
    r->LIGHT(1);
    r->UNLIMITED_VOID();*/
    /*cout << "......." << endl;
    r->PURPLE();
    r->LIGHT(1); 
    cout << "......." << endl;
    r->DOMAIN_EXPANSION();
    cout << "........" << endl;
    r->LIGHT(1);*/



    ///*cout << "...." << endl;
    //r->PURPLE();
    //r->DOMAIN_EXPANSION();
    //cout << "..." << endl;
    //r->LIGHT(1);
    //cout << "......." << endl;*/
    //r->UNLIMITED_VOID();
   /* r->LIGHT(1); 
    cout << "................" << endl;
    r->DOMAIN_EXPANSION();
    cout << "............" << endl; 
    r->LIGHT(1);
    cout << "............." << endl;
    r->BLUE(5); */
    /*r->LIGHT(1);
    r->LIGHT(1); 
    cout << "......" << endl;
    r->LIGHT(-1);
    cout << "......" << endl;
    r->LIGHT(0);
    r->UNLIMITED_VOID();
    r->LIGHT(1);*/
    //delete r;


    return 0;
}

