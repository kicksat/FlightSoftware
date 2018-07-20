#include "PiksiGPS.h"

#define SBP_MSG(sbp_state, msg_type, item) sbp_send_message(&sbp_state, msg_type, 0x42, sizeof(item), (u8 *)&(item), &my_write)

sbp_state_t sbp_state;

/* SBP structs that messages from Piksi will feed. */
msg_pos_llh_t                 pos_llh;
msg_baseline_ned_t            baseline_ned;
msg_baseline_ecef_t           baseline_ecef;
msg_vel_ned_t                 vel_ned;
msg_vel_ecef_t                vel_ecef;
msg_dops_t                    dops;
msg_gps_time_t                gps_time;
msg_pos_ecef_t                pos_ecef;
msg_settings_read_resp_t      settings;
msg_log_t                     logging;
msg_settings_read_req_t       setting_req;
msg_startup_t                 startup;
msg_heartbeat_t               heartbeat;
msg_acq_result_t              acq_result;
msg_tracking_state_dep_a_t    tracking_state;
msg_obs_dep_a_t               obs;

/*
 * SBP callback nodes must be statically allocated. Each message ID / callback
 * pair must have a unique sbp_msg_callbacks_node_t associated with it.
 */
sbp_msg_callbacks_node_t pos_llh_node;
sbp_msg_callbacks_node_t baseline_ned_node;
sbp_msg_callbacks_node_t baseline_ecef_node;
sbp_msg_callbacks_node_t vel_ned_node;
sbp_msg_callbacks_node_t vel_ecef_node;
sbp_msg_callbacks_node_t dops_node;
sbp_msg_callbacks_node_t gps_time_node;
sbp_msg_callbacks_node_t pos_ecef_node;
sbp_msg_callbacks_node_t settings_node;
sbp_msg_callbacks_node_t setting_req_node;
sbp_msg_callbacks_node_t logging_node;
sbp_msg_callbacks_node_t startup_node;
sbp_msg_callbacks_node_t heartbeat_node;
sbp_msg_callbacks_node_t acq_result_node;
sbp_msg_callbacks_node_t tracking_state_node;
sbp_msg_callbacks_node_t obs_node;

// volatile static int read_wait;
// int write_wait;
// int write_wait = 0;
/*
 * Callback functions to interpret SBP messages.
 * Every message ID has a callback associated with it to
 * receive and interpret the message payload.
 */
void sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  pos_llh = *(msg_pos_llh_t *)msg;
  Serial.println("position recevied");
}
void sbp_baseline_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  baseline_ned = *(msg_baseline_ned_t *)msg;
   Serial.println("baseline recevied");
}
void sbp_baseline_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  baseline_ecef = *(msg_baseline_ecef_t *)msg;
   Serial.println("baseline recevied");
}
void sbp_vel_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  vel_ned = *(msg_vel_ned_t *)msg;
    Serial.println("vel recevied");
}
void sbp_vel_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  vel_ecef = *(msg_vel_ecef_t *)msg;
    Serial.println("vel recevied");
}
void sbp_dops_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  dops = *(msg_dops_t *)msg;
    Serial.println("posdop recevied");
}
void sbp_gps_time_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
   gps_time = *(msg_gps_time_t *)msg;
   Serial.println("potime recevied");
}
void sbp_pos_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  pos_ecef = *(msg_pos_ecef_t *)msg;
    Serial.println("ECEF recevied");
}
void sbp_settings_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  Serial.print("setting recieved****************************:  ");
  // Serial.println(len);
  // Serial.println(strlen(msg));
  // Serial.println(*msg, HEX);
  // settings = *(msg_settings_read_resp_t )msg;
  // settings = msg;
  char temp[1024];
  memcpy(temp, msg, len);
  // settings = *(msg_settings_read_resp_t )msg;
  
  // strncpy(temp, settings.setting, 30);
  // Serial.println(temp);
  // Serial.println(sizeof(msg));
  for (int i = 0; i < len; i++) {
    // Serial.print(temp[i]);
    Serial.print(msg[i]);
    Serial.print(" ");
  }
  Serial.println("");
  // read_wait = 0;
  // Serial.println(read_wait);
}
void sbp_logging_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  logging = *(msg_log_t *)msg;
  Serial.print("---log message---");
  int i = 0;
  while (i<50) {
      Serial.print(logging.text[i]);
      i++;
    }

}

void sbp_startup_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  // startup = *(msg_startup_t *)msg;
    Serial.println("GPS started up");
}
void sbp_heartbeat_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  // startup = *(msg_startup_t *)msg;
    Serial.println("HEARTBEAT******************");
}
void sbp_acq_result_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  acq_result = *(msg_acq_result_t *)msg;
    Serial.println("acq_result recevied");
}
void sbp_tracking_state_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  tracking_state = *(msg_tracking_state_dep_a_t *)msg;
    Serial.println("tracking state recevied");
}
void sbp_obs_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  obs = *(msg_obs_dep_a_t *)msg;
    Serial.println("Observation recevied");
}

void sbp_setup(void)
{
  // digitalWrite(13, HIGH);

  int temp;

  /* SBP parser state must be initialized before sbp_process is called. */
  sbp_state_init(&sbp_state);
  /* Register a node and callback, and associate them with a specific message ID. */

  sbp_register_callback(&sbp_state, SBP_MSG_POS_LLH, &sbp_pos_llh_callback,
                        NULL, &pos_llh_node);
  sbp_register_callback(&sbp_state, SBP_MSG_BASELINE_NED, &sbp_baseline_ned_callback,
                        NULL, &baseline_ned_node);
  sbp_register_callback(&sbp_state, SBP_MSG_BASELINE_ECEF, &sbp_baseline_ecef_callback,
                        NULL, &baseline_ecef_node);
  sbp_register_callback(&sbp_state, SBP_MSG_VEL_NED, &sbp_vel_ned_callback,
                        NULL, &vel_ned_node);
  sbp_register_callback(&sbp_state, SBP_MSG_VEL_ECEF, &sbp_vel_ecef_callback,
                        NULL, &vel_ecef_node);
  sbp_register_callback(&sbp_state, SBP_MSG_DOPS, &sbp_dops_callback,
                        NULL, &dops_node);
  sbp_register_callback(&sbp_state, SBP_MSG_GPS_TIME, &sbp_gps_time_callback,
                        NULL, &gps_time_node);
  sbp_register_callback(&sbp_state, SBP_MSG_POS_ECEF, &sbp_pos_ecef_callback,
                        NULL, &pos_ecef_node);
  sbp_register_callback(&sbp_state, SBP_MSG_SETTINGS_READ_RESP, &sbp_settings_callback,
                        NULL, &settings_node);
  sbp_register_callback(&sbp_state, SBP_MSG_LOG, &sbp_logging_callback,
                        NULL, &logging_node);
  sbp_register_callback(&sbp_state, SBP_MSG_STARTUP, &sbp_startup_callback,
                        NULL, &startup_node);
  sbp_register_callback(&sbp_state, SBP_MSG_HEARTBEAT, &sbp_heartbeat_callback,
                        NULL, &heartbeat_node);
  sbp_register_callback(&sbp_state, SBP_MSG_ACQ_RESULT, &sbp_acq_result_callback,
                        NULL, &acq_result_node);
  sbp_register_callback(&sbp_state, SBP_MSG_TRACKING_STATE, &sbp_tracking_state_callback,
                        NULL, &tracking_state_node);
  sbp_register_callback(&sbp_state, SBP_MSG_OBS_DEP_A, &sbp_obs_callback,
                        NULL, &obs_node);

}


/*
 * FIFO to hold received UART bytes before libsbp parses them.
 */
#define FIFO_LEN 1024
char sbp_msg_fifo[FIFO_LEN];
u16 head = 0;
u16 tail = 0;


/* Return 1 if true, 0 otherwise. */
u8 fifo_full(void){
  if (((tail+1)%FIFO_LEN) == head) {
    return 1;
  }
  return 0;
}

/* Return 1 if true, 0 otherwise. */
u8 fifo_empty(void){
  if (head == tail)
    return 1;
  return 0;
}

/*
 * Append a character to our SBP message fifo.
 * Returns 1 if char successfully appended to fifo.
 * Returns 0 if fifo is full.
 */
u8 fifo_write(char c){
  if (fifo_full())
    return 0;

  sbp_msg_fifo[tail] = c;
  tail = (tail+1) % FIFO_LEN;
  return 1;
}

/*
 * Read 1 char from fifo.
 * Returns 0 if fifo is empty, otherwise 1.
 */
u8 fifo_read_char(char *c) {
  if (fifo_empty())
    return 0;

  *c = sbp_msg_fifo[head];
  head = (head+1) % FIFO_LEN;
  return 1;
}

/*
 * Read arbitrary number of chars from FIFO. Must conform to
 * function definition that is passed to the function
 * sbp_process().
 * Returns the number of characters successfully read.
 */
u32 fifo_read(u8 *buff, u32 n, void *context) {
  int i;
  for (i=0; i<n; i++)
    if (!fifo_read_char((char *)(buff + i)))
      break;
  return i;
}

u32 my_write(u8 *buff, u32 n, void *context) {  
  int i;
  for (i=0; i<n; i++) {
    // Serial.print(i);
    // Serial.print(" : ");
    // Serial.println((u8)(*(buff+i)));
    int ret = Serial3.write((u8)(*(buff+i)));
    // Serial.print("serail value");
    // Serial.println(ret);
    if (!ret){
      Serial.println("in if loop");
      break;
    }
  }
  // Serial.print("Return value: ");
  // Serial.println(i, BIN);
  return i;

}

PiksiGPS::PiksiGPS() {
	// TODO: add argument to determine serial port to use
  read_wait = 0;

}

void PiksiGPS::initialize() {
	// TODO: use saved serial port reference

	//Serial1.begin(115200);

	sbp_setup();
}

void PiksiGPS::update() {

	while(Serial3.available() > 0) {
    // Serial.println("got something");
		// store all available bytes in queue
		fifo_write(Serial3.read());
	

	
	s8 ret = sbp_process(&sbp_state, &fifo_read);
  if (ret != SBP_OK) {
    // Serial.print(ret, DEC);
    // Serial.print(sbp_state.msg_type);
  }
}

}

void PiksiGPS::setThis(char* group, char* setting, char* value) {
  // settings_read_index.index = 2;
  // char temp = new char[21];
  // char temp[] = "[simulator, enabled]";
  // char temp[][1] = {"simulator", "enabled"};
  // char temp[1024] = "simulator";
  // strcat(temp, "\0");
  // strcat(temp, "\0enabled");
  // strcat(temp, "\0");
  char temp[] = {'s','i','m','u', 'l', 'a','t','o','r','\0', 'e','n','a','b', 'l', 'e','d', '\0', 'T', 'r', 'u', 'e', '\0'};

  // char* temp[3] = {group, setting, value};
  // for (int i = 0; i< sizeof(temp); i++) {
  //   Serial.print(temp[i]);
  // }
  // strcpy(setting_req.setting, temp);
  // Serial.println(sizeof(temp));
  s8 error;
  error = SBP_MSG(sbp_state, SBP_MSG_SETTINGS_WRITE, temp);
  Serial.print("error:");
  Serial.println(error);
  // SBP_MSG(sbp_state, SBP_MSG_SETTINGS, msg);
  // return settings.setting;
}

void PiksiGPS::readThis(char* group, char* setting, char* result) {

  // char* temp[4] = {group, setting};


  int group_len = strlen(group);
  int setting_len = strlen(setting);
  Serial.print(setting_len);
  int total_len = group_len + setting_len + 2;
  char temp[total_len];
  strncpy(temp, group, group_len);
  temp[group_len] = '\0';
  strncpy(&temp[group_len+1], setting, setting_len);
  temp[total_len-1] = '\0';


  // char temp[] = {'s','i','m','u', 'l', 'a','t','o','r','\0', 'r','a','d','i', 'u', 's', '\0'};
  // char temp[] = {'s','i','m','u', 'l', 'a','t','r','\0', 'n','u','m','s', 'a', 't', 's', '\0'};
  // char temp[] = {'s','i','m','u', 'l', 'a','t','o','r', '\0', 'e','n','a','b', 'l', 'e', 'd', '\0'};

  // char temp[18];
  // int templen;  
  // strncpy(temp, group, 17);
  // templen = strlen(group) + 1;
  // temp[9] = '\0';
  // strncpy(temp+templen, setting, 17-templen);
  // templen += strlen(setting) + 1;
  // temp[17] = '\0';
  // for (int i = 0; i < total_len; i++) {
  //   Serial.println(temp[i]);
  // }
  // read_wait = 1;
  s8 error;
  error = SBP_MSG(sbp_state, SBP_MSG_SETTINGS_READ_REQ, temp);
  Serial.print("error:");
  Serial.println(error);
  // Serial.print(read_wait);
  // while (read_wait == 1) {
  //   PiksiGPS::update();
  //   // Serial.println(read_wait);
  // }
  // result = settings.setting;
  // Serial.println("done of read");
}


msg_log_t PiksiGPS::get_logging() {
  return logging;
}

msg_pos_ecef_t PiksiGPS::get_pos_ecef(){
	return pos_ecef;
}

msg_vel_ecef_t PiksiGPS::get_vel_ecef() {
	return vel_ecef;
}

msg_acq_result_t PiksiGPS::get_acq_result() {
  return acq_result;
}

msg_tracking_state_dep_a_t  PiksiGPS::get_tracking_state() {
  return tracking_state;
}

msg_obs_dep_a_t  PiksiGPS::get_obs() {
  return obs;
}

double PiksiGPS::get_pos_x(){
	return pos_ecef.x;
}

double PiksiGPS::get_pos_y(){
	return pos_ecef.y;		
}

double PiksiGPS::get_pos_z(){
	return pos_ecef.z;
}

uint32_t PiksiGPS::get_tow() {
	return gps_time.tow;
}

uint16_t get_week_num() {
	return gps_time.wn;
}

double PiksiGPS::get_long() {
	return pos_llh.lon;
}

double PiksiGPS::get_lat() {
	return pos_llh.lat;
}

double PiksiGPS::get_height() {
	return pos_llh.height;
}
