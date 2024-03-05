#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    Network();
    ~Network();

    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                     const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename);
    int num_clients;
    int num_commands;
    vector<stack<Packet*>> clients_all;


    void message_function(string sender, string receiver, string message, vector<Client> &clients, int message_limit,
                          const string &sender_port, const string &receiver_port);



    string return_time();



    void print_log_entries(const string& client, vector<Client> &clients);

    string activiy_status(ActivityType activityType);


    void show_frame_info(string client, string postion, int index, vector<Client> &clients);

    void print_frame(stack<Packet *> frame, int frame_number, string message_data);


    stack<Packet *> reverseStack(stack<Packet *> &originalStack);

    void show_q_info(const string& client, const string& position, vector<Client> &clients);

    void send(vector<Client> &clients);

    bool check_message_end(stack<Packet *> packet);

    ApplicationLayerPacket * return_application_layer(stack<Packet *> packet);

    void receive(vector<Client> &clients);


    bool client_not_exist(vector<Client> &clients, Client &client, const string& receiver_id);
};

#endif  // NETWORK_H
