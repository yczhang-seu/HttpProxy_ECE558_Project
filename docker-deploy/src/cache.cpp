#include "cache.hpp"

bool Cache::storeResponse(string uri,Response rsp,int id){
  string code=rsp.getCode();
  string cacheControl=rsp.getCacheControl();
  if(code=="200"&&!rsp.isPrivate()&&!rsp.isNoStore()){
    put(uri,rsp.getResponse());
    string expire=rsp.getExpire();
    string message;
    if(expire!=""){
      message=generateLogMsg(id,"cached, expires at "+expire);
    }
    else{
      message=generateLogMsg(id,"cached, but requires re-validation");
    }
    writeToLog(message);
    //cout<<message<<endl;
    return true;
  }
  else{
    string reason="";
    if(code!="200"){
      reason="not 200 OK";
    }
    else if(rsp.isPrivate()){
      reason="Cache-control: private";
    }
    else if(rsp.isNoStore()){
      reason="Cache-control: no-store";
    }
    string message=generateLogMsg(id,"not cacheable because "+reason);
    writeToLog(message);
    //cout<<message;
    return false;
  }
}

string Cache::revalidate(Request request, Response response,int socket,int id){
  string etag=response.getEtag();
  string lastModify=response.getLastModify();
  //if has e-tag, send if-none-match
  if(etag!=""){
    cout<<"use etag"<<endl;
    return checkIfNoneMatch(request,response,socket,etag,id);
  }
  //else if has last-modify, send if-modified-since
  else if(lastModify!=""){
    cout<<"use last modify"<<endl;
    return checkIfModifiedSince(request,response,socket,lastModify,id);
  }
  //else send all request
  else{
    return reSendRequest(request,socket,id);
  }
}

string Cache::checkIfNoneMatch(Request request,Response response,int socket,string etag,int id){
  return checkValidate(request,response,socket,"If-None-Match: ",etag,id);
}


string Cache::checkIfModifiedSince(Request request,Response response,int socket,string lastModify,int id){
  return checkValidate(request,response,socket,"If-Modified-Since: ",lastModify,id);
}

string Cache::reSendRequest(Request request, int socket,int id){
  cout<<"resend"<<endl;
  string origin=request.getRequest();
  //cout<<origin<<endl;
  sendString(socket,origin);
  vector<char> v;
  my_recvFrom(socket,v);
  Response newResponse(v);
  storeResponse(request.getURI(),newResponse,id);
  return newResponse.getResponse();
}

string Cache::checkValidate(Request request,Response response,int socket, string type,string content,int id){
  //send new request
  cout<<"re validate"<<endl;
  string origin=request.getHeader();
  string newRequest=origin+"\r\n"+type+content+"\r\n\r\n";
  //cout<<newRequest;
  sendString(socket,newRequest);
  //receive new response
  vector<char> v;
  my_recvFrom(socket,v);
  Response newResponse(v);
  if(newResponse.getCode()=="304"){
    return response.getResponse();
  }
  else{
    storeResponse(request.getURI(),newResponse,id);
    return newResponse.getResponse();
  }
}


