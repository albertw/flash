/*

   Set variables that are use in the rest of the code

   - mailnotify [x]      - notifys user of new mail every x seconds
                           [-1 = not notify] or on a screen change

   - noclobber           - left arrow will not exit flash on main menu    

   - background x        - load file x into flash's background. x can be a ':' 
                           seperated list of files to overlay as background.
                
   - logging             - turn on exec logging
   
   - execpager           - pager to use when P flag specified in menu item

   - lockscreensaver [t] - turn on the (inappropriatly titled) lock screensaver
                           with update time t

   - lockbackdoor [x]    - Enable (encrypted) backdoor password [x] into the 
                           lock screen.			

   - barclock            - Enable clock in top bar

   - notimeout           - Disable timeouts

*/

#define SF_SET     0x0001
#define SF_NORESET 0x0002

struct set_node
{
  char *variable;
  char *value;
  int flags;
};

int find_variable(char *key, char **data);
void set_variable(char *key,char *data,int resettable);
void unset_variable(char *key);
void global_variables(void);
