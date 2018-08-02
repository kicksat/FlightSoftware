void gps_parse(String gps_data){
//void setup(){
  Serial.begin(9600);
  String gps_data = "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W,4,13,1.00,495.144,M,29.200,M,0.10,0000*40";
  String date = "";
  String latitude = "";
  String longitude = "";
  String altitude = "";
  int num_commas = 0;
  int last_comma = 0;
  for(int i = 0; i < gps_data.length(); i++){
      if(gps_data.charAt(i) == ','){
        num_commas++;
        if(date == 0 & num_commas == 2){
          date = gps_data.substring(last_comma + 1, i);
          //date = date_string.toFloat();
        }else if(latitude == 0 & num_commas == 3){
          latitude = gps_data.substring(last_comma + 1, i);
          //latitude = lat_string.toFloat();
        }else if(longitude == 0 & num_commas == 5){
          longitude = gps_data.substring(last_comma + 1, i);
          //longitude = lon_string.toFloat();
        }else if(altitude == 0 & num_commas == 10){
          altitude = gps_data.substring(last_comma + 1, i);
          //altitude = alt_string.toFloat();
        }
        last_comma = i;
     }
  }
  Serial.print("date: ");
  Serial.println(date);
  Serial.print("latitude: ");
  Serial.println(latitude);
  Serial.print("longitude: ");
  Serial.println(longitude);
  Serial.print("altitude: ");
  Serial.println(altitude);
  
}

//void loop(){ 
//}
