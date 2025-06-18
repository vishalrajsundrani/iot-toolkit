#include <HTTPClient.h>

HTTPClient http;

String api_url = "https://c238fcn8lf.execute-api.ap-southeast-2.amazonaws.com/WriteHTTPDynamoData";

String sendHTTP(StaticJsonDocument<4096> doc){

  char buffer[4096];
  size_t payloadSize = serializeJson(doc, buffer);

  http.begin(api_url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(buffer);

  
  String response = "0";

  if(httpResponseCode>0){
  
    response = http.getString();                       //Get the response to the request
  
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  
  }else{
    
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();  //Free resources
  return response;

}