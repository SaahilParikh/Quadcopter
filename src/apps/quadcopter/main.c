
typedef enum{
    start,
    fly,
    stop
}command;

command listen_for_command() {
    /*implement ble listing for command*/
    /*for now just return fly*/
    return fly;
}

command curr_command;
int main() {
    curr_command = listen_for_command();

    while(1) {
        switch(curr_command){
            case start:
            /** start motors**/
            /** fly upwards w stabilization stuff*/
            break;
            case fly:

            break;
            case stop:

            break;      
        }
    }
}