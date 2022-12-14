#include "server.h"
#include "MyException.h"


void Server::startAsServer(){
    memset(&this->host_info, 0, sizeof(this->host_info));

    this->host_info.ai_family   = AF_UNSPEC;
    this->host_info.ai_socktype = SOCK_STREAM;
    this->host_info.ai_flags    = AI_PASSIVE;

    this->status = getaddrinfo(this->hostname, this->port, &this->host_info, &this->host_info_list);
    if (this->status != 0) {
        throw MyException("ERROR cannot get address info for host\n",this->getHostPortInfo());
    }

    this->socket_fd = socket(this->host_info_list->ai_family, 
		     this->host_info_list->ai_socktype, 
		     this->host_info_list->ai_protocol);
    if (this->socket_fd == -1) {
        throw MyException("ERROR cannot create socket\n",this->getHostPortInfo());
    }

    int yes = 1;
    this->status = setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    this->status = ::bind(this->socket_fd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
    if (this->status == -1) {
        throw MyException("ERROR cannot bind socket\n",this->getHostPortInfo());
    }

    this->status = listen(this->socket_fd, 100);
    if (this->status == -1) {
        throw MyException("ERROR cannot listen on socket\n",this->getHostPortInfo());
    }
}

void Client::startAsClient(){
    memset(&this->host_info, 0, sizeof(this->host_info));

    this->host_info.ai_family   = AF_UNSPEC;
    this->host_info.ai_socktype = SOCK_STREAM;

    this->status = getaddrinfo(this->hostname, this->port, &this->host_info, &this->host_info_list);
    if (this->status != 0) {
        throw MyException("ERROR cannot get address info for host\n",this->getHostPortInfo());
    }

    this->socket_fd = socket(this->host_info_list->ai_family, 
		     this->host_info_list->ai_socktype, 
		     this->host_info_list->ai_protocol);
    if (this->socket_fd == -1) {
        throw MyException("ERROR cannot create socket\n",this->getHostPortInfo());
    }

    this->status = connect(this->socket_fd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
    if (this->status == -1) {
        throw MyException("ERROR cannot connect to socket\n",this->getHostPortInfo());
    } //if
}

int Server::accept_connection(string *ip_addr){
    //string *ip_addr as parameter, temporarily commented
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(this->socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1) {
        throw MyException("ERROR cannot accept connection on socket","");
    } //if
    *ip_addr = inet_ntoa(((struct sockaddr_in *)&socket_addr)->sin_addr);
    return client_connection_fd;
}

void my_recvFrom(int fd, vector<char> &v){
    ssize_t index = 0;
    v.resize(65536);
    ssize_t msg_len = recv(fd,&v.data()[index],v.size(),0);
    checkMsgLen(msg_len);
    index+=msg_len;
    Response r=Response(v);
    if(r.getCode()=="304"){
        return;
    }
    if(r.isChunked()){
        cout<<"is chunked"<<endl;
        string res;
        res.insert(res.begin(),v.begin(),v.end());
        while(res.find("0\r\n\r\n")==string::npos){
            v.resize(index+65536);
            msg_len = recv(fd,&v.data()[index],65536,0);
            if(msg_len<=0){
                break;
            }
            res="";
            res.insert(res.begin(),v.begin()+index,v.begin()+index+msg_len);
            index+=msg_len;
        }
        cout<<"the recieved length is "<<index<<endl;
        cout<<"finish receiving"<<endl;
        return;
    }
    else{
        int len=r.getContentLength();
        cout<<"not chunked, content-length is "<<len<<endl;
        while(index<len){
            v.resize(index+65536);
            msg_len = recv(fd,&v.data()[index],v.size(),0);
            if(msg_len<=0){
                break;
            }
            index+=msg_len;
        }
        cout<<"the recieved length is "<<index<<endl;
        cout<<"finish receiving"<<endl;
    }
}

void checkMsgLen(int msg_len){
    if(msg_len==0){
        throw MyException("Received nothing","");
    }
    if(msg_len==-1){
        throw MyException("Error while receiving","");
    }
}

void Client::my_recv(vector<char> &v){
    my_recvFrom(this->socket_fd, v);
}


string Socket::getHostPortInfo(){
    string ans;
    if(this->hostname==NULL || this->port==NULL){
        cerr<<"hostname or port info is not available"<<endl;
    }else{
        ans.append("(");
        ans.append(this->hostname);
        ans.append(",");
        ans.append(this->port);
        ans.append(")");
    }
    return ans;
}

void sendString(int socket,string message){
    send(socket,message.data(),message.size()+1,0);
}

