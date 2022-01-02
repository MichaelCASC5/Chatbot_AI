/*
    * C H A T B O T . C P P
    * F R O M :
    * VERTEX OF HUNTER
    * 
    * This program learns your personality and imitates you!
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <time.h>
using namespace std;

string capitalize(string str){
    string response = str;
    char fc = response[0];
    fc = toupper(fc);
    response = fc + str.substr(1);

    return response;
}

string shorten(string str){
    string output = "";
    char save = ' ';
    int count = 0;
    for(int i=0;i<str.length();i++){
        char c;
        c = str[i];

        if(c == save){
            count++;
        }else{
            count = 0;
        }
        if(count < 1){
            output+=c;
        }

        save = c;
    }

    return output;
}

string cleanup(string input){
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    input = shorten(input);
    
    if(input.substr(input.length(),1) == " "){
        input = input.substr(0,input.length()-1);
    }
    
    for(int i=0;i<input.length();i++){
        char c = input[i];

        if(!isalnum(c)){
            input.erase(std::remove(input.begin(), input.end(), c), input.end());
        }
    }

    return input;
}

double search(string temp, string input){
    double count = 0;

    double temp_count = 0;
    double input_count = 0;
    bool stop_input_count = false;

    string input_save = input;

    //GETTING TEMP TOKENS
    string temp_token;
    while(temp.length() > 0){
        int n = temp.find(" ");

        if(n == -1){
            n = temp.size();

            temp_token = temp.substr(0,n);
            temp = "";
        }else{
            temp_token = temp.substr(0,n);
            temp = temp.substr(n+1);
        }

        temp_count++;

        input = input_save;
        
        //GETTING INPUT TOKENS
        string input_token;
        while(input.length() > 0){
            int x = input.find(" ");
            
            if(x == -1){
                x = input.size();

                input_token = input.substr(0,x);
                input = "";
            }else{
                input_token = input.substr(0,x);
                input = input.substr(x+1);
            }
            
            if(!stop_input_count){
                input_count++;
            }
            
            input_token = cleanup(input_token);
            temp_token = cleanup(temp_token);

            //COMPARING TOKENS
            if(temp_token == input_token){
                count++;
            }
        }
        
        stop_input_count = true;
    }

    int greatest;
    if(input_count > temp_count){
        greatest = input_count;
    }else{
        greatest = temp_count;
    }

    return count/greatest;
}

int main(int argc, char const *argv[]){
    srand(time(NULL));
    
    ifstream fin("brain");
    if(fin.fail()){
        cerr << "File cannot be opened for reading." << endl;
        cerr << "Creating new file." << endl;

        ofstream file("brain");
        file << "";
        file.close();
        
        ifstream fin("brain");
    }

    ifstream f(argv[1]);
    string line;

    vector <string> data;

    while(getline(fin,line)){
        data.push_back(line);
    }

    cout << "Welcome to C H A T B O T . C P P\n" << endl;
    cout << "/new : Starts a new conversation.\n/exit : Exit the program.\n* * *\n=====\nSay hi!\n" << endl;
    
    string newline = "";
    bool write = false;
    while(true){
        cout << "\nYou:" << endl;
        string input;
        getline(cin, input);
        cout << endl;

        //Updates the brain
        if(write){
            data.push_back(newline);
        }
        write = false;
        ofstream file("brain");
        for(int i=0;i<data.size();i++){
            file << data[i] << endl;
        }
        file.close();

        if(input == "/new"){
            newline = "";

            cout << "\n\nNEW CONVO STARTED\n\n* * *\n=====" << endl;
        }else if(input == "/exit"){
            exit(1);
        }else{
            string temp;
            vector <string> res;
            for(int i=0;i<data.size();i++){
                line = data[i];

                bool ready = false;
                
                while(line.length() > 0){
                    int n = line.find("::");
                    
                    temp = line.substr(0,n);
                    line = line.substr(n+2);
                    
                    if(ready){
                        if(temp.length() > 1){
                            res.push_back(temp);
                        }
                        
                        ready = false;
                    }
                    
                    if(search(temp,input) >= 0.75){
                    // if(temp == input){
                        ready = true;
                    }
                }
            }

            int len;
            len = res.size();

            //Removing copies from possible responses
            string a;
            string b;
            for(int i=0;i<len;i++){
                a = cleanup(res[i]);
                
                for(int j=i+1;j<len;j++){
                    b = cleanup(res[j]);

                    if(a == b){
                        res.erase(res.begin() + j);
                        j--;
                        len--;
                    }
                }
            }
            
            int r = 1;
            string response;
            if(len > 0){
                r = int(rand() % len);

                response = capitalize(res[r]);

                cout << "Bot: [LVL " << data.size() << "]" << endl;
                cout << response << endl;

                newline+=input + "::";
                newline+=res[r]+ "::";
            }else{
                response = capitalize(input);
                
                cout << "Bot: [LVL " << data.size() << "]" << endl;
                cout << response << endl;
                
                vector <string> datacopy;
                for(int i=0;i<data.size();i++){
                    // cout << newline << endl;
                    // cout << data[i] << endl;
                    if(data[i] != newline){
                        datacopy.push_back(data[i]);
                    }
                    /*else{
                        cout << "found" << endl;
                    }*/
                }
                
                data = datacopy;

                newline+=input + "::";
                write = true;
            }
        }
    }
    
    return 0;
}