#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <map>
#include <ctime>
#include <vector>
#include <limits.h>

using namespace std;

class Contact{
public:
    char last_name[50];
    char first_name[50];
    char middle_name[50];
    char phone[50];
    char email[50];
};

int main(int atgc, char **argv)
{
    fstream config;
    char path[100];
    char P[100];      //path to the folder
    char confPath[100]; //path to the configuration file
    strcpy(P, argv[1]);
    multimap <string, string> FileList; //list of lists-files
    multimap <string, string>::iterator it = FileList.begin();
    DIR *dirp;
    struct dirent *dp;
    strcpy(path, P);
    strcat(path, "/ab.cfg");
    strcpy (confPath, path);
    config.open(path);
    int ind;
    //checking if the configuration file is open correctly
    if (config.is_open()){
        //if it is, we continue
        string buf1, buf2;
        int n = 0;
        ind = n;
        //reading data from the configuration file
        multimap <string, string>::iterator iter = FileList.begin();
        while (config >> buf1){
            config >> buf2; // " - "
            config >> buf2; // the name of the file
            FileList.insert(pair<string, string>(buf1, buf2)); //creating a list listname-filename
            multimap <string, string>::iterator iterat = FileList.begin();
            //checking if the name of the list is repeated more than once
            while (iterat != FileList.end()){
				const char *tm = iterat->first.c_str();
				char t[20];
				strcpy(t,tm);
				const char *ttm = buf1.c_str();
				char tt[20];
				strcpy(tt,ttm);
                if (!strcmp(tt,t) && iterat!=iter && iter!=FileList.begin() && iterat!=FileList.begin()){
                    cout << "Error! The same ListName detected!" << endl;
                    exit(1);
                }
                iterat++;
            }
            //counting the files
            n++;
            ind++;
            iter++;
        }
        int flag[n];
        for (int i = 0; i < n; i++)
            flag[i] = 0;
        dirp = opendir(P);
        dp = readdir(dirp);
        //checking files of the folder
        while (dp != NULL){
            int fl = 0;
            int i = 0;
            for(it = FileList.begin(); it != FileList.end(); it++){
                i++;
                const char* tm = it->second.c_str();
                char t[20];
                strcpy(t, tm);
                if (!strcmp(dp->d_name,t)){
					cout<<1;
                    fl = 1;
                    flag[i] = 1;
                    break;
                }
            }
            if ((fl == 0) && strcmp(dp->d_name,"ab.cfg") && strcmp(dp->d_name,"..") && strcmp(dp->d_name,".") && strcmp(dp->d_name,"ab.cfg~")){
                cout << "Caution! The file is existing, but not listed!:" << dp->d_name << endl;
            }
            dp = readdir(dirp);
        }
        (void)closedir(dirp);
        for(int i = 0; i < n; i++){
            if (!flag[i]){
                it = FileList.begin();
                for(int j = 0; j < n-i+1; j++){
                    it++;
                }
                cout<< "Caution! The file is listed, but not existing!:" << it->second << endl;
            }
            if (!flag[i]){
                it = FileList.begin();
                for(int j = 0; j < n - i + 1; j++){
                    it++;
                }
                FileList.erase(it);
                n--;
            }
        }

        //the main part of the interface

        char cmd[100];
        //reading a command while not exited
        while(1){
            cin >> cmd;
            if(!strcmp(cmd, "new_list")){                                               //new_list function
                char name[20];
                int sign = 0;
                do {
                    if(cin >> name){
						sign = 0;
                        //check if this name is unique:
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char* tm = it->first.c_str();
							char tt[20];
							strcpy(tt,tm);
                            if (!strcmp(name, tt)){
                                sign = 1;
                                cout << "Caution! The list with this name already exists! Please, pick up another name." << endl;
                                break;
                            }
                        }
                        if (sign == 1)
                            continue;
                        //create a random name:
                        int tr = ind;
                        string fn = "list";
                        int co = tr;
                        while(co%10){
							fn+=static_cast<char>(co/10+'0');
							co/=10;
						}
						fn+=static_cast<char>(tr%10+'0');
						fn+=".txt";
                        //create this file:
                        strcpy(path, P);
                        strcat(path, "/");
                        const char *ttm = fn.c_str();
                        char ttt[50];
                        strcpy(ttt,ttm);
                        strcat(path,ttt);
                        fstream flist;
                        flist.open(path, ios_base::app);
                        flist.close();
                        //add to the list of name-files:
                        FileList.insert(FileList.begin(), pair<string, string>(name, fn));
                        n++;
                        ind++;
                        break;
                    } else {
                        cout << "Please, enter the name of the list" << endl;
                    }
                }while(1);
                //rewriting configuration file
                config.close();
                config.open(confPath, ios_base::in | ios_base::trunc | ios_base::out);
                for(it = FileList.begin(); it != FileList.end(); it++){
                    config << it->first << " - " << it->second << endl;
                }

            }else if(!strcmp(cmd, "new_contact")){                                        //new_contact function
                vector <Contact> ContList;
                vector <Contact>::iterator Cit = ContList.begin();
                fstream flist;
                char listname[20];
                char Lname[20];
                char Fname[20];
                char Mname[20];
                char ph[20];
                char em[20];
                int nn = 0;
                do {
                    if(cin >> listname){
                        nn = 0;
                        //checking if this list really exists
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char *first = it->first.c_str();
							char fir[20];
							strcpy(fir, first);
                            if(!strcmp(listname, fir)){
								const char *second = it->second.c_str();
                                strcpy(listname, second);
                                //opening the file
                                strcpy(path, P);
                                strcat(path, "/");
                                strcat(path, listname);
                                flist.open(path, ios_base::in | ios_base::out);
                                //creating a vector for contacts in this list:
                                while (!flist.eof()){
                                    flist >> Lname;
                                    flist >> Fname;
                                    flist >> Fname;
                                    flist >> Mname;
                                    flist >> Mname;
                                    flist >> ph;
                                    flist >> ph;
                                    flist >> em;
                                    flist >> em;
                                    Contact tmp;
                                    strcpy(tmp.last_name, Lname);
                                    strcpy(tmp.first_name, Fname);
                                    strcpy(tmp.middle_name, Mname);
                                    strcpy(tmp.phone, ph);
                                    strcpy(tmp.email, em);
                                    ContList.insert(Cit, tmp);
                                }
                                //reading new contact data:
                                cin >> Lname;
                                cin >> Fname;
                                cin >> Mname;
                                cin >> ph;
                                cin >> em;
                                // looking for a right place
                                Cit = ContList.begin();
                                while((strcmp(Lname, Cit->last_name) < 0) && (Cit != ContList.end())){
                                      Cit++;
                                }
                                //inserting this contact
                                Contact tmp;
                                strcpy(tmp.last_name, Lname);
                                strcpy(tmp.first_name, Fname);
                                strcpy(tmp.middle_name, Mname);
                                strcpy(tmp.phone, ph);
                                strcpy(tmp.email, em);
                                ContList.insert(Cit, tmp);
                            } else{
                                nn++;
                            }
                        }
                        if (nn == n){
                            cout << "Caution! Not existing ListName! Please try again." << endl;
                            continue;
                        }
                        break;
                    } else {
                        cout << "Please, enter ListName, Last_Name, First_Name, Middle_Name, Phone_number, Email" << endl;
                    }
                }while(1);
                //rewriting the file:
                flist.close();
                flist.open(path, ios_base::out | ios_base::trunc);
                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                    flist << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                    flist << " , " << Cit->phone << " , " << Cit->email << endl;
                }
                flist.close();

            }else if(!strcmp(cmd, "delete_list")){                                        //delete_list function
                char name[20];
                char file_name[100];
                do {
                    if(cin >> name){
                        //check if this name exists:
                        //and if exists remember the filename and delete from the list of files
                        int f = 0;
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char *first = it->first.c_str();
                            if (!strcmp(name,first)){
                                f = 1;
                                const char *sec = it->second.c_str();
                                strcpy(file_name, sec);
                                FileList.erase(it);
                                n--;
                                break;
                            }
                        }
                        if (!f){
                            cout << "Caution! This list is not existing! Please try again!" << endl;
                            continue;
                        }
                        //delete this file:
                        strcpy(path, P);
                        strcat(path, "/");
                        strcat(path,file_name);
                        remove(path);
                        break;
                    } else {
                        cout << "Please, enter the name of the list" << endl;
                    }
                }while(1);
                //rewriting configuration file
                config.close();
                config.open(confPath, ios_base::in | ios_base::trunc | ios_base::out);
                for(it = FileList.begin(); it != FileList.end(); it++){
                    config << it->first << " - " << it->second << endl;
                }

            }else if(!strcmp(cmd, "delete_contact")){                                        //delete_contact function
                vector <Contact> ContList;
                vector <Contact>::iterator Cit = ContList.begin();
                fstream flist;
                char listname[20];
                char Lname[20];
                char Fname[20];
                char Mname[20];
                char ph[20];
                char em[20];
                int num;
                int nn = 0;
                int i;
                do {
                    if(cin >> listname){
                        nn = 0;
                        //checking if the list exists
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char *first = it->first.c_str();
                            if(!strcmp(listname, first)){
                                //opening file
                                i = 1;
                                const char *second = it->second.c_str();
                                strcpy(listname, second);
                                strcpy(path, P);
                                strcat(path, "/");
                                strcat(path, listname);
                                flist.open(path, ios_base::in);
                                while (!flist.eof()){
                                    flist >> Lname;
                                    flist >> Fname;
                                    flist >> Fname;
                                    flist >> Mname;
                                    flist >> Mname;
                                    flist >> ph;
                                    flist >> ph;
                                    flist >> em;
                                    flist >> em;
                                    Contact tmp;
                                    strcpy(tmp.last_name, Lname);
                                    strcpy(tmp.first_name, Fname);
                                    strcpy(tmp.middle_name, Mname);
                                    strcpy(tmp.phone, ph);
                                    strcpy(tmp.email, em);
                                    ContList.insert(Cit, tmp);
                                }
                                //showing contacts:
                                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                                    cout << i << ") " << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                                    cout << " , " << Cit->phone << " , " << Cit->email << endl;
                                    i++;
                                }
                                //reading the number
                                while(1){
                                    if (!(cin >> num))
                                    {
                                        cout << "Caution! Wrong input! Please type in a number!" << endl;
                                        continue;
                                    }
                                    //creating a vector for contacts in this list:
                                    if (i < num){
                                        cout << "Caution! Wrong number! Please try again!" << endl;
                                        continue;
                                    }
                                    break;
                                }
                                //deleting the contact
                                Cit = ContList.begin();
                                for (i = 1; i < num; i++){
                                    Cit++;
                                }
                                ContList.erase(Cit);
                            } else{
                                nn++;
                            }
                        }
                        if (nn == n){
                            cout << "Caution! Not existing ListName! Please try again." << endl;
                            continue;
                        }
                        break;
                    } else {
                        cout << "Please, enter ListName and the number of the contact" << endl;
                    }
                }while(1);
                //rewriting the file:
                flist.close();
                flist.open(path, ios_base::out | ios_base::trunc);
                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                    flist << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                    flist << " , " << Cit->phone << " , " << Cit->email << endl;
                }
                flist.close();

            }else if(!strcmp(cmd, "show_lists")){                                        //show_lists function
                for(it = FileList.begin(); it != FileList.end(); it++){
                    cout << it->first << " - " << it->second << endl;
                }

            }else if(!strcmp(cmd, "show_contacts")){                                        //show_contacts function
                vector <Contact> ContList;
                vector <Contact>::iterator Cit = ContList.begin();
                char listname[20];
                char Lname[20];
                char Fname[20];
                char Mname[20];
                char ph[20];
                char em[20];
                int nn = 0;
                do {
                    if(cin >> listname){
                        nn = 0;
                        //checking if the list exists
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char *first = it->first.c_str();
                            if(!strcmp(listname, first)){
                                //opening file
                                const char *second = it->second.c_str();
                                strcpy(listname, second);
                                fstream flist;
                                strcpy(path, P);
                                strcat(path, "/");
                                strcat(path, listname);
                                flist.open(path, ios_base::in);
                                //creating a vector for contacts in this list:
                                while (!flist.eof()){
                                    flist >> Lname;
                                    flist >> Fname;
                                    flist >> Fname;
                                    flist >> Mname;
                                    flist >> Mname;
                                    flist >> ph;
                                    flist >> ph;
                                    flist >> em;
                                    flist >> em;
                                    Contact tmp;
                                    strcpy(tmp.last_name, Lname);
                                    strcpy(tmp.first_name, Fname);
                                    strcpy(tmp.middle_name, Mname);
                                    strcpy(tmp.phone, ph);
                                    strcpy(tmp.email, em);
                                    ContList.insert(Cit, tmp);
                                }
                                flist.close();
                            } else{
                                nn++;
                            }
                        }
                        if (nn == n){
                            cout << "Caution! Not existing ListName! Please try again." << endl;
                            continue;
                        }
                        break;
                    } else {
                        cout << "Please, enter ListName" << endl;
                    }
                }while(1);
                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                    cout << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                    cout << " , " << Cit->phone << " , " << Cit->email << endl;
                }

            }else if(!strcmp(cmd, "rename_list")){                                        //rename_list function
                string listname;
                string newlistname;
                int nn = 0;
                do {
                    if(cin >> newlistname){
                        while(1){
                            cin >> listname;
                            nn = 0;
                            //checking if this list really exists
                            for(it = FileList.begin(); it != FileList.end(); it++){
								const char *first = it->first.c_str();
								const char *lin = listname.c_str();
                                if(!strcmp(lin, first)){
                                    //changing the previous name to the new one
                                    FileList.erase(it);
                                    FileList.insert(pair<string, string>(newlistname, listname));
                                } else{
                                    nn++;
                                }
                            }
                            if (nn == n){
                                cout << "Caution! Not existing ListName! Please try again." << endl;
                                continue;
                            }
                            break;
                        }
                        break;
                    } else {
                        cout << "Please, enter New ListName and Old ListName" << endl;
                    }
                }while(1);
                //rewriting configuration file
                config.close();
                config.open(confPath, ios_base::in | ios_base::trunc | ios_base::out);
                for(it = FileList.begin(); it != FileList.end(); it++){
                    config << it->first << " - " << it->second << endl;
                }

            }else if(!strcmp(cmd, "edit_contact")){                                        //edit_contact function
                vector <Contact> ContList;
                vector <Contact>::iterator Cit = ContList.begin();
                fstream flist;
                char listname[20];
                char Lname[20];
                char Fname[20];
                char Mname[20];
                char ph[20];
                char em[20];
                char newLname[20];
                char newFname[20];
                char newMname[20];
                char newph[20];
                char newem[20];
                int num;
                int nn = 0;
                int i;
                do {
                    if(cin >> listname){
                        nn = 0;
                        //checking if the list exists
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char *first = it->first.c_str();
                            if(!strcmp(listname, first)){
                                //opening file
                                i = 1;
                                const char *second = it->second.c_str();
                                strcpy(listname, second);
                                strcpy(path, P);
                                strcat(path, "/");
                                strcat(path, listname);
                                flist.open(path, ios_base::in);
                                while (!flist.eof()){
                                    flist >> Lname;
                                    flist >> Fname;
                                    flist >> Fname;
                                    flist >> Mname;
                                    flist >> Mname;
                                    flist >> ph;
                                    flist >> ph;
                                    flist >> em;
                                    flist >> em;
                                    Contact tmp;
                                    strcpy(tmp.last_name, Lname);
                                    strcpy(tmp.first_name, Fname);
                                    strcpy(tmp.middle_name, Mname);
                                    strcpy(tmp.phone, ph);
                                    strcpy(tmp.email, em);
                                    ContList.insert(Cit, tmp);
                                }
                                //showing contacts:
                                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                                    cout << i << ") " << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                                    cout << " , " << Cit->phone << " , " << Cit->email << endl;
                                    i++;
                                }
                                //reading the number
                                while(1){
                                    if (!(cin >> num))
                                    {
                                        cout << "Caution! Wrong input! Please type in a number!" << endl;
                                        continue;
                                    }
                                    //creating a vector for contacts in this list:
                                    if (i < num){
                                        ContList.clear();
                                        cout << "Caution! Wrong number! Please try again!" << endl;
                                        continue;
                                    }
                                    break;
                                }
                                //changing the contact
                                Cit = ContList.begin();
                                for (i = 1; i < num; i++){
                                    Cit++;
                                }
                                strcpy(Lname, Cit->last_name);
                                strcpy(Fname, Cit->first_name);
                                strcpy(Mname, Cit->middle_name);
                                strcpy(ph, Cit->phone);
                                strcpy(em, Cit->email);
                                cin >> newLname;
                                if(strcmp(newLname, " "))
                                    strcpy(Lname, newLname);
                                cin >> newFname;
                                if(strcmp(newFname, " "))
                                    strcpy(Fname, newFname);
                                cin >> newMname;
                                if(strcmp(newMname, " "))
                                    strcpy(Mname, newMname);
                                cin >> newph;
                                if(strcmp(newph, " "))
                                    strcpy(ph, newph);
                                cin >> newem;
                                if(strcmp(newem, " "))
                                    strcpy(em, newem);
                                ContList.erase(Cit);
                                Contact tmp;
                                strcpy(tmp.last_name, Lname);
                                strcpy(tmp.first_name, Fname);
                                strcpy(tmp.middle_name, Mname);
                                strcpy(tmp.phone, ph);
                                strcpy(tmp.email, em);
                                ContList.insert(Cit, tmp);
                            } else{
                                nn++;
                            }
                        }
                        if (nn == n){
                            cout << "Caution! Not existing ListName! Please try again." << endl;
                            continue;
                        }
                        break;
                    } else {
                        cout << "Please, enter ListName, the number, and the data" << endl;
                    }
                }while(1);
                //rewriting the file:
                flist.close();
                flist.open(path, ios_base::out | ios_base::trunc);
                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                    flist << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                    flist << " , " << Cit->phone << " , " << Cit->email << endl;
                }
                flist.close();

            }else if(!strcmp(cmd, "find_contact")){                                        //find_contact function
                vector <Contact> ContList;
                vector <Contact>::iterator Cit = ContList.begin();
                char listname[20];
                char Lname[20];
                char Fname[20];
                char Mname[20];
                char ph[20];
                char em[20];
                int nn = 0;
                do {
                    if(cin >> listname){
                        nn = 0;
                        //checking if the list exists
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char *first = it->first.c_str();
                            if(!strcmp(listname, first)){
                                //opening file
                                const char *second = it->second.c_str();
                                strcpy(listname, second);
                                fstream flist;
                                strcpy(path, P);
                                strcat(path, "/");
                                strcat(path, listname);
                                flist.open(path, ios_base::in);
                                while (!flist.eof()){
                                    flist >> Lname;
                                    flist >> Fname;
                                    flist >> Fname;
                                    flist >> Mname;
                                    flist >> Mname;
                                    flist >> ph;
                                    flist >> ph;
                                    flist >> em;
                                    flist >> em;
                                    Contact tmp;
                                    strcpy(tmp.last_name, Lname);
                                    strcpy(tmp.first_name, Fname);
                                    strcpy(tmp.middle_name, Mname);
                                    strcpy(tmp.phone, ph);
                                    strcpy(tmp.email, em);
                                    ContList.insert(Cit, tmp);
                                }
                                //reading the contact
                                cin >> Lname;
                                cin >> Fname;
                                cin >> Mname;
                                //looking for the contact
                                for (Cit=ContList.begin(); Cit != ContList.end(); Cit++){
                                    if (strcmp(Lname, " ")){
                                        if (strcmp(Lname, Cit->last_name))
                                            break;
                                        if (strcmp(Fname, " ")){
                                            if (strcmp(Fname, Cit->first_name))
                                                break;
                                            if (strcmp(Mname, " ")){
                                                if (strcmp(Mname, Cit->middle_name))
                                                    break;
                                            }
                                        }

                                    } else {
                                        if (strcmp(Fname, " ")){
                                            if (strcmp(Fname, Cit->first_name))
                                                break;
                                            if (strcmp(Mname, " ")){
                                                if (strcmp(Mname, Cit->middle_name))
                                                    break;
                                            }
                                        } else {
                                            if (strcmp(Mname, " ")){
                                                if (strcmp(Mname, Cit->middle_name))
                                                    break;
                                            }
                                        }
                                    }
								}
                                    cout << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                                    cout << " , " << Cit->phone << " , " << Cit->email << endl;
                            } else{
                                nn++;
                            }
                        }
                        if (nn == n){
                            cout << "Caution! Not existing ListName! Please try again." << endl;
                            continue;
                        }
                        break;
                    } else {
                        cout << "Please, enter ListName and the data" << endl;
                    }
                }while(1);

            }else if(!strcmp(cmd, "make_list_copy")){                                        //make_list_copy function
                vector <Contact> ContList;
                vector <Contact>::iterator Cit = ContList.begin();
                char listname[20];
                string newlistname;
                char Lname[20];
                char Fname[20];
                char Mname[20];
                char ph[20];
                char em[20];
                int nn = 0;
                int sign = 0;
                do {
                    if(cin >> listname){
                        nn = 0;
                        //checking if this list really exists
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char *first = it->first.c_str();
							const char *second = it->second.c_str();
                            if(!strcmp(listname, first)){
                                strcpy(listname, second);
                                fstream flist;
                                strcpy(path, P);
                                strcat(path, "/");
                                strcat(path, listname);
                                flist.open(path, ios_base::in);
                                while (!flist.eof()){
                                    flist >> Lname;
                                    flist >> Fname;
                                    flist >> Fname;
                                    flist >> Mname;
                                    flist >> Mname;
                                    flist >> ph;
                                    flist >> ph;
                                    flist >> em;
                                    flist >> em;
                                    Contact tmp;
                                    strcpy(tmp.last_name, Lname);
                                    strcpy(tmp.first_name, Fname);
                                    strcpy(tmp.middle_name, Mname);
                                    strcpy(tmp.phone, ph);
                                    strcpy(tmp.email, em);
                                    ContList.insert(Cit, tmp);
                                }
                            } else{
                                nn++;
                            }
                        }
                        if (nn == n){
                            cout << "Caution! Not existing ListName! Please try again." << endl;
                            continue;
                        }
                        do {
                            if(cin >> newlistname){
                                //check if this name is unique:
                                for(it = FileList.begin(); it != FileList.end(); it++){
									const char *nln = newlistname.c_str();
									const char *cmp = it->first.c_str();
                                    if (!strcmp(nln, cmp)){
                                        sign = 1;
                                        cout << "Caution! The list with this name already exists! Please, pick up another name." << endl;
                                        break;
                                    }
                                }
                                if (sign==1)
                                    continue;
                                //create a random name:
								int tr = ind;
								string fn = "list";
								int co = tr;
								while(co%10){
									fn+=static_cast<char>(co/10+'0');
									co/=10;
								}
								fn+=static_cast<char>(tr%10+'0');
								fn+=".txt";
								//create this file:
								strcpy(path, P);
								strcat(path, "/");
								const char *ttm = fn.c_str();
								char ttt[50];
								strcpy(ttt,ttm);
								strcat(path,ttt);
								fstream flist;
								flist.open(path, ios_base::app);
								flist.close();
								//add to the list of name-files:
								FileList.insert(FileList.begin(), pair<string, string>(newlistname, fn));
								n++;
								ind++;
                                break;
                            } else {
                                cout << "Please, enter the name of the list" << endl;
                            }
                        }while(1);
                        break;
                    } else {
                        cout << "Please, enter ListName and Copy ListName" << endl;
                    }
                }while(1);
                //rewriting configuration file
                config.close();
                config.open(confPath, ios_base::in | ios_base::trunc | ios_base::out);
                for(it = FileList.begin(); it != FileList.end(); it++){
                    config << it->first << " - " << it->second << endl;
                }

            }else if(!strcmp(cmd, "make_contact_copy")){                                        //make_contact_copy function
                vector <Contact> ContList;
                vector <Contact>::iterator Cit = ContList.begin();
                char listname[20];
                char movlist[20];
                char Lname[20];
                char Fname[20];
                char Mname[20];
                char ph[20];
                char em[20];
                char Lname1[20];
                char Fname1[20];
                char Mname1[20];
                char ph1[20];
                char em1[20];
                Contact tmp;
                int num;
                int nn = 0;
                int nn1 = 0;
                int i;
                do {
                    if(cin >> listname){
                        nn = 0;
                        //checking if the list exists
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char* first = it->first.c_str();
                            if(!strcmp(listname, first)){
                                //opening file
                                i = 1;
                                const char *second = it->second.c_str();
                                strcpy(listname, second);
                                fstream flist;
                                strcpy(path, P);
                                strcat(path, "/");
                                strcat(path, listname);
                                flist.open(path, ios_base::in);
                                while (!flist.eof()){
                                    flist >> Lname;
                                    flist >> Fname;
                                    flist >> Fname;
                                    flist >> Mname;
                                    flist >> Mname;
                                    flist >> ph;
                                    flist >> ph;
                                    flist >> em;
                                    flist >> em;
                                    strcpy(tmp.last_name, Lname);
                                    strcpy(tmp.first_name, Fname);
                                    strcpy(tmp.middle_name, Mname);
                                    strcpy(tmp.phone, ph);
                                    strcpy(tmp.email, em);
                                    ContList.insert(Cit, tmp);
                                }
                                flist.close();
                                //showing contacts:
                                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                                    cout << i << ") " <<Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                                    cout << " , " << Cit->phone << " , " << Cit->email << endl;
                                    i++;
                                }
                                //reading the number
                                while(1){
                                    if (!(cin >> num))
                                    {
                                        cout << "Caution! Wrong input! Please type in a number!" << endl;
                                        continue;
                                    }
                                    //creating a vector for contacts in this list:
                                    if (i < num){
                                        cout << "Caution! Wrong number! Please try again!" << endl;
                                        continue;
                                    }
                                    break;
                                }
                                //remember the contact's data
                                Cit = ContList.begin();
                                for (i = 1; i < num; i++){
                                    Cit++;
                                }
                                strcpy(Lname, Cit->last_name);
                                strcpy(Fname, Cit->first_name);
                                strcpy(Mname, Cit->middle_name);
                                strcpy(ph, Cit->phone);
                                strcpy(em, Cit->email);
                                do {
                                    if(cin >> movlist){
                                        nn1 = 0;
                                        //checking if this list really exists
                                        for(it = FileList.begin(); it != FileList.end(); it++){
											const char *str1 = it->first.c_str();
											const char *str2 = it->second.c_str();
                                            if(!strcmp(movlist, str1)){
                                                strcpy(movlist, str2);
                                                //opening the file
                                                fstream flist;
                                                strcpy(path, P);
                                                strcat(path, "/");
                                                strcat(path, movlist);
                                                flist.open(path, ios_base::in);
                                                //creating a vector for contacts in this list:
                                                while (!flist.eof()){
                                                    flist >> Lname1;
                                                    flist >> Fname1;
                                                    flist >> Fname1;
                                                    flist >> Mname1;
                                                    flist >> Mname1;
                                                    flist >> ph1;
                                                    flist >> ph1;
                                                    flist >> em1;
                                                    flist >> em1;
                                                    strcpy(tmp.last_name, Lname1);
                                                    strcpy(tmp.first_name, Fname1);
                                                    strcpy(tmp.middle_name, Mname1);
                                                    strcpy(tmp.phone, ph1);
                                                    strcpy(tmp.email, em1);
                                                    ContList.insert(Cit, tmp);
                                                }
                                                // looking for a right place
                                                Cit = ContList.begin();
                                                while((strcmp(Lname, Cit->last_name) < 0) && (Cit != ContList.end())){
                                                    Cit++;
                                                }
                                                //inserting this contact
                                                strcpy(tmp.last_name, Lname);
                                                strcpy(tmp.first_name, Fname);
                                                strcpy(tmp.middle_name, Mname);
                                                strcpy(tmp.phone, ph);
                                                strcpy(tmp.email, em);
                                                ContList.insert(Cit, tmp);
                                            } else{
                                                nn++;
                                            }
                                        }
                                        if (nn == n){
                                            cout << "Caution! Not existing ListName! Please try again." << endl;
                                            continue;
                                        }
                                        break;
                                    } else {
                                        cout << "Please, enter Copy ListName" << endl;
                                    }
                                }while(1);
                                //rewriting the file:
                                flist.close();
                                flist.open(path, ios_base::out | ios_base::trunc);
                                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                                    flist << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                                    flist << " , " << Cit->phone << " , " << Cit->email << endl;
                                }
                                flist.close();
                            } else{
                                nn++;
                            }
                        }
                        if (nn == n){
                            cout << "Caution! Not existing ListName! Please try again." << endl;
                            continue;
                        }
                        break;
                    } else {
                        cout << "Please, enter ListName and the number of the contact" << endl;
                    }
                }while(1);

            }else if(!strcmp(cmd, "move_contact")){                                        //move_contact function
                vector <Contact> ContList;
                vector <Contact>::iterator Cit = ContList.begin();
                vector <Contact> ContList1;
                vector <Contact>::iterator Cit1 = ContList1.begin();
                char listname[20];
                char movlist[20];
                char Lname[20];
                char Fname[20];
                char Mname[20];
                char ph[20];
                char em[20];
                char Lname1[20];
                char Fname1[20];
                char Mname1[20];
                char ph1[20];
                char em1[20];
                int num;
                int nn = 0;
                int nn1 = 0;
                int i;
                Contact tmp;
                fstream flist;
                fstream flist1;
                do {
                    if(cin >> listname){
                        nn = 0;
                        //checking if the list exists
                        for(it = FileList.begin(); it != FileList.end(); it++){
							const char *first = it->first.c_str();
                            if(!strcmp(listname, first)){
                                //opening file
                                i = 1;
                                const char *second = it->second.c_str();
                                strcpy(listname, second);
                                strcpy(path, P);
                                strcat(path, "/");
                                strcat(path, listname);
                                flist.open(path, ios_base::in);
                                while (!flist.eof()){
                                    flist >> Lname;
                                    flist >> Fname;
                                    flist >> Fname;
                                    flist >> Mname;
                                    flist >> Mname;
                                    flist >> ph;
                                    flist >> ph;
                                    flist >> em;
                                    flist >> em;
                                    strcpy(tmp.last_name, Lname);
                                    strcpy(tmp.first_name, Fname);
                                    strcpy(tmp.middle_name, Mname);
                                    strcpy(tmp.phone, ph);
                                    strcpy(tmp.email, em);
                                    ContList.insert(Cit, tmp);
                                }
                                //showing contacts:
                                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                                    cout << i << ") " <<Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                                    cout << " , " << Cit->phone << " , " << Cit->email << endl;
                                    i++;
                                }
                                //reading the number
                                while(1){
                                    if (!(cin >> num))
                                    {
                                        cout << "Caution! Wrong input! Please type in a number!" << endl;
                                        continue;
                                    }
                                    //creating a vector for contacts in this list:
                                    if (i < num){
                                        cout << "Caution! Wrong number! Please try again!" << endl;
                                        continue;
                                    }
                                    break;
                                }
                                //remember the contact's data
                                Cit = ContList.begin();
                                for (i = 1; i < num; i++){
                                    Cit++;
                                }
                                strcpy(Lname, Cit->last_name);
                                strcpy(Fname, Cit->first_name);
                                strcpy(Mname, Cit->middle_name);
                                strcpy(ph, Cit->phone);
                                strcpy(em, Cit->email);
                                ContList.erase(Cit);
                                do {
                                    if(cin >> movlist){
                                        nn1 = 0;
                                        //checking if this list really exists
                                        for(it = FileList.begin(); it != FileList.end(); it++){
											const char *str1 = it->first.c_str();
											const char *str2 = it->second.c_str();
                                            if(!strcmp(movlist, str1)){
                                                strcpy(movlist, str2);
                                                //opening the file
                                                strcpy(path, P);
                                                strcat(path, "/");
                                                strcat(path, movlist);
                                                flist1.open(path, ios_base::in);
                                                //creating a vector for contacts in this list:
                                                while (!flist1.eof()){
                                                    flist1 >> Lname1;
                                                    flist1 >> Fname1;
                                                    flist1 >> Fname1;
                                                    flist1 >> Mname1;
                                                    flist1 >> Mname1;
                                                    flist1 >> ph1;
                                                    flist1 >> ph1;
                                                    flist1 >> em1;
                                                    flist1 >> em1;
                                                    strcpy(tmp.last_name, Lname1);
                                                    strcpy(tmp.first_name, Fname1);
                                                    strcpy(tmp.middle_name, Mname1);
                                                    strcpy(tmp.phone, ph1);
                                                    strcpy(tmp.email, em1);
                                                    ContList1.insert(Cit, tmp);
                                                }
                                                // looking for a right place
                                                Cit1 = ContList1.begin();
                                                while((strcmp(Lname, Cit1->last_name) < 0) && (Cit1 != ContList1.end())){
                                                    Cit1++;
                                                }
                                                //inserting this contact
                                                strcpy(tmp.last_name, Lname);
                                                strcpy(tmp.first_name, Fname);
                                                strcpy(tmp.middle_name, Mname);
                                                strcpy(tmp.phone, ph);
                                                strcpy(tmp.email, em);
                                                ContList1.insert(Cit, tmp);
                                            } else{
                                                nn++;
                                            }
                                        }
                                        if (nn == n){
                                            cout << "Caution! Not existing ListName! Please try again." << endl;
                                            continue;
                                        }
                                        break;
                                    } else {
                                        cout << "Please, enter Copy ListName" << endl;
                                    }
                                }while(1);
                                //rewriting the file:
                                flist1.close();
                                flist1.open(path, ios_base::out | ios_base::trunc);
                                for (Cit1 = ContList1.begin(); Cit1 != ContList1.end(); Cit1++){
                                    flist1 << Cit1->last_name << " , " << Cit1->first_name << " , " << Cit1->middle_name;
                                    flist1 << " , " << Cit1->phone << " , " << Cit1->email << endl;
                                }
                                flist1.close();
                            } else{
                                nn++;
                            }
                        }
                        if (nn == n){
                            cout << "Caution! Not existing ListName! Please try again." << endl;
                            continue;
                        }
                        break;
                    } else {
                        cout << "Please, enter ListName and the number of the contact" << endl;
                    }
                }while(1);
                flist.close();
                flist.open(path, ios_base::out | ios_base::trunc);
                for (Cit = ContList.begin(); Cit != ContList.end(); Cit++){
                    flist << Cit->last_name << " , " << Cit->first_name << " , " << Cit->middle_name;
                    flist << " , " << Cit->phone << " , " << Cit->email << endl;
                }
                flist.close();

            }else if(!strcmp(cmd, "exit")){                                        //exit function
                    config.close();
                    return 0;

            }else{                                                                  //Unknown command
                cout << "Caution! Unknown command! Please try again." << endl;
                continue;
            }
        }
    } else{
    //if it's not we close the program
        cout << "Error with opening configuration file." << endl;
        exit(1);
    }

    return 0;
}
