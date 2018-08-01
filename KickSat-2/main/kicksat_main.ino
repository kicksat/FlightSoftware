/* 
 *  General structure to manage all of the flight code
 */

// Modes
bool emo_mode;
bool standby_mode;
bool deployment_mode;

// Flags to let us know whether or not we have initialized the mode
bool did_init_emo;
bool did_init_standby;
bool did_init_deployment;

// Flags that result in mode changes
bool antenna_deployed;
bool received_command_to_arm;
bool sprites_did_deploy;
bool deployment_timeout;

/*
 * Setup initial conditions
 */
void setup() 
{
  // put your setup code here, to run once:

  // initial state = early_mission_operations
  emo_mode = true;
  standby_mode = false;
  deployment_mode = false;

  // no modes have been initialized
  did_init_emo = false;
  did_init_standby = false;
  did_init_deployment = false;
  
  // state change flags are all false
  antenna_deployed = false;
  received_command_to_arm = false;
  sprites_did_deploy = false;
  deployment_timeout = false;
}

void loop() 
{
  // put your main code here, to run repeatedly:

  /* 
   *  check which state we are in --> run that code
   */
  if(emo_mode)
  {
    if(!did_init_emo)
    {
      setup_emo_mode();
    }

    // this is the flight code that will make the SC hold 45 mins --> deploy the antennae
    loop_emo_mode();
  
    // mode transition condition
    if(antenna_deployed)
    {
      emo_mode = false; 
      standby_mode = true;
      deployment_mode = false;
    }
  }
  else if(standby_mode)
  {
    if(!did_init_standby_mode)
    {
      setup_standby_mode();
    }

    // this is the flight code that will make the mothership collect data, beacon, and all other wonderful stuff!
    loop_standby_mode();
  
    // mode transition condition
    if(received_command_to_arm)
    {
      emo_mode = false; 
      standby_mode = false;
      deployment_mode = true;
    }
  }
  else if(deployment_mode)
  {
    if(!did_init_deployment)
    {
      setup_deployment_mode();
    }

    // this is the flight code that will make the mothership collect data, beacon, and all other wonderful stuff!
    loop_deployment_mode();
  
    // mode transition condition back to standby mode if sprites deploy or there is a timeout
    if(sprites_did_deploy || deployment_timeout)
    {
      emo_mode = false; 
      standby_mode = true;
      deployment_mode = false;
    } 
  }
  else
  {
    // we are in no known modes --> possibly reflash the board from MRAM...
  }
   

}


void setup_emo_mode()
{
  
}

void setup_standby_mode()
{
  
}

void setup_arming_mode()
{
  
}

void setup_deployment_mode()
{
  
}

// probably not going to loop, just the "loop()" function if emo_mode were its own file
void loop_emo_mode()
{
  
}

void loop_standby_mode()
{
  
}

// this is probably not going to loop, but would be the "loop()" if deployment mode were its own file
void loop_deployment_mode()
{
  
}

