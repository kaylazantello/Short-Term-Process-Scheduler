/*********************************************************
Project 1: Improved UNIX/Linux Command Line Interpreter
Ethan Macalaguim and Kayla Zantello
CST-315: Operating Systems
February 20, 2022
Commands implemented:
quit
ls
pwd
mkdir
rmdir
df
free
date
cat
cd
Execute multiple commands by entering a ; between commands
************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;
namespace fs = std::experimental::filesystem;

// returns path of current directory as a string
string pwd() {
    fs::path p = fs::current_path();   // use filesystem library to get current path
    return p.string();  // convert the variable from type path object to string and return
}

int main() {

    string input;
    vector<vector<string>> commands{};
    vector<string> temp{};
    vector<string> cmd{};

    while(1) {
        
        // display user input prompt
        cout << "\n> ";

        commands.clear();  // clear command vector for new input
        temp.clear();  // clear temp vector for new input

        getline(cin, input);  // get entire input entered by user (including spaces) and store in string variable called input
        size_t pos = 0;
        
        // while a semicolon is present in string
        while((pos = input.find_first_of(';')) != string::npos) {
            temp.push_back(input.substr(0, pos));  // insert a substring of input from 0 to the semicolon into temp
            input.erase(0, pos + 1);  // erase that same string, plus the semicolon, from input
        }
        temp.push_back(input);  // insert the remaining input string into temp

        // for each string vector in temp
        for(int i = 0; i < temp.size(); i++) {
            cmd.clear();  // clear the cmd vector for new input
            
            // while a blank character is present in the string
            while((pos = temp[i].find_first_of(' ')) != string::npos) {
                // erase any leading blank characters before a string
                if(temp[i].substr(0, pos).size() == 0) { 
                    temp[i].erase(0, pos + 1);
                    continue; 
                }
                cmd.push_back(temp[i].substr(0, pos));  // insert a substring of temp[i] from 0 to the blank character into cmd
                temp[i].erase(0, pos + 1);  // erase that same string, plus the trailing blank character, from temp[i]
            }
            // erase any trailing blank characters after a string
            if(temp[i].size() == 0) {
                commands.push_back(cmd);
                continue;
            }
            cmd.push_back(temp[i]);  // insert remaining string at temp[i] into cmd
            commands.push_back(cmd);  // insert cmd into commands
        }

        // print commands vector & other details (for debugging purposes)
        /*cout << "size of commands vector: " << commands.size() << endl;
        for(int i = 0; i < commands.size(); i++) {
            cout << "commands[" << i << "] (" << commands[i].size() << ") = ";
            for(int j = 0; j < commands[i].size(); j++) {
                cout << "|" << commands[i][j] << "|" << " (" << commands[i][j].size() << ")" << " ";
            }
            cout << endl;
        }*/
        
        // for each cmd vector in commands
        for(int i = 0; i < commands.size(); i++) {

            // exit command line interpreter program
            if(commands[i][0] == "quit") {
                return 0;
            }

            // print path of current directory
            if(commands[i][0] == "pwd") {
                cout << pwd() << endl;
                continue;
            }

            // display list of the current directory's contents
            if(commands[i][0] == "ls") {
                system(commands[i][0].c_str());  // convert command from string to char array, then use system() to execute command
                continue;
            }

            // print the current date
            if(commands[i][0] == "date") {
                system(commands[i][0].c_str());
                continue;
            }

            // create a new emtpy directory within current directory, name of new directory is specified by user
            if(commands[i][0] == "mkdir") {
                mkdir(commands[i][1].c_str(), 0777);  // convert user's desired directory name to char array
                continue;
            }

            // remove an empty directory specified by user
            if(commands[i][0] == "rmdir") {
                // construct path string of directory to delete by adding directory name to the end of the current path
                string dirPath = pwd().append("/");
                dirPath.append(commands[i][1]);

                // convert path from string to char array, if rmdir() returns 0, then removal was successful
                if(rmdir(dirPath.c_str()) == 0) {
                    continue;
                }
                else {
                    cout << "rmdir: failed to remove \'" << commands[i][1] << "\'" << endl;  // notify user if unsuccessful
                    continue;
                }
            }

            // change the current working directory
            if(commands[i][0] == "cd") {
                chdir(commands[i][1].c_str());
                continue;
            }

            // concatenate and print files
            if(commands[i][0] == "cat") {
                if(commands[i][1] == ">") {
                    string newCommand = commands[i][0] + " > " + commands[i][2];
                    system(newCommand.c_str());
                    continue;
                }
                else {
                    string newCommand2 = commands[i][0] + " " + commands[i][1];
                    system(newCommand2.c_str());
                    continue;
                }
            }

            // display storage usage
            if(commands[i][0] == "df") {
                system(commands[i][0].c_str());
                continue;
            }

            // display RAM availability
            if(commands[i][0] == "free") {
                system(commands[i][0].c_str());
                continue;
            }

            if(commands[i][0] == "./scheduler") {
                system(commands[i][0].c_str());
                continue;
            }
            
        }
    
    }
    return 0;
}