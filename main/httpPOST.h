HTTPClient http;

String sendHTTP(StaticJsonDocument<4096> doc){

  char buffer[4096];
  size_t payloadSize = serializeJson(doc, buffer);

  http.begin(API_URL);
  http.addHeader("Content-Type", "application/json");
 
  int httpResponseCode = http.POST(buffer);

  String response = "0";

  if(httpResponseCode>0){
  
    response = http.getString();   //Get the response to the request
  
  }else{
    
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();  //Free resources

  if (response == "0") return false;
  else return true;

}
