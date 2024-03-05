#include "Network.h"

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */

    for (const auto &command : commands) {
        int line_lenght = command.size();

        std::istringstream iss(command);
        std::string function;
        iss >> function;

        if (function == "MESSAGE") {
            std::string sender, receiver, message;
            iss >> sender >> receiver;
            std::getline(iss, message);
            for (int i = 0; i < line_lenght+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;

            std::cout <<"Command: "<< function <<" "<< sender << " " << receiver  << message << std::endl;
            for (int i = 0; i < line_lenght+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            message_function(sender,receiver,message,clients,message_limit,sender_port,receiver_port);


        } else if (function == "SHOW_FRAME_INFO") {
            std::string client, position;
            string index;
            iss >> client >> position >> index;
            for (int i = 0; i < line_lenght+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout <<"Command: "<< function << " " << client<<" "<< position <<" " << index << std::endl;

            for (int i = 0; i < line_lenght+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;


            show_frame_info(client,position,std::stoi(index),clients);

        } else if (function == "SHOW_Q_INFO") {
            std::string client, position;
            iss >> client >> position;
            for (int i = 0; i < line_lenght+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout <<"Command: "<< function <<" "<< client << " "<< position << std::endl;
            for (int i = 0; i < line_lenght+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            show_q_info(client,position,clients);


        } else if (function == "SEND") {
            std::cout << "-------------\n"
                         "Command: "<<function << "\n"
                         "-------------"  << std::endl;
            send(clients);


        } else if (function == "RECEIVE") {
            std::cout << "----------------\n"
                         "Command: "<< function << "\n"
                         "----------------" << std::endl;

            receive(clients);



        } else if (function == "PRINT_LOG") {
            std::string client;
            iss >> client;
            std::cout << "--------------------\n"
                         "Command: " << function <<" " << client << "\n"
                         "--------------------" << std::endl;
            print_log_entries(client,clients);




        } else {
            std::string a;
            getline(iss,a);
            for (int i = 0; i < line_lenght+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout <<"Command: "<< function << a << std::endl;
            for (int i = 0; i < line_lenght+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Invalid command." << std::endl;
        }
    }



}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.
    std::ifstream infile(filename);

    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return clients;
    }

    infile >> num_clients;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string id ,ip, name;

        if (iss >> id >> ip >> name) {
            clients.emplace_back(id,ip,name);
        }
    }

    infile.close();
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    std::ifstream infile(filename);

    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    int index = 0;
    std::string line;
    while (std::getline(infile, line)) {

        std::istringstream iss(line);
        std::string first, second;

        if (iss >> first >> second) {
            clients[index].routing_table.insert(std::make_pair(first, second));
        }
        if (!line.empty()){
            if (line.find('-')!= string::npos){
                index++;
            }
        }


    }

    infile.close();



}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.
    std::ifstream infile(filename);

    if (!infile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return commands;
    }

    infile >> num_commands;

    std::string line;
    std::getline(infile, line);
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        commands.push_back(line);
    }

    infile.close();
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.

    for (int i = 0; i < clients_all.size(); ++i) {
        for (int j = 0; j < 4; ++j) {
            delete clients_all[i].top();
            clients_all[i].pop();
        }
    }

}
void Network::message_function(std::string sender,std::string receiver,std::string message,vector<Client> &clients, int message_limit,
                               const string &sender_port, const string &receiver_port){

    vector<string> messages_part;
    size_t startPos = message.find('#');
    if (startPos != std::string::npos) {
        message.erase(0, startPos + 1);
    }
    size_t endPos = message.rfind('#');
    if (endPos != std::string::npos) {
        message.erase(endPos);
    }
    std::cout << "Message to be sent: \"" << message << "\"\n" << std::endl;

    for (int i = 0; i < message.length(); i += message_limit) {
        std::string chunk = message.substr(i, message_limit);
        messages_part.push_back(chunk);
    }
    int frame_number = 1;
    std::string sender_ip,receiver_ip,sender_mac,receiver_mac;

    for (int i = 0; i < messages_part.size(); ++i) {
        std::stack<Packet*> frame;

        for (int j = 0; j < clients.size(); ++j) {
            if (sender == clients[j].client_id){
                sender_ip = clients[j].client_ip;
                sender_mac = clients[j].client_mac;
                for (const auto& entry : clients[j].routing_table) {
                    if (entry.first == receiver){
                        for (int k = 0; k < clients.size(); ++k) {
                            if (entry.second == clients[k].client_id){
                                receiver_mac = clients[k].client_mac;
                            }
                        }
                    }
                }
            }
            if (receiver == clients[j].client_id){
                receiver_ip = clients[j].client_ip;
            }
        }
        Packet *applicationLayerPacket = new ApplicationLayerPacket(0,sender,receiver,messages_part[i]);

        Packet *transportLayerPacket = new TransportLayerPacket(1,sender_port,receiver_port);

        Packet *networkLayerPacket = new NetworkLayerPacket(2,sender_ip,receiver_ip);

        Packet *physicalLayerPacket = new PhysicalLayerPacket(3,sender_mac,receiver_mac);

        frame.push(applicationLayerPacket);
        frame.push(transportLayerPacket);
        frame.push(networkLayerPacket);
        frame.push(physicalLayerPacket);

        for (int j = 0; j < clients.size(); ++j) {
            if (sender == clients[j].client_id){
                clients[j].outgoing_queue.push(frame);
                print_frame(frame,frame_number,messages_part[i]);
            }
        }
        clients_all.push_back(frame);

        frame_number++;


    }


}
void Network::print_frame(std::stack<Packet*> frame,int frame_number,std::string message_data){
    bool print_frame = true;
    while (!frame.empty()) {
        if (print_frame){
            std::cout << "Frame #" << frame_number << std::endl;
            print_frame = false;
        }
        Packet* currentPacket = frame.top();
        currentPacket->print();
        frame.pop();
    }
    std::cout << "Message chunk carried: \""<< message_data <<"\"" << std::endl;
    std::cout << "Number of hops so far: 0" << std::endl;
    std::cout << "--------" << std::endl;

}
std::string Network::return_time(){
    time_t current = time(nullptr);
    struct tm *timeinfo = localtime(&current);
    char buffer[20];

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    return buffer;
}

void Network::print_log_entries(const std::string& client,vector<Client> &clients){
    if (!clients.empty()){
        for (int i = 0; i < clients.size(); ++i) {
            if (clients[i].client_id == client){
                if (!clients[i].log_entries.empty()){
                    std::cout << "Client "<< client << " Logs:"<< std::endl;
                    for (int j = 0; j < clients[i].log_entries.size(); ++j) {
                        std::cout << "--------------\n"
                                     "Log Entry #"<< j+1 <<":\n" <<
                                  "Activity: "<< activiy_status(clients[i].log_entries[j].activity_type) << "\n" <<
                                  "Timestamp: " << clients[i].log_entries[j].timestamp<< "\n" <<
                                  "Number of frames: " <<clients[i].log_entries[j].number_of_frames <<"\n" <<
                                  "Number of hops: "<< clients[i].log_entries[j].number_of_hops <<"\n" <<
                                  "Sender ID: "<< clients[i].log_entries[j].sender_id <<"\n" <<
                                  "Receiver ID: " << clients[i].log_entries[j].receiver_id << "\n" <<
                                  "Success: " << (clients[i].log_entries[j].success_status ? "Yes" : "No") << std::endl;
                        if (activiy_status(clients[i].log_entries[j].activity_type) == "Message Sent" || activiy_status(clients[i].log_entries[j].activity_type) == "Message Received"){
                            std:: cout <<"Message: \""<< clients[i].log_entries[j].message_content <<"\"" << std::endl;
                        }

                    }

                }
            }

        }
    }
}

std::string Network::activiy_status(ActivityType activityType){
    if (activityType == ActivityType::MESSAGE_SENT){
        return "Message Sent";
    } else if (activityType == ActivityType::MESSAGE_DROPPED){
        return "Message Dropped";
    }else if (activityType == ActivityType::MESSAGE_FORWARDED){
        return "Message Forwarded";
    }else if (activityType == ActivityType::MESSAGE_RECEIVED){
        return "Message Received";
    }
}

void Network::show_frame_info(std::string client,std::string position,int index,vector<Client> &clients) {
    for (int i = 0; i < clients.size(); ++i) {
        if (clients[i].client_id == client) {
            if (position == "out"){
                if (!clients[i].outgoing_queue.empty() && index <= clients[i].outgoing_queue.size() ) {
                    int current_log = 0;
                    std::queue<stack<Packet*>> tempQueue = clients[i].outgoing_queue;
                    for (int j = 0; j < index-1; ++j) {
                        if (check_message_end(tempQueue.front())){
                            current_log++;
                        }
                        tempQueue.pop();
                    }
                    std::cout << "Current Frame #"<< index <<" on the outgoing queue of client "<< client <<"\n" <<
                                 "Carried Message: \"" << return_application_layer(tempQueue.front())->message_data << "\"" << std::endl;
                    stack<Packet*> temp = reverseStack(tempQueue.front());
                    while (!temp.empty()) {
                        std::cout << "Layer "<<temp.top()->layer_ID <<" info: ";
                        temp.top()->print();
                        temp.pop();
                    }


                    if (!clients[i].log_entries.empty()){
                        std::cout << "Number of hops so far: "<< clients[i].log_entries[current_log].number_of_hops << std::endl;
                    } else{
                        std::cout << "Number of hops so far: "<< 0 << std::endl;
                    }

                }else{
                    std::cout << "No such frame." << std::endl;
                }
            }else if (position == "in"){
                if (!clients[i].incoming_queue.empty() && index <= clients[i].incoming_queue.size() ) {
                    int current_log = 0;

                    std::queue<stack<Packet*>> tempQueue = clients[i].incoming_queue;
                    if (index != 1){
                        for (int j = 0; j < index-1; ++j) {
                            if (check_message_end(tempQueue.front())){
                                current_log++;
                            }
                            tempQueue.pop();
                        }
                    }


                    std::cout << "Current Frame #"<< index <<" on the incoming queue of client "<< client <<"\n" <<
                              "Carried Message: \"" << return_application_layer(tempQueue.front())->message_data << "\"" << std::endl;
                    stack<Packet*> temp = reverseStack(tempQueue.front());
                    while (!temp.empty()) {
                        std::cout << "Layer "<<temp.top()->layer_ID <<" info: ";
                        temp.top()->print();
                        temp.pop();
                    }
                    if (!clients[i].log_entries.empty()){
                        std::cout << "Number of hops so far: "<< clients[i].log_entries[current_log].number_of_hops+1 << std::endl;

                    } else{
                        std::cout << "Number of hops so far: "<< 1 << std::endl;

                    }

                } else{
                    std::cout << "No such frame." << std::endl;
                }
            }

        }

    }
}

void Network::show_q_info(const std::string& client, const std::string& position,vector<Client> &clients){
    for (int i = 0; i < clients.size(); ++i) {
        if (clients[i].client_id == client){
            if (position == "in"){
                std::cout << "Client "<< client <<" Incoming Queue Status\n" <<
                             "Current total number of frames: "<< clients[i].incoming_queue.size() << std::endl;
            }else if (position == "out"){
                std::cout << "Client "<< client <<" Outgoing Queue Status\n" <<
                          "Current total number of frames: "<< clients[i].outgoing_queue.size() << std::endl;
            }
        }
    }
}

void Network::send(vector<Client> &clients){
    for (int i = 0; i < clients.size(); ++i) {
        int frame_number = 1;
        int current_log = 0;
        std::string message;

        while (!clients[i].outgoing_queue.empty()){

            stack<Packet*> temp = clients[i].outgoing_queue.front();

            ApplicationLayerPacket *temp_app = return_application_layer(clients[i].outgoing_queue.front());

            std::string new_receiver;
            for (const auto& entry : clients[i].routing_table){
                if (entry.first == temp_app->receiver_ID){
                    for (int o = 0; o < clients.size(); ++o) {
                        if (clients[o].client_id == entry.second){
                            new_receiver = clients[o].client_id;
                        }
                    }
                }
            }


            if (check_message_end(clients[i].outgoing_queue.front())){

                std::cout << "Client "<< clients[i].client_id <<" sending frame #"<< frame_number <<" to client " << new_receiver << std::endl;


                while (!temp.empty()) {
                    temp.top()->print();
                    temp.pop();
                }

                for (int j = 0; j < clients[i].log_entries.size(); ++j) {
                    if (clients[i].log_entries[j].activity_type == ActivityType::MESSAGE_FORWARDED){
                        if (clients[i].log_entries[j].message_content == message){
                            current_log = j;
                        }
                    }
                }


                std::cout << "Message chunk carried: \"" << temp_app->message_data << "\"" << std::endl;
                if (!clients[i].log_entries.empty()){
                    std::cout << "Number of hops so far: " << clients[i].log_entries[current_log].number_of_hops+1 << std::endl;
                } else{
                    std::cout << "Number of hops so far: " << 1 << std::endl;

                }

                for (const auto& entry : clients[i].routing_table) {
                    if (entry.first == temp_app->receiver_ID){
                        for (int k = 0; k < clients.size(); ++k) {
                            if (entry.second == clients[k].client_id){
                                clients[k].incoming_queue.push(clients[i].outgoing_queue.front());
                                clients[i].outgoing_queue.pop();
                            }
                        }
                    }
                }
                message += temp_app->message_data;




                bool exist = true;

                for (int j = 0; j < clients[i].log_entries.size(); ++j) {
                        if (clients[i].log_entries[j].message_content == message && clients[i].log_entries[j].activity_type == ActivityType::MESSAGE_FORWARDED){
                            exist = false;
                    }
                }

                if (exist){
                    try {
                        if (!clients[i].log_entries.empty()){
                            Log log1(return_time(),message,frame_number,clients[i].log_entries[current_log].number_of_hops,temp_app->sender_ID,temp_app->receiver_ID, true,ActivityType(ActivityType::MESSAGE_SENT));
                            clients[i].log_entries.push_back(log1);

                        } else{
                            Log log1(return_time(),message,frame_number,0,temp_app->sender_ID,temp_app->receiver_ID, true,ActivityType(ActivityType::MESSAGE_SENT));
                            clients[i].log_entries.push_back(log1);

                        }

                    } catch (const std::bad_alloc& e) {

                        std::cerr << "Caught bad_alloc exception: " << e.what() << std::endl;

                    }
                }


                message.clear();

                frame_number = 1;
                std::cout << "--------" << std::endl;
            } else{
                std::cout << "Client "<< clients[i].client_id <<" sending frame #"<< frame_number <<" to client " << new_receiver << std::endl;

                while (!temp.empty()) {
                    temp.top()->print();
                    temp.pop();
                }


                for (int j = 0; j < clients[i].log_entries.size(); ++j) {
                    if (clients[i].log_entries[j].activity_type == ActivityType::MESSAGE_FORWARDED){
                        if(clients[i].log_entries[j].message_content.find(temp_app->message_data) != std::string::npos){
                            current_log = j;
                        }
                    }
                }

                std::cout << "Message chunk carried: \"" << temp_app->message_data << "\"" << std::endl;

                if (!clients[i].log_entries.empty()){
                    std::cout << "Number of hops so far: " << clients[i].log_entries[current_log].number_of_hops+1 << std::endl;
                } else{
                    std::cout << "Number of hops so far: " << 1 << std::endl;

                }
                for (const auto& entry : clients[i].routing_table) {
                    if (entry.first == temp_app->receiver_ID){
                        for (int k = 0; k < clients.size(); ++k) {
                            if (entry.second == clients[k].client_id){
                                clients[k].incoming_queue.push(clients[i].outgoing_queue.front());
                                clients[i].outgoing_queue.pop();
                            }
                        }
                    }
                }
                frame_number++;
                message += temp_app->message_data;
                std::cout << "--------" << std::endl;
            }
        }
    }

}
bool Network::check_message_end(stack<Packet*> packet){
    stack<Packet*> temp = reverseStack(packet);

    while (!temp.empty()) {
        if (temp.top()->layer_ID == 0) {

            ApplicationLayerPacket* applicationLayerPacket = dynamic_cast<ApplicationLayerPacket*>(temp.top());

            if (applicationLayerPacket) {

                for (char ch : applicationLayerPacket->message_data) {
                    if (ch == '.' || ch == '?' || ch == '!') {
                        return true;
                    }
                }
                return false;

            }
        } else {
            temp.pop();
        }
    }

}

ApplicationLayerPacket* Network::return_application_layer(stack<Packet*> packet){
    stack<Packet*> temp = reverseStack(packet);
    while (!temp.empty()){
        if (temp.top()->layer_ID == 0){
            ApplicationLayerPacket* applicationLayerPacket = dynamic_cast<ApplicationLayerPacket*>(temp.top());

            if (applicationLayerPacket) {
                return applicationLayerPacket;
            }
        } else {
            temp.pop();
        }
    }

}


std::stack<Packet*> Network::reverseStack(std::stack<Packet*> &originalStack) {
    std::stack<Packet*> auxStack;

    while (!originalStack.empty()) {
        auxStack.push(originalStack.top());
        originalStack.pop();
    }

    return auxStack;
}

void Network::receive(vector<Client> &clients){

    for (int i = 0; i < clients.size(); ++i) {
        int frame_number = 1;
        std::string message;
        while (!clients[i].incoming_queue.empty()){
            for (int m = 0; m < clients.size(); ++m) {
                if (!clients[i].incoming_queue.empty() && dynamic_cast<PhysicalLayerPacket*>(clients[i].incoming_queue.front().top())->sender_MAC_address == clients[m].client_mac){
                    for (const auto& entry : clients[m].routing_table) {
                        if(!clients[i].incoming_queue.empty()){
                            ApplicationLayerPacket *temp_app2 = return_application_layer(clients[i].incoming_queue.front());

                            if (client_not_exist(clients,clients[i],temp_app2->receiver_ID)){


                                if (entry.first == temp_app2->receiver_ID){

                                    if (entry.second == temp_app2->receiver_ID){

                                        //Message received

                                        int current_log = 0;
                                        bool cont = true;

                                        while (!clients[i].incoming_queue.empty() && cont) {

                                            stack<Packet*> temp = clients[i].incoming_queue.front();

                                            ApplicationLayerPacket *temp_app = return_application_layer(clients[i].incoming_queue.front());

                                            std::string new_receiver;
                                            for (const auto& entry1 : clients[i].routing_table){
                                                if (entry1.first == temp_app->receiver_ID){
                                                    for (int o = 0; o < clients.size(); ++o) {
                                                        if (clients[o].client_id == entry1.second){
                                                            new_receiver = clients[o].client_mac;
                                                        }
                                                    }
                                                }
                                            }

                                            if (check_message_end(clients[i].incoming_queue.front())){


                                                std::cout << "Client "<< clients[i].client_id <<" receiving frame #"<< frame_number <<" from client "<< clients[m].client_id<<", originating from client "<<temp_app->sender_ID << std::endl;

                                                while (!temp.empty()) {
                                                    temp.top()->print();
                                                    temp.pop();
                                                }
                                                std::cout << "Message chunk carried: \"" << temp_app->message_data << "\"" << std::endl;
                                                message += temp_app->message_data;

                                                for (int j = 0; j < clients[m].log_entries.size(); ++j) {
                                                    if(clients[m].log_entries[j].message_content == message && clients[m].log_entries[j].receiver_id == temp_app->receiver_ID){
                                                        current_log = j;
                                                    }
                                                }
                                                if (!clients[m].log_entries.empty()){
                                                    std::cout <<  "Number of hops so far: " << clients[m].log_entries[current_log].number_of_hops+1 << std::endl;

                                                } else{
                                                    std::cout <<  "Number of hops so far: " << 1 << std::endl;

                                                }

                                                std::cout << "--------" << std::endl;
                                                std::cout << "Client "<< temp_app->receiver_ID <<" received the message \""<< message <<"\" from client "<< temp_app->sender_ID <<"." << std::endl;
                                                std::cout << "--------" << std::endl;


                                                if (!clients[m].log_entries.empty()){
                                                    try {
                                                        Log log1(return_time(),message,frame_number,clients[m].log_entries[current_log].number_of_hops+1,temp_app->sender_ID,temp_app->receiver_ID,true,ActivityType(ActivityType::MESSAGE_RECEIVED));
                                                        clients[i].log_entries.push_back(log1);
                                                    } catch (const std::bad_alloc& e) {

                                                        std::cerr << "Caught bad_alloc exception: " << e.what() << std::endl;

                                                    }
                                                }


                                                frame_number = 1;
                                                message.clear();


                                                clients[i].incoming_queue.pop();
                                                cont = false;

                                            } else{


                                                std::cout << "Client "<< clients[i].client_id <<" receiving frame #"<< frame_number <<" from client "<< clients[m].client_id<<", originating from client "<<temp_app->sender_ID << std::endl;

                                                while (!temp.empty()) {
                                                    temp.top()->print();
                                                    temp.pop();
                                                }

                                                for (int j = 0; j < clients[m].log_entries.size(); ++j) {
                                                    if(clients[m].log_entries[j].message_content.find(temp_app->message_data) != std::string::npos && clients[m].log_entries[j].receiver_id == temp_app->receiver_ID){
                                                        current_log = j;
                                                    }
                                                }

                                                std::cout << "Message chunk carried: \"" << temp_app->message_data << "\"" << std::endl;
                                                if (!clients[m].log_entries.empty()){
                                                    std::cout <<  "Number of hops so far: " << clients[m].log_entries[current_log].number_of_hops+1 << std::endl;

                                                } else{
                                                    std::cout <<  "Number of hops so far: " << 1 << std::endl;

                                                }

                                                std::cout << "--------" << std::endl;
                                                message += temp_app->message_data;

                                                frame_number++;

                                                clients[i].incoming_queue.pop();

                                            }
                                        }
                                    } else {
                                        //Message Forwarded

                                        bool print_line = true;
                                        int current_log = 0;
                                        bool cont = true;


                                        while (!clients[i].incoming_queue.empty() && cont) {
                                            stack<Packet*> temp = clients[i].incoming_queue.front();
                                            ApplicationLayerPacket *temp_app = return_application_layer(clients[i].incoming_queue.front());
                                            std::string new_receiver;
                                            for (const auto& entry1 : clients[i].routing_table){
                                                if (entry1.first == temp_app->receiver_ID){
                                                    for (int o = 0; o < clients.size(); ++o) {
                                                        if (clients[o].client_id == entry1.second){
                                                            new_receiver = clients[o].client_mac;
                                                        }
                                                    }
                                                }
                                            }

                                            if (check_message_end(clients[i].incoming_queue.front())){


                                                if (print_line){
                                                    std::cout << "Client "<< clients[i].client_id <<" receiving a message from client "<< clients[m].client_id <<
                                                              ", but intended for client "<< temp_app->receiver_ID<<". Forwarding... "<< std::endl;
                                                    print_line = false;
                                                }


                                                message += temp_app->message_data;

                                                for (int j = 0; j < clients[m].log_entries.size(); ++j) {
                                                    if(clients[m].log_entries[j].message_content == message && clients[m].log_entries[j].receiver_id == temp_app->receiver_ID){
                                                        current_log = j;
                                                    }
                                                }
                                                if (!clients[m].log_entries.empty()){
                                                    try {
                                                        Log log1(return_time(),message,frame_number,clients[m].log_entries[current_log].number_of_hops+1,temp_app->sender_ID,temp_app->receiver_ID,
                                                                 true,ActivityType(ActivityType::MESSAGE_FORWARDED));
                                                        clients[i].log_entries.push_back(log1);
                                                    } catch (const std::bad_alloc& e) {

                                                        std::cerr << "Caught bad_alloc exception: " << e.what() << std::endl;

                                                    }
                                                }

                                                std::cout << "Frame #" << frame_number <<" MAC address change: New sender MAC "<<clients[i].client_mac <<", new receiver MAC "<< new_receiver << std::endl;
                                                frame_number = 1;
                                                std::cout << "--------" << std::endl;


                                                dynamic_cast<PhysicalLayerPacket*>(clients[i].incoming_queue.front().top())->sender_MAC_address = clients[i].client_mac;
                                                dynamic_cast<PhysicalLayerPacket*>(clients[i].incoming_queue.front().top())->receiver_MAC_address = new_receiver;

                                                clients[i].outgoing_queue.push(clients[i].incoming_queue.front());
                                                clients[i].incoming_queue.pop();
                                                print_line = true;
                                                message.clear();
                                                cont = false;

                                            } else{


                                                if (print_line){

                                                    std::cout << "Client "<< clients[i].client_id <<" receiving a message from client "<< clients[m].client_id <<
                                                              ", but intended for client "<< temp_app->receiver_ID<<". Forwarding... "<< std::endl;
                                                    print_line = false;
                                                }


                                                std::cout << "Frame #" << frame_number <<" MAC address change: New sender MAC "<<clients[i].client_mac <<", new receiver MAC "<< new_receiver << std::endl;
                                                frame_number++;


                                                message += temp_app->message_data;


                                                dynamic_cast<PhysicalLayerPacket*>(clients[i].incoming_queue.front().top())->sender_MAC_address = clients[i].client_mac;
                                                dynamic_cast<PhysicalLayerPacket*>(clients[i].incoming_queue.front().top())->receiver_MAC_address = new_receiver;

                                                clients[i].outgoing_queue.push(clients[i].incoming_queue.front());
                                                clients[i].incoming_queue.pop();

                                            }
                                        }
                                    }

                                }
                            } else{ // Message Drop

                                int current_log = 0;
                                bool cont = true;
                                while (!clients[i].incoming_queue.empty() && cont) {

                                    stack<Packet*> temp = clients[i].incoming_queue.front();
                                    ApplicationLayerPacket *temp_app = return_application_layer(clients[i].incoming_queue.front());

                                    std::string new_receiver;
                                    for (const auto& entry1 : clients[i].routing_table){
                                        if (entry1.first == temp_app->receiver_ID){
                                            for (int o = 0; o < clients.size(); ++o) {
                                                if (clients[o].client_id == entry1.second){
                                                    new_receiver = clients[o].client_mac;
                                                }
                                            }
                                        }
                                    }

                                    if (check_message_end(clients[i].incoming_queue.front())){



                                        std::cout << "Client "<< clients[i].client_id <<" receiving frame #"<< frame_number <<" from client "<<
                                                  clients[m].client_id<<", but intended for client "<< temp_app->receiver_ID <<". Forwarding... " << std::endl;

                                        message += temp_app->message_data;

                                        for (int j = 0; j < clients[m].log_entries.size(); ++j) {
                                            if(clients[m].log_entries[j].message_content == message && clients[m].log_entries[j].receiver_id == temp_app->receiver_ID){
                                                current_log = j;
                                            }
                                        }
                                        if (!clients[m].log_entries.empty()){
                                            std::cout << "Error: Unreachable destination. Packets are dropped after "<< clients[m].log_entries[current_log].number_of_hops + 1 <<" hops!"  << std::endl;

                                        } else{
                                            std::cout << "Error: Unreachable destination. Packets are dropped after "<< 1 <<" hops!"  << std::endl;

                                        }
                                        std::cout << "--------" << std::endl;



                                        if (!clients[m].log_entries.empty()){
                                            try {
                                                Log log1(return_time(),message,frame_number,clients[m].log_entries[current_log].number_of_hops+1,temp_app->sender_ID,temp_app->receiver_ID,
                                                         false,ActivityType(ActivityType::MESSAGE_DROPPED));
                                                clients[i].log_entries.push_back(log1);
                                            } catch (const std::bad_alloc& e) {

                                                std::cerr << "Caught bad_alloc exception: " << e.what() << std::endl;

                                            }
                                        }

                                        frame_number = 1;

                                        clients[i].incoming_queue.pop();
                                        message.clear();

                                        cont = false;

                                    } else{


                                        for (int j = 0; j < clients[m].log_entries.size(); ++j) {
                                            if(clients[m].log_entries[j].message_content.find(temp_app->message_data) != std::string::npos && clients[m].log_entries[j].receiver_id == temp_app->receiver_ID){
                                                current_log = j;
                                            }
                                        }

                                        std::cout << "Client "<< clients[i].client_id <<" receiving frame #"<< frame_number <<" from client "<<
                                                  clients[m].client_id<<", but intended for client "<< temp_app->receiver_ID <<". Forwarding... " << std::endl;

                                        if (!clients[m].log_entries.empty()){
                                            std::cout << "Error: Unreachable destination. Packets are dropped after "<< clients[m].log_entries[current_log].number_of_hops + 1 <<" hops!"  << std::endl;

                                        } else{
                                            std::cout << "Error: Unreachable destination. Packets are dropped after "<< 1 <<" hops!"  << std::endl;

                                        }



                                        message += temp_app->message_data;
                                        frame_number++;

                                        clients[i].incoming_queue.pop();

                                    }
                                }

                            }
                        }

                    }
                }
            }

        }
    }

}
bool Network::client_not_exist(vector<Client> &clients,Client &client,const std::string& receiver_id){

    for (const auto& entry : client.routing_table) {
        if (entry.first == receiver_id){
            for (int i = 0; i < clients.size(); ++i) {
                if (clients[i].client_id == entry.second){

                    return true;

                }
            }
        }
    }
    if (client.client_id == receiver_id){
        return true;
    }
    return false;

}